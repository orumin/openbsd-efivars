/*	$OpenBSD: if_pppvar.h,v 1.12 2002/07/01 19:31:34 deraadt Exp $	*/
/*	$NetBSD: if_pppvar.h,v 1.5 1997/01/03 07:23:29 mikel Exp $	*/
/*
 * if_pppvar.h - private structures and declarations for PPP.
 *
 * Copyright (c) 1994 The Australian National University.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, provided that the above copyright
 * notice appears in all copies.  This software is provided without any
 * warranty, express or implied. The Australian National University
 * makes no representations about the suitability of this software for
 * any purpose.
 *
 * IN NO EVENT SHALL THE AUSTRALIAN NATIONAL UNIVERSITY BE LIABLE TO ANY
 * PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
 * THE AUSTRALIAN NATIONAL UNIVERSITY HAVE BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * THE AUSTRALIAN NATIONAL UNIVERSITY SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE AUSTRALIAN NATIONAL UNIVERSITY HAS NO
 * OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS,
 * OR MODIFICATIONS.
 *
 * Copyright (c) 1984-2000 Carnegie Mellon University. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The name "Carnegie Mellon University" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For permission or any legal
 *    details, please contact
 *      Office of Technology Transfer
 *      Carnegie Mellon University
 *      5000 Forbes Avenue
 *      Pittsburgh, PA  15213-3890
 *      (412) 268-4387, fax: (412) 268-7395
 *      tech-transfer@andrew.cmu.edu
 *
 * 4. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by Computing Services
 *     at Carnegie Mellon University (http://www.cmu.edu/computing/)."
 *
 * CARNEGIE MELLON UNIVERSITY DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY BE LIABLE
 * FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _NET_IF_PPPVAR_H_
#define _NET_IF_PPPVAR_H_

/*
 * Supported network protocols.  These values are used for
 * indexing sc_npmode.
 */
#define NP_IP	0		/* Internet Protocol */
#define NUM_NP	1		/* Number of NPs. */

/*
 * Structure describing each ppp unit.
 */
struct ppp_softc {
	struct	ifnet sc_if;		/* network-visible interface */
	struct	timeout sc_timo;	/* timeout control (for ptys) */
	int	sc_unit;		/* XXX unit number */
	u_int	sc_flags;		/* control/status bits; see if_ppp.h */
	void	*sc_devp;		/* pointer to device-dep structure */
	void	(*sc_start)(struct ppp_softc *); /* start output proc */
	void	(*sc_ctlp)(struct ppp_softc *); /* rcvd control pkt */
	void	(*sc_relinq)(struct ppp_softc *); /* relinquish ifunit */
	u_int16_t sc_mru;		/* max receive unit */
	pid_t	sc_xfer;		/* used in transferring unit */
	struct	ifqueue sc_rawq;	/* received packets */
	struct	ifqueue sc_inq;		/* queue of input packets for daemon */
	struct	ifqueue sc_fastq;	/* interactive output packet q */
	struct	mbuf *sc_togo;		/* output packet ready to go */
	struct	mbuf *sc_npqueue;	/* output packets not to be sent yet */
	struct	mbuf **sc_npqtail;	/* ptr to last next ptr in npqueue */
	struct	pppstat sc_stats;	/* count of bytes/pkts sent/rcvd */
	caddr_t	sc_bpf;			/* hook for BPF */
	enum	NPmode sc_npmode[NUM_NP]; /* what to do with each NP */
	struct	compressor *sc_xcomp;	/* transmit compressor */
	void	*sc_xc_state;		/* transmit compressor state */
	struct	compressor *sc_rcomp;	/* receive decompressor */
	void	*sc_rc_state;		/* receive decompressor state */
	time_t	sc_last_sent;		/* time (secs) last NP pkt sent */
	time_t	sc_last_recv;		/* time (secs) last NP pkt rcvd */
	struct	bpf_program sc_pass_filt; /* filter for packets to pass */
	struct	bpf_program sc_active_filt; /* filter for "non-idle" packets */
#ifdef	VJC
	struct	slcompress *sc_comp; 	/* vjc control buffer */
#endif

	/* Device-dependent part for async lines. */
	ext_accm sc_asyncmap;		/* async control character map */
	u_int32_t sc_rasyncmap;		/* receive async control char map */
	struct	mbuf *sc_outm;		/* mbuf chain currently being output */
	struct	mbuf *sc_m;		/* pointer to input mbuf chain */
	struct	mbuf *sc_mc;		/* pointer to current input mbuf */
	char	*sc_mp;			/* ptr to next char in input mbuf */
	u_int16_t sc_ilen;		/* length of input packet so far */
	u_int16_t sc_fcs;		/* FCS so far (input) */
	u_int16_t sc_outfcs;		/* FCS so far for output packet */
	u_char	sc_rawin[16];		/* chars as received */
	int	sc_rawin_count;		/* # in sc_rawin */
};

#ifdef _KERNEL
struct	ppp_softc ppp_softc[NPPP];

struct	ppp_softc *pppalloc(pid_t pid);
void	pppdealloc(struct ppp_softc *sc);
int	pppioctl(struct ppp_softc *sc, u_long cmd, caddr_t data,
		      int flag, struct proc *p);
void	ppppktin(struct ppp_softc *sc, struct mbuf *m, int lost);
struct	mbuf *ppp_dequeue(struct ppp_softc *sc);
void	ppp_restart(struct ppp_softc *sc);
int	pppoutput(struct ifnet *, struct mbuf *,
		       struct sockaddr *, struct rtentry *);
#endif /* _KERNEL */
#endif /* _NET_IF_PPPVAR_H_ */
