/*	$OpenBSD: rf_raid1.h,v 1.1 1999/01/11 14:29:42 niklas Exp $	*/
/*	$NetBSD: rf_raid1.h,v 1.1 1998/11/13 04:20:33 oster Exp $	*/
/*
 * Copyright (c) 1995 Carnegie-Mellon University.
 * All rights reserved.
 *
 * Author: William V. Courtright II
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

/* header file for RAID Level 1 */

/*
 * :  
 * Log: rf_raid1.h,v 
 * Revision 1.17  1996/07/27 23:36:08  jimz
 * Solaris port of simulator
 *
 * Revision 1.16  1996/07/13  00:00:59  jimz
 * sanitized generalized reconstruction architecture
 * cleaned up head sep, rbuf problems
 *
 * Revision 1.15  1996/07/11  19:08:00  jimz
 * generalize reconstruction mechanism
 * allow raid1 reconstructs via copyback (done with array
 * quiesced, not online, therefore not disk-directed)
 *
 * Revision 1.14  1996/06/19  22:23:01  jimz
 * parity verification is now a layout-configurable thing
 * not all layouts currently support it (correctly, anyway)
 *
 * Revision 1.13  1996/06/10  11:55:47  jimz
 * Straightened out some per-array/not-per-array distinctions, fixed
 * a couple bugs related to confusion. Added shutdown lists. Removed
 * layout shutdown function (now subsumed by shutdown lists).
 *
 * Revision 1.12  1996/06/07  22:26:27  jimz
 * type-ify which_ru (RF_ReconUnitNum_t)
 *
 * Revision 1.11  1996/06/07  21:33:04  jimz
 * begin using consistent types for sector numbers,
 * stripe numbers, row+col numbers, recon unit numbers
 *
 * Revision 1.10  1996/06/03  23:28:26  jimz
 * more bugfixes
 * check in tree to sync for IPDS runs with current bugfixes
 * there still may be a problem with threads in the script test
 * getting I/Os stuck- not trivially reproducible (runs ~50 times
 * in a row without getting stuck)
 *
 * Revision 1.9  1996/05/31  22:26:54  jimz
 * fix a lot of mapping problems, memory allocation problems
 * found some weird lock issues, fixed 'em
 * more code cleanup
 *
 * Revision 1.8  1996/05/27  18:56:37  jimz
 * more code cleanup
 * better typing
 * compiles in all 3 environments
 *
 * Revision 1.7  1996/05/24  01:59:45  jimz
 * another checkpoint in code cleanup for release
 * time to sync kernel tree
 *
 * Revision 1.6  1996/05/18  19:51:34  jimz
 * major code cleanup- fix syntax, make some types consistent,
 * add prototypes, clean out dead code, et cetera
 *
 * Revision 1.5  1996/05/03  19:35:34  wvcii
 * moved dags to dag library
 *
 * Revision 1.4  1995/11/30  16:07:26  wvcii
 * added copyright info
 *
 * Revision 1.3  1995/11/16  14:56:41  wvcii
 * updated prototypes
 *
 * Revision 1.2  1995/11/07  15:23:01  wvcii
 * changed RAID1DagSelect prototype
 * function no longer generates numHdrSucc, numTermAnt
 *
 * Revision 1.1  1995/10/04  03:52:59  wvcii
 * Initial revision
 *
 *
 */

#ifndef _RF__RF_RAID1_H_
#define _RF__RF_RAID1_H_

#include "rf_types.h"

int  rf_ConfigureRAID1(RF_ShutdownList_t **listp, RF_Raid_t *raidPtr,
	RF_Config_t *cfgPtr);
void rf_MapSectorRAID1(RF_Raid_t *raidPtr, RF_RaidAddr_t raidSector,
	RF_RowCol_t *row, RF_RowCol_t *col, RF_SectorNum_t *diskSector, int remap);
void rf_MapParityRAID1(RF_Raid_t *raidPtr, RF_RaidAddr_t raidSector,
	RF_RowCol_t *row, RF_RowCol_t *col, RF_SectorNum_t *diskSector, int remap);
void rf_IdentifyStripeRAID1(RF_Raid_t *raidPtr, RF_RaidAddr_t addr,
	RF_RowCol_t **diskids, RF_RowCol_t *outRow);
void rf_MapSIDToPSIDRAID1(RF_RaidLayout_t *layoutPtr,
	RF_StripeNum_t stripeID, RF_StripeNum_t *psID,
	RF_ReconUnitNum_t *which_ru);
void rf_RAID1DagSelect(RF_Raid_t *raidPtr, RF_IoType_t type,
	RF_AccessStripeMap_t *asmap, RF_VoidFuncPtr *createFunc);
int rf_VerifyParityRAID1(RF_Raid_t *raidPtr, RF_RaidAddr_t raidAddr,
	RF_PhysDiskAddr_t *parityPDA, int correct_it, RF_RaidAccessFlags_t flags);
int rf_SubmitReconBufferRAID1(RF_ReconBuffer_t *rbuf, int keep_int,
	int use_committed);

#endif /* !_RF__RF_RAID1_H_ */
