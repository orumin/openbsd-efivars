/* $OpenBSD: fusefs.h,v 1.2 2013/06/09 12:51:40 tedu Exp $ */
/*
 * Copyright (c) 2012-2013 Sylvestre Gallon <ccna.syl@gmail.com>
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

#ifndef __FUSEFS_H__
#define __FUSEFS_H__

/* sysctl defines */
#define FUSEFS_OPENDEVS		1	/* # of fuse devices opened */
#define FUSEFS_INFBUFS		2	/* # of in fbufs */
#define FUSEFS_WAITFBUFS	3	/* # of fbufs waiting for a response */
#define FUSEFS_POOL_NBPAGES	4	/* # total fusefs size */
#define FUSEFS_MAXID		5	/* number of valid fusefs ids */

#define FUSEFS_NAMES { \
	{ 0, 0}, \
	{ "fusefs_open_devices", CTLTYPE_INT }, \
	{ "fusefs_fbufs_in", CTLTYPE_INT }, \
	{ "fusefs_fbufs_wait", CTLTYPE_INT }, \
	{ "fusefs_pool_pages", CTLTYPE_INT }, \
}

#ifdef _KERNEL

struct fuse_msg;

struct fusefs_mnt {
	struct mount *mp;
	uint32_t undef_op;
	uint32_t max_write;
	int sess_init;
	dev_t dev;
};

#define	UNDEF_ACCESS	1<<0
#define	UNDEF_MKDIR	1<<1
#define	UNDEF_CREATE	1<<2
#define	UNDEF_LINK	1<<3
#define UNDEF_READLINK	1<<4
#define UNDEF_RMDIR	1<<5
#define UNDEF_REMOVE	1<<6
#define UNDEF_SETATTR	1<<7
#define UNDEF_RENAME	1<<8
#define UNDEF_SYMLINK	1<<9

extern struct vops fusefs_vops;
extern struct pool fusefs_fbuf_pool;

/* fuse helpers */
#define TSLEEP_TIMEOUT 5
void update_vattr(struct mount *mp, struct vattr *v);

/* files helpers. */
int fusefs_file_open(struct fusefs_mnt *, struct fusefs_node *, enum fufh_type,
    int, int, struct proc *);
int fusefs_file_close(struct fusefs_mnt *, struct fusefs_node *,
    enum fufh_type, int, int, struct proc *);

/* device helpers. */
void fuse_device_cleanup(dev_t, struct fusebuf *);
void fuse_device_queue_fbuf(dev_t, struct fusebuf *);
void fuse_device_set_fmp(struct fusefs_mnt *);

/*
 * The root inode is the root of the file system.  Inode 0 can't be used for
 * normal purposes.
 */
#define	FUSE_ROOTINO ((ino_t)1)
#define VFSTOFUSEFS(mp)	((struct fusefs_mnt *)((mp)->mnt_data))

/* #define FUSE_DEBUG_VNOP
#define FUSE_DEBUG */

#endif /* _KERNEL */
#endif /* __FUSEFS_H__ */