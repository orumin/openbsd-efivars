/*	$OpenBSD: umap.h,v 1.5 1996/03/25 18:02:55 mickey Exp $	*/
/*	$NetBSD: umap.h,v 1.6 1996/02/09 22:41:00 christos Exp $	*/

/*
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software donated to Berkeley by
 * the UCLA Ficus project.
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
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	from: @(#)null_vnops.c       1.5 (Berkeley) 7/10/92
 *	@(#)umap.h	8.3 (Berkeley) 1/21/94
 */

#define UMAPFILEENTRIES 64
#define GMAPFILEENTRIES 16
#define NOBODY 32767
#define NULLGROUP 65534

typedef	u_int32_t	id_t;
typedef	id_t		(*id_map_t)[2];

struct umap_args {
	char	*target;	/* Target of loopback  */
	int 	unentries;	/* # of entries in user map array */
	int 	gnentries;	/* # of entries in group map array */
	id_map_t umapdata;	/* pointer to array of user mappings */
	id_map_t gmapdata;	/* pointer to array of group mappings */
};

struct umap_mount {
	struct mount	*umapm_vfs;
	struct vnode	*umapm_rootvp;	/* Reference to root umap_node */
	int		info_unentries;	/* number of uid mappings */
	int		info_gnentries;	/* number of gid mappings */
	id_t		info_umapdata[UMAPFILEENTRIES][2]; /* mapping data for 
	    user mapping in ficus */
	id_t		info_gmapdata[GMAPFILEENTRIES][2]; /*mapping data for 
	    group mapping in ficus */
};

#ifdef _KERNEL
/*
 * A cache of vnode references
 */
struct umap_node {
	LIST_ENTRY(umap_node) umap_hash;	/* Hash list */
	struct vnode	*umap_lowervp;	/* Aliased vnode - VREFed once */
	struct vnode	*umap_vnode;	/* Back pointer to vnode/umap_node */
};

extern int	umap_node_create __P((struct mount *mp, struct vnode *target, struct vnode **vpp));
extern id_t	umap_reverse_findid __P((id_t id, id_map_t, int nentries));
extern void	umap_mapids __P((struct mount *v_mount, struct ucred *credp));

#define	MOUNTTOUMAPMOUNT(mp) ((struct umap_mount *)((mp)->mnt_data))
#define	VTOUMAP(vp) ((struct umap_node *)(vp)->v_data)
#define UMAPTOV(xp) ((xp)->umap_vnode)
#ifdef UMAPFS_DIAGNOSTIC
extern struct vnode *umap_checkvp __P((struct vnode *vp, char *fil, int lno));
#define	UMAPVPTOLOWERVP(vp) umap_checkvp((vp), __FILE__, __LINE__)
#else
#define	UMAPVPTOLOWERVP(vp) (VTOUMAP(vp)->umap_lowervp)
#endif

extern int (**umap_vnodeop_p) __P((void *));
extern struct vfsops umap_vfsops;

void umapfs_init __P((void));

#endif /* _KERNEL */
