/*	$NetBSD: dec_3000_500.c,v 1.1.6.2 1996/06/13 18:35:15 cgd Exp $	*/

/*
 * Copyright (c) 1994, 1995, 1996 Carnegie-Mellon University.
 * All rights reserved.
 *
 * Author: Chris G. Demetriou
 * 
 * Permission to use, copy, modify and distribute this software and
 * its documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS" 
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND 
 * FOR ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 *
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 *
 * any improvements or extensions that they make and grant Carnegie the
 * rights to redistribute these changes.
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/device.h>
#include <machine/rpb.h>

#include <machine/autoconf.h>

#include <dev/tc/tcvar.h>

#include <alpha/tc/tcdsvar.h>

#include <scsi/scsi_all.h>
#include <scsi/scsiconf.h>

char *
dec_3000_500_modelname()
{

	switch (hwrpb->rpb_variation & SV_ST_MASK) {
	case SV_ST_SANDPIPER:
systype_sandpiper:
		return "DEC 3000/400 (\"Sandpiper\")";

	case SV_ST_FLAMINGO:
systype_flamingo:
		return "DEC 3000/500 (\"Flamingo\")";

	case SV_ST_HOTPINK:
		return "DEC 3000/500X (\"Hot Pink\")";

	case SV_ST_FLAMINGOPLUS:
	case SV_ST_ULTRA:
		return "DEC 3000/800 (\"Flamingo+\")";

	case SV_ST_SANDPLUS:
		return "DEC 3000/600 (\"Sandpiper+\")";

	case SV_ST_SANDPIPER45:
		return "DEC 3000/700 (\"Sandpiper45\")";

	case SV_ST_FLAMINGO45:
		return "DEC 3000/900 (\"Flamingo45\")";

	case SV_ST_RESERVED: /* this is how things used to be done */
		if (hwrpb->rpb_variation & SV_GRAPHICS)
			goto systype_flamingo;
		else
			goto systype_sandpiper;

	default:
		printf("unknown system variation %lx\n",
		    hwrpb->rpb_variation & SV_ST_MASK);
		return NULL;
	}
}

void
dec_3000_500_consinit()
{

}

void
dec_3000_500_device_register(dev, aux)
	struct device *dev;
	void *aux;
{
	static int found;
	static struct device *scsidev;
	struct bootdev_data *b = bootdev_data;
	struct device *parent = dev->dv_parent;
	struct cfdata *cf = dev->dv_cfdata;
	struct cfdriver *cd = cf->cf_driver;

	if (found)
		return;

	if (strcmp(cd->cd_name, "esp") == 0) {
		if (b->slot == 6 &&
		    strcmp(parent->dv_cfdata->cf_driver->cd_name, "tcds")
		      == 0) {
			struct tcdsdev_attach_args *tcdsdev = aux;

			if (tcdsdev->tcdsda_slot == b->channel) {
				scsidev = dev;
#if 0
				printf("\nscsidev = %s\n", dev->dv_xname);
#endif
			}
		}
	} else if (strcmp(cd->cd_name, "sd") == 0 ||
	    strcmp(cd->cd_name, "st") == 0 ||
	    strcmp(cd->cd_name, "cd") == 0) {
		struct scsibus_attach_args *sa = aux;

		if (scsidev == NULL)
			return;

		if (parent->dv_parent != scsidev)
			return;

		if (b->unit / 100 != sa->sa_sc_link->target)
			return;

		/* XXX LUN! */

		switch (b->boot_dev_type) {
		case 0:
			if (strcmp(cd->cd_name, "sd") &&
			    strcmp(cd->cd_name, "cd"))
				return;
			break;
		case 1:
			if (strcmp(cd->cd_name, "st"))
				return;
			break;
		default:
			return;
		}

		/* we've found it! */
		booted_device = dev;
#if 0
		printf("\nbooted_device = %s\n", booted_device->dv_xname);
#endif
		found = 1;
	}
}
