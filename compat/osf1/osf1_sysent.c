/*	$OpenBSD: osf1_sysent.c,v 1.12 2007/11/27 18:05:59 art Exp $	*/

/*
 * System call switch table.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	OpenBSD: syscalls.master,v 1.10 2004/05/28 18:28:14 tedu Exp 
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/signal.h>
#include <sys/mount.h>
#include <sys/poll.h>
#include <sys/syscallargs.h>
#include <compat/osf1/osf1.h>
#include <compat/osf1/osf1_syscallargs.h>

#define	s(type)	sizeof(type)

struct sysent osf1_sysent[] = {
	{ 0, 0, 0,
	    sys_nosys },			/* 0 = syscall */
	{ 1, s(struct sys_exit_args), 0,
	    sys_exit },				/* 1 = exit */
	{ 0, 0, 0,
	    sys_fork },				/* 2 = fork */
	{ 3, s(struct sys_read_args), 0,
	    sys_read },				/* 3 = read */
	{ 3, s(struct sys_write_args), 0,
	    sys_write },			/* 4 = write */
	{ 0, 0, 0,
	    sys_nosys },			/* 5 = unimplemented old open */
	{ 1, s(struct sys_close_args), 0,
	    sys_close },			/* 6 = close */
	{ 4, s(struct osf1_sys_wait4_args), 0,
	    osf1_sys_wait4 },			/* 7 = wait4 */
	{ 0, 0, 0,
	    sys_nosys },			/* 8 = unimplemented old creat */
	{ 2, s(struct sys_link_args), 0,
	    sys_link },				/* 9 = link */
	{ 1, s(struct sys_unlink_args), 0,
	    sys_unlink },			/* 10 = unlink */
	{ 0, 0, 0,
	    sys_nosys },			/* 11 = unimplemented execv */
	{ 1, s(struct sys_chdir_args), 0,
	    sys_chdir },			/* 12 = chdir */
	{ 1, s(struct sys_fchdir_args), 0,
	    sys_fchdir },			/* 13 = fchdir */
	{ 3, s(struct osf1_sys_mknod_args), 0,
	    osf1_sys_mknod },			/* 14 = mknod */
	{ 2, s(struct sys_chmod_args), 0,
	    sys_chmod },			/* 15 = chmod */
	{ 3, s(struct sys_chown_args), 0,
	    sys_chown },			/* 16 = chown */
	{ 1, s(struct sys_obreak_args), 0,
	    sys_obreak },			/* 17 = obreak */
	{ 3, s(struct osf1_sys_getfsstat_args), 0,
	    osf1_sys_getfsstat },		/* 18 = getfsstat */
	{ 3, s(struct osf1_sys_lseek_args), 0,
	    osf1_sys_lseek },			/* 19 = lseek */
	{ 0, 0, 0,
	    sys_getpid },			/* 20 = getpid */
	{ 4, s(struct osf1_sys_mount_args), 0,
	    osf1_sys_mount },			/* 21 = mount */
	{ 2, s(struct osf1_sys_unmount_args), 0,
	    osf1_sys_unmount },			/* 22 = unmount */
	{ 1, s(struct osf1_sys_setuid_args), 0,
	    osf1_sys_setuid },			/* 23 = setuid */
	{ 0, 0, 0,
	    sys_getuid },			/* 24 = getuid */
	{ 0, 0, 0,
	    sys_nosys },			/* 25 = unimplemented exec_with_loader */
	{ 0, 0, 0,
	    sys_nosys },			/* 26 = unimplemented ptrace */
	{ 3, s(struct osf1_sys_recvmsg_xopen_args), 0,
	    osf1_sys_recvmsg_xopen },		/* 27 = recvmsg_xopen */
	{ 3, s(struct osf1_sys_sendmsg_xopen_args), 0,
	    osf1_sys_sendmsg_xopen },		/* 28 = sendmsg_xopen */
	{ 0, 0, 0,
	    sys_nosys },			/* 29 = unimplemented recvfrom */
	{ 0, 0, 0,
	    sys_nosys },			/* 30 = unimplemented accept */
	{ 0, 0, 0,
	    sys_nosys },			/* 31 = unimplemented getpeername */
	{ 0, 0, 0,
	    sys_nosys },			/* 32 = unimplemented getsockname */
	{ 2, s(struct osf1_sys_access_args), 0,
	    osf1_sys_access },			/* 33 = access */
	{ 0, 0, 0,
	    sys_nosys },			/* 34 = unimplemented chflags */
	{ 0, 0, 0,
	    sys_nosys },			/* 35 = unimplemented fchflags */
	{ 0, 0, 0,
	    sys_sync },				/* 36 = sync */
	{ 2, s(struct sys_kill_args), 0,
	    sys_kill },				/* 37 = kill */
	{ 0, 0, 0,
	    sys_nosys },			/* 38 = unimplemented old stat */
	{ 2, s(struct sys_setpgid_args), 0,
	    sys_setpgid },			/* 39 = setpgid */
	{ 0, 0, 0,
	    sys_nosys },			/* 40 = unimplemented old lstat */
	{ 1, s(struct sys_dup_args), 0,
	    sys_dup },				/* 41 = dup */
	{ 0, 0, 0,
	    sys_pipe },				/* 42 = pipe */
	{ 4, s(struct osf1_sys_set_program_attributes_args), 0,
	    osf1_sys_set_program_attributes },	/* 43 = set_program_attributes */
	{ 0, 0, 0,
	    sys_nosys },			/* 44 = unimplemented profil */
	{ 3, s(struct osf1_sys_open_args), 0,
	    osf1_sys_open },			/* 45 = open */
	{ 0, 0, 0,
	    sys_nosys },			/* 46 = obsolete sigaction */
	{ 0, 0, 0,
	    sys_getgid },			/* 47 = getgid */
	{ 2, s(struct sys_sigprocmask_args), 0,
	    sys_sigprocmask },			/* 48 = sigprocmask */
	{ 2, s(struct sys_getlogin_args), 0,
	    sys_getlogin },			/* 49 = getlogin */
	{ 1, s(struct sys_setlogin_args), 0,
	    sys_setlogin },			/* 50 = setlogin */
#ifdef	ACCOUNTING
	{ 1, s(struct sys_acct_args), 0,
	    sys_acct },				/* 51 = acct */
#else
	{ 0, 0, 0,
	    sys_nosys },			/* 51 = unimplemented acct */
#endif
	{ 0, 0, 0,
	    sys_nosys },			/* 52 = unimplemented sigpending */
	{ 4, s(struct osf1_sys_classcntl_args), 0,
	    osf1_sys_classcntl },		/* 53 = classcntl */
	{ 3, s(struct osf1_sys_ioctl_args), 0,
	    osf1_sys_ioctl },			/* 54 = ioctl */
	{ 1, s(struct osf1_sys_reboot_args), 0,
	    osf1_sys_reboot },			/* 55 = reboot */
	{ 1, s(struct sys_revoke_args), 0,
	    sys_revoke },			/* 56 = revoke */
	{ 2, s(struct sys_symlink_args), 0,
	    sys_symlink },			/* 57 = symlink */
	{ 3, s(struct sys_readlink_args), 0,
	    sys_readlink },			/* 58 = readlink */
	{ 3, s(struct osf1_sys_execve_args), 0,
	    osf1_sys_execve },			/* 59 = execve */
	{ 1, s(struct sys_umask_args), 0,
	    sys_umask },			/* 60 = umask */
	{ 1, s(struct sys_chroot_args), 0,
	    sys_chroot },			/* 61 = chroot */
	{ 0, 0, 0,
	    sys_nosys },			/* 62 = unimplemented old fstat */
	{ 0, 0, 0,
	    sys_getpgrp },			/* 63 = getpgrp */
	{ 0, 0, 0,
	    compat_43_sys_getpagesize },	/* 64 = getpagesize */
	{ 0, 0, 0,
	    sys_nosys },			/* 65 = unimplemented mremap */
	{ 0, 0, 0,
	    sys_vfork },			/* 66 = vfork */
	{ 2, s(struct osf1_sys_stat_args), 0,
	    osf1_sys_stat },			/* 67 = stat */
	{ 2, s(struct osf1_sys_lstat_args), 0,
	    osf1_sys_lstat },			/* 68 = lstat */
	{ 0, 0, 0,
	    sys_nosys },			/* 69 = unimplemented sbrk */
	{ 0, 0, 0,
	    sys_nosys },			/* 70 = unimplemented sstk */
	{ 6, s(struct osf1_sys_mmap_args), 0,
	    osf1_sys_mmap },			/* 71 = mmap */
	{ 0, 0, 0,
	    sys_nosys },			/* 72 = unimplemented ovadvise */
	{ 2, s(struct sys_munmap_args), 0,
	    sys_munmap },			/* 73 = munmap */
	{ 3, s(struct osf1_sys_mprotect_args), 0,
	    osf1_sys_mprotect },		/* 74 = mprotect */
	{ 3, s(struct osf1_sys_madvise_args), 0,
	    osf1_sys_madvise },			/* 75 = madvise */
	{ 0, 0, 0,
	    sys_nosys },			/* 76 = unimplemented old vhangup */
	{ 0, 0, 0,
	    sys_nosys },			/* 77 = unimplemented kmodcall */
	{ 0, 0, 0,
	    sys_nosys },			/* 78 = unimplemented mincore */
	{ 2, s(struct sys_getgroups_args), 0,
	    sys_getgroups },			/* 79 = getgroups */
	{ 2, s(struct sys_setgroups_args), 0,
	    sys_setgroups },			/* 80 = setgroups */
	{ 0, 0, 0,
	    sys_nosys },			/* 81 = unimplemented old getpgrp */
	{ 2, s(struct sys_setpgid_args), 0,
	    sys_setpgid },			/* 82 = setpgrp */
	{ 3, s(struct osf1_sys_setitimer_args), 0,
	    osf1_sys_setitimer },		/* 83 = setitimer */
	{ 0, 0, 0,
	    sys_nosys },			/* 84 = unimplemented old wait */
	{ 0, 0, 0,
	    sys_nosys },			/* 85 = unimplemented table */
	{ 0, 0, 0,
	    sys_nosys },			/* 86 = unimplemented getitimer */
	{ 2, s(struct compat_43_sys_gethostname_args), 0,
	    compat_43_sys_gethostname },	/* 87 = gethostname */
	{ 2, s(struct compat_43_sys_sethostname_args), 0,
	    compat_43_sys_sethostname },	/* 88 = sethostname */
	{ 0, 0, 0,
	    compat_43_sys_getdtablesize },	/* 89 = getdtablesize */
	{ 2, s(struct sys_dup2_args), 0,
	    sys_dup2 },				/* 90 = dup2 */
	{ 2, s(struct osf1_sys_fstat_args), 0,
	    osf1_sys_fstat },			/* 91 = fstat */
	{ 3, s(struct osf1_sys_fcntl_args), 0,
	    osf1_sys_fcntl },			/* 92 = fcntl */
	{ 5, s(struct osf1_sys_select_args), 0,
	    osf1_sys_select },			/* 93 = select */
	{ 3, s(struct sys_poll_args), 0,
	    sys_poll },				/* 94 = poll */
	{ 1, s(struct sys_fsync_args), 0,
	    sys_fsync },			/* 95 = fsync */
	{ 3, s(struct sys_setpriority_args), 0,
	    sys_setpriority },			/* 96 = setpriority */
	{ 3, s(struct osf1_sys_socket_args), 0,
	    osf1_sys_socket },			/* 97 = socket */
	{ 3, s(struct sys_connect_args), 0,
	    sys_connect },			/* 98 = connect */
	{ 3, s(struct compat_43_sys_accept_args), 0,
	    compat_43_sys_accept },		/* 99 = accept */
	{ 2, s(struct sys_getpriority_args), 0,
	    sys_getpriority },			/* 100 = getpriority */
	{ 4, s(struct compat_43_sys_send_args), 0,
	    compat_43_sys_send },		/* 101 = send */
	{ 4, s(struct compat_43_sys_recv_args), 0,
	    compat_43_sys_recv },		/* 102 = recv */
	{ 1, s(struct sys_sigreturn_args), 0,
	    sys_sigreturn },			/* 103 = sigreturn */
	{ 3, s(struct sys_bind_args), 0,
	    sys_bind },				/* 104 = bind */
	{ 5, s(struct sys_setsockopt_args), 0,
	    sys_setsockopt },			/* 105 = setsockopt */
	{ 2, s(struct sys_listen_args), 0,
	    sys_listen },			/* 106 = listen */
	{ 0, 0, 0,
	    sys_nosys },			/* 107 = unimplemented plock */
	{ 0, 0, 0,
	    sys_nosys },			/* 108 = unimplemented old sigvec */
	{ 0, 0, 0,
	    sys_nosys },			/* 109 = unimplemented old sigblock */
	{ 0, 0, 0,
	    sys_nosys },			/* 110 = unimplemented old sigsetmask */
	{ 1, s(struct sys_sigsuspend_args), 0,
	    sys_sigsuspend },			/* 111 = sigsuspend */
	{ 2, s(struct compat_43_sys_sigstack_args), 0,
	    compat_43_sys_sigstack },		/* 112 = sigstack */
	{ 0, 0, 0,
	    sys_nosys },			/* 113 = unimplemented old recvmsg */
	{ 0, 0, 0,
	    sys_nosys },			/* 114 = unimplemented old sendmsg */
	{ 0, 0, 0,
	    sys_nosys },			/* 115 = obsolete vtrace */
	{ 2, s(struct osf1_sys_gettimeofday_args), 0,
	    osf1_sys_gettimeofday },		/* 116 = gettimeofday */
	{ 2, s(struct osf1_sys_getrusage_args), 0,
	    osf1_sys_getrusage },		/* 117 = getrusage */
	{ 5, s(struct sys_getsockopt_args), 0,
	    sys_getsockopt },			/* 118 = getsockopt */
	{ 0, 0, 0,
	    sys_nosys },			/* 119 = unimplemented */
	{ 3, s(struct osf1_sys_readv_args), 0,
	    osf1_sys_readv },			/* 120 = readv */
	{ 3, s(struct osf1_sys_writev_args), 0,
	    osf1_sys_writev },			/* 121 = writev */
	{ 2, s(struct osf1_sys_settimeofday_args), 0,
	    osf1_sys_settimeofday },		/* 122 = settimeofday */
	{ 3, s(struct sys_fchown_args), 0,
	    sys_fchown },			/* 123 = fchown */
	{ 2, s(struct sys_fchmod_args), 0,
	    sys_fchmod },			/* 124 = fchmod */
	{ 6, s(struct compat_43_sys_recvfrom_args), 0,
	    compat_43_sys_recvfrom },		/* 125 = recvfrom */
	{ 2, s(struct sys_setreuid_args), 0,
	    sys_setreuid },			/* 126 = setreuid */
	{ 2, s(struct sys_setregid_args), 0,
	    sys_setregid },			/* 127 = setregid */
	{ 2, s(struct sys_rename_args), 0,
	    sys_rename },			/* 128 = rename */
	{ 2, s(struct osf1_sys_truncate_args), 0,
	    osf1_sys_truncate },		/* 129 = truncate */
	{ 2, s(struct osf1_sys_ftruncate_args), 0,
	    osf1_sys_ftruncate },		/* 130 = ftruncate */
	{ 0, 0, 0,
	    sys_nosys },			/* 131 = unimplemented flock */
	{ 1, s(struct osf1_sys_setgid_args), 0,
	    osf1_sys_setgid },			/* 132 = setgid */
	{ 6, s(struct osf1_sys_sendto_args), 0,
	    osf1_sys_sendto },			/* 133 = sendto */
	{ 2, s(struct sys_shutdown_args), 0,
	    sys_shutdown },			/* 134 = shutdown */
	{ 4, s(struct osf1_sys_socketpair_args), 0,
	    osf1_sys_socketpair },		/* 135 = socketpair */
	{ 2, s(struct sys_mkdir_args), 0,
	    sys_mkdir },			/* 136 = mkdir */
	{ 1, s(struct sys_rmdir_args), 0,
	    sys_rmdir },			/* 137 = rmdir */
	{ 2, s(struct osf1_sys_utimes_args), 0,
	    osf1_sys_utimes },			/* 138 = utimes */
	{ 0, 0, 0,
	    sys_nosys },			/* 139 = obsolete 4.2 sigreturn */
	{ 0, 0, 0,
	    sys_nosys },			/* 140 = unimplemented adjtime */
	{ 3, s(struct compat_43_sys_getpeername_args), 0,
	    compat_43_sys_getpeername },	/* 141 = getpeername */
	{ 0, 0, 0,
	    compat_43_sys_gethostid },		/* 142 = gethostid */
	{ 1, s(struct compat_43_sys_sethostid_args), 0,
	    compat_43_sys_sethostid },		/* 143 = sethostid */
	{ 2, s(struct osf1_sys_getrlimit_args), 0,
	    osf1_sys_getrlimit },		/* 144 = getrlimit */
	{ 2, s(struct osf1_sys_setrlimit_args), 0,
	    osf1_sys_setrlimit },		/* 145 = setrlimit */
	{ 0, 0, 0,
	    sys_nosys },			/* 146 = unimplemented old killpg */
	{ 0, 0, 0,
	    sys_setsid },			/* 147 = setsid */
	{ 0, 0, 0,
	    sys_nosys },			/* 148 = unimplemented quotactl */
	{ 0, 0, 0,
	    compat_43_sys_quota },		/* 149 = quota */
	{ 3, s(struct compat_43_sys_getsockname_args), 0,
	    compat_43_sys_getsockname },	/* 150 = getsockname */
	{ 0, 0, 0,
	    sys_nosys },			/* 151 = unimplemented pread */
	{ 0, 0, 0,
	    sys_nosys },			/* 152 = unimplemented pwrite */
	{ 0, 0, 0,
	    sys_nosys },			/* 153 = unimplemented pid_block */
	{ 0, 0, 0,
	    sys_nosys },			/* 154 = unimplemented pid_unblock */
	{ 0, 0, 0,
	    sys_nosys },			/* 155 = unimplemented signal_urti */
	{ 3, s(struct osf1_sys_sigaction_args), 0,
	    osf1_sys_sigaction },		/* 156 = sigaction */
	{ 0, 0, 0,
	    sys_nosys },			/* 157 = unimplemented sigwaitprim */
	{ 0, 0, 0,
	    sys_nosys },			/* 158 = unimplemented nfssvc */
	{ 4, s(struct compat_43_sys_getdirentries_args), 0,
	    compat_43_sys_getdirentries },	/* 159 = getdirentries */
	{ 3, s(struct osf1_sys_statfs_args), 0,
	    osf1_sys_statfs },			/* 160 = statfs */
	{ 3, s(struct osf1_sys_fstatfs_args), 0,
	    osf1_sys_fstatfs },			/* 161 = fstatfs */
	{ 0, 0, 0,
	    sys_nosys },			/* 162 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 163 = unimplemented async_daemon */
	{ 0, 0, 0,
	    sys_nosys },			/* 164 = unimplemented getfh */
	{ 2, s(struct compat_09_sys_getdomainname_args), 0,
	    compat_09_sys_getdomainname },	/* 165 = getdomainname */
	{ 2, s(struct compat_09_sys_setdomainname_args), 0,
	    compat_09_sys_setdomainname },	/* 166 = setdomainname */
	{ 0, 0, 0,
	    sys_nosys },			/* 167 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 168 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 169 = unimplemented exportfs */
	{ 0, 0, 0,
	    sys_nosys },			/* 170 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 171 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 172 = unimplemented alt msgctl */
	{ 0, 0, 0,
	    sys_nosys },			/* 173 = unimplemented alt msgget */
	{ 0, 0, 0,
	    sys_nosys },			/* 174 = unimplemented alt msgrcv */
	{ 0, 0, 0,
	    sys_nosys },			/* 175 = unimplemented alt msgsnd */
	{ 0, 0, 0,
	    sys_nosys },			/* 176 = unimplemented alt semctl */
	{ 0, 0, 0,
	    sys_nosys },			/* 177 = unimplemented alt semget */
	{ 0, 0, 0,
	    sys_nosys },			/* 178 = unimplemented alt semop */
	{ 0, 0, 0,
	    sys_nosys },			/* 179 = unimplemented alt uname */
	{ 0, 0, 0,
	    sys_nosys },			/* 180 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 181 = unimplemented alt plock */
	{ 0, 0, 0,
	    sys_nosys },			/* 182 = unimplemented lockf */
	{ 0, 0, 0,
	    sys_nosys },			/* 183 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 184 = unimplemented getmnt */
	{ 0, 0, 0,
	    sys_nosys },			/* 185 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 186 = unimplemented unmount */
	{ 0, 0, 0,
	    sys_nosys },			/* 187 = unimplemented alt sigpending */
	{ 0, 0, 0,
	    sys_nosys },			/* 188 = unimplemented alt setsid */
	{ 0, 0, 0,
	    sys_nosys },			/* 189 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 190 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 191 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 192 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 193 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 194 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 195 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 196 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 197 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 198 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 199 = unimplemented swapon */
	{ 0, 0, 0,
	    sys_nosys },			/* 200 = unimplemented msgctl */
	{ 0, 0, 0,
	    sys_nosys },			/* 201 = unimplemented msgget */
	{ 0, 0, 0,
	    sys_nosys },			/* 202 = unimplemented msgrcv */
	{ 0, 0, 0,
	    sys_nosys },			/* 203 = unimplemented msgsnd */
	{ 0, 0, 0,
	    sys_nosys },			/* 204 = unimplemented semctl */
	{ 0, 0, 0,
	    sys_nosys },			/* 205 = unimplemented semget */
	{ 0, 0, 0,
	    sys_nosys },			/* 206 = unimplemented semop */
	{ 1, s(struct osf1_sys_uname_args), 0,
	    osf1_sys_uname },			/* 207 = uname */
	{ 3, s(struct sys_lchown_args), 0,
	    sys_lchown },			/* 208 = lchown */
	{ 3, s(struct osf1_sys_shmat_args), 0,
	    osf1_sys_shmat },			/* 209 = shmat */
	{ 3, s(struct osf1_sys_shmctl_args), 0,
	    osf1_sys_shmctl },			/* 210 = shmctl */
	{ 1, s(struct osf1_sys_shmdt_args), 0,
	    osf1_sys_shmdt },			/* 211 = shmdt */
	{ 3, s(struct osf1_sys_shmget_args), 0,
	    osf1_sys_shmget },			/* 212 = shmget */
	{ 0, 0, 0,
	    sys_nosys },			/* 213 = unimplemented mvalid */
	{ 0, 0, 0,
	    sys_nosys },			/* 214 = unimplemented getaddressconf */
	{ 0, 0, 0,
	    sys_nosys },			/* 215 = unimplemented msleep */
	{ 0, 0, 0,
	    sys_nosys },			/* 216 = unimplemented mwakeup */
	{ 0, 0, 0,
	    sys_nosys },			/* 217 = unimplemented msync */
	{ 0, 0, 0,
	    sys_nosys },			/* 218 = unimplemented signal */
	{ 0, 0, 0,
	    sys_nosys },			/* 219 = unimplemented utc gettime */
	{ 0, 0, 0,
	    sys_nosys },			/* 220 = unimplemented utc adjtime */
	{ 0, 0, 0,
	    sys_nosys },			/* 221 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 222 = unimplemented security */
	{ 0, 0, 0,
	    sys_nosys },			/* 223 = unimplemented kloadcall */
	{ 0, 0, 0,
	    sys_nosys },			/* 224 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 225 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 226 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 227 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 228 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 229 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 230 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 231 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 232 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 233 = unimplemented getpgid */
	{ 1, s(struct sys_getsid_args), 0,
	    sys_getsid },			/* 234 = getsid */
	{ 2, s(struct osf1_sys_sigaltstack_args), 0,
	    osf1_sys_sigaltstack },		/* 235 = sigaltstack */
	{ 0, 0, 0,
	    sys_nosys },			/* 236 = unimplemented waitid */
	{ 0, 0, 0,
	    sys_nosys },			/* 237 = unimplemented priocntlset */
	{ 0, 0, 0,
	    sys_nosys },			/* 238 = unimplemented sigsendset */
	{ 0, 0, 0,
	    sys_nosys },			/* 239 = unimplemented set_speculative */
	{ 0, 0, 0,
	    sys_nosys },			/* 240 = unimplemented msfs_syscall */
	{ 3, s(struct osf1_sys_sysinfo_args), 0,
	    osf1_sys_sysinfo },			/* 241 = sysinfo */
	{ 0, 0, 0,
	    sys_nosys },			/* 242 = unimplemented uadmin */
	{ 0, 0, 0,
	    sys_nosys },			/* 243 = unimplemented fuser */
	{ 0, 0, 0,
	    sys_nosys },			/* 244 = unimplemented proplist_syscall */
	{ 0, 0, 0,
	    sys_nosys },			/* 245 = unimplemented ntp_adjtime */
	{ 0, 0, 0,
	    sys_nosys },			/* 246 = unimplemented ntp_gettime */
	{ 2, s(struct osf1_sys_pathconf_args), 0,
	    osf1_sys_pathconf },		/* 247 = pathconf */
	{ 2, s(struct osf1_sys_fpathconf_args), 0,
	    osf1_sys_fpathconf },		/* 248 = fpathconf */
	{ 0, 0, 0,
	    sys_nosys },			/* 249 = unimplemented */
	{ 0, 0, 0,
	    sys_nosys },			/* 250 = unimplemented uswitch */
	{ 2, s(struct osf1_sys_usleep_thread_args), 0,
	    osf1_sys_usleep_thread },		/* 251 = usleep_thread */
	{ 0, 0, 0,
	    sys_nosys },			/* 252 = unimplemented audcntl */
	{ 0, 0, 0,
	    sys_nosys },			/* 253 = unimplemented audgen */
	{ 0, 0, 0,
	    sys_nosys },			/* 254 = unimplemented sysfs */
	{ 0, 0, 0,
	    sys_nosys },			/* 255 = unimplemented subsys_info */
	{ 0, 0, 0,
	    sys_nosys },			/* 256 = unimplemented getsysinfo */
	{ 5, s(struct osf1_sys_setsysinfo_args), 0,
	    osf1_sys_setsysinfo },		/* 257 = setsysinfo */
	{ 0, 0, 0,
	    sys_nosys },			/* 258 = unimplemented afs_syscall */
	{ 0, 0, 0,
	    sys_nosys },			/* 259 = unimplemented swapctl */
	{ 0, 0, 0,
	    sys_nosys },			/* 260 = unimplemented memcntl */
	{ 0, 0, 0,
	    sys_nosys },			/* 261 = unimplemented fdatasync */
};

