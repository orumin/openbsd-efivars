/*	$OpenBSD: sysreg.c,v 1.1 2015/06/08 06:33:16 jsg Exp $	*/

/*
 * Copyright (c) 2015 Jonathan Gray <jsg@openbsd.org>
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

#include <sys/param.h> 
#include <sys/systm.h> 
#include <sys/device.h> 
#include <machine/bus.h> 
#include <armv7/armv7/armv7var.h>

#define SYS_ID			0x00
#define SYS_PROCID0		0x84
#define SYS_PROCID1		0x88
#define SYS_CFGDATA		0xa0
#define SYS_CFGCTRL		0xa4
#define SYS_CFGSTAT		0xa8

#define SYS_CFG_WRITE		(1 << 30)
#define SYS_CFG_START		(1U << 31)

#define SYS_CFG_RESET		5
#define SYS_CFG_SHUTDOWN	8
#define SYS_CFG_REBOOT		9

#define SYS_CFGSTAT_COMPLETE	(1 << 0)
#define SYS_CFGSTAT_ERROR	(1 << 1)

struct sysreg_softc {
	struct device		sc_dev;
	bus_space_tag_t		sc_iot;
	bus_space_handle_t	sc_ioh;
};

struct sysreg_softc *sysreg_sc;

void sysreg_attach(struct device *, struct device *, void *);
void sysreg_reset(void);

struct cfattach sysreg_ca = {
	sizeof (struct sysreg_softc), NULL, sysreg_attach
};

struct cfdriver sysreg_cd = {
	NULL, "sysreg", DV_DULL
};

void
sysreg_attach(struct device *parent, struct device *self, void *args)
{
	struct armv7_attach_args *aa = args;
	struct sysreg_softc *sc = (struct sysreg_softc *)self;
	uint32_t id;

	sc->sc_iot = aa->aa_iot;
	if (bus_space_map(sc->sc_iot, aa->aa_dev->mem[0].addr,
	    aa->aa_dev->mem[0].size, 0, &sc->sc_ioh))
		panic(": bus_space_map failed!");
	sysreg_sc = sc;
	
	id = bus_space_read_4(sc->sc_iot, sc->sc_ioh, SYS_ID);
	printf(": ID 0x%x", id);

	id = bus_space_read_4(sc->sc_iot, sc->sc_ioh, SYS_PROCID0);
	printf(" PROCID0 0x%x\n", id);
}

void
sysconf_function(struct sysreg_softc *sc, int function)
{
	int dcc, site, position, device;

	dcc = 0;
	site = 0;
	position = 0;
	device = 0;

	bus_space_write_4(sc->sc_iot, sc->sc_ioh, SYS_CFGSTAT, 0);
	bus_space_write_4(sc->sc_iot, sc->sc_ioh, SYS_CFGCTRL,
	    SYS_CFG_START | SYS_CFG_WRITE |
	    (dcc << 26) | (function << 20) | (site << 16) |
	    (position << 12) | device);

	while ((bus_space_read_4(sc->sc_iot, sc->sc_ioh, SYS_CFGSTAT) &
	    SYS_CFGSTAT_COMPLETE) == 0);

	if (bus_space_read_4(sc->sc_iot, sc->sc_ioh, SYS_CFGSTAT) &
	    SYS_CFGSTAT_ERROR)
		printf("SYS_CFGSTAT error\n");
}

void
sysconf_reboot(void)
{
	struct sysreg_softc *sc = sysreg_sc;

	if (sc == NULL)
		return;

	sysconf_function(sc, SYS_CFG_REBOOT);
}

void
sysconf_shutdown(void)
{
	struct sysreg_softc *sc = sysreg_sc;

	if (sc == NULL)
		return;

	sysconf_function(sc, SYS_CFG_SHUTDOWN);
}
