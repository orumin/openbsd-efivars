/*	$OpenBSD: init_sysent.c,v 1.116 2010/10/28 15:03:51 millert Exp $	*/

/*
 * System call switch table.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from;	OpenBSD: syscalls.master,v 1.104 2010/10/28 15:02:41 millert Exp 
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/mount.h>
#include <sys/syscallargs.h>
#include <sys/poll.h>
#include <sys/event.h>
#include <nnpfs/nnpfs_pioctl.h>

#ifdef COMPAT_43
#define compat_43(func) __CONCAT(compat_43_,func)
#else
#define compat_43(func) sys_nosys
#endif

#ifdef COMPAT_09
#define compat_09(func) __CONCAT(compat_09_,func)
#else
#define compat_09(func) sys_nosys
#endif

#ifdef COMPAT_10
#define compat_10(func) __CONCAT(compat_10_,func)
#else
#define compat_10(func) sys_nosys
#endif

#ifdef COMPAT_25
#define compat_25(func) __CONCAT(compat_25_,func)
#else
#define compat_25(func) sys_nosys
#endif

#ifdef COMPAT_35
#define compat_35(func) __CONCAT(compat_35_,func)
#else
#define compat_35(func) sys_nosys
#endif

#ifdef COMPAT_O47
#define compat_o47(func) __CONCAT(compat_o47_,func)
#else
#define compat_o47(func) sys_nosys
#endif

#define	s(type)	sizeof(type)

struct sysent sysent[] = {
	{ 0, 0, 0,
	    sys_nosys },			/* 0 = syscall (indir) */
	{ 1, s(struct sys_exit_args), 0,
	    sys_exit },				/* 1 = exit */
	{ 0, 0, 0,
	    sys_fork },				/* 2 = fork */
	{ 3, s(struct sys_read_args), 0,
	    sys_read },				/* 3 = read */
	{ 3, s(struct sys_write_args), 0,
	    sys_write },			/* 4 = write */
	{ 3, s(struct sys_open_args), 0,
	    sys_open },				/* 5 = open */
	{ 1, s(struct sys_close_args), 0,
	    sys_close },			/* 6 = close */
	{ 4, s(struct sys_wait4_args), 0,
	    sys_wait4 },			/* 7 = wait4 */
	{ 2, s(struct compat_43_sys_creat_args), 0,
	    compat_43(sys_creat) },		/* 8 = compat_43 ocreat */
	{ 2, s(struct sys_link_args), 0,
	    sys_link },				/* 9 = link */
	{ 1, s(struct sys_unlink_args), 0,
	    sys_unlink },			/* 10 = unlink */
	{ 0, 0, 0,
	    sys_nosys },			/* 11 = obsolete execv */
	{ 1, s(struct sys_chdir_args), 0,
	    sys_chdir },			/* 12 = chdir */
	{ 1, s(struct sys_fchdir_args), 0,
	    sys_fchdir },			/* 13 = fchdir */
	{ 3, s(struct sys_mknod_args), 0,
	    sys_mknod },			/* 14 = mknod */
	{ 2, s(struct sys_chmod_args), 0,
	    sys_chmod },			/* 15 = chmod */
	{ 3, s(struct sys_chown_args), 0,
	    sys_chown },			/* 16 = chown */
	{ 1, s(struct sys_obreak_args), 0,
	    sys_obreak },			/* 17 = break */
	{ 3, s(struct compat_25_sys_getfsstat_args), 0,
	    compat_25(sys_getfsstat) },		/* 18 = compat_25 ogetfsstat */
	{ 3, s(struct compat_43_sys_lseek_args), 0,
	    compat_43(sys_lseek) },		/* 19 = compat_43 olseek */
	{ 0, 0, SY_NOLOCK | 0,
	    sys_getpid },			/* 20 = getpid */
	{ 4, s(struct sys_mount_args), 0,
	    sys_mount },			/* 21 = mount */
	{ 2, s(struct sys_unmount_args), 0,
	    sys_unmount },			/* 22 = unmount */
	{ 1, s(struct sys_setuid_args), 0,
	    sys_setuid },			/* 23 = setuid */
	{ 0, 0, 0,
	    sys_getuid },			/* 24 = getuid */
	{ 0, 0, 0,
	    sys_geteuid },			/* 25 = geteuid */
#ifdef PTRACE
	{ 4, s(struct sys_ptrace_args), 0,
	    sys_ptrace },			/* 26 = ptrace */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 26 = unimplemented ptrace */
#endif
	{ 3, s(struct sys_recvmsg_args), 0,
	    sys_recvmsg },			/* 27 = recvmsg */
	{ 3, s(struct sys_sendmsg_args), 0,
	    sys_sendmsg },			/* 28 = sendmsg */
	{ 6, s(struct sys_recvfrom_args), 0,
	    sys_recvfrom },			/* 29 = recvfrom */
	{ 3, s(struct sys_accept_args), 0,
	    sys_accept },			/* 30 = accept */
	{ 3, s(struct sys_getpeername_args), 0,
	    sys_getpeername },			/* 31 = getpeername */
	{ 3, s(struct sys_getsockname_args), 0,
	    sys_getsockname },			/* 32 = getsockname */
	{ 2, s(struct sys_access_args), 0,
	    sys_access },			/* 33 = access */
	{ 2, s(struct sys_chflags_args), 0,
	    sys_chflags },			/* 34 = chflags */
	{ 2, s(struct sys_fchflags_args), 0,
	    sys_fchflags },			/* 35 = fchflags */
	{ 0, 0, 0,
	    sys_sync },				/* 36 = sync */
	{ 2, s(struct sys_kill_args), 0,
	    sys_kill },				/* 37 = kill */
	{ 2, s(struct compat_43_sys_stat_args), 0,
	    compat_43(sys_stat) },		/* 38 = compat_43 stat43 */
	{ 0, 0, 0,
	    sys_getppid },			/* 39 = getppid */
	{ 2, s(struct compat_43_sys_lstat_args), 0,
	    compat_43(sys_lstat) },		/* 40 = compat_43 lstat43 */
	{ 1, s(struct sys_dup_args), 0,
	    sys_dup },				/* 41 = dup */
	{ 0, 0, 0,
	    sys_opipe },			/* 42 = opipe */
	{ 0, 0, 0,
	    sys_getegid },			/* 43 = getegid */
	{ 4, s(struct sys_profil_args), 0,
	    sys_profil },			/* 44 = profil */
#ifdef KTRACE
	{ 4, s(struct sys_ktrace_args), 0,
	    sys_ktrace },			/* 45 = ktrace */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 45 = unimplemented ktrace */
#endif
	{ 3, s(struct sys_sigaction_args), 0,
	    sys_sigaction },			/* 46 = sigaction */
	{ 0, 0, 0,
	    sys_getgid },			/* 47 = getgid */
	{ 2, s(struct sys_sigprocmask_args), 0,
	    sys_sigprocmask },			/* 48 = sigprocmask */
	{ 2, s(struct sys_getlogin_args), 0,
	    sys_getlogin },			/* 49 = getlogin */
	{ 1, s(struct sys_setlogin_args), 0,
	    sys_setlogin },			/* 50 = setlogin */
#ifdef ACCOUNTING
	{ 1, s(struct sys_acct_args), 0,
	    sys_acct },				/* 51 = acct */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 51 = unimplemented acct */
#endif
	{ 0, 0, 0,
	    sys_sigpending },			/* 52 = sigpending */
	{ 2, s(struct sys_osigaltstack_args), 0,
	    sys_osigaltstack },			/* 53 = osigaltstack */
	{ 3, s(struct sys_ioctl_args), 0,
	    sys_ioctl },			/* 54 = ioctl */
	{ 1, s(struct sys_reboot_args), 0,
	    sys_reboot },			/* 55 = reboot */
	{ 1, s(struct sys_revoke_args), 0,
	    sys_revoke },			/* 56 = revoke */
	{ 2, s(struct sys_symlink_args), 0,
	    sys_symlink },			/* 57 = symlink */
	{ 3, s(struct sys_readlink_args), 0,
	    sys_readlink },			/* 58 = readlink */
	{ 3, s(struct sys_execve_args), 0,
	    sys_execve },			/* 59 = execve */
	{ 1, s(struct sys_umask_args), 0,
	    sys_umask },			/* 60 = umask */
	{ 1, s(struct sys_chroot_args), 0,
	    sys_chroot },			/* 61 = chroot */
	{ 2, s(struct compat_43_sys_fstat_args), 0,
	    compat_43(sys_fstat) },		/* 62 = compat_43 fstat43 */
	{ 4, s(struct compat_43_sys_getkerninfo_args), 0,
	    compat_43(sys_getkerninfo) },	/* 63 = compat_43 ogetkerninfo */
	{ 0, 0, 0,
	    compat_43(sys_getpagesize) },	/* 64 = compat_43 ogetpagesize */
	{ 2, s(struct compat_25_sys_omsync_args), 0,
	    compat_25(sys_omsync) },		/* 65 = compat_25 omsync */
	{ 0, 0, 0,
	    sys_vfork },			/* 66 = vfork */
	{ 0, 0, 0,
	    sys_nosys },			/* 67 = obsolete vread */
	{ 0, 0, 0,
	    sys_nosys },			/* 68 = obsolete vwrite */
	{ 1, s(struct sys_sbrk_args), 0,
	    sys_sbrk },				/* 69 = sbrk */
	{ 1, s(struct sys_sstk_args), 0,
	    sys_sstk },				/* 70 = sstk */
	{ 6, s(struct compat_43_sys_mmap_args), 0,
	    compat_43(sys_mmap) },		/* 71 = compat_43 ommap */
	{ 0, 0, 0,
	    sys_nosys },			/* 72 = obsolete vadvise */
	{ 2, s(struct sys_munmap_args), 0,
	    sys_munmap },			/* 73 = munmap */
	{ 3, s(struct sys_mprotect_args), 0,
	    sys_mprotect },			/* 74 = mprotect */
	{ 3, s(struct sys_madvise_args), 0,
	    sys_madvise },			/* 75 = madvise */
	{ 0, 0, 0,
	    sys_nosys },			/* 76 = obsolete vhangup */
	{ 0, 0, 0,
	    sys_nosys },			/* 77 = obsolete vlimit */
	{ 3, s(struct sys_mincore_args), 0,
	    sys_mincore },			/* 78 = mincore */
	{ 2, s(struct sys_getgroups_args), 0,
	    sys_getgroups },			/* 79 = getgroups */
	{ 2, s(struct sys_setgroups_args), 0,
	    sys_setgroups },			/* 80 = setgroups */
	{ 0, 0, 0,
	    sys_getpgrp },			/* 81 = getpgrp */
	{ 2, s(struct sys_setpgid_args), 0,
	    sys_setpgid },			/* 82 = setpgid */
	{ 3, s(struct sys_setitimer_args), 0,
	    sys_setitimer },			/* 83 = setitimer */
	{ 0, 0, 0,
	    compat_43(sys_wait) },		/* 84 = compat_43 owait */
	{ 1, s(struct compat_25_sys_swapon_args), 0,
	    compat_25(sys_swapon) },		/* 85 = compat_25 swapon */
	{ 2, s(struct sys_getitimer_args), 0,
	    sys_getitimer },			/* 86 = getitimer */
	{ 2, s(struct compat_43_sys_gethostname_args), 0,
	    compat_43(sys_gethostname) },	/* 87 = compat_43 ogethostname */
	{ 2, s(struct compat_43_sys_sethostname_args), 0,
	    compat_43(sys_sethostname) },	/* 88 = compat_43 osethostname */
	{ 0, 0, 0,
	    compat_43(sys_getdtablesize) },	/* 89 = compat_43 ogetdtablesize */
	{ 2, s(struct sys_dup2_args), 0,
	    sys_dup2 },				/* 90 = dup2 */
	{ 0, 0, 0,
	    sys_nosys },			/* 91 = unimplemented getdopt */
	{ 3, s(struct sys_fcntl_args), 0,
	    sys_fcntl },			/* 92 = fcntl */
	{ 5, s(struct sys_select_args), 0,
	    sys_select },			/* 93 = select */
	{ 0, 0, 0,
	    sys_nosys },			/* 94 = unimplemented setdopt */
	{ 1, s(struct sys_fsync_args), 0,
	    sys_fsync },			/* 95 = fsync */
	{ 3, s(struct sys_setpriority_args), 0,
	    sys_setpriority },			/* 96 = setpriority */
	{ 3, s(struct sys_socket_args), 0,
	    sys_socket },			/* 97 = socket */
	{ 3, s(struct sys_connect_args), 0,
	    sys_connect },			/* 98 = connect */
	{ 3, s(struct compat_43_sys_accept_args), 0,
	    compat_43(sys_accept) },		/* 99 = compat_43 oaccept */
	{ 2, s(struct sys_getpriority_args), 0,
	    sys_getpriority },			/* 100 = getpriority */
	{ 4, s(struct compat_43_sys_send_args), 0,
	    compat_43(sys_send) },		/* 101 = compat_43 osend */
	{ 4, s(struct compat_43_sys_recv_args), 0,
	    compat_43(sys_recv) },		/* 102 = compat_43 orecv */
	{ 1, s(struct sys_sigreturn_args), 0,
	    sys_sigreturn },			/* 103 = sigreturn */
	{ 3, s(struct sys_bind_args), 0,
	    sys_bind },				/* 104 = bind */
	{ 5, s(struct sys_setsockopt_args), 0,
	    sys_setsockopt },			/* 105 = setsockopt */
	{ 2, s(struct sys_listen_args), 0,
	    sys_listen },			/* 106 = listen */
	{ 0, 0, 0,
	    sys_nosys },			/* 107 = obsolete vtimes */
	{ 3, s(struct compat_43_sys_sigvec_args), 0,
	    compat_43(sys_sigvec) },		/* 108 = compat_43 osigvec */
	{ 1, s(struct compat_43_sys_sigblock_args), 0,
	    compat_43(sys_sigblock) },		/* 109 = compat_43 osigblock */
	{ 1, s(struct compat_43_sys_sigsetmask_args), 0,
	    compat_43(sys_sigsetmask) },	/* 110 = compat_43 osigsetmask */
	{ 1, s(struct sys_sigsuspend_args), 0,
	    sys_sigsuspend },			/* 111 = sigsuspend */
	{ 2, s(struct compat_43_sys_sigstack_args), 0,
	    compat_43(sys_sigstack) },		/* 112 = compat_43 osigstack */
	{ 3, s(struct compat_43_sys_recvmsg_args), 0,
	    compat_43(sys_recvmsg) },		/* 113 = compat_43 orecvmsg */
	{ 3, s(struct compat_43_sys_sendmsg_args), 0,
	    compat_43(sys_sendmsg) },		/* 114 = compat_43 osendmsg */
	{ 0, 0, 0,
	    sys_nosys },			/* 115 = obsolete vtrace */
	{ 2, s(struct sys_gettimeofday_args), 0,
	    sys_gettimeofday },			/* 116 = gettimeofday */
	{ 2, s(struct sys_getrusage_args), 0,
	    sys_getrusage },			/* 117 = getrusage */
	{ 5, s(struct sys_getsockopt_args), 0,
	    sys_getsockopt },			/* 118 = getsockopt */
	{ 0, 0, 0,
	    sys_nosys },			/* 119 = obsolete resuba */
	{ 3, s(struct sys_readv_args), 0,
	    sys_readv },			/* 120 = readv */
	{ 3, s(struct sys_writev_args), 0,
	    sys_writev },			/* 121 = writev */
	{ 2, s(struct sys_settimeofday_args), 0,
	    sys_settimeofday },			/* 122 = settimeofday */
	{ 3, s(struct sys_fchown_args), 0,
	    sys_fchown },			/* 123 = fchown */
	{ 2, s(struct sys_fchmod_args), 0,
	    sys_fchmod },			/* 124 = fchmod */
	{ 6, s(struct compat_43_sys_recvfrom_args), 0,
	    compat_43(sys_recvfrom) },		/* 125 = compat_43 orecvfrom */
	{ 2, s(struct sys_setreuid_args), 0,
	    sys_setreuid },			/* 126 = setreuid */
	{ 2, s(struct sys_setregid_args), 0,
	    sys_setregid },			/* 127 = setregid */
	{ 2, s(struct sys_rename_args), 0,
	    sys_rename },			/* 128 = rename */
	{ 2, s(struct compat_43_sys_truncate_args), 0,
	    compat_43(sys_truncate) },		/* 129 = compat_43 otruncate */
	{ 2, s(struct compat_43_sys_ftruncate_args), 0,
	    compat_43(sys_ftruncate) },		/* 130 = compat_43 oftruncate */
	{ 2, s(struct sys_flock_args), 0,
	    sys_flock },			/* 131 = flock */
	{ 2, s(struct sys_mkfifo_args), 0,
	    sys_mkfifo },			/* 132 = mkfifo */
	{ 6, s(struct sys_sendto_args), 0,
	    sys_sendto },			/* 133 = sendto */
	{ 2, s(struct sys_shutdown_args), 0,
	    sys_shutdown },			/* 134 = shutdown */
	{ 4, s(struct sys_socketpair_args), 0,
	    sys_socketpair },			/* 135 = socketpair */
	{ 2, s(struct sys_mkdir_args), 0,
	    sys_mkdir },			/* 136 = mkdir */
	{ 1, s(struct sys_rmdir_args), 0,
	    sys_rmdir },			/* 137 = rmdir */
	{ 2, s(struct sys_utimes_args), 0,
	    sys_utimes },			/* 138 = utimes */
	{ 0, 0, 0,
	    sys_nosys },			/* 139 = obsolete 4.2 sigreturn */
	{ 2, s(struct sys_adjtime_args), 0,
	    sys_adjtime },			/* 140 = adjtime */
	{ 3, s(struct compat_43_sys_getpeername_args), 0,
	    compat_43(sys_getpeername) },	/* 141 = compat_43 ogetpeername */
	{ 0, 0, 0,
	    compat_43(sys_gethostid) },		/* 142 = compat_43 ogethostid */
	{ 1, s(struct compat_43_sys_sethostid_args), 0,
	    compat_43(sys_sethostid) },		/* 143 = compat_43 osethostid */
	{ 2, s(struct compat_43_sys_getrlimit_args), 0,
	    compat_43(sys_getrlimit) },		/* 144 = compat_43 ogetrlimit */
	{ 2, s(struct compat_43_sys_setrlimit_args), 0,
	    compat_43(sys_setrlimit) },		/* 145 = compat_43 osetrlimit */
	{ 2, s(struct compat_43_sys_killpg_args), 0,
	    compat_43(sys_killpg) },		/* 146 = compat_43 okillpg */
	{ 0, 0, 0,
	    sys_setsid },			/* 147 = setsid */
	{ 4, s(struct sys_quotactl_args), 0,
	    sys_quotactl },			/* 148 = quotactl */
	{ 0, 0, 0,
	    compat_43(sys_quota) },		/* 149 = compat_43 oquota */
	{ 3, s(struct compat_43_sys_getsockname_args), 0,
	    compat_43(sys_getsockname) },	/* 150 = compat_43 ogetsockname */
	{ 0, 0, 0,
	    sys_nosys },			/* 151 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 152 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 153 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 154 = unimplemented */
#if defined(NFSCLIENT) || defined(NFSSERVER)
	{ 2, s(struct sys_nfssvc_args), 0,
	    sys_nfssvc },			/* 155 = nfssvc */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 155 = unimplemented */
#endif
	{ 4, s(struct compat_43_sys_getdirentries_args), 0,
	    compat_43(sys_getdirentries) },	/* 156 = compat_43 ogetdirentries */
	{ 2, s(struct compat_25_sys_statfs_args), 0,
	    compat_25(sys_statfs) },		/* 157 = compat_25 ostatfs */
	{ 2, s(struct compat_25_sys_fstatfs_args), 0,
	    compat_25(sys_fstatfs) },		/* 158 = compat_25 ostatfs */
	{ 0, 0, 0,
	    sys_nosys },			/* 159 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 160 = unimplemented */
	{ 2, s(struct sys_getfh_args), 0,
	    sys_getfh },			/* 161 = getfh */
	{ 2, s(struct compat_09_sys_getdomainname_args), 0,
	    compat_09(sys_getdomainname) },	/* 162 = compat_09 ogetdomainname */
	{ 2, s(struct compat_09_sys_setdomainname_args), 0,
	    compat_09(sys_setdomainname) },	/* 163 = compat_09 osetdomainname */
	{ 0, 0, 0,
	    sys_nosys },			/* 164 = unimplemented ouname */
	{ 2, s(struct sys_sysarch_args), 0,
	    sys_sysarch },			/* 165 = sysarch */
	{ 0, 0, 0,
	    sys_nosys },			/* 166 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 167 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 168 = unimplemented */
#if defined(SYSVSEM) && !defined(__LP64__)
	{ 5, s(struct compat_10_sys_semsys_args), 0,
	    compat_10(sys_semsys) },		/* 169 = compat_10 osemsys */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 169 = unimplemented 1.0 semsys */
#endif
#if defined(SYSVMSG) && !defined(__LP64__)
	{ 6, s(struct compat_10_sys_msgsys_args), 0,
	    compat_10(sys_msgsys) },		/* 170 = compat_10 omsgsys */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 170 = unimplemented 1.0 msgsys */
#endif
#if defined(SYSVSHM) && !defined(__LP64__)
	{ 4, s(struct compat_10_sys_shmsys_args), 0,
	    compat_10(sys_shmsys) },		/* 171 = compat_10 oshmsys */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 171 = unimplemented 1.0 shmsys */
#endif
	{ 0, 0, 0,
	    sys_nosys },			/* 172 = unimplemented */
	{ 5, s(struct sys_pread_args), 0,
	    sys_pread },			/* 173 = pread */
	{ 5, s(struct sys_pwrite_args), 0,
	    sys_pwrite },			/* 174 = pwrite */
	{ 0, 0, 0,
	    sys_nosys },			/* 175 = unimplemented ntp_gettime */
	{ 0, 0, 0,
	    sys_nosys },			/* 176 = unimplemented ntp_adjtime */
	{ 0, 0, 0,
	    sys_nosys },			/* 177 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 178 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 179 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 180 = unimplemented */
	{ 1, s(struct sys_setgid_args), 0,
	    sys_setgid },			/* 181 = setgid */
	{ 1, s(struct sys_setegid_args), 0,
	    sys_setegid },			/* 182 = setegid */
	{ 1, s(struct sys_seteuid_args), 0,
	    sys_seteuid },			/* 183 = seteuid */
#ifdef LFS
	{ 3, s(struct lfs_bmapv_args), 0,
	    lfs_bmapv },			/* 184 = lfs_bmapv */
	{ 3, s(struct lfs_markv_args), 0,
	    lfs_markv },			/* 185 = lfs_markv */
	{ 2, s(struct lfs_segclean_args), 0,
	    lfs_segclean },			/* 186 = lfs_segclean */
	{ 2, s(struct lfs_segwait_args), 0,
	    lfs_segwait },			/* 187 = lfs_segwait */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 184 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 185 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 186 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 187 = unimplemented */
#endif
	{ 2, s(struct compat_35_sys_stat_args), 0,
	    compat_35(sys_stat) },		/* 188 = compat_35 stat35 */
	{ 2, s(struct compat_35_sys_fstat_args), 0,
	    compat_35(sys_fstat) },		/* 189 = compat_35 fstat35 */
	{ 2, s(struct compat_35_sys_lstat_args), 0,
	    compat_35(sys_lstat) },		/* 190 = compat_35 lstat35 */
	{ 2, s(struct sys_pathconf_args), 0,
	    sys_pathconf },			/* 191 = pathconf */
	{ 2, s(struct sys_fpathconf_args), 0,
	    sys_fpathconf },			/* 192 = fpathconf */
	{ 3, s(struct sys_swapctl_args), 0,
	    sys_swapctl },			/* 193 = swapctl */
	{ 2, s(struct sys_getrlimit_args), 0,
	    sys_getrlimit },			/* 194 = getrlimit */
	{ 2, s(struct sys_setrlimit_args), 0,
	    sys_setrlimit },			/* 195 = setrlimit */
	{ 4, s(struct sys_ogetdirentries_args), 0,
	    sys_ogetdirentries },		/* 196 = ogetdirentries */
	{ 7, s(struct sys_mmap_args), 0,
	    sys_mmap },				/* 197 = mmap */
	{ 0, 0, 0,
	    sys_nosys },			/* 198 = __syscall (indir) */
	{ 4, s(struct sys_lseek_args), 0,
	    sys_lseek },			/* 199 = lseek */
	{ 3, s(struct sys_truncate_args), 0,
	    sys_truncate },			/* 200 = truncate */
	{ 3, s(struct sys_ftruncate_args), 0,
	    sys_ftruncate },			/* 201 = ftruncate */
	{ 6, s(struct sys___sysctl_args), 0,
	    sys___sysctl },			/* 202 = __sysctl */
	{ 2, s(struct sys_mlock_args), 0,
	    sys_mlock },			/* 203 = mlock */
	{ 2, s(struct sys_munlock_args), 0,
	    sys_munlock },			/* 204 = munlock */
	{ 0, 0, 0,
	    sys_nosys },			/* 205 = unimplemented sys_undelete */
	{ 2, s(struct sys_futimes_args), 0,
	    sys_futimes },			/* 206 = futimes */
	{ 1, s(struct sys_getpgid_args), 0,
	    sys_getpgid },			/* 207 = getpgid */
	{ 5, s(struct sys_nnpfspioctl_args), 0,
	    sys_nnpfspioctl },			/* 208 = nnpfspioctl */
	{ 0, 0, 0,
	    sys_nosys },			/* 209 = unimplemented */
#ifdef LKM
	{ 0, 0, 0,
	    sys_lkmnosys },			/* 210 = lkmnosys */
	{ 0, 0, 0,
	    sys_lkmnosys },			/* 211 = lkmnosys */
	{ 0, 0, 0,
	    sys_lkmnosys },			/* 212 = lkmnosys */
	{ 0, 0, 0,
	    sys_lkmnosys },			/* 213 = lkmnosys */
	{ 0, 0, 0,
	    sys_lkmnosys },			/* 214 = lkmnosys */
	{ 0, 0, 0,
	    sys_lkmnosys },			/* 215 = lkmnosys */
	{ 0, 0, 0,
	    sys_lkmnosys },			/* 216 = lkmnosys */
	{ 0, 0, 0,
	    sys_lkmnosys },			/* 217 = lkmnosys */
	{ 0, 0, 0,
	    sys_lkmnosys },			/* 218 = lkmnosys */
	{ 0, 0, 0,
	    sys_lkmnosys },			/* 219 = lkmnosys */
#else	/* !LKM */
	{ 0, 0, 0,
	    sys_nosys },			/* 210 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 211 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 212 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 213 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 214 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 215 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 216 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 217 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 218 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 219 = unimplemented */
#endif	/* !LKM */
#ifdef SYSVSEM
	{ 0, 0, 0,
	    sys_nosys },			/* 220 = unimplemented */
	{ 3, s(struct sys_semget_args), 0,
	    sys_semget },			/* 221 = semget */
	{ 3, s(struct compat_35_sys_semop_args), 0,
	    compat_35(sys_semop) },		/* 222 = compat_35 semop */
	{ 0, 0, 0,
	    sys_nosys },			/* 223 = obsolete sys_semconfig */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 220 = unimplemented semctl */
	{ 0, 0, 0,
	    sys_nosys },			/* 221 = unimplemented semget */
	{ 0, 0, 0,
	    sys_nosys },			/* 222 = unimplemented semop */
	{ 0, 0, 0,
	    sys_nosys },			/* 223 = unimplemented semconfig */
#endif
#ifdef SYSVMSG
	{ 0, 0, 0,
	    sys_nosys },			/* 224 = unimplemented */
	{ 2, s(struct sys_msgget_args), 0,
	    sys_msgget },			/* 225 = msgget */
	{ 4, s(struct sys_msgsnd_args), 0,
	    sys_msgsnd },			/* 226 = msgsnd */
	{ 5, s(struct sys_msgrcv_args), 0,
	    sys_msgrcv },			/* 227 = msgrcv */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 224 = unimplemented msgctl */
	{ 0, 0, 0,
	    sys_nosys },			/* 225 = unimplemented msgget */
	{ 0, 0, 0,
	    sys_nosys },			/* 226 = unimplemented msgsnd */
	{ 0, 0, 0,
	    sys_nosys },			/* 227 = unimplemented msgrcv */
#endif
#ifdef SYSVSHM
	{ 3, s(struct sys_shmat_args), 0,
	    sys_shmat },			/* 228 = shmat */
	{ 0, 0, 0,
	    sys_nosys },			/* 229 = unimplemented */
	{ 1, s(struct sys_shmdt_args), 0,
	    sys_shmdt },			/* 230 = shmdt */
	{ 3, s(struct compat_35_sys_shmget_args), 0,
	    compat_35(sys_shmget) },		/* 231 = compat_35 shmget */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 228 = unimplemented shmat */
	{ 0, 0, 0,
	    sys_nosys },			/* 229 = unimplemented shmctl */
	{ 0, 0, 0,
	    sys_nosys },			/* 230 = unimplemented shmdt */
	{ 0, 0, 0,
	    sys_nosys },			/* 231 = unimplemented shmget */
#endif
	{ 2, s(struct sys_clock_gettime_args), 0,
	    sys_clock_gettime },		/* 232 = clock_gettime */
	{ 2, s(struct sys_clock_settime_args), 0,
	    sys_clock_settime },		/* 233 = clock_settime */
	{ 2, s(struct sys_clock_getres_args), 0,
	    sys_clock_getres },			/* 234 = clock_getres */
	{ 0, 0, 0,
	    sys_nosys },			/* 235 = unimplemented timer_create */
	{ 0, 0, 0,
	    sys_nosys },			/* 236 = unimplemented timer_delete */
	{ 0, 0, 0,
	    sys_nosys },			/* 237 = unimplemented timer_settime */
	{ 0, 0, 0,
	    sys_nosys },			/* 238 = unimplemented timer_gettime */
	{ 0, 0, 0,
	    sys_nosys },			/* 239 = unimplemented timer_getoverrun */
	{ 2, s(struct sys_nanosleep_args), 0,
	    sys_nanosleep },			/* 240 = nanosleep */
	{ 0, 0, 0,
	    sys_nosys },			/* 241 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 242 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 243 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 244 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 245 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 246 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 247 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 248 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 249 = unimplemented */
	{ 3, s(struct sys_minherit_args), 0,
	    sys_minherit },			/* 250 = minherit */
	{ 1, s(struct sys_rfork_args), 0,
	    sys_rfork },			/* 251 = rfork */
	{ 3, s(struct sys_poll_args), 0,
	    sys_poll },				/* 252 = poll */
	{ 0, 0, 0,
	    sys_issetugid },			/* 253 = issetugid */
	{ 3, s(struct sys_lchown_args), 0,
	    sys_lchown },			/* 254 = lchown */
	{ 1, s(struct sys_getsid_args), 0,
	    sys_getsid },			/* 255 = getsid */
	{ 3, s(struct sys_msync_args), 0,
	    sys_msync },			/* 256 = msync */
#ifdef SYSVSEM
	{ 4, s(struct compat_35_sys___semctl_args), 0,
	    compat_35(sys___semctl) },		/* 257 = compat_35 semctl35 */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 257 = unimplemented */
#endif
#ifdef SYSVSHM
	{ 3, s(struct compat_35_sys_shmctl_args), 0,
	    compat_35(sys_shmctl) },		/* 258 = compat_35 shmctl35 */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 258 = unimplemented */
#endif
#ifdef SYSVMSG
	{ 3, s(struct compat_35_sys_msgctl_args), 0,
	    compat_35(sys_msgctl) },		/* 259 = compat_35 msgctl35 */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 259 = unimplemented */
#endif
	{ 0, 0, 0,
	    sys_nosys },			/* 260 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 261 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 262 = unimplemented */
	{ 1, s(struct sys_pipe_args), 0,
	    sys_pipe },				/* 263 = pipe */
	{ 2, s(struct sys_fhopen_args), 0,
	    sys_fhopen },			/* 264 = fhopen */
	{ 0, 0, 0,
	    sys_nosys },			/* 265 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 266 = unimplemented */
	{ 5, s(struct sys_preadv_args), 0,
	    sys_preadv },			/* 267 = preadv */
	{ 5, s(struct sys_pwritev_args), 0,
	    sys_pwritev },			/* 268 = pwritev */
	{ 0, 0, 0,
	    sys_kqueue },			/* 269 = kqueue */
	{ 6, s(struct sys_kevent_args), 0,
	    sys_kevent },			/* 270 = kevent */
	{ 1, s(struct sys_mlockall_args), 0,
	    sys_mlockall },			/* 271 = mlockall */
	{ 0, 0, 0,
	    sys_munlockall },			/* 272 = munlockall */
	{ 3, s(struct compat_o47_sys_getpeereid_args), 0,
	    compat_o47(sys_getpeereid) },	/* 273 = compat_o47 getpeereid */
	{ 0, 0, 0,
	    sys_nosys },			/* 274 = unimplemented sys_extattrctl */
	{ 0, 0, 0,
	    sys_nosys },			/* 275 = unimplemented sys_extattr_set_file */
	{ 0, 0, 0,
	    sys_nosys },			/* 276 = unimplemented sys_extattr_get_file */
	{ 0, 0, 0,
	    sys_nosys },			/* 277 = unimplemented sys_extattr_delete_file */
	{ 0, 0, 0,
	    sys_nosys },			/* 278 = unimplemented sys_extattr_set_fd */
	{ 0, 0, 0,
	    sys_nosys },			/* 279 = unimplemented sys_extattr_get_fd */
	{ 0, 0, 0,
	    sys_nosys },			/* 280 = unimplemented sys_extattr_delete_fd */
	{ 3, s(struct sys_getresuid_args), 0,
	    sys_getresuid },			/* 281 = getresuid */
	{ 3, s(struct sys_setresuid_args), 0,
	    sys_setresuid },			/* 282 = setresuid */
	{ 3, s(struct sys_getresgid_args), 0,
	    sys_getresgid },			/* 283 = getresgid */
	{ 3, s(struct sys_setresgid_args), 0,
	    sys_setresgid },			/* 284 = setresgid */
	{ 0, 0, 0,
	    sys_nosys },			/* 285 = obsolete sys_omquery */
	{ 7, s(struct sys_mquery_args), 0,
	    sys_mquery },			/* 286 = mquery */
	{ 1, s(struct sys_closefrom_args), 0,
	    sys_closefrom },			/* 287 = closefrom */
	{ 2, s(struct sys_sigaltstack_args), 0,
	    sys_sigaltstack },			/* 288 = sigaltstack */
#ifdef SYSVSHM
	{ 3, s(struct sys_shmget_args), 0,
	    sys_shmget },			/* 289 = shmget */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 289 = unimplemented shmget */
#endif
#ifdef SYSVSEM
	{ 3, s(struct sys_semop_args), 0,
	    sys_semop },			/* 290 = semop */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 290 = unimplemented semop */
#endif
	{ 2, s(struct sys_stat_args), 0,
	    sys_stat },				/* 291 = stat */
	{ 2, s(struct sys_fstat_args), 0,
	    sys_fstat },			/* 292 = fstat */
	{ 2, s(struct sys_lstat_args), 0,
	    sys_lstat },			/* 293 = lstat */
	{ 2, s(struct sys_fhstat_args), 0,
	    sys_fhstat },			/* 294 = fhstat */
#ifdef SYSVSEM
	{ 4, s(struct sys___semctl_args), 0,
	    sys___semctl },			/* 295 = __semctl */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 295 = unimplemented */
#endif
#ifdef SYSVSHM
	{ 3, s(struct sys_shmctl_args), 0,
	    sys_shmctl },			/* 296 = shmctl */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 296 = unimplemented */
#endif
#ifdef SYSVMSG
	{ 3, s(struct sys_msgctl_args), 0,
	    sys_msgctl },			/* 297 = msgctl */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 297 = unimplemented */
#endif
	{ 0, 0, 0,
	    sys_sched_yield },			/* 298 = sched_yield */
	{ 0, 0, 0,
	    sys_getthrid },			/* 299 = getthrid */
	{ 4, s(struct sys_thrsleep_args), 0,
	    sys_thrsleep },			/* 300 = thrsleep */
	{ 2, s(struct sys_thrwakeup_args), 0,
	    sys_thrwakeup },			/* 301 = thrwakeup */
	{ 1, s(struct sys_threxit_args), 0,
	    sys_threxit },			/* 302 = threxit */
	{ 3, s(struct sys_thrsigdivert_args), 0,
	    sys_thrsigdivert },			/* 303 = thrsigdivert */
	{ 2, s(struct sys___getcwd_args), 0,
	    sys___getcwd },			/* 304 = __getcwd */
	{ 2, s(struct sys_adjfreq_args), 0,
	    sys_adjfreq },			/* 305 = adjfreq */
	{ 3, s(struct sys_getfsstat_args), 0,
	    sys_getfsstat },			/* 306 = getfsstat */
	{ 2, s(struct sys_statfs_args), 0,
	    sys_statfs },			/* 307 = statfs */
	{ 2, s(struct sys_fstatfs_args), 0,
	    sys_fstatfs },			/* 308 = fstatfs */
	{ 2, s(struct sys_fhstatfs_args), 0,
	    sys_fhstatfs },			/* 309 = fhstatfs */
	{ 1, s(struct sys_setrtable_args), 0,
	    sys_setrtable },			/* 310 = setrtable */
	{ 0, 0, 0,
	    sys_getrtable },			/* 311 = getrtable */
	{ 4, s(struct sys_getdirentries_args), 0,
	    sys_getdirentries },		/* 312 = getdirentries */
};

