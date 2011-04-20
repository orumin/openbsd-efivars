/*	$OpenBSD: arc.c,v 1.93 2011/04/20 07:13:51 claudio Exp $ */

/*
 * Copyright (c) 2006 David Gwynne <dlg@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "bio.h"

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/buf.h>
#include <sys/kernel.h>
#include <sys/malloc.h>
#include <sys/mutex.h>
#include <sys/device.h>
#include <sys/rwlock.h>

#include <machine/bus.h>

#include <dev/pci/pcireg.h>
#include <dev/pci/pcivar.h>
#include <dev/pci/pcidevs.h>

#include <scsi/scsi_all.h>
#include <scsi/scsiconf.h>

#include <sys/sensors.h>
#if NBIO > 0
#include <sys/ioctl.h>
#include <dev/biovar.h>
#endif

#ifdef ARC_DEBUG
#define ARC_D_INIT	(1<<0)
#define ARC_D_RW	(1<<1)
#define ARC_D_DB	(1<<2)

int arcdebug = 0;

#define DPRINTF(p...)		do { if (arcdebug) printf(p); } while (0)
#define DNPRINTF(n, p...)	do { if ((n) & arcdebug) printf(p); } while (0)

#else
#define DPRINTF(p...)		/* p */
#define DNPRINTF(n, p...)	/* n, p */
#endif

/* Areca boards using the Intel IOP are Revision A (RA) */

#define ARC_RA_PCI_BAR			PCI_MAPREG_START

#define ARC_RA_INB_MSG0			0x0010
#define  ARC_RA_INB_MSG0_NOP			(0x00000000)
#define  ARC_RA_INB_MSG0_GET_CONFIG		(0x00000001)
#define  ARC_RA_INB_MSG0_SET_CONFIG		(0x00000002)
#define  ARC_RA_INB_MSG0_ABORT_CMD		(0x00000003)
#define  ARC_RA_INB_MSG0_STOP_BGRB		(0x00000004)
#define  ARC_RA_INB_MSG0_FLUSH_CACHE		(0x00000005)
#define  ARC_RA_INB_MSG0_START_BGRB		(0x00000006)
#define  ARC_RA_INB_MSG0_CHK331PENDING		(0x00000007)
#define  ARC_RA_INB_MSG0_SYNC_TIMER		(0x00000008)
#define ARC_RA_INB_MSG1			0x0014
#define ARC_RA_OUTB_ADDR0		0x0018
#define ARC_RA_OUTB_ADDR1		0x001c
#define  ARC_RA_OUTB_ADDR1_FIRMWARE_OK		(1<<31)
#define ARC_RA_INB_DOORBELL		0x0020
#define  ARC_RA_INB_DOORBELL_WRITE_OK		(1<<0)
#define  ARC_RA_INB_DOORBELL_READ_OK		(1<<1)
#define ARC_RA_OUTB_DOORBELL		0x002c
#define  ARC_RA_OUTB_DOORBELL_WRITE_OK		(1<<0)
#define  ARC_RA_OUTB_DOORBELL_READ_OK		(1<<1)
#define ARC_RA_INTRSTAT			0x0030
#define  ARC_RA_INTRSTAT_MSG0			(1<<0)
#define  ARC_RA_INTRSTAT_MSG1			(1<<1)
#define  ARC_RA_INTRSTAT_DOORBELL		(1<<2)
#define  ARC_RA_INTRSTAT_POSTQUEUE		(1<<3)
#define  ARC_RA_INTRSTAT_PCI			(1<<4)
#define ARC_RA_INTRMASK			0x0034
#define  ARC_RA_INTRMASK_MSG0			(1<<0)
#define  ARC_RA_INTRMASK_MSG1			(1<<1)
#define  ARC_RA_INTRMASK_DOORBELL		(1<<2)
#define  ARC_RA_INTRMASK_POSTQUEUE		(1<<3)
#define  ARC_RA_INTRMASK_PCI			(1<<4)
#define ARC_RA_POST_QUEUE		0x0040
#define  ARC_RA_POST_QUEUE_ADDR_SHIFT		5
#define  ARC_RA_POST_QUEUE_IAMBIOS		(1<<30)
#define  ARC_RA_POST_QUEUE_BIGFRAME		(1<<31)
#define ARC_RA_REPLY_QUEUE		0x0044
#define  ARC_RA_REPLY_QUEUE_ADDR_SHIFT		5
#define  ARC_RA_REPLY_QUEUE_ERR			(1<<28)
#define  ARC_RA_REPLY_QUEUE_IAMBIOS		(1<<30)
#define ARC_RA_MSGBUF			0x0a00
#define  ARC_RA_MSGBUF_LEN			1024
#define ARC_RA_IOC_WBUF_LEN		0x0e00
#define ARC_RA_IOC_WBUF			0x0e04
#define ARC_RA_IOC_RBUF_LEN		0x0f00
#define ARC_RA_IOC_RBUF			0x0f04
#define  ARC_RA_IOC_RWBUF_MAXLEN	124 /* for both RBUF and WBUF */

/* Areca boards using the Marvel IOP are Revision B (RB) */

#define ARC_RB_DRV2IOP_DOORBELL		0x00020400
#define ARC_RB_DRV2IOP_DOORBELL_MASK	0x00020404
#define ARC_RB_IOP2DRV_DOORBELL		0x00020408
#define  ARC_RB_IOP2DRV_DOORBELL_FIRMWARE_OK	(1<<31)
#define ARC_RB_IOP2DRV_DOORBELL_MASK	0x0002040c

struct arc_msg_firmware_info {
	u_int32_t		signature;
#define ARC_FWINFO_SIGNATURE_GET_CONFIG		(0x87974060)
	u_int32_t		request_len;
	u_int32_t		queue_len;
	u_int32_t		sdram_size;
	u_int32_t		sata_ports;
	u_int8_t		vendor[40];
	u_int8_t		model[8];
	u_int8_t		fw_version[16];
	u_int8_t		device_map[16];
} __packed;

struct arc_msg_scsicmd {
	u_int8_t		bus;
	u_int8_t		target;
	u_int8_t		lun;
	u_int8_t		function;

	u_int8_t		cdb_len;
	u_int8_t		sgl_len;
	u_int8_t		flags;
#define ARC_MSG_SCSICMD_FLAG_SGL_BSIZE_512	(1<<0)
#define ARC_MSG_SCSICMD_FLAG_FROM_BIOS		(1<<1)
#define ARC_MSG_SCSICMD_FLAG_WRITE		(1<<2)
#define ARC_MSG_SCSICMD_FLAG_SIMPLEQ		(0x00)
#define ARC_MSG_SCSICMD_FLAG_HEADQ		(0x08)
#define ARC_MSG_SCSICMD_FLAG_ORDERQ		(0x10)
	u_int8_t		reserved;

	u_int32_t		context;
	u_int32_t		data_len;

#define ARC_MSG_CDBLEN				16
	u_int8_t		cdb[ARC_MSG_CDBLEN];

	u_int8_t		status;
#define ARC_MSG_STATUS_SELTIMEOUT		0xf0
#define ARC_MSG_STATUS_ABORTED			0xf1
#define ARC_MSG_STATUS_INIT_FAIL		0xf2
#define ARC_MSG_SENSELEN			15
	u_int8_t		sense_data[ARC_MSG_SENSELEN];

	/* followed by an sgl */
} __packed;

struct arc_sge {
	u_int32_t		sg_hdr;
#define ARC_SGE_64BIT				(1<<24)
	u_int32_t		sg_lo_addr;
	u_int32_t		sg_hi_addr;
} __packed;

#define ARC_MAX_TARGET		16
#define ARC_MAX_LUN		8
#define ARC_MAX_IOCMDLEN	512
#define ARC_BLOCKSIZE		512

/* the firmware deals with up to 256 or 512 byte command frames. */
/* sizeof(struct arc_msg_scsicmd) + (sizeof(struct arc_sge) * 38) == 508 */
#define ARC_SGL_MAXLEN		38
/* sizeof(struct arc_msg_scsicmd) + (sizeof(struct arc_sge) * 17) == 252 */
#define ARC_SGL_256LEN		17

struct arc_io_cmd {
	struct arc_msg_scsicmd	cmd;
	struct arc_sge		sgl[ARC_SGL_MAXLEN];
} __packed;

/* definitions of the firmware commands sent via the doorbells */

struct arc_fw_hdr {
	u_int8_t		byte1;
	u_int8_t		byte2;
	u_int8_t		byte3;
} __packed;

/* the fw header must always equal this */
struct arc_fw_hdr arc_fw_hdr = { 0x5e, 0x01, 0x61 };

struct arc_fw_bufhdr {
	struct arc_fw_hdr	hdr;
	u_int16_t		len;
} __packed;

#define ARC_FW_RAIDINFO		0x20	/* opcode + raid# */
#define ARC_FW_VOLINFO		0x21	/* opcode + vol# */
#define ARC_FW_DISKINFO		0x22	/* opcode + physdisk# */
#define ARC_FW_SYSINFO		0x23	/* opcode. reply is fw_sysinfo */
#define ARC_FW_MUTE_ALARM	0x30	/* opcode only */
#define ARC_FW_SET_ALARM	0x31	/* opcode + 1 byte for setting */
#define  ARC_FW_SET_ALARM_DISABLE		0x00
#define  ARC_FW_SET_ALARM_ENABLE		0x01
#define ARC_FW_NOP		0x38	/* opcode only */

#define ARC_FW_CMD_OK		0x41
#define ARC_FW_BLINK		0x43
#define  ARC_FW_BLINK_ENABLE			0x00
#define  ARC_FW_BLINK_DISABLE			0x01
#define ARC_FW_CMD_PASS_REQD	0x4d

struct arc_fw_comminfo {
	u_int8_t		baud_rate;
	u_int8_t		data_bits;
	u_int8_t		stop_bits;
	u_int8_t		parity;
	u_int8_t		flow_control;
} __packed;

struct arc_fw_scsiattr {
	u_int8_t		channel;// channel for SCSI target (0/1)
	u_int8_t		target;
	u_int8_t		lun;
	u_int8_t		tagged;
	u_int8_t		cache;
	u_int8_t		speed;
} __packed;

struct arc_fw_raidinfo {
	u_int8_t		set_name[16];
	u_int32_t		capacity;
	u_int32_t		capacity2;
	u_int32_t		fail_mask;
	u_int8_t		device_array[32];
	u_int8_t		member_devices;
	u_int8_t		new_member_devices;
	u_int8_t		raid_state;
	u_int8_t		volumes;
	u_int8_t		volume_list[16];
	u_int8_t		reserved1[3];
	u_int8_t		free_segments;
	u_int32_t		raw_stripes[8];
	u_int8_t		reserved2[12];
} __packed;

struct arc_fw_volinfo {
	u_int8_t		set_name[16];
	u_int32_t		capacity;
	u_int32_t		capacity2;
	u_int32_t		fail_mask;
	u_int32_t		stripe_size; /* in blocks */
	u_int32_t		new_fail_mask;
	u_int32_t		new_stripe_size;
	u_int32_t		volume_status;
#define ARC_FW_VOL_STATUS_NORMAL	0x00
#define ARC_FW_VOL_STATUS_INITTING	(1<<0)
#define ARC_FW_VOL_STATUS_FAILED	(1<<1)
#define ARC_FW_VOL_STATUS_MIGRATING	(1<<2)
#define ARC_FW_VOL_STATUS_REBUILDING	(1<<3)
#define ARC_FW_VOL_STATUS_NEED_INIT	(1<<4)
#define ARC_FW_VOL_STATUS_NEED_MIGRATE	(1<<5)
#define ARC_FW_VOL_STATUS_INIT_FLAG	(1<<6)
#define ARC_FW_VOL_STATUS_NEED_REGEN	(1<<7)
#define ARC_FW_VOL_STATUS_CHECKING	(1<<8)
#define ARC_FW_VOL_STATUS_NEED_CHECK	(1<<9)
	u_int32_t		progress;
	struct arc_fw_scsiattr	scsi_attr;
	u_int8_t		member_disks;
	u_int8_t		raid_level;
#define ARC_FW_VOL_RAIDLEVEL_0		0x00
#define ARC_FW_VOL_RAIDLEVEL_1		0x01
#define ARC_FW_VOL_RAIDLEVEL_3		0x02
#define ARC_FW_VOL_RAIDLEVEL_5		0x03
#define ARC_FW_VOL_RAIDLEVEL_6		0x04
#define ARC_FW_VOL_RAIDLEVEL_PASSTHRU	0x05
	u_int8_t		new_member_disks;
	u_int8_t		new_raid_level;
	u_int8_t		raid_set_number;
	u_int8_t		reserved[5];
} __packed;

struct arc_fw_diskinfo {
	u_int8_t		model[40];
	u_int8_t		serial[20];
	u_int8_t		firmware_rev[8];
	u_int32_t		capacity;
	u_int32_t		capacity2;
	u_int8_t		device_state;
	u_int8_t		pio_mode;
	u_int8_t		current_udma_mode;
	u_int8_t		udma_mode;
	u_int8_t		drive_select;
	u_int8_t		raid_number; // 0xff unowned
	struct arc_fw_scsiattr	scsi_attr;
	u_int8_t		reserved[44];
} __packed;

struct arc_fw_sysinfo {
	u_int8_t		vendor_name[40];
	u_int8_t		serial_number[16];
	u_int8_t		firmware_version[16];
	u_int8_t		boot_version[16];
	u_int8_t		mb_version[16];
	u_int8_t		model_name[8];

	u_int8_t		local_ip[4];
	u_int8_t		current_ip[4];

	u_int32_t		time_tick;
	u_int32_t		cpu_speed;
	u_int32_t		icache;
	u_int32_t		dcache;
	u_int32_t		scache;
	u_int32_t		memory_size;
	u_int32_t		memory_speed;
	u_int32_t		events;

	u_int8_t		gsiMacAddress[6];
	u_int8_t		gsiDhcp;

	u_int8_t		alarm;
	u_int8_t		channel_usage;
	u_int8_t		max_ata_mode;
	u_int8_t		sdram_ecc;
	u_int8_t		rebuild_priority;
	struct arc_fw_comminfo	comm_a;
	struct arc_fw_comminfo	comm_b;
	u_int8_t		ide_channels;
	u_int8_t		scsi_host_channels;
	u_int8_t		ide_host_channels;
	u_int8_t		max_volume_set;
	u_int8_t		max_raid_set;
	u_int8_t		ether_port;
	u_int8_t		raid6_engine;
	u_int8_t		reserved[75];
} __packed;

int			arc_match(struct device *, void *, void *);
void			arc_attach(struct device *, struct device *, void *);
int			arc_detach(struct device *, int);
void			arc_shutdown(void *);
int			arc_intr(void *);

struct arc_iop;
struct arc_ccb;
SLIST_HEAD(arc_ccb_list, arc_ccb);

struct arc_softc {
	struct device		sc_dev;
	const struct arc_iop	*sc_iop;
	struct scsi_link	sc_link;

	pci_chipset_tag_t	sc_pc;
	pcitag_t		sc_tag;

	bus_space_tag_t		sc_iot;
	bus_space_handle_t	sc_ioh;
	bus_size_t		sc_ios;
	bus_dma_tag_t		sc_dmat;

	void			*sc_ih;

	void			*sc_shutdownhook;

	int			sc_req_count;

	struct arc_dmamem	*sc_requests;
	struct arc_ccb		*sc_ccbs;
	struct arc_ccb_list	sc_ccb_free;
	struct mutex		sc_ccb_mtx;

	struct scsi_iopool	sc_iopool;
	struct scsibus_softc	*sc_scsibus;

	struct rwlock		sc_lock;
	volatile int		sc_talking;

	struct ksensor		*sc_sensors;
	struct ksensordev	sc_sensordev;
	int			sc_nsensors;

	u_int32_t		sc_ledmask;
};
#define DEVNAME(_s)		((_s)->sc_dev.dv_xname)

struct cfattach arc_ca = {
	sizeof(struct arc_softc), arc_match, arc_attach, arc_detach
};

struct cfdriver arc_cd = {
	NULL, "arc", DV_DULL
};

/* interface for scsi midlayer to talk to */
void			arc_scsi_cmd(struct scsi_xfer *);
void			arc_minphys(struct buf *, struct scsi_link *);

struct scsi_adapter arc_switch = {
	arc_scsi_cmd, arc_minphys, NULL, NULL, NULL
};

/* code to deal with getting bits in and out of the bus space */
u_int32_t		arc_read(struct arc_softc *, bus_size_t);
void			arc_read_region(struct arc_softc *, bus_size_t,
			    void *, size_t);
void			arc_write(struct arc_softc *, bus_size_t, u_int32_t);
void			arc_write_region(struct arc_softc *, bus_size_t,
			    void *, size_t);
int			arc_wait_eq(struct arc_softc *, bus_size_t,
			    u_int32_t, u_int32_t);
int			arc_wait_ne(struct arc_softc *, bus_size_t,
			    u_int32_t, u_int32_t);
int			arc_msg0(struct arc_softc *, u_int32_t);

#define arc_push(_s, _r)	arc_write((_s), ARC_RA_POST_QUEUE, (_r))
#define arc_pop(_s)		arc_read((_s), ARC_RA_REPLY_QUEUE)

/* wrap up the bus_dma api */
struct arc_dmamem {
	bus_dmamap_t		adm_map;
	bus_dma_segment_t	adm_seg;
	size_t			adm_size;
	caddr_t			adm_kva;
};
#define ARC_DMA_MAP(_adm)	((_adm)->adm_map)
#define ARC_DMA_DVA(_adm)	((_adm)->adm_map->dm_segs[0].ds_addr)
#define ARC_DMA_KVA(_adm)	((void *)(_adm)->adm_kva)

struct arc_dmamem	*arc_dmamem_alloc(struct arc_softc *, size_t);
void			arc_dmamem_free(struct arc_softc *,
			    struct arc_dmamem *);

/* stuff to manage a scsi command */
struct arc_ccb {
	struct arc_softc	*ccb_sc;
	int			ccb_id;

	struct scsi_xfer	*ccb_xs;

	bus_dmamap_t		ccb_dmamap;
	bus_addr_t		ccb_offset;
	struct arc_io_cmd	*ccb_cmd;
	u_int32_t		ccb_cmd_post;

	SLIST_ENTRY(arc_ccb)	ccb_link;
};

int			arc_alloc_ccbs(struct arc_softc *);
struct arc_ccb		*arc_get_ccb(struct arc_softc *);
void			arc_put_ccb(struct arc_softc *, struct arc_ccb *);
int			arc_load_xs(struct arc_ccb *);
int			arc_complete(struct arc_softc *, struct arc_ccb *,
			    int);
void			arc_scsi_cmd_done(struct arc_softc *, struct arc_ccb *,
			    u_int32_t);

/* real stuff for dealing with the hardware */
struct arc_iop {
	int			(*iop_query_firmware)(struct arc_softc *);
};

int			arc_map_pci_resources(struct arc_softc *,
			    struct pci_attach_args *);
void			arc_unmap_pci_resources(struct arc_softc *);
int			arc_intel_query_firmware(struct arc_softc *);
int			arc_marvell_query_firmware(struct arc_softc *);

#if NBIO > 0
/* stuff to do messaging via the doorbells */
void			arc_lock(struct arc_softc *);
void			arc_unlock(struct arc_softc *);
void			arc_wait(struct arc_softc *);
u_int8_t		arc_msg_cksum(void *, u_int16_t);
int			arc_msgbuf(struct arc_softc *, void *, size_t,
			    void *, size_t, int);

/* bioctl */
int			arc_bioctl(struct device *, u_long, caddr_t);
int			arc_bio_inq(struct arc_softc *, struct bioc_inq *);
int			arc_bio_vol(struct arc_softc *, struct bioc_vol *);
int			arc_bio_disk(struct arc_softc *, struct bioc_disk *);
int			arc_bio_alarm(struct arc_softc *, struct bioc_alarm *);
int			arc_bio_alarm_state(struct arc_softc *,
			    struct bioc_alarm *);
int			arc_bio_blink(struct arc_softc *, struct bioc_blink *);

int			arc_bio_getvol(struct arc_softc *, int,
			    struct arc_fw_volinfo *);

#ifndef SMALL_KERNEL
/* sensors */
void			arc_create_sensors(void *, void *);
void			arc_refresh_sensors(void *);
#endif /* SMALL_KERNEL */
#endif

static const struct arc_iop arc_intel = {
	arc_intel_query_firmware
};

static const struct arc_iop arc_marvell = {
	arc_marvell_query_firmware
};

struct arc_board {
	pcireg_t		ab_vendor;
	pcireg_t		ab_product;
	const struct arc_iop	*ab_iop;
};
const struct arc_board	*arc_match_board(struct pci_attach_args *);

static const struct arc_board arc_devices[] = {
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1110, &arc_intel },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1120, &arc_intel },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1130, &arc_intel },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1160, &arc_intel },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1170, &arc_intel },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1200, &arc_intel },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1200_B, &arc_marvell },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1202, &arc_intel },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1210, &arc_intel },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1220, &arc_intel },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1230, &arc_intel },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1260, &arc_intel },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1270, &arc_intel },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1280, &arc_intel },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1380, &arc_intel },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1381, &arc_intel },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1680, &arc_intel },
	{ PCI_VENDOR_ARECA, PCI_PRODUCT_ARECA_ARC1681, &arc_intel }
};

const struct arc_board *
arc_match_board(struct pci_attach_args *pa)
{
	const struct arc_board		*ab;
	int				i;

	for (i = 0; i < sizeof(arc_devices) / sizeof(arc_devices[0]); i++) {
		ab = &arc_devices[i];

		if (PCI_VENDOR(pa->pa_id) == ab->ab_vendor &&
		    PCI_PRODUCT(pa->pa_id) == ab->ab_product)
			return (ab);
	}

	return (NULL);
}

int
arc_match(struct device *parent, void *match, void *aux)
{
	return ((arc_match_board(aux) == NULL) ? 0 : 1);
}

void
arc_attach(struct device *parent, struct device *self, void *aux)
{
	struct arc_softc		*sc = (struct arc_softc *)self;
	struct pci_attach_args		*pa = aux;
	struct scsibus_attach_args	saa;
	struct device			*child;

	sc->sc_talking = 0;
	rw_init(&sc->sc_lock, "arcmsg");

	sc->sc_iop = arc_match_board(pa)->ab_iop;

	if (arc_map_pci_resources(sc, pa) != 0) {
		/* error message printed by arc_map_pci_resources */
		return;
	}

	if (sc->sc_iop->iop_query_firmware(sc) != 0) {
		/* error message printed by arc_query_firmware */
		goto unmap_pci;
	}

	if (arc_alloc_ccbs(sc) != 0) {
		/* error message printed by arc_alloc_ccbs */
		goto unmap_pci;
	}

	sc->sc_shutdownhook = shutdownhook_establish(arc_shutdown, sc);
	if (sc->sc_shutdownhook == NULL)
		panic("unable to establish arc shutdownhook");

	sc->sc_link.adapter = &arc_switch;
	sc->sc_link.adapter_softc = sc;
	sc->sc_link.adapter_target = ARC_MAX_TARGET;
	sc->sc_link.adapter_buswidth = ARC_MAX_TARGET;
	sc->sc_link.openings = sc->sc_req_count;
	sc->sc_link.pool = &sc->sc_iopool;

	bzero(&saa, sizeof(saa));
	saa.saa_sc_link = &sc->sc_link;

	child = config_found(self, &saa, scsiprint);
	sc->sc_scsibus = (struct scsibus_softc *)child;

	/* enable interrupts */
	arc_write(sc, ARC_RA_INTRMASK,
	    ~(ARC_RA_INTRMASK_POSTQUEUE|ARC_RA_INTRSTAT_DOORBELL));

#if NBIO > 0
	if (bio_register(self, arc_bioctl) != 0)
		panic("%s: bioctl registration failed", DEVNAME(sc));

#ifndef SMALL_KERNEL
	/*
	 * you need to talk to the firmware to get volume info. our firmware
	 * interface relies on being able to sleep, so we need to use a thread
	 * to do the work.
	 */
	if (scsi_task(arc_create_sensors, sc, NULL, 1) != 0)
		printf("%s: unable to schedule arc_create_sensors as a "
		    "scsi task", DEVNAME(sc));
#endif
#endif

	return;
unmap_pci:
	arc_unmap_pci_resources(sc);
}

int
arc_detach(struct device *self, int flags)
{
	struct arc_softc		*sc = (struct arc_softc *)self;

	shutdownhook_disestablish(sc->sc_shutdownhook);

	if (arc_msg0(sc, ARC_RA_INB_MSG0_STOP_BGRB) != 0)
		printf("%s: timeout waiting to stop bg rebuild\n", DEVNAME(sc));

	if (arc_msg0(sc, ARC_RA_INB_MSG0_FLUSH_CACHE) != 0)
		printf("%s: timeout waiting to flush cache\n", DEVNAME(sc));

	return (0);
}

void
arc_shutdown(void *xsc)
{
	struct arc_softc		*sc = xsc;

	if (arc_msg0(sc, ARC_RA_INB_MSG0_STOP_BGRB) != 0)
		printf("%s: timeout waiting to stop bg rebuild\n", DEVNAME(sc));

	if (arc_msg0(sc, ARC_RA_INB_MSG0_FLUSH_CACHE) != 0)
		printf("%s: timeout waiting to flush cache\n", DEVNAME(sc));
}

int
arc_intr(void *arg)
{
	struct arc_softc		*sc = arg;
	struct arc_ccb			*ccb = NULL;
	char				*kva = ARC_DMA_KVA(sc->sc_requests);
	struct arc_io_cmd		*cmd;
	u_int32_t			reg, intrstat;
	int				ret = 0;

	intrstat = arc_read(sc, ARC_RA_INTRSTAT);
	intrstat &= ARC_RA_INTRSTAT_POSTQUEUE | ARC_RA_INTRSTAT_DOORBELL;
	arc_write(sc, ARC_RA_INTRSTAT, intrstat);

	if (intrstat & ARC_RA_INTRSTAT_DOORBELL) {
		ret = 1;
		if (sc->sc_talking) {
			/* if an ioctl is talking, wake it up */
			arc_write(sc, ARC_RA_INTRMASK,
			    ~ARC_RA_INTRMASK_POSTQUEUE);
			wakeup(sc);
		} else {
			/* otherwise drop it */
			reg = arc_read(sc, ARC_RA_OUTB_DOORBELL);
			arc_write(sc, ARC_RA_OUTB_DOORBELL, reg);
			if (reg & ARC_RA_OUTB_DOORBELL_WRITE_OK)
				arc_write(sc, ARC_RA_INB_DOORBELL,
				    ARC_RA_INB_DOORBELL_READ_OK);
		}
	}

	while ((reg = arc_pop(sc)) != 0xffffffff) {
		ret = 1;
		cmd = (struct arc_io_cmd *)(kva +
		    ((reg << ARC_RA_REPLY_QUEUE_ADDR_SHIFT) -
		    (u_int32_t)ARC_DMA_DVA(sc->sc_requests)));
		ccb = &sc->sc_ccbs[letoh32(cmd->cmd.context)];

		bus_dmamap_sync(sc->sc_dmat, ARC_DMA_MAP(sc->sc_requests),
		    ccb->ccb_offset, ARC_MAX_IOCMDLEN,
		    BUS_DMASYNC_PREREAD | BUS_DMASYNC_PREWRITE);

		arc_scsi_cmd_done(sc, ccb, reg);
	}

	return (ret);
}

void
arc_scsi_cmd(struct scsi_xfer *xs)
{
	struct scsi_link		*link = xs->sc_link;
	struct arc_softc		*sc = link->adapter_softc;
	struct arc_ccb			*ccb;
	struct arc_msg_scsicmd		*cmd;
	u_int32_t			reg;
	int				s;

	if (xs->cmdlen > ARC_MSG_CDBLEN) {
		bzero(&xs->sense, sizeof(xs->sense));
		xs->sense.error_code = SSD_ERRCODE_VALID | 0x70;
		xs->sense.flags = SKEY_ILLEGAL_REQUEST;
		xs->sense.add_sense_code = 0x20;
		xs->error = XS_SENSE;
		scsi_done(xs);
		return;
	}

	ccb = xs->io;
	ccb->ccb_xs = xs;

	if (arc_load_xs(ccb) != 0) {
		xs->error = XS_DRIVER_STUFFUP;
		scsi_done(xs);
		return;
	}

	cmd = &ccb->ccb_cmd->cmd;
	reg = ccb->ccb_cmd_post;

	/* bus is always 0 */
	cmd->target = link->target;
	cmd->lun = link->lun;
	cmd->function = 1; /* XXX magic number */

	cmd->cdb_len = xs->cmdlen;
	cmd->sgl_len = ccb->ccb_dmamap->dm_nsegs;
	if (xs->flags & SCSI_DATA_OUT)
		cmd->flags = ARC_MSG_SCSICMD_FLAG_WRITE;
	if (ccb->ccb_dmamap->dm_nsegs > ARC_SGL_256LEN) {
		cmd->flags |= ARC_MSG_SCSICMD_FLAG_SGL_BSIZE_512;
		reg |= ARC_RA_POST_QUEUE_BIGFRAME;
	}

	cmd->context = htole32(ccb->ccb_id);
	cmd->data_len = htole32(xs->datalen);

	bcopy(xs->cmd, cmd->cdb, xs->cmdlen);

	/* we've built the command, let's put it on the hw */
	bus_dmamap_sync(sc->sc_dmat, ARC_DMA_MAP(sc->sc_requests),
	    ccb->ccb_offset, ARC_MAX_IOCMDLEN,
	    BUS_DMASYNC_PREREAD | BUS_DMASYNC_PREWRITE);

	s = splbio();
	arc_push(sc, reg);
	if (xs->flags & SCSI_POLL) {
		if (arc_complete(sc, ccb, xs->timeout) != 0) {
			xs->error = XS_DRIVER_STUFFUP;
			scsi_done(xs);
		}
	}
	splx(s);
}

int
arc_load_xs(struct arc_ccb *ccb)
{
	struct arc_softc		*sc = ccb->ccb_sc;
	struct scsi_xfer		*xs = ccb->ccb_xs;
	bus_dmamap_t			dmap = ccb->ccb_dmamap;
	struct arc_sge			*sgl = ccb->ccb_cmd->sgl, *sge;
	u_int64_t			addr;
	int				i, error;

	if (xs->datalen == 0)
		return (0);

	error = bus_dmamap_load(sc->sc_dmat, dmap,
	    xs->data, xs->datalen, NULL,
	    (xs->flags & SCSI_NOSLEEP) ? BUS_DMA_NOWAIT : BUS_DMA_WAITOK);
	if (error != 0) {
		printf("%s: error %d loading dmamap\n", DEVNAME(sc), error);
		return (1);
	}

	for (i = 0; i < dmap->dm_nsegs; i++) {
		sge = &sgl[i];

		sge->sg_hdr = htole32(ARC_SGE_64BIT | dmap->dm_segs[i].ds_len);
		addr = dmap->dm_segs[i].ds_addr;
		sge->sg_hi_addr = htole32((u_int32_t)(addr >> 32));
		sge->sg_lo_addr = htole32((u_int32_t)addr);
	}

	bus_dmamap_sync(sc->sc_dmat, dmap, 0, dmap->dm_mapsize,
	    (xs->flags & SCSI_DATA_IN) ? BUS_DMASYNC_PREREAD :
	    BUS_DMASYNC_PREWRITE);

	return (0);
}

void
arc_scsi_cmd_done(struct arc_softc *sc, struct arc_ccb *ccb, u_int32_t reg)
{
	struct scsi_xfer		*xs = ccb->ccb_xs;
	struct arc_msg_scsicmd		*cmd;

	if (xs->datalen != 0) {
		bus_dmamap_sync(sc->sc_dmat, ccb->ccb_dmamap, 0,
		    ccb->ccb_dmamap->dm_mapsize, (xs->flags & SCSI_DATA_IN) ?
		    BUS_DMASYNC_POSTREAD : BUS_DMASYNC_POSTWRITE);
		bus_dmamap_unload(sc->sc_dmat, ccb->ccb_dmamap);
	}

	if (reg & ARC_RA_REPLY_QUEUE_ERR) {
		cmd = &ccb->ccb_cmd->cmd;

		switch (cmd->status) {
		case ARC_MSG_STATUS_SELTIMEOUT:
		case ARC_MSG_STATUS_ABORTED:
		case ARC_MSG_STATUS_INIT_FAIL:
			xs->status = SCSI_OK;
			xs->error = XS_SELTIMEOUT;
			break;

		case SCSI_CHECK:
			bzero(&xs->sense, sizeof(xs->sense));
			bcopy(cmd->sense_data, &xs->sense,
			    min(ARC_MSG_SENSELEN, sizeof(xs->sense)));
			xs->sense.error_code = SSD_ERRCODE_VALID | 0x70;
			xs->status = SCSI_CHECK;
			xs->error = XS_SENSE;
			xs->resid = 0;
			break;

		default:
			/* unknown device status */
			xs->error = XS_BUSY; /* try again later? */
			xs->status = SCSI_BUSY;
			break;
		}
	} else {
		xs->status = SCSI_OK;
		xs->error = XS_NOERROR;
		xs->resid = 0;
	}

	scsi_done(xs);
}

int
arc_complete(struct arc_softc *sc, struct arc_ccb *nccb, int timeout)
{
	struct arc_ccb			*ccb = NULL;
	char				*kva = ARC_DMA_KVA(sc->sc_requests);
	struct arc_io_cmd		*cmd;
	u_int32_t			reg;

	do {
		reg = arc_pop(sc);
		if (reg == 0xffffffff) {
			if (timeout-- == 0)
				return (1);

			delay(1000);
			continue;
		}

		cmd = (struct arc_io_cmd *)(kva +
		    ((reg << ARC_RA_REPLY_QUEUE_ADDR_SHIFT) -
		    ARC_DMA_DVA(sc->sc_requests)));
		ccb = &sc->sc_ccbs[letoh32(cmd->cmd.context)];

		bus_dmamap_sync(sc->sc_dmat, ARC_DMA_MAP(sc->sc_requests),
		    ccb->ccb_offset, ARC_MAX_IOCMDLEN,
		    BUS_DMASYNC_PREREAD | BUS_DMASYNC_PREWRITE);

		arc_scsi_cmd_done(sc, ccb, reg);
	} while (nccb != ccb);

	return (0);
}

void
arc_minphys(struct buf *bp, struct scsi_link *sl)
{
	if (bp->b_bcount > MAXPHYS)
		bp->b_bcount = MAXPHYS;
	minphys(bp);
}

int
arc_map_pci_resources(struct arc_softc *sc, struct pci_attach_args *pa)
{
	pcireg_t			memtype;
	pci_intr_handle_t		ih;

	sc->sc_pc = pa->pa_pc;
	sc->sc_tag = pa->pa_tag;
	sc->sc_dmat = pa->pa_dmat;

	memtype = pci_mapreg_type(sc->sc_pc, sc->sc_tag, ARC_RA_PCI_BAR);
	if (pci_mapreg_map(pa, ARC_RA_PCI_BAR, memtype, 0, &sc->sc_iot,
	    &sc->sc_ioh, NULL, &sc->sc_ios, 0) != 0) {
		printf(": unable to map system interface register\n");
		return(1);
	}

	if (pci_intr_map(pa, &ih) != 0) {
		printf(": unable to map interrupt\n");
		goto unmap;
	}
	sc->sc_ih = pci_intr_establish(pa->pa_pc, ih, IPL_BIO,
	    arc_intr, sc, DEVNAME(sc));
	if (sc->sc_ih == NULL) {
		printf(": unable to map interrupt\n");
		goto unmap;
	}
	printf(": %s\n", pci_intr_string(pa->pa_pc, ih));

	return (0);

unmap:
	bus_space_unmap(sc->sc_iot, sc->sc_ioh, sc->sc_ios);
	sc->sc_ios = 0;
	return (1);
}

void
arc_unmap_pci_resources(struct arc_softc *sc)
{
	pci_intr_disestablish(sc->sc_pc, sc->sc_ih);
	bus_space_unmap(sc->sc_iot, sc->sc_ioh, sc->sc_ios);
	sc->sc_ios = 0;
}

int
arc_intel_query_firmware(struct arc_softc *sc)
{
	struct arc_msg_firmware_info	fwinfo;
	char				string[81]; /* sizeof(vendor)*2+1 */

	if (arc_wait_eq(sc, ARC_RA_OUTB_ADDR1, ARC_RA_OUTB_ADDR1_FIRMWARE_OK,
	    ARC_RA_OUTB_ADDR1_FIRMWARE_OK) != 0) {
		printf("%s: timeout waiting for firmware ok\n", DEVNAME(sc));
		return (1);
	}

	if (arc_msg0(sc, ARC_RA_INB_MSG0_GET_CONFIG) != 0) {
		printf("%s: timeout waiting for get config\n", DEVNAME(sc));
		return (1);
	}

	arc_read_region(sc, ARC_RA_MSGBUF, &fwinfo, sizeof(fwinfo));

	DNPRINTF(ARC_D_INIT, "%s: signature: 0x%08x\n", DEVNAME(sc),
	    letoh32(fwinfo.signature));

	if (letoh32(fwinfo.signature) != ARC_FWINFO_SIGNATURE_GET_CONFIG) {
		printf("%s: invalid firmware info from iop\n", DEVNAME(sc));
		return (1);
	}

	DNPRINTF(ARC_D_INIT, "%s: request_len: %d\n", DEVNAME(sc),
	    letoh32(fwinfo.request_len));
	DNPRINTF(ARC_D_INIT, "%s: queue_len: %d\n", DEVNAME(sc),
	    letoh32(fwinfo.queue_len));
	DNPRINTF(ARC_D_INIT, "%s: sdram_size: %d\n", DEVNAME(sc),
	    letoh32(fwinfo.sdram_size));
	DNPRINTF(ARC_D_INIT, "%s: sata_ports: %d\n", DEVNAME(sc),
	    letoh32(fwinfo.sata_ports), letoh32(fwinfo.sata_ports));

#ifdef ARC_DEBUG
	scsi_strvis(string, fwinfo.vendor, sizeof(fwinfo.vendor));
	DNPRINTF(ARC_D_INIT, "%s: vendor: \"%s\"\n", DEVNAME(sc), string);
	scsi_strvis(string, fwinfo.model, sizeof(fwinfo.model));
	DNPRINTF(ARC_D_INIT, "%s: model: \"%s\"\n", DEVNAME(sc), string);
#endif /* ARC_DEBUG */

	scsi_strvis(string, fwinfo.fw_version, sizeof(fwinfo.fw_version));
	DNPRINTF(ARC_D_INIT, "%s: model: \"%s\"\n", DEVNAME(sc), string);

	if (letoh32(fwinfo.request_len) != ARC_MAX_IOCMDLEN) {
		printf("%s: unexpected request frame size (%d != %d)\n",
		    DEVNAME(sc), letoh32(fwinfo.request_len), ARC_MAX_IOCMDLEN);
		return (1);
	}

	sc->sc_req_count = letoh32(fwinfo.queue_len);

	if (arc_msg0(sc, ARC_RA_INB_MSG0_START_BGRB) != 0) {
		printf("%s: timeout waiting to start bg rebuild\n",
		    DEVNAME(sc));
		return (1);
	}

	printf("%s: %d ports, %dMB SDRAM, firmware %s\n",
	    DEVNAME(sc), letoh32(fwinfo.sata_ports),
	    letoh32(fwinfo.sdram_size), string);

	return (0);
}

int
arc_marvell_query_firmware(struct arc_softc *sc)
{
	if (arc_wait_eq(sc, ARC_RB_IOP2DRV_DOORBELL,
	    ARC_RA_OUTB_ADDR1_FIRMWARE_OK,
	    ARC_RA_OUTB_ADDR1_FIRMWARE_OK) != 0) {
		printf("%s: timeout waiting for firmware ok\n", DEVNAME(sc));
		return (1);
	}

	return (1);
}

#if NBIO > 0
int
arc_bioctl(struct device *self, u_long cmd, caddr_t addr)
{
	struct arc_softc		*sc = (struct arc_softc *)self;
	int				error = 0;

	switch (cmd) {
	case BIOCINQ:
		error = arc_bio_inq(sc, (struct bioc_inq *)addr);
		break;

	case BIOCVOL:
		error = arc_bio_vol(sc, (struct bioc_vol *)addr);
		break;

	case BIOCDISK:
		error = arc_bio_disk(sc, (struct bioc_disk *)addr);
		break;

	case BIOCALARM:
		error = arc_bio_alarm(sc, (struct bioc_alarm *)addr);
		break;

	case BIOCBLINK:
		error = arc_bio_blink(sc, (struct bioc_blink *)addr);
		break;

	default:
		error = ENOTTY;
		break;
	}

	return (error);
}

int
arc_bio_alarm(struct arc_softc *sc, struct bioc_alarm *ba)
{
	u_int8_t			request[2];
	u_int8_t			reply[1];
	size_t				len;
	int				error = 0;

	switch (ba->ba_opcode) {
	case BIOC_SAENABLE:
	case BIOC_SADISABLE:
		request[0] = ARC_FW_SET_ALARM;
		request[1] = (ba->ba_opcode == BIOC_SAENABLE) ?
		    ARC_FW_SET_ALARM_ENABLE : ARC_FW_SET_ALARM_DISABLE;
		len = sizeof(request);

		break;

	case BIOC_SASILENCE:
		request[0] = ARC_FW_MUTE_ALARM;
		len = 1;

		break;

	case BIOC_GASTATUS:
		/* system info is too big/ugly to deal with here */
		return (arc_bio_alarm_state(sc, ba));

	default:
		return (EOPNOTSUPP);
	}

	arc_lock(sc);
	error = arc_msgbuf(sc, request, len, reply, sizeof(reply), 0);
	arc_unlock(sc);

	if (error != 0)
		return (error);

	switch (reply[0]) {
	case ARC_FW_CMD_OK:
		return (0);
	case ARC_FW_CMD_PASS_REQD:
		return (EPERM);
	default:
		return (EIO);
	}
}

int
arc_bio_alarm_state(struct arc_softc *sc, struct bioc_alarm *ba)
{
	u_int8_t			request = ARC_FW_SYSINFO;
	struct arc_fw_sysinfo		*sysinfo;
	int				error = 0;

	sysinfo = malloc(sizeof(struct arc_fw_sysinfo), M_TEMP, M_WAITOK);

	request = ARC_FW_SYSINFO;

	arc_lock(sc);
	error = arc_msgbuf(sc, &request, sizeof(request),
	    sysinfo, sizeof(struct arc_fw_sysinfo), 0);
	arc_unlock(sc);

	if (error != 0)
		goto out;

	ba->ba_status = sysinfo->alarm;

out:
	free(sysinfo, M_TEMP);
	return (error);
}


int
arc_bio_inq(struct arc_softc *sc, struct bioc_inq *bi)
{
	u_int8_t			request[2];
	struct arc_fw_sysinfo		*sysinfo;
	struct arc_fw_volinfo		*volinfo;
	int				maxvols, nvols = 0, i;
	int				error = 0;

	sysinfo = malloc(sizeof(struct arc_fw_sysinfo), M_TEMP, M_WAITOK);
	volinfo = malloc(sizeof(struct arc_fw_volinfo), M_TEMP, M_WAITOK);

	arc_lock(sc);

	request[0] = ARC_FW_SYSINFO;
	error = arc_msgbuf(sc, request, 1, sysinfo,
	    sizeof(struct arc_fw_sysinfo), 0);
	if (error != 0)
		goto out;

	maxvols = sysinfo->max_volume_set;

	request[0] = ARC_FW_VOLINFO;
	for (i = 0; i < maxvols; i++) {
		request[1] = i;
		error = arc_msgbuf(sc, request, sizeof(request), volinfo,
		    sizeof(struct arc_fw_volinfo), 0);
		if (error != 0)
			goto out;

		/*
		 * I can't find an easy way to see if the volume exists or not
		 * except to say that if it has no capacity then it isn't there.
		 * Ignore passthru volumes, bioc_vol doesn't understand them.
		 */
		if ((volinfo->capacity != 0 || volinfo->capacity2 != 0) &&
		    volinfo->raid_level != ARC_FW_VOL_RAIDLEVEL_PASSTHRU)
			nvols++;
	}

	strlcpy(bi->bi_dev, DEVNAME(sc), sizeof(bi->bi_dev));
	bi->bi_novol = nvols;
out:
	arc_unlock(sc);
	free(volinfo, M_TEMP);
	free(sysinfo, M_TEMP);
	return (error);
}

int
arc_bio_blink(struct arc_softc *sc, struct bioc_blink *blink)
{
	u_int8_t			 request[5];
	u_int32_t			 mask;
	int				 error = 0;

	request[0] = ARC_FW_BLINK;
	request[1] = ARC_FW_BLINK_ENABLE;

	switch (blink->bb_status) {
	case BIOC_SBUNBLINK:
		sc->sc_ledmask &= ~(1 << blink->bb_target);
		break;
	case BIOC_SBBLINK:
		sc->sc_ledmask |= (1 << blink->bb_target);
		break;
	default:
		return (EINVAL);
	}

	mask = htole32(sc->sc_ledmask);
	bcopy(&mask, &request[2], 3);

	error = arc_msgbuf(sc, request, sizeof(request), NULL, 0, 0);
	if (error)
		return (EIO);

	return (0);
}

int
arc_bio_getvol(struct arc_softc *sc, int vol, struct arc_fw_volinfo *volinfo)
{
	u_int8_t			request[2];
	struct arc_fw_sysinfo		*sysinfo;
	int				error = 0;
	int				maxvols, nvols = 0, i;

	sysinfo = malloc(sizeof(struct arc_fw_sysinfo), M_TEMP, M_WAITOK);

	request[0] = ARC_FW_SYSINFO;
	error = arc_msgbuf(sc, request, 1, sysinfo,
	    sizeof(struct arc_fw_sysinfo), 0);
	if (error != 0)
		goto out;

	maxvols = sysinfo->max_volume_set;

	request[0] = ARC_FW_VOLINFO;
	for (i = 0; i < maxvols; i++) {
		request[1] = i;
		error = arc_msgbuf(sc, request, sizeof(request), volinfo,
		    sizeof(struct arc_fw_volinfo), 0);
		if (error != 0)
			goto out;

		if ((volinfo->capacity == 0 && volinfo->capacity2 == 0) ||
		    volinfo->raid_level == ARC_FW_VOL_RAIDLEVEL_PASSTHRU)
			continue;

		if (nvols == vol)
			break;

		nvols++;
	}

	if (nvols != vol ||
	    (volinfo->capacity == 0 && volinfo->capacity2 == 0) ||
	    volinfo->raid_level == ARC_FW_VOL_RAIDLEVEL_PASSTHRU) {
		error = ENODEV;
		goto out;
	}

out:
	free(sysinfo, M_TEMP);
	return (error);
}

int
arc_bio_vol(struct arc_softc *sc, struct bioc_vol *bv)
{
	struct arc_fw_volinfo		*volinfo;
	struct scsi_link		*sc_link;
	struct device			*dev;
	u_int64_t			blocks;
	u_int32_t			status;
	int				error = 0;

	volinfo = malloc(sizeof(struct arc_fw_volinfo), M_TEMP, M_WAITOK);

	arc_lock(sc);
	error = arc_bio_getvol(sc, bv->bv_volid, volinfo);
	arc_unlock(sc);

	if (error != 0)
		goto out;

	bv->bv_percent = -1;
	bv->bv_seconds = 0;

	status = letoh32(volinfo->volume_status);
	if (status == 0x0) {
		if (letoh32(volinfo->fail_mask) == 0x0)
			bv->bv_status = BIOC_SVONLINE;
		else
			bv->bv_status = BIOC_SVDEGRADED;
	} else if (status & ARC_FW_VOL_STATUS_NEED_REGEN)
		bv->bv_status = BIOC_SVDEGRADED;
	else if (status & ARC_FW_VOL_STATUS_FAILED)
		bv->bv_status = BIOC_SVOFFLINE;
	else if (status & ARC_FW_VOL_STATUS_INITTING) {
		bv->bv_status = BIOC_SVBUILDING;
		bv->bv_percent = letoh32(volinfo->progress) / 10;
	} else if (status & ARC_FW_VOL_STATUS_REBUILDING) {
		bv->bv_status = BIOC_SVREBUILD;
		bv->bv_percent = letoh32(volinfo->progress) / 10;
	}

	blocks = (u_int64_t)letoh32(volinfo->capacity2) << 32;
	blocks += (u_int64_t)letoh32(volinfo->capacity);
	bv->bv_size = blocks * ARC_BLOCKSIZE; /* XXX */

	switch (volinfo->raid_level) {
	case ARC_FW_VOL_RAIDLEVEL_0:
		bv->bv_level = 0;
		break;
	case ARC_FW_VOL_RAIDLEVEL_1:
		bv->bv_level = 1;
		break;
	case ARC_FW_VOL_RAIDLEVEL_3:
		bv->bv_level = 3;
		break;
	case ARC_FW_VOL_RAIDLEVEL_5:
		bv->bv_level = 5;
		break;
	case ARC_FW_VOL_RAIDLEVEL_6:
		bv->bv_level = 6;
		break;
	case ARC_FW_VOL_RAIDLEVEL_PASSTHRU:
	default:
		bv->bv_level = -1;
		break;
	}

	bv->bv_nodisk = volinfo->member_disks;
	sc_link = scsi_get_link(sc->sc_scsibus, volinfo->scsi_attr.target,
	    volinfo->scsi_attr.lun);
	if (sc_link != NULL) {
		dev = sc_link->device_softc;
		strlcpy(bv->bv_dev, dev->dv_xname, sizeof(bv->bv_dev));
	}

out:
	free(volinfo, M_TEMP);
	return (error);
}

int
arc_bio_disk(struct arc_softc *sc, struct bioc_disk *bd)
{
	u_int8_t			request[2];
	struct arc_fw_volinfo		*volinfo;
	struct arc_fw_raidinfo		*raidinfo;
	struct arc_fw_diskinfo		*diskinfo;
	int				error = 0;
	u_int64_t			blocks;
	char				model[81];
	char				serial[41];
	char				rev[17];

	volinfo = malloc(sizeof(struct arc_fw_volinfo), M_TEMP, M_WAITOK);
	raidinfo = malloc(sizeof(struct arc_fw_raidinfo), M_TEMP, M_WAITOK);
	diskinfo = malloc(sizeof(struct arc_fw_diskinfo), M_TEMP, M_WAITOK);

	arc_lock(sc);

	error = arc_bio_getvol(sc, bd->bd_volid, volinfo);
	if (error != 0)
		goto out;

	request[0] = ARC_FW_RAIDINFO;
	request[1] = volinfo->raid_set_number;
	error = arc_msgbuf(sc, request, sizeof(request), raidinfo,
	    sizeof(struct arc_fw_raidinfo), 0);
	if (error != 0)
		goto out;

	if (bd->bd_diskid > raidinfo->member_devices) {
		error = ENODEV;
		goto out;
	}

	if (raidinfo->device_array[bd->bd_diskid] == 0xff) {
		/*
		 * the disk doesn't exist anymore. bio is too dumb to be
		 * able to display that, so put it on another bus
		 */
		bd->bd_channel = 1;
		bd->bd_target = 0;
		bd->bd_lun = 0;
		bd->bd_status = BIOC_SDOFFLINE;
		strlcpy(bd->bd_vendor, "disk missing", sizeof(bd->bd_vendor));
		goto out;
	}

	request[0] = ARC_FW_DISKINFO;
	request[1] = raidinfo->device_array[bd->bd_diskid];
	error = arc_msgbuf(sc, request, sizeof(request), diskinfo,
	    sizeof(struct arc_fw_diskinfo), 1);
	if (error != 0)
		goto out;

#if 0
	bd->bd_channel = diskinfo->scsi_attr.channel;
	bd->bd_target = diskinfo->scsi_attr.target;
	bd->bd_lun = diskinfo->scsi_attr.lun;
#endif
	/*
	 * the firwmare doesnt seem to fill scsi_attr in, so fake it with
	 * the diskid.
	 */
	bd->bd_channel = 0;
	bd->bd_target = raidinfo->device_array[bd->bd_diskid];
	bd->bd_lun = 0;

	bd->bd_status = BIOC_SDONLINE;
	blocks = (u_int64_t)letoh32(diskinfo->capacity2) << 32;
	blocks += (u_int64_t)letoh32(diskinfo->capacity);
	bd->bd_size = blocks * ARC_BLOCKSIZE; /* XXX */

	scsi_strvis(model, diskinfo->model, sizeof(diskinfo->model));
	scsi_strvis(serial, diskinfo->serial, sizeof(diskinfo->serial));
	scsi_strvis(rev, diskinfo->firmware_rev,
	    sizeof(diskinfo->firmware_rev));

	snprintf(bd->bd_vendor, sizeof(bd->bd_vendor), "%s %s",
	    model, rev);
	strlcpy(bd->bd_serial, serial, sizeof(bd->bd_serial));

out:
	arc_unlock(sc);
	free(diskinfo, M_TEMP);
	free(raidinfo, M_TEMP);
	free(volinfo, M_TEMP);
	return (error);
}

u_int8_t
arc_msg_cksum(void *cmd, u_int16_t len)
{
	u_int8_t			*buf = cmd;
	u_int8_t			cksum;
	int				i;

	cksum = (u_int8_t)(len >> 8) + (u_int8_t)len;
	for (i = 0; i < len; i++)
		cksum += buf[i];

	return (cksum);
}


int
arc_msgbuf(struct arc_softc *sc, void *wptr, size_t wbuflen, void *rptr,
    size_t rbuflen, int sreadok)
{
	u_int8_t			rwbuf[ARC_RA_IOC_RWBUF_MAXLEN];
	u_int8_t			*wbuf, *rbuf;
	int				wlen, wdone = 0, rlen, rdone = 0;
	u_int16_t			rlenhdr = 0;
	struct arc_fw_bufhdr		*bufhdr;
	u_int32_t			reg, rwlen;
	int				error = 0;
#ifdef ARC_DEBUG
	int				i;
#endif

	DNPRINTF(ARC_D_DB, "%s: arc_msgbuf wbuflen: %d rbuflen: %d\n",
	    DEVNAME(sc), wbuflen, rbuflen);

	if (arc_read(sc, ARC_RA_OUTB_DOORBELL) != 0)
		return (EBUSY);

	wlen = sizeof(struct arc_fw_bufhdr) + wbuflen + 1; /* 1 for cksum */
	wbuf = malloc(wlen, M_TEMP, M_WAITOK);

	rlen = sizeof(struct arc_fw_bufhdr) + rbuflen + 1; /* 1 for cksum */
	rbuf = malloc(rlen, M_TEMP, M_WAITOK);

	DNPRINTF(ARC_D_DB, "%s: arc_msgbuf wlen: %d rlen: %d\n", DEVNAME(sc),
	    wlen, rlen);

	bufhdr = (struct arc_fw_bufhdr *)wbuf;
	bufhdr->hdr = arc_fw_hdr;
	bufhdr->len = htole16(wbuflen);
	bcopy(wptr, wbuf + sizeof(struct arc_fw_bufhdr), wbuflen);
	wbuf[wlen - 1] = arc_msg_cksum(wptr, wbuflen);

	reg = ARC_RA_OUTB_DOORBELL_READ_OK;

	do {
		if ((reg & ARC_RA_OUTB_DOORBELL_READ_OK) && wdone < wlen) {
			bzero(rwbuf, sizeof(rwbuf));
			rwlen = (wlen - wdone) % sizeof(rwbuf);
			bcopy(&wbuf[wdone], rwbuf, rwlen);

#ifdef ARC_DEBUG
			if (arcdebug & ARC_D_DB) {
				printf("%s: write %d:", DEVNAME(sc), rwlen);
				for (i = 0; i < rwlen; i++)
					printf(" 0x%02x", rwbuf[i]);
				printf("\n");
			}
#endif

			/* copy the chunk to the hw */
			arc_write(sc, ARC_RA_IOC_WBUF_LEN, rwlen);
			arc_write_region(sc, ARC_RA_IOC_WBUF, rwbuf,
			    sizeof(rwbuf));

			/* say we have a buffer for the hw */
			arc_write(sc, ARC_RA_INB_DOORBELL,
			    ARC_RA_INB_DOORBELL_WRITE_OK);

			wdone += rwlen;
		}

		if (rptr == NULL)
			goto out;

		while ((reg = arc_read(sc, ARC_RA_OUTB_DOORBELL)) == 0)
			arc_wait(sc);
		arc_write(sc, ARC_RA_OUTB_DOORBELL, reg);

		DNPRINTF(ARC_D_DB, "%s: reg: 0x%08x\n", DEVNAME(sc), reg);

		if ((reg & ARC_RA_OUTB_DOORBELL_WRITE_OK) && rdone < rlen) {
			rwlen = arc_read(sc, ARC_RA_IOC_RBUF_LEN);
			if (rwlen > sizeof(rwbuf)) {
				DNPRINTF(ARC_D_DB, "%s:  rwlen too big\n",
				    DEVNAME(sc));
				error = EIO;
				goto out;
			}

			arc_read_region(sc, ARC_RA_IOC_RBUF, rwbuf,
			    sizeof(rwbuf));

			arc_write(sc, ARC_RA_INB_DOORBELL,
			    ARC_RA_INB_DOORBELL_READ_OK);

#ifdef ARC_DEBUG
			printf("%s:  len: %d+%d=%d/%d\n", DEVNAME(sc),
			    rwlen, rdone, rwlen + rdone, rlen);
			if (arcdebug & ARC_D_DB) {
				printf("%s: read:", DEVNAME(sc));
				for (i = 0; i < rwlen; i++)
					printf(" 0x%02x", rwbuf[i]);
				printf("\n");
			}
#endif

			if ((rdone + rwlen) > rlen) {
				DNPRINTF(ARC_D_DB, "%s:  rwbuf too big\n",
				    DEVNAME(sc));
				error = EIO;
				goto out;
			}

			bcopy(rwbuf, &rbuf[rdone], rwlen);
			rdone += rwlen;

			/*
			 * Allow for short reads, by reading the length
			 * value from the response header and shrinking our
			 * idea of size, if required.
			 * This deals with the growth of diskinfo struct from
			 * 128 to 132 bytes.
			 */ 
			if (sreadok && rdone >= sizeof(struct arc_fw_bufhdr) &&
			    rlenhdr == 0) {
				bufhdr = (struct arc_fw_bufhdr *)rbuf;
				rlenhdr = letoh16(bufhdr->len);
				if (rlenhdr < rbuflen) {
					rbuflen = rlenhdr;
					rlen = sizeof(struct arc_fw_bufhdr) +
					    rbuflen + 1; /* 1 for cksum */
				}
			}
		}
	} while (rdone != rlen);

	bufhdr = (struct arc_fw_bufhdr *)rbuf;
	if (memcmp(&bufhdr->hdr, &arc_fw_hdr, sizeof(bufhdr->hdr)) != 0 ||
	    bufhdr->len != htole16(rbuflen)) {
		DNPRINTF(ARC_D_DB, "%s:  rbuf hdr is wrong\n", DEVNAME(sc));
		error = EIO;
		goto out;
	}

	bcopy(rbuf + sizeof(struct arc_fw_bufhdr), rptr, rbuflen);

	if (rbuf[rlen - 1] != arc_msg_cksum(rptr, rbuflen)) {
		DNPRINTF(ARC_D_DB, "%s:  invalid cksum\n", DEVNAME(sc));
		error = EIO;
		goto out;
	}

out:
	free(wbuf, M_TEMP);
	free(rbuf, M_TEMP);

	return (error);
}

void
arc_lock(struct arc_softc *sc)
{
	int				s;

	rw_enter_write(&sc->sc_lock);
	s = splbio();
	arc_write(sc, ARC_RA_INTRMASK, ~ARC_RA_INTRMASK_POSTQUEUE);
	sc->sc_talking = 1;
	splx(s);
}

void
arc_unlock(struct arc_softc *sc)
{
	int				s;

	s = splbio();
	sc->sc_talking = 0;
	arc_write(sc, ARC_RA_INTRMASK,
	    ~(ARC_RA_INTRMASK_POSTQUEUE|ARC_RA_INTRMASK_DOORBELL));
	splx(s);
	rw_exit_write(&sc->sc_lock);
}

void
arc_wait(struct arc_softc *sc)
{
	int				s;

	s = splbio();
	arc_write(sc, ARC_RA_INTRMASK,
	    ~(ARC_RA_INTRMASK_POSTQUEUE|ARC_RA_INTRMASK_DOORBELL));
	if (tsleep(sc, PWAIT, "arcdb", hz) == EWOULDBLOCK)
		arc_write(sc, ARC_RA_INTRMASK, ~ARC_RA_INTRMASK_POSTQUEUE);
	splx(s);
}

#ifndef SMALL_KERNEL
void
arc_create_sensors(void *xsc, void *arg)
{
	struct arc_softc	*sc = xsc;
	struct bioc_inq		bi;
	struct bioc_vol		bv;
	int			i;

	/*
	 * XXX * this is bollocks. the firmware has garbage coming out of it
	 * so we have to wait a bit for it to finish spewing.
	 */
	tsleep(sc, PWAIT, "arcspew", 2 * hz);

	bzero(&bi, sizeof(bi));
	if (arc_bio_inq(sc, &bi) != 0) {
		printf("%s: unable to query firmware for sensor info\n",
		    DEVNAME(sc));
		return;
	}
	sc->sc_nsensors = bi.bi_novol;

	sc->sc_sensors = malloc(sizeof(struct ksensor) * sc->sc_nsensors,
	    M_DEVBUF, M_WAITOK | M_ZERO);

	strlcpy(sc->sc_sensordev.xname, DEVNAME(sc),
	    sizeof(sc->sc_sensordev.xname));

	for (i = 0; i < sc->sc_nsensors; i++) {
		bzero(&bv, sizeof(bv));
		bv.bv_volid = i;
		if (arc_bio_vol(sc, &bv) != 0)
			goto bad;

		sc->sc_sensors[i].type = SENSOR_DRIVE;
		sc->sc_sensors[i].status = SENSOR_S_UNKNOWN;

		strlcpy(sc->sc_sensors[i].desc, bv.bv_dev,
		    sizeof(sc->sc_sensors[i].desc));

		sensor_attach(&sc->sc_sensordev, &sc->sc_sensors[i]);
	}

	if (sensor_task_register(sc, arc_refresh_sensors, 120) == NULL)
		goto bad;

	sensordev_install(&sc->sc_sensordev);

	return;

bad:
	free(sc->sc_sensors, M_DEVBUF);
}

void
arc_refresh_sensors(void *arg)
{
	struct arc_softc	*sc = arg;
	struct bioc_vol		bv;
	int			i;

	for (i = 0; i < sc->sc_nsensors; i++) {
		bzero(&bv, sizeof(bv));
		bv.bv_volid = i;
		if (arc_bio_vol(sc, &bv)) {
			sc->sc_sensors[i].flags = SENSOR_FINVALID;
			return;
		}

		switch(bv.bv_status) {
		case BIOC_SVOFFLINE:
			sc->sc_sensors[i].value = SENSOR_DRIVE_FAIL;
			sc->sc_sensors[i].status = SENSOR_S_CRIT;
			break;

		case BIOC_SVDEGRADED:
			sc->sc_sensors[i].value = SENSOR_DRIVE_PFAIL;
			sc->sc_sensors[i].status = SENSOR_S_WARN;
			break;

		case BIOC_SVSCRUB:
		case BIOC_SVONLINE:
			sc->sc_sensors[i].value = SENSOR_DRIVE_ONLINE;
			sc->sc_sensors[i].status = SENSOR_S_OK;
			break;

		case BIOC_SVINVALID:
			/* FALLTRHOUGH */
		default:
			sc->sc_sensors[i].value = 0; /* unknown */
			sc->sc_sensors[i].status = SENSOR_S_UNKNOWN;
		}

	}
}
#endif /* SMALL_KERNEL */
#endif /* NBIO > 0 */

u_int32_t
arc_read(struct arc_softc *sc, bus_size_t r)
{
	u_int32_t			v;

	bus_space_barrier(sc->sc_iot, sc->sc_ioh, r, 4,
	    BUS_SPACE_BARRIER_READ);
	v = bus_space_read_4(sc->sc_iot, sc->sc_ioh, r);

	DNPRINTF(ARC_D_RW, "%s: arc_read 0x%x 0x%08x\n", DEVNAME(sc), r, v);

	return (v);
}

void
arc_read_region(struct arc_softc *sc, bus_size_t r, void *buf, size_t len)
{
	bus_space_barrier(sc->sc_iot, sc->sc_ioh, r, len,
	    BUS_SPACE_BARRIER_READ);
	bus_space_read_raw_region_4(sc->sc_iot, sc->sc_ioh, r, buf, len);
}

void
arc_write(struct arc_softc *sc, bus_size_t r, u_int32_t v)
{
	DNPRINTF(ARC_D_RW, "%s: arc_write 0x%x 0x%08x\n", DEVNAME(sc), r, v);

	bus_space_write_4(sc->sc_iot, sc->sc_ioh, r, v);
	bus_space_barrier(sc->sc_iot, sc->sc_ioh, r, 4,
	    BUS_SPACE_BARRIER_WRITE);
}

void
arc_write_region(struct arc_softc *sc, bus_size_t r, void *buf, size_t len)
{
	bus_space_write_raw_region_4(sc->sc_iot, sc->sc_ioh, r, buf, len);
	bus_space_barrier(sc->sc_iot, sc->sc_ioh, r, len,
	    BUS_SPACE_BARRIER_WRITE);
}

int
arc_wait_eq(struct arc_softc *sc, bus_size_t r, u_int32_t mask,
    u_int32_t target)
{
	int				i;

	DNPRINTF(ARC_D_RW, "%s: arc_wait_eq 0x%x 0x%08x 0x%08x\n",
	    DEVNAME(sc), r, mask, target);

	for (i = 0; i < 10000; i++) {
		if ((arc_read(sc, r) & mask) == target)
			return (0);
		delay(1000);
	}

	return (1);
}

int
arc_wait_ne(struct arc_softc *sc, bus_size_t r, u_int32_t mask,
    u_int32_t target)
{
	int				i;

	DNPRINTF(ARC_D_RW, "%s: arc_wait_ne 0x%x 0x%08x 0x%08x\n",
	    DEVNAME(sc), r, mask, target);

	for (i = 0; i < 10000; i++) {
		if ((arc_read(sc, r) & mask) != target)
			return (0);
		delay(1000);
	}

	return (1);
}

int
arc_msg0(struct arc_softc *sc, u_int32_t m)
{
	/* post message */
	arc_write(sc, ARC_RA_INB_MSG0, m);
	/* wait for the fw to do it */
	if (arc_wait_eq(sc, ARC_RA_INTRSTAT, ARC_RA_INTRSTAT_MSG0,
	    ARC_RA_INTRSTAT_MSG0) != 0)
		return (1);

	/* ack it */
	arc_write(sc, ARC_RA_INTRSTAT, ARC_RA_INTRSTAT_MSG0);

	return (0);
}

struct arc_dmamem *
arc_dmamem_alloc(struct arc_softc *sc, size_t size)
{
	struct arc_dmamem		*adm;
	int				nsegs;

	adm = malloc(sizeof(*adm), M_DEVBUF, M_NOWAIT | M_ZERO);
	if (adm == NULL)
		return (NULL);

	adm->adm_size = size;

	if (bus_dmamap_create(sc->sc_dmat, size, 1, size, 0,
	    BUS_DMA_NOWAIT | BUS_DMA_ALLOCNOW, &adm->adm_map) != 0)
		goto admfree;

	if (bus_dmamem_alloc(sc->sc_dmat, size, PAGE_SIZE, 0, &adm->adm_seg,
	    1, &nsegs, BUS_DMA_NOWAIT | BUS_DMA_ZERO) != 0)
		goto destroy;

	if (bus_dmamem_map(sc->sc_dmat, &adm->adm_seg, nsegs, size,
	    &adm->adm_kva, BUS_DMA_NOWAIT) != 0)
		goto free;

	if (bus_dmamap_load(sc->sc_dmat, adm->adm_map, adm->adm_kva, size,
	    NULL, BUS_DMA_NOWAIT) != 0)
		goto unmap;

	return (adm);

unmap:
	bus_dmamem_unmap(sc->sc_dmat, adm->adm_kva, size);
free:
	bus_dmamem_free(sc->sc_dmat, &adm->adm_seg, 1);
destroy:
	bus_dmamap_destroy(sc->sc_dmat, adm->adm_map);
admfree:
	free(adm, M_DEVBUF);

	return (NULL);
}

void
arc_dmamem_free(struct arc_softc *sc, struct arc_dmamem *adm)
{
	bus_dmamap_unload(sc->sc_dmat, adm->adm_map);
	bus_dmamem_unmap(sc->sc_dmat, adm->adm_kva, adm->adm_size);
	bus_dmamem_free(sc->sc_dmat, &adm->adm_seg, 1);
	bus_dmamap_destroy(sc->sc_dmat, adm->adm_map);
	free(adm, M_DEVBUF);
}

int
arc_alloc_ccbs(struct arc_softc *sc)
{
	struct arc_ccb			*ccb;
	u_int8_t			*cmd;
	int				i;

	SLIST_INIT(&sc->sc_ccb_free);
	mtx_init(&sc->sc_ccb_mtx, IPL_BIO);

	sc->sc_ccbs = malloc(sizeof(struct arc_ccb) * sc->sc_req_count,
	    M_DEVBUF, M_WAITOK | M_ZERO);

	sc->sc_requests = arc_dmamem_alloc(sc,
	    ARC_MAX_IOCMDLEN * sc->sc_req_count);
	if (sc->sc_requests == NULL) {
		printf("%s: unable to allocate ccb dmamem\n", DEVNAME(sc));
		goto free_ccbs;
	}
	cmd = ARC_DMA_KVA(sc->sc_requests);

	for (i = 0; i < sc->sc_req_count; i++) {
		ccb = &sc->sc_ccbs[i];

		if (bus_dmamap_create(sc->sc_dmat, MAXPHYS, ARC_SGL_MAXLEN,
		    MAXPHYS, 0, 0, &ccb->ccb_dmamap) != 0) {
			printf("%s: unable to create dmamap for ccb %d\n",
			    DEVNAME(sc), i);
			goto free_maps;
		}

		ccb->ccb_sc = sc;
		ccb->ccb_id = i;
		ccb->ccb_offset = ARC_MAX_IOCMDLEN * i;

		ccb->ccb_cmd = (struct arc_io_cmd *)&cmd[ccb->ccb_offset];
		ccb->ccb_cmd_post = (ARC_DMA_DVA(sc->sc_requests) +
		    ccb->ccb_offset) >> ARC_RA_POST_QUEUE_ADDR_SHIFT;

		arc_put_ccb(sc, ccb);
	}

	scsi_iopool_init(&sc->sc_iopool, sc,
	    (void *(*)(void *))arc_get_ccb,
	    (void (*)(void *, void *))arc_put_ccb);

	return (0);

free_maps:
	while ((ccb = arc_get_ccb(sc)) != NULL)
	    bus_dmamap_destroy(sc->sc_dmat, ccb->ccb_dmamap);
	arc_dmamem_free(sc, sc->sc_requests);

free_ccbs:
	free(sc->sc_ccbs, M_DEVBUF);

	return (1);
}

struct arc_ccb *
arc_get_ccb(struct arc_softc *sc)
{
	struct arc_ccb			*ccb;

	mtx_enter(&sc->sc_ccb_mtx);
	ccb = SLIST_FIRST(&sc->sc_ccb_free);
	if (ccb != NULL)
		SLIST_REMOVE_HEAD(&sc->sc_ccb_free, ccb_link);
	mtx_leave(&sc->sc_ccb_mtx);

	return (ccb);
}

void
arc_put_ccb(struct arc_softc *sc, struct arc_ccb *ccb)
{
	ccb->ccb_xs = NULL;
	bzero(ccb->ccb_cmd, ARC_MAX_IOCMDLEN);
	mtx_enter(&sc->sc_ccb_mtx);
	SLIST_INSERT_HEAD(&sc->sc_ccb_free, ccb, ccb_link);
	mtx_leave(&sc->sc_ccb_mtx);
}
