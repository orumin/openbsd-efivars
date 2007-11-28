/*	$OpenBSD: freebsd_syscallargs.h,v 1.36 2007/11/28 13:48:31 deraadt Exp $	*/

/*
 * System call argument lists.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	OpenBSD: syscalls.master,v 1.32 2007/11/28 13:47:02 deraadt Exp 
 */

#ifdef	syscallarg
#undef	syscallarg
#endif

#define	syscallarg(x)							\
	union {								\
		register_t pad;						\
		struct { x datum; } le;					\
		struct {						\
			int8_t pad[ (sizeof (register_t) < sizeof (x))	\
				? 0					\
				: sizeof (register_t) - sizeof (x)];	\
			x datum;					\
		} be;							\
	}

struct freebsd_sys_open_args {
	syscallarg(char *) path;
	syscallarg(int) flags;
	syscallarg(int) mode;
};

struct compat_43_freebsd_sys_creat_args {
	syscallarg(char *) path;
	syscallarg(int) mode;
};

struct freebsd_sys_link_args {
	syscallarg(char *) path;
	syscallarg(char *) link;
};

struct freebsd_sys_unlink_args {
	syscallarg(char *) path;
};

struct freebsd_sys_chdir_args {
	syscallarg(char *) path;
};

struct freebsd_sys_mknod_args {
	syscallarg(char *) path;
	syscallarg(int) mode;
	syscallarg(int) dev;
};

struct freebsd_sys_chmod_args {
	syscallarg(char *) path;
	syscallarg(int) mode;
};

struct freebsd_sys_chown_args {
	syscallarg(char *) path;
	syscallarg(int) uid;
	syscallarg(int) gid;
};

struct freebsd_sys_getfsstat_args {
	syscallarg(struct freebsd_statfs *) buf;
	syscallarg(long) bufsize;
	syscallarg(int) flags;
};

struct freebsd_sys_mount_args {
	syscallarg(int) type;
	syscallarg(char *) path;
	syscallarg(int) flags;
	syscallarg(caddr_t) data;
};

struct freebsd_sys_unmount_args {
	syscallarg(char *) path;
	syscallarg(int) flags;
};

struct freebsd_sys_ptrace_args {
	syscallarg(int) req;
	syscallarg(pid_t) pid;
	syscallarg(caddr_t) addr;
	syscallarg(int) data;
};

struct freebsd_sys_access_args {
	syscallarg(char *) path;
	syscallarg(int) flags;
};

struct freebsd_sys_chflags_args {
	syscallarg(char *) path;
	syscallarg(int) flags;
};

struct compat_43_freebsd_sys_stat_args {
	syscallarg(char *) path;
	syscallarg(struct stat43 *) ub;
};

struct compat_43_freebsd_sys_lstat_args {
	syscallarg(char *) path;
	syscallarg(struct stat43 *) ub;
};

struct freebsd_sys_ioctl_args {
	syscallarg(int) fd;
	syscallarg(u_long) com;
	syscallarg(caddr_t) data;
};

struct freebsd_sys_revoke_args {
	syscallarg(char *) path;
};

struct freebsd_sys_symlink_args {
	syscallarg(char *) path;
	syscallarg(char *) link;
};

struct freebsd_sys_readlink_args {
	syscallarg(char *) path;
	syscallarg(char *) buf;
	syscallarg(int) count;
};

struct freebsd_sys_execve_args {
	syscallarg(char *) path;
	syscallarg(char **) argp;
	syscallarg(char **) envp;
};

struct freebsd_sys_chroot_args {
	syscallarg(char *) path;
};

struct freebsd_sys_madvise_args {
	syscallarg(caddr_t) addr;
	syscallarg(size_t) len;
	syscallarg(int) behav;
};

struct freebsd_sys_fcntl_args {
	syscallarg(int) fd;
	syscallarg(int) cmd;
	syscallarg(void *) arg;
};

struct freebsd_sys_sigreturn_args {
	syscallarg(struct freebsd_sigcontext *) scp;
};

struct freebsd_sys_rename_args {
	syscallarg(char *) from;
	syscallarg(char *) to;
};

struct compat_43_freebsd_sys_truncate_args {
	syscallarg(char *) path;
	syscallarg(long) length;
};

struct freebsd_sys_mkfifo_args {
	syscallarg(char *) path;
	syscallarg(int) mode;
};

struct freebsd_sys_mkdir_args {
	syscallarg(char *) path;
	syscallarg(int) mode;
};

struct freebsd_sys_rmdir_args {
	syscallarg(char *) path;
};

struct freebsd_sys_statfs_args {
	syscallarg(char *) path;
	syscallarg(struct freebsd_statfs *) buf;
};

struct freebsd_sys_fstatfs_args {
	syscallarg(int) fd;
	syscallarg(struct freebsd_statfs *) buf;
};

struct freebsd_sys_getfh_args {
	syscallarg(char *) fname;
	syscallarg(fhandle_t *) fhp;
};

struct compat_freebsd_sys_uname_args {
	syscallarg(struct outsname *) name;
};

struct freebsd_sys_rtprio_args {
	syscallarg(int) function;
	syscallarg(pid_t) pid;
	syscallarg(struct freebsd_rtprio *) rtp;
};

struct freebsd_sys_stat_args {
	syscallarg(char *) path;
	syscallarg(struct stat35 *) ub;
};

struct freebsd_sys_lstat_args {
	syscallarg(char *) path;
	syscallarg(struct stat35 *) ub;
};

struct freebsd_sys_pathconf_args {
	syscallarg(char *) path;
	syscallarg(int) name;
};

struct freebsd_sys_mmap_args {
	syscallarg(caddr_t) addr;
	syscallarg(size_t) len;
	syscallarg(int) prot;
	syscallarg(int) flags;
	syscallarg(int) fd;
	syscallarg(long) pad;
	syscallarg(off_t) pos;
};

struct freebsd_sys_truncate_args {
	syscallarg(char *) path;
	syscallarg(int) pad;
	syscallarg(off_t) length;
};

struct freebsd_sys_poll2_args {
	syscallarg(struct pollfd *) fds;
	syscallarg(unsigned long) nfds;
	syscallarg(int) timeout;
};

struct freebsd_sys_getdents_args {
	syscallarg(int) fd;
	syscallarg(void *) dirent;
	syscallarg(unsigned) count;
};

struct freebsd_sys_sigprocmask40_args {
	syscallarg(int) how;
	syscallarg(const freebsd_sigset_t *) set;
	syscallarg(freebsd_sigset_t *) oset;
};

struct freebsd_sys_sigsuspend40_args {
	syscallarg(const freebsd_sigset_t *) sigmask;
};

struct freebsd_sys_sigaction40_args {
	syscallarg(int) sig;
	syscallarg(const struct freebsd_sigaction *) act;
	syscallarg(struct freebsd_sigaction *) oact;
};

struct freebsd_sys_sigpending40_args {
	syscallarg(freebsd_sigset_t *) set;
};

/*
 * System call prototypes.
 */

int	sys_nosys(struct proc *, void *, register_t *);
int	sys_exit(struct proc *, void *, register_t *);
int	sys_fork(struct proc *, void *, register_t *);
int	sys_read(struct proc *, void *, register_t *);
int	sys_write(struct proc *, void *, register_t *);
int	freebsd_sys_open(struct proc *, void *, register_t *);
int	sys_close(struct proc *, void *, register_t *);
int	sys_wait4(struct proc *, void *, register_t *);
int	compat_43_freebsd_sys_creat(struct proc *, void *, register_t *);
int	freebsd_sys_link(struct proc *, void *, register_t *);
int	freebsd_sys_unlink(struct proc *, void *, register_t *);
int	freebsd_sys_chdir(struct proc *, void *, register_t *);
int	sys_fchdir(struct proc *, void *, register_t *);
int	freebsd_sys_mknod(struct proc *, void *, register_t *);
int	freebsd_sys_chmod(struct proc *, void *, register_t *);
int	freebsd_sys_chown(struct proc *, void *, register_t *);
int	sys_obreak(struct proc *, void *, register_t *);
int	freebsd_sys_getfsstat(struct proc *, void *, register_t *);
int	compat_43_sys_lseek(struct proc *, void *, register_t *);
int	sys_getpid(struct proc *, void *, register_t *);
int	freebsd_sys_mount(struct proc *, void *, register_t *);
int	freebsd_sys_unmount(struct proc *, void *, register_t *);
int	sys_setuid(struct proc *, void *, register_t *);
int	sys_getuid(struct proc *, void *, register_t *);
int	sys_geteuid(struct proc *, void *, register_t *);
#ifdef PTRACE
int	freebsd_sys_ptrace(struct proc *, void *, register_t *);
#else
#endif
int	sys_recvmsg(struct proc *, void *, register_t *);
int	sys_sendmsg(struct proc *, void *, register_t *);
int	sys_recvfrom(struct proc *, void *, register_t *);
int	sys_accept(struct proc *, void *, register_t *);
int	sys_getpeername(struct proc *, void *, register_t *);
int	sys_getsockname(struct proc *, void *, register_t *);
int	freebsd_sys_access(struct proc *, void *, register_t *);
int	freebsd_sys_chflags(struct proc *, void *, register_t *);
int	sys_fchflags(struct proc *, void *, register_t *);
int	sys_sync(struct proc *, void *, register_t *);
int	sys_kill(struct proc *, void *, register_t *);
int	compat_43_freebsd_sys_stat(struct proc *, void *, register_t *);
int	sys_getppid(struct proc *, void *, register_t *);
int	compat_43_freebsd_sys_lstat(struct proc *, void *, register_t *);
int	sys_dup(struct proc *, void *, register_t *);
int	sys_opipe(struct proc *, void *, register_t *);
int	sys_getegid(struct proc *, void *, register_t *);
int	sys_profil(struct proc *, void *, register_t *);
#ifdef KTRACE
int	sys_ktrace(struct proc *, void *, register_t *);
#else
#endif
int	sys_sigaction(struct proc *, void *, register_t *);
int	sys_getgid(struct proc *, void *, register_t *);
int	sys_sigprocmask(struct proc *, void *, register_t *);
int	sys_getlogin(struct proc *, void *, register_t *);
int	sys_setlogin(struct proc *, void *, register_t *);
#ifdef ACCOUNTING
int	sys_acct(struct proc *, void *, register_t *);
#else
#endif
int	sys_sigpending(struct proc *, void *, register_t *);
int	sys_sigaltstack(struct proc *, void *, register_t *);
int	freebsd_sys_ioctl(struct proc *, void *, register_t *);
int	sys_reboot(struct proc *, void *, register_t *);
int	freebsd_sys_revoke(struct proc *, void *, register_t *);
int	freebsd_sys_symlink(struct proc *, void *, register_t *);
int	freebsd_sys_readlink(struct proc *, void *, register_t *);
int	freebsd_sys_execve(struct proc *, void *, register_t *);
int	sys_umask(struct proc *, void *, register_t *);
int	freebsd_sys_chroot(struct proc *, void *, register_t *);
int	compat_43_sys_fstat(struct proc *, void *, register_t *);
int	compat_43_sys_getkerninfo(struct proc *, void *, register_t *);
int	compat_43_sys_getpagesize(struct proc *, void *, register_t *);
int	sys_msync(struct proc *, void *, register_t *);
int	sys_vfork(struct proc *, void *, register_t *);
int	sys_sbrk(struct proc *, void *, register_t *);
int	sys_sstk(struct proc *, void *, register_t *);
int	compat_43_sys_mmap(struct proc *, void *, register_t *);
int	sys_ovadvise(struct proc *, void *, register_t *);
int	sys_munmap(struct proc *, void *, register_t *);
int	sys_mprotect(struct proc *, void *, register_t *);
int	freebsd_sys_madvise(struct proc *, void *, register_t *);
int	sys_mincore(struct proc *, void *, register_t *);
int	sys_getgroups(struct proc *, void *, register_t *);
int	sys_setgroups(struct proc *, void *, register_t *);
int	sys_getpgrp(struct proc *, void *, register_t *);
int	sys_setpgid(struct proc *, void *, register_t *);
int	sys_setitimer(struct proc *, void *, register_t *);
int	compat_43_sys_wait(struct proc *, void *, register_t *);
int	compat_25_sys_swapon(struct proc *, void *, register_t *);
int	sys_getitimer(struct proc *, void *, register_t *);
int	compat_43_sys_gethostname(struct proc *, void *, register_t *);
int	compat_43_sys_sethostname(struct proc *, void *, register_t *);
int	compat_43_sys_getdtablesize(struct proc *, void *, register_t *);
int	sys_dup2(struct proc *, void *, register_t *);
int	freebsd_sys_fcntl(struct proc *, void *, register_t *);
int	sys_select(struct proc *, void *, register_t *);
int	sys_fsync(struct proc *, void *, register_t *);
int	sys_setpriority(struct proc *, void *, register_t *);
int	sys_socket(struct proc *, void *, register_t *);
int	sys_connect(struct proc *, void *, register_t *);
int	compat_43_sys_accept(struct proc *, void *, register_t *);
int	sys_getpriority(struct proc *, void *, register_t *);
int	compat_43_sys_send(struct proc *, void *, register_t *);
int	compat_43_sys_recv(struct proc *, void *, register_t *);
int	freebsd_sys_sigreturn(struct proc *, void *, register_t *);
int	sys_bind(struct proc *, void *, register_t *);
int	sys_setsockopt(struct proc *, void *, register_t *);
int	sys_listen(struct proc *, void *, register_t *);
int	compat_43_sys_sigvec(struct proc *, void *, register_t *);
int	compat_43_sys_sigblock(struct proc *, void *, register_t *);
int	compat_43_sys_sigsetmask(struct proc *, void *, register_t *);
int	sys_sigsuspend(struct proc *, void *, register_t *);
int	compat_43_sys_sigstack(struct proc *, void *, register_t *);
int	compat_43_sys_recvmsg(struct proc *, void *, register_t *);
int	compat_43_sys_sendmsg(struct proc *, void *, register_t *);
#ifdef TRACE
int	sys_vtrace(struct proc *, void *, register_t *);
#else
#endif
int	sys_gettimeofday(struct proc *, void *, register_t *);
int	sys_getrusage(struct proc *, void *, register_t *);
int	sys_getsockopt(struct proc *, void *, register_t *);
int	sys_readv(struct proc *, void *, register_t *);
int	sys_writev(struct proc *, void *, register_t *);
int	sys_settimeofday(struct proc *, void *, register_t *);
int	sys_fchown(struct proc *, void *, register_t *);
int	sys_fchmod(struct proc *, void *, register_t *);
int	compat_43_sys_recvfrom(struct proc *, void *, register_t *);
int	sys_setreuid(struct proc *, void *, register_t *);
int	sys_setregid(struct proc *, void *, register_t *);
int	freebsd_sys_rename(struct proc *, void *, register_t *);
int	compat_43_freebsd_sys_truncate(struct proc *, void *, register_t *);
int	compat_43_sys_ftruncate(struct proc *, void *, register_t *);
int	sys_flock(struct proc *, void *, register_t *);
int	freebsd_sys_mkfifo(struct proc *, void *, register_t *);
int	sys_sendto(struct proc *, void *, register_t *);
int	sys_shutdown(struct proc *, void *, register_t *);
int	sys_socketpair(struct proc *, void *, register_t *);
int	freebsd_sys_mkdir(struct proc *, void *, register_t *);
int	freebsd_sys_rmdir(struct proc *, void *, register_t *);
int	sys_utimes(struct proc *, void *, register_t *);
int	sys_adjtime(struct proc *, void *, register_t *);
int	compat_43_sys_getpeername(struct proc *, void *, register_t *);
int	compat_43_sys_gethostid(struct proc *, void *, register_t *);
int	compat_43_sys_sethostid(struct proc *, void *, register_t *);
int	compat_43_sys_getrlimit(struct proc *, void *, register_t *);
int	compat_43_sys_setrlimit(struct proc *, void *, register_t *);
int	compat_43_sys_killpg(struct proc *, void *, register_t *);
int	sys_setsid(struct proc *, void *, register_t *);
int	sys_quotactl(struct proc *, void *, register_t *);
int	compat_43_sys_quota(struct proc *, void *, register_t *);
int	compat_43_sys_getsockname(struct proc *, void *, register_t *);
#if defined(NFSCLIENT) || defined(NFSSERVER)
int	sys_nfssvc(struct proc *, void *, register_t *);
#else
#endif
int	compat_43_sys_getdirentries(struct proc *, void *, register_t *);
int	freebsd_sys_statfs(struct proc *, void *, register_t *);
int	freebsd_sys_fstatfs(struct proc *, void *, register_t *);
#ifdef NFSCLIENT
int	freebsd_sys_getfh(struct proc *, void *, register_t *);
#else
#endif
int	compat_09_sys_getdomainname(struct proc *, void *, register_t *);
int	compat_09_sys_setdomainname(struct proc *, void *, register_t *);
int	compat_freebsd_sys_uname(struct proc *, void *, register_t *);
int	sys_sysarch(struct proc *, void *, register_t *);
int	freebsd_sys_rtprio(struct proc *, void *, register_t *);
#if defined(SYSVSEM) && !defined(alpha)
int	compat_10_sys_semsys(struct proc *, void *, register_t *);
#else
#endif
#if defined(SYSVMSG) && !defined(alpha)
int	compat_10_sys_msgsys(struct proc *, void *, register_t *);
#else
#endif
#if defined(SYSVSHM) && !defined(alpha)
int	compat_10_sys_shmsys(struct proc *, void *, register_t *);
#else
#endif
int	sys_pread(struct proc *, void *, register_t *);
int	sys_pwrite(struct proc *, void *, register_t *);
int	sys_setgid(struct proc *, void *, register_t *);
int	sys_setegid(struct proc *, void *, register_t *);
int	sys_seteuid(struct proc *, void *, register_t *);
int	freebsd_sys_stat(struct proc *, void *, register_t *);
int	compat_35_sys_fstat(struct proc *, void *, register_t *);
int	freebsd_sys_lstat(struct proc *, void *, register_t *);
int	freebsd_sys_pathconf(struct proc *, void *, register_t *);
int	sys_fpathconf(struct proc *, void *, register_t *);
int	sys_getrlimit(struct proc *, void *, register_t *);
int	sys_setrlimit(struct proc *, void *, register_t *);
int	sys_getdirentries(struct proc *, void *, register_t *);
int	freebsd_sys_mmap(struct proc *, void *, register_t *);
int	sys_nosys(struct proc *, void *, register_t *);
int	sys_lseek(struct proc *, void *, register_t *);
int	freebsd_sys_truncate(struct proc *, void *, register_t *);
int	sys_ftruncate(struct proc *, void *, register_t *);
int	sys___sysctl(struct proc *, void *, register_t *);
int	sys_mlock(struct proc *, void *, register_t *);
int	sys_munlock(struct proc *, void *, register_t *);
int	sys_getpgid(struct proc *, void *, register_t *);
int	sys_poll(struct proc *, void *, register_t *);
#ifdef SYSVSEM
int	sys___semctl(struct proc *, void *, register_t *);
int	sys_semget(struct proc *, void *, register_t *);
int	sys_semop(struct proc *, void *, register_t *);
#else
#endif
#ifdef SYSVMSG
int	sys_msgctl(struct proc *, void *, register_t *);
int	sys_msgget(struct proc *, void *, register_t *);
int	sys_msgsnd(struct proc *, void *, register_t *);
int	sys_msgrcv(struct proc *, void *, register_t *);
#else
#endif
#ifdef SYSVSHM
int	sys_shmat(struct proc *, void *, register_t *);
int	sys_shmctl(struct proc *, void *, register_t *);
int	sys_shmdt(struct proc *, void *, register_t *);
int	sys_shmget(struct proc *, void *, register_t *);
#else
#endif
int	sys_clock_gettime(struct proc *, void *, register_t *);
int	sys_nanosleep(struct proc *, void *, register_t *);
int	sys_minherit(struct proc *, void *, register_t *);
int	sys_rfork(struct proc *, void *, register_t *);
int	freebsd_sys_poll2(struct proc *, void *, register_t *);
int	sys_issetugid(struct proc *, void *, register_t *);
int	sys_lchown(struct proc *, void *, register_t *);
int	freebsd_sys_getdents(struct proc *, void *, register_t *);
int	sys_setresuid(struct proc *, void *, register_t *);
int	sys_setresgid(struct proc *, void *, register_t *);
int	freebsd_sys_sigprocmask40(struct proc *, void *, register_t *);
int	freebsd_sys_sigsuspend40(struct proc *, void *, register_t *);
int	freebsd_sys_sigaction40(struct proc *, void *, register_t *);
int	freebsd_sys_sigpending40(struct proc *, void *, register_t *);
int	sys_kqueue(struct proc *, void *, register_t *);
int	sys_kevent(struct proc *, void *, register_t *);
