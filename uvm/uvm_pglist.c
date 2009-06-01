/*	$OpenBSD: uvm_pglist.c,v 1.30 2009/06/01 17:42:33 ariane Exp $	*/
/*	$NetBSD: uvm_pglist.c,v 1.13 2001/02/18 21:19:08 chs Exp $	*/

/*-
 * Copyright (c) 1997 The NetBSD Foundation, Inc.
 * All rights reserved.
 *  
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe of the Numerical Aerospace Simulation Facility,
 * NASA Ames Research Center.  
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *      
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * uvm_pglist.c: pglist functions
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/proc.h>

#include <uvm/uvm.h>

#ifdef VM_PAGE_ALLOC_MEMORY_STATS
#define	STAT_INCR(v)	(v)++
#define	STAT_DECR(v)	do { \
		if ((v) == 0) \
			printf("%s:%d -- Already 0!\n", __FILE__, __LINE__); \
		else \
			(v)--; \
	} while (0)
u_long	uvm_pglistalloc_npages;
#else
#define	STAT_INCR(v)
#define	STAT_DECR(v)
#endif

/*
 * uvm_pglistalloc: allocate a list of pages
 *
 * => allocated pages are placed at the tail of rlist.  rlist is
 *    assumed to be properly initialized by caller.
 * => returns 0 on success or errno on failure
 * => XXX: implementation allocates only a single segment, also
 *	might be able to better advantage of vm_physeg[].
 * => doesn't take into account clean non-busy pages on inactive list
 *	that could be used(?)
 * => params:
 *	size		the size of the allocation, rounded to page size.
 *	low		the low address of the allowed allocation range.
 *	high		the high address of the allowed allocation range.
 *	alignment	memory must be aligned to this power-of-two boundary.
 *	boundary	no segment in the allocation may cross this 
 *			power-of-two boundary (relative to zero).
 * => flags:
 *	UVM_PLA_NOWAIT	fail if allocation fails
 *	UVM_PLA_WAITOK	wait for memory to become avail if allocation fails
 *	UVM_PLA_ZERO	return zeroed memory
 *	UVM_PLA_TRY_CONTIG device prefers p-lineair mem
 */

int
uvm_pglistalloc(psize_t size, paddr_t low, paddr_t high, paddr_t alignment,
    paddr_t boundary, struct pglist *rlist, int nsegs, int flags)
{
	UVMHIST_FUNC("uvm_pglistalloc"); UVMHIST_CALLED(pghist);

	KASSERT((alignment & (alignment - 1)) == 0);
	KASSERT((boundary & (boundary - 1)) == 0);
	KASSERT(!(flags & UVM_PLA_WAITOK) ^ !(flags & UVM_PLA_NOWAIT));

	if (size == 0)
		return (EINVAL);

	/*
	 * Convert byte addresses to page numbers.
	 */
	if (alignment < PAGE_SIZE)
		alignment = PAGE_SIZE;
	low = atop(roundup(low, alignment));
	/* Allows for overflow: 0xffff + 1 = 0x0000 */
	if ((high & PAGE_MASK) == PAGE_MASK)
		high = atop(high) + 1;
	else
		high = atop(high);
	size = atop(round_page(size));
	alignment = atop(alignment);
	if (boundary < PAGE_SIZE && boundary != 0)
		boundary = PAGE_SIZE;
	boundary = atop(boundary);

	return uvm_pmr_getpages(size, low, high, alignment, boundary, nsegs,
	    flags, rlist);
}

/*
 * uvm_pglistfree: free a list of pages
 *
 * => pages should already be unmapped
 */

void
uvm_pglistfree(struct pglist *list)
{
	struct vm_page *m;
	UVMHIST_FUNC("uvm_pglistfree"); UVMHIST_CALLED(pghist);

	TAILQ_FOREACH(m, list, pageq) {
		KASSERT((m->pg_flags & (PQ_ACTIVE|PQ_INACTIVE)) == 0);
#ifdef DEBUG
		if (m->uobject == (void *)0xdeadbeef &&
		    m->uanon == (void *)0xdeadbeef) {
			panic("uvm_pglistfree: freeing free page %p", m);
		}

		m->uobject = (void *)0xdeadbeef;
		m->offset = 0xdeadbeef;
		m->uanon = (void *)0xdeadbeef;
#endif
		atomic_clearbits_int(&m->pg_flags, PQ_MASK);
	}
	uvm_pmr_freepageq(list);
}
