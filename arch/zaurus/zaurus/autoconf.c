/*	$OpenBSD: autoconf.c,v 1.15 2011/09/20 22:02:13 miod Exp $	*/
/*	$NetBSD: autoconf.c,v 1.2 2001/09/05 16:17:36 matt Exp $	*/

/*
 * Copyright (c) 1994-1998 Mark Brinicombe.
 * Copyright (c) 1994 Brini.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Mark Brinicombe for
 *      the NetBSD project.
 * 4. The name of the company nor the name of the author may be used to
 *    endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * RiscBSD kernel project
 *
 * autoconf.c
 *
 * Autoconfiguration functions
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/reboot.h>
#include <sys/disklabel.h>
#include <sys/device.h>
#include <sys/conf.h>
#include <sys/kernel.h>
#include <sys/timeout.h>
#include <sys/malloc.h>

#include <uvm/uvm_extern.h>

#include <machine/bootconfig.h>
#include <machine/intr.h>
#include <dev/cons.h>

struct device *bootdv = NULL;

void dumpconf(void);

/*
 * Now that we are fully operational, we can checksum the
 * disks, and using some heuristics, hopefully are able to
 * always determine the correct root disk.
 */
void
diskconf()
{
	extern struct timeout	scoop_checkdisk;
	dev_t tmpdev;

#if 0
	/*
	 * Configure root, swap, and dump area.  This is
	 * currently done by running the same checksum
	 * algorithm over all known disks, as was done in
	 * /boot.  Then we basically fixup the *dev vars
	 * from the info we gleaned from this.
	 */
	dkcsumattach();
#endif

	/*
	 * XXX
	 * zaurus bootblocks currently pass in "bsd" instead of
	 * "device:bsd", or any such thing, making this a real pain.
	 */
	if (bootdv == NULL)
		bootdv = parsedisk("wd0a", strlen("wd0a"), 0, &tmpdev);
	if (bootdv == NULL)
		printf("boot device: lookup '%s' failed.\n", boot_file);
	else
		printf("boot device: %s\n", bootdv->dv_xname);

	setroot(bootdv, 0, RB_USERREQ);
	dumpconf();

	timeout_add(&scoop_checkdisk, hz/25);
}

void
device_register(struct device *dev, void *aux)
{
}

/*
 * void cpu_configure()
 *
 * Configure all the root devices
 * The root devices are expected to configure their own children
 */
void
cpu_configure(void)
{
	softintr_init();

	/*
	 * Since various PCI interrupts could be routed via the ICU
	 * (for PCI devices in the bridge) we need to set up the ICU
	 * now so that these interrupts can be established correctly
	 * i.e. This is a hack.
	 */

	config_rootfound("mainbus", NULL);

	/*
	 * We can not know which is our root disk, defer
	 * until we can checksum blocks to figure it out.
	 */
	cold = 0;

	/* Time to start taking interrupts so lets open the flood gates .... */
	(void)spl0();

}

struct nam2blk nam2blk[] = {
	{ "wd",		16 },
	{ "sd",		24 },
	{ "cd",		26 },
	{ "rd",		18 },
	{ "raid",	71 },
	{ "vnd",	19 },
	{ NULL,		-1 }
};
