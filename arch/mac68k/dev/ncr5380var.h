/*	$OpenBSD: ncr5380var.h,v 1.1 1996/05/26 19:02:08 briggs Exp $	*/
/*	$NetBSD: ncr5380var.h,v 1.2 1996/05/25 16:42:31 briggs Exp $	*/

/*
 * Copyright (c) 1995 Allen Briggs.  All rights reserved.
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
 *	This product includes software developed by Allen Briggs.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

static volatile u_char	*scsi_enable = NULL;
static volatile u_char	*scsi_flag   = NULL;

static __inline__ void
scsi_clear_drq __P((void))
{
	int	s;

	s = splhigh();
	*scsi_flag = 0x80 | V2IF_SCSIDRQ;
	splx(s);
}

static __inline__ void
scsi_clear_irq __P((void))
{
	int	s;

	s = splhigh();
	*scsi_flag = 0x80 | V2IF_SCSIIRQ;
	splx(s);
}

static __inline__ void
scsi_ienable __P((void))
{
	int	s;

	s = splhigh();
	*scsi_enable = 0x80 | (V2IF_SCSIIRQ | V2IF_SCSIDRQ);
	splx(s);
}

static __inline__ void
scsi_idisable __P((void))
{
	int	s;

	s = splhigh();
	*scsi_enable = V2IF_SCSIIRQ | V2IF_SCSIDRQ;
	splx(s);
}

void	pdma_stat __P((void));
void	pdma_cleanup __P((void));
void	scsi_show __P((void));

