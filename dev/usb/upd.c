/*	$OpenBSD: upd.c,v 1.17 2015/04/27 09:14:45 mpi Exp $ */

/*
 * Copyright (c) 2014 Andre de Oliveira <andre@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* Driver for USB Power Devices sensors */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/malloc.h>
#include <sys/device.h>
#include <sys/sensors.h>

#include <dev/usb/hid.h>
#include <dev/usb/usb.h>
#include <dev/usb/usbdi.h>
#include <dev/usb/usbdevs.h>
#include <dev/usb/usbhid.h>
#include <dev/usb/uhidev.h>
#include <dev/usb/usbdi_util.h>

#ifdef UPD_DEBUG
#define DPRINTF(x)	do { printf x; } while (0)
#else
#define DPRINTF(x)
#endif

#define DEVNAME(sc)	((sc)->sc_hdev.sc_dev.dv_xname)

struct upd_usage_entry {
	uint8_t			usage_pg;
	uint8_t			usage_id;
	enum sensor_type	senstype;
	char			*usage_name; /* sensor string */
};

static struct upd_usage_entry upd_usage_table[] = {
	{ HUP_BATTERY,	HUB_REL_STATEOF_CHARGE,
	    SENSOR_PERCENT,	 "RelativeStateOfCharge" },
	{ HUP_BATTERY,	HUB_ABS_STATEOF_CHARGE,
	    SENSOR_PERCENT,	 "AbsoluteStateOfCharge" },
	{ HUP_BATTERY,	HUB_REM_CAPACITY,
	    SENSOR_PERCENT,	 "RemainingCapacity" },
	{ HUP_BATTERY,	HUB_FULLCHARGE_CAPACITY,
	    SENSOR_PERCENT,	 "FullChargeCapacity" },
	{ HUP_BATTERY,	HUB_CHARGING,
	    SENSOR_INDICATOR,	 "Charging" },
	{ HUP_BATTERY,	HUB_DISCHARGING,
	    SENSOR_INDICATOR,	 "Discharging" },
	{ HUP_BATTERY,	HUB_BATTERY_PRESENT,
	    SENSOR_INDICATOR,	 "BatteryPresent" },
	{ HUP_POWER,	HUP_SHUTDOWN_IMMINENT,
	    SENSOR_INDICATOR,	 "ShutdownImminent" },
	{ HUP_BATTERY,	HUB_AC_PRESENT,
	    SENSOR_INDICATOR,	 "ACPresent" },
	{ HUP_BATTERY,	HUB_ATRATE_TIMETOFULL,
	    SENSOR_TIMEDELTA,	 "AtRateTimeToFull" }
};

struct upd_report {
	size_t		size;
	int		enabled;
};

struct upd_sensor {
	struct ksensor		ksensor;
	struct hid_item		hitem;
	int			attached;
};

struct upd_softc {
	struct uhidev		 sc_hdev;
	int			 sc_num_sensors;
	u_int			 sc_max_repid;

	/* sensor framework */
	struct ksensordev	 sc_sensordev;
	struct sensor_task	*sc_sensortask;
	struct upd_report	*sc_reports;
	struct upd_sensor	*sc_sensors;
};

int  upd_match(struct device *, void *, void *);
void upd_attach(struct device *, struct device *, void *);
int  upd_detach(struct device *, int);

void upd_refresh(void *);
void upd_update_sensors(struct upd_softc *, uint8_t *, unsigned int, int);
void upd_update_sensor_value(struct upd_softc *, struct upd_sensor *,
    uint8_t *, int);
void upd_intr(struct uhidev *, void *, uint);
int upd_lookup_usage_entry(void *, int, struct upd_usage_entry *,
    struct hid_item *);
struct upd_sensor *upd_lookup_sensor(struct upd_softc *, int, int);

struct cfdriver upd_cd = {
	NULL, "upd", DV_DULL
};

const struct cfattach upd_ca = {
	sizeof(struct upd_softc),
	upd_match,
	upd_attach,
	upd_detach
};

int
upd_match(struct device *parent, void *match, void *aux)
{
	struct uhidev_attach_arg *uha = (struct uhidev_attach_arg *)aux;
	int			  size;
	void			 *desc;
	struct hid_item		  item;
	int			  ret = UMATCH_NONE;
	int			  i;

	if (uha->reportid != UHIDEV_CLAIM_ALLREPORTID)
		return (ret);

	DPRINTF(("upd: vendor=0x%04x, product=0x%04x\n", uha->uaa->vendor,
	    uha->uaa->product));

	/*
	 * look for at least one sensor of our table
	 */
	uhidev_get_report_desc(uha->parent, &desc, &size);
	for (i = 0; i < nitems(upd_usage_table); i++)
		if (upd_lookup_usage_entry(desc, size,
		    upd_usage_table + i, &item)) {
			ret = UMATCH_VENDOR_PRODUCT;
			break;
		}

	return (ret);
}

void
upd_attach(struct device *parent, struct device *self, void *aux)
{
	struct upd_softc	 *sc = (struct upd_softc *)self;
	struct uhidev_attach_arg *uha = (struct uhidev_attach_arg *)aux;
	struct hid_item		  item;
	struct upd_usage_entry	 *entry;
	struct upd_sensor	 *sensor;
	int			  size;
	int			  i;
	void			 *desc;

	sc->sc_hdev.sc_intr = upd_intr;
	sc->sc_hdev.sc_parent = uha->parent;
	sc->sc_reports = NULL;
	sc->sc_sensors = NULL;

	strlcpy(sc->sc_sensordev.xname, DEVNAME(sc),
	    sizeof(sc->sc_sensordev.xname));

	sc->sc_max_repid = uha->parent->sc_nrepid;
	DPRINTF(("\nupd: devname=%s sc_max_repid=%d\n",
	    DEVNAME(sc), sc->sc_max_repid));

	sc->sc_reports = mallocarray(sc->sc_max_repid,
	    sizeof(struct upd_report), M_USBDEV, M_WAITOK | M_ZERO);
	sc->sc_sensors = mallocarray(nitems(upd_usage_table),
	    sizeof(struct upd_sensor), M_USBDEV, M_WAITOK | M_ZERO);
	sc->sc_num_sensors = 0;

	uhidev_get_report_desc(uha->parent, &desc, &size);
	for (i = 0; i < nitems(upd_usage_table); i++) {
		entry = &upd_usage_table[i];
		if (!upd_lookup_usage_entry(desc, size, entry, &item))
			continue;

		DPRINTF(("%s: found %s on repid=%d\n", DEVNAME(sc),
		    entry->usage_name, item.report_ID));
		if (item.report_ID < 0 ||
		    item.report_ID >= sc->sc_max_repid)
			continue;

		sensor = &sc->sc_sensors[sc->sc_num_sensors];
		memcpy(&sensor->hitem, &item, sizeof(struct hid_item));
		strlcpy(sensor->ksensor.desc, entry->usage_name,
		    sizeof(sensor->ksensor.desc));
		sensor->ksensor.type = entry->senstype;
		sensor->ksensor.flags |= SENSOR_FINVALID;
		sensor->ksensor.status = SENSOR_S_UNKNOWN;
		sensor->ksensor.value = 0;
		sensor_attach(&sc->sc_sensordev, &sensor->ksensor);
		sensor->attached = 1;
		sc->sc_num_sensors++;

		if (sc->sc_reports[item.report_ID].enabled)
			continue;

		sc->sc_reports[item.report_ID].size = hid_report_size(desc,
		    size, item.kind, item.report_ID);
		sc->sc_reports[item.report_ID].enabled = 1;
	}
	DPRINTF(("upd: sc_num_sensors=%d\n", sc->sc_num_sensors));

	sc->sc_sensortask = sensor_task_register(sc, upd_refresh, 6);
	if (sc->sc_sensortask == NULL) {
		printf(", unable to register update task\n");
		return;
	}
	sensordev_install(&sc->sc_sensordev);

	printf("\n");

	DPRINTF(("upd_attach: complete\n"));
}

int
upd_detach(struct device *self, int flags)
{
	struct upd_softc	*sc = (struct upd_softc *)self;
	struct upd_sensor	*sensor;
	int			 i;

	if (sc->sc_sensortask != NULL) {
		wakeup(&sc->sc_sensortask);
		sensor_task_unregister(sc->sc_sensortask);
	}

	sensordev_deinstall(&sc->sc_sensordev);

	for (i = 0; i < sc->sc_num_sensors; i++) {
		sensor = &sc->sc_sensors[i];
		if (sensor->attached)
			sensor_detach(&sc->sc_sensordev, &sensor->ksensor);
		DPRINTF(("upd_detach: %s\n", sensor->ksensor.desc));
	}

	free(sc->sc_reports, M_USBDEV, 0);
	free(sc->sc_sensors, M_USBDEV, 0);
	DPRINTF(("upd_detach: complete\n"));
	return (0);
}

void
upd_refresh(void *arg)
{
	struct upd_softc	*sc = (struct upd_softc *)arg;
	struct upd_report	*report;
	uint8_t			buf[256];
	int			repid, actlen;

	for (repid = 0; repid < sc->sc_max_repid; repid++) {
		report = &sc->sc_reports[repid];
		if (!report->enabled)
			continue;

		memset(buf, 0x0, sizeof(buf));
		actlen = uhidev_get_report(sc->sc_hdev.sc_parent,
		    UHID_FEATURE_REPORT, repid, buf, report->size);

		if (actlen == -1) {
			DPRINTF(("upd: failed to get report id=%02x\n", repid));
			continue;
		}

		/* Deal with buggy firmwares. */
		if (actlen < report->size)
			report->size = actlen;

		upd_update_sensors(sc, buf, report->size, repid);
	}
}

int
upd_lookup_usage_entry(void *desc, int size, struct upd_usage_entry *entry,
    struct hid_item *item)
{
	struct hid_data	*hdata;
	int 		 ret = 0;

	for (hdata = hid_start_parse(desc, size, hid_feature);
	     hid_get_item(hdata, item); ) {
		if (item->kind == hid_feature &&
		    entry->usage_pg == HID_GET_USAGE_PAGE(item->usage) &&
		    entry->usage_id == HID_GET_USAGE(item->usage)) {
			ret = 1;
			break;
		}
	}
	hid_end_parse(hdata);

	return (ret);
}

struct upd_sensor *
upd_lookup_sensor(struct upd_softc *sc, int page, int usage)
{
	struct upd_sensor	*sensor = NULL;
	int			 i;

	for (i = 0; i < sc->sc_num_sensors; i++) {
		sensor = &sc->sc_sensors[i];
		if (page == HID_GET_USAGE_PAGE(sensor->hitem.usage) &&
		    usage == HID_GET_USAGE(sensor->hitem.usage))
			return (sensor);
	}
	return (NULL);
}

void
upd_update_sensors(struct upd_softc *sc, uint8_t *buf, unsigned int len,
    int repid)
{
	struct upd_sensor	*sensor;
	ulong			batpres;
	int			i;

	sensor = upd_lookup_sensor(sc, HUP_BATTERY, HUB_BATTERY_PRESENT);
	batpres = sensor ? sensor->ksensor.value : -1;

	for (i = 0; i < sc->sc_num_sensors; i++) {
		sensor = &sc->sc_sensors[i];
		if (!(sensor->hitem.report_ID == repid && sensor->attached))
			continue;

		/* invalidate battery dependent sensors */
		if (HID_GET_USAGE_PAGE(sensor->hitem.usage) == HUP_BATTERY &&
		    batpres <= 0) {
			/* exception to the battery sensor itself */
			if (HID_GET_USAGE(sensor->hitem.usage) !=
			    HUB_BATTERY_PRESENT) {
				sensor->ksensor.status = SENSOR_S_UNKNOWN;
				sensor->ksensor.flags |= SENSOR_FINVALID;
				continue;
			}
		}

		upd_update_sensor_value(sc, sensor, buf, len);
	}
}

void
upd_update_sensor_value(struct upd_softc *sc, struct upd_sensor *sensor,
    uint8_t *buf, int len)
{
	int64_t	hdata, adjust;

	switch (HID_GET_USAGE(sensor->hitem.usage)) {
	case HUB_REL_STATEOF_CHARGE:
	case HUB_ABS_STATEOF_CHARGE:
	case HUB_REM_CAPACITY:
	case HUB_FULLCHARGE_CAPACITY:
		adjust = 1000; /* scale adjust */
		break;
	default:
		adjust = 1; /* no scale adjust */
		break;
	}

	hdata = hid_get_data(buf, len, &sensor->hitem.loc);
	sensor->ksensor.value = hdata * adjust;
	sensor->ksensor.status = SENSOR_S_OK;
	sensor->ksensor.flags &= ~SENSOR_FINVALID;
	DPRINTF(("%s: %s hidget data: %lld\n", DEVNAME(sc),
	    sensor->ksensor.desc, hdata));
}

void
upd_intr(struct uhidev *uh, void *p, uint len)
{
	/* noop */
}
