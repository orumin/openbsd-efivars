/*	$OpenBSD: osf1_syscall.h,v 1.6 1999/06/07 07:18:35 deraadt Exp $	*/

/*
 * System call numbers.
 *
 * DO NOT EDIT-- this file is automatically generated.
 * created from	OpenBSD: syscalls.master,v 1.6 1999/06/07 07:17:47 deraadt Exp 
 */

/* syscall: "syscall" ret: "int" args: */
#define	OSF1_SYS_syscall	0

/* syscall: "exit" ret: "int" args: "int" */
#define	OSF1_SYS_exit	1

/* syscall: "fork" ret: "int" args: */
#define	OSF1_SYS_fork	2

/* syscall: "read" ret: "int" args: "int" "char *" "u_int" */
#define	OSF1_SYS_read	3

/* syscall: "write" ret: "int" args: "int" "char *" "u_int" */
#define	OSF1_SYS_write	4

/* syscall: "close" ret: "int" args: "int" */
#define	OSF1_SYS_close	6

/* syscall: "wait4" ret: "int" args: "int" "int *" "int" "struct rusage *" */
#define	OSF1_SYS_wait4	7

/* syscall: "link" ret: "int" args: "char *" "char *" */
#define	OSF1_SYS_link	9

/* syscall: "unlink" ret: "int" args: "char *" */
#define	OSF1_SYS_unlink	10

/* syscall: "chdir" ret: "int" args: "char *" */
#define	OSF1_SYS_chdir	12

/* syscall: "fchdir" ret: "int" args: "int" */
#define	OSF1_SYS_fchdir	13

/* syscall: "mknod" ret: "int" args: "char *" "int" "int" */
#define	OSF1_SYS_mknod	14

/* syscall: "chmod" ret: "int" args: "char *" "int" */
#define	OSF1_SYS_chmod	15

/* syscall: "chown" ret: "int" args: "char *" "int" "int" */
#define	OSF1_SYS_chown	16

/* syscall: "obreak" ret: "int" args: "char *" */
#define	OSF1_SYS_obreak	17

/* syscall: "getfsstat" ret: "int" args: "struct osf1_statfs *" "long" "int" */
#define	OSF1_SYS_getfsstat	18

/* syscall: "lseek" ret: "off_t" args: "int" "off_t" "int" */
#define	OSF1_SYS_lseek	19

/* syscall: "getpid" ret: "pid_t" args: */
#define	OSF1_SYS_getpid	20

/* syscall: "mount" ret: "int" args: "int" "char *" "int" "caddr_t" */
#define	OSF1_SYS_mount	21

/* syscall: "unmount" ret: "int" args: "char *" "int" */
#define	OSF1_SYS_unmount	22

/* syscall: "setuid" ret: "int" args: "uid_t" */
#define	OSF1_SYS_setuid	23

/* syscall: "getuid" ret: "uid_t" args: */
#define	OSF1_SYS_getuid	24

/* syscall: "access" ret: "int" args: "char *" "int" */
#define	OSF1_SYS_access	33

/* syscall: "sync" ret: "int" args: */
#define	OSF1_SYS_sync	36

/* syscall: "kill" ret: "int" args: "int" "int" */
#define	OSF1_SYS_kill	37

/* syscall: "setpgid" ret: "int" args: "int" "int" */
#define	OSF1_SYS_setpgid	39

/* syscall: "dup" ret: "int" args: "u_int" */
#define	OSF1_SYS_dup	41

/* syscall: "opipe" ret: "int" args: */
#define	OSF1_SYS_opipe	42

/* syscall: "open" ret: "int" args: "char *" "int" "int" */
#define	OSF1_SYS_open	45

				/* 46 is obsolete sigaction */
/* syscall: "getgid" ret: "gid_t" args: */
#define	OSF1_SYS_getgid	47

/* syscall: "sigprocmask" ret: "int" args: "int" "sigset_t" */
#define	OSF1_SYS_sigprocmask	48

/* syscall: "getlogin" ret: "int" args: "char *" "u_int" */
#define	OSF1_SYS_getlogin	49

/* syscall: "setlogin" ret: "int" args: "char *" */
#define	OSF1_SYS_setlogin	50

/* syscall: "acct" ret: "int" args: "char *" */
#define	OSF1_SYS_acct	51

/* syscall: "ioctl" ret: "int" args: "int" "int" "caddr_t" */
#define	OSF1_SYS_ioctl	54

/* syscall: "reboot" ret: "int" args: "int" */
#define	OSF1_SYS_reboot	55

/* syscall: "revoke" ret: "int" args: "char *" */
#define	OSF1_SYS_revoke	56

/* syscall: "symlink" ret: "int" args: "char *" "char *" */
#define	OSF1_SYS_symlink	57

/* syscall: "readlink" ret: "int" args: "char *" "char *" "int" */
#define	OSF1_SYS_readlink	58

/* syscall: "execve" ret: "int" args: "char *" "char **" "char **" */
#define	OSF1_SYS_execve	59

/* syscall: "umask" ret: "int" args: "int" */
#define	OSF1_SYS_umask	60

/* syscall: "chroot" ret: "int" args: "char *" */
#define	OSF1_SYS_chroot	61

/* syscall: "getpgrp" ret: "int" args: */
#define	OSF1_SYS_getpgrp	63

/* syscall: "getpagesize" ret: "int" args: */
#define	OSF1_SYS_getpagesize	64

/* syscall: "vfork" ret: "int" args: */
#define	OSF1_SYS_vfork	66

/* syscall: "stat" ret: "int" args: "char *" "struct osf1_stat *" */
#define	OSF1_SYS_stat	67

/* syscall: "lstat" ret: "int" args: "char *" "struct osf1_stat *" */
#define	OSF1_SYS_lstat	68

/* syscall: "mmap" ret: "caddr_t" args: "caddr_t" "size_t" "int" "int" "int" "off_t" */
#define	OSF1_SYS_mmap	71

/* syscall: "munmap" ret: "int" args: "caddr_t" "size_t" */
#define	OSF1_SYS_munmap	73

/* syscall: "madvise" ret: "int" args: */
#define	OSF1_SYS_madvise	75

/* syscall: "getgroups" ret: "int" args: "u_int" "gid_t *" */
#define	OSF1_SYS_getgroups	79

/* syscall: "setgroups" ret: "int" args: "u_int" "gid_t *" */
#define	OSF1_SYS_setgroups	80

/* syscall: "setpgrp" ret: "int" args: "int" "int" */
#define	OSF1_SYS_setpgrp	82

/* syscall: "setitimer" ret: "int" args: "u_int" "struct itimerval *" "struct itimerval *" */
#define	OSF1_SYS_setitimer	83

/* syscall: "gethostname" ret: "int" args: "char *" "u_int" */
#define	OSF1_SYS_gethostname	87

/* syscall: "sethostname" ret: "int" args: "char *" "u_int" */
#define	OSF1_SYS_sethostname	88

/* syscall: "getdtablesize" ret: "int" args: */
#define	OSF1_SYS_getdtablesize	89

/* syscall: "dup2" ret: "int" args: "u_int" "u_int" */
#define	OSF1_SYS_dup2	90

/* syscall: "fstat" ret: "int" args: "int" "void *" */
#define	OSF1_SYS_fstat	91

/* syscall: "fcntl" ret: "int" args: "int" "int" "void *" */
#define	OSF1_SYS_fcntl	92

/* syscall: "select" ret: "int" args: "u_int" "fd_set *" "fd_set *" "fd_set *" "struct timeval *" */
#define	OSF1_SYS_select	93

/* syscall: "poll" ret: "int" args: "struct pollfd *" "unsigned int" "int" */
#define	OSF1_SYS_poll	94

/* syscall: "fsync" ret: "int" args: "int" */
#define	OSF1_SYS_fsync	95

/* syscall: "setpriority" ret: "int" args: "int" "int" "int" */
#define	OSF1_SYS_setpriority	96

/* syscall: "socket" ret: "int" args: "int" "int" "int" */
#define	OSF1_SYS_socket	97

/* syscall: "connect" ret: "int" args: "int" "caddr_t" "int" */
#define	OSF1_SYS_connect	98

/* syscall: "getpriority" ret: "int" args: "int" "int" */
#define	OSF1_SYS_getpriority	100

/* syscall: "send" ret: "int" args: "int" "caddr_t" "int" "int" */
#define	OSF1_SYS_send	101

/* syscall: "recv" ret: "int" args: "int" "caddr_t" "int" "int" */
#define	OSF1_SYS_recv	102

/* syscall: "sigreturn" ret: "int" args: "struct sigcontext *" */
#define	OSF1_SYS_sigreturn	103

/* syscall: "bind" ret: "int" args: "int" "caddr_t" "int" */
#define	OSF1_SYS_bind	104

/* syscall: "setsockopt" ret: "int" args: "int" "int" "int" "caddr_t" "int" */
#define	OSF1_SYS_setsockopt	105

/* syscall: "sigsuspend" ret: "int" args: "int" */
#define	OSF1_SYS_sigsuspend	111

/* syscall: "sigstack" ret: "int" args: "struct sigstack *" "struct sigstack *" */
#define	OSF1_SYS_sigstack	112

/* syscall: "gettimeofday" ret: "int" args: "struct timeval *" "struct timezone *" */
#define	OSF1_SYS_gettimeofday	116

/* syscall: "getrusage" ret: "int" args: */
#define	OSF1_SYS_getrusage	117

/* syscall: "getsockopt" ret: "int" args: "int" "int" "int" "caddr_t" "int *" */
#define	OSF1_SYS_getsockopt	118

/* syscall: "readv" ret: "int" args: "int" "struct osf1_iovec *" "u_int" */
#define	OSF1_SYS_readv	120

/* syscall: "writev" ret: "int" args: "int" "struct osf1_iovec *" "u_int" */
#define	OSF1_SYS_writev	121

/* syscall: "settimeofday" ret: "int" args: "struct timeval *" "struct timezone *" */
#define	OSF1_SYS_settimeofday	122

/* syscall: "fchown" ret: "int" args: "int" "int" "int" */
#define	OSF1_SYS_fchown	123

/* syscall: "fchmod" ret: "int" args: "int" "int" */
#define	OSF1_SYS_fchmod	124

/* syscall: "recvfrom" ret: "int" args: "int" "caddr_t" "size_t" "int" "caddr_t" "int *" */
#define	OSF1_SYS_recvfrom	125

/* syscall: "rename" ret: "int" args: "char *" "char *" */
#define	OSF1_SYS_rename	128

/* syscall: "truncate" ret: "int" args: "char *" "off_t" */
#define	OSF1_SYS_truncate	129

/* syscall: "ftruncate" ret: "int" args: "int" "off_t" */
#define	OSF1_SYS_ftruncate	130

/* syscall: "setgid" ret: "int" args: "gid_t" */
#define	OSF1_SYS_setgid	132

/* syscall: "sendto" ret: "int" args: "int" "const void *" "size_t" "int" "const struct sockaddr *" "int" */
#define	OSF1_SYS_sendto	133

/* syscall: "shutdown" ret: "int" args: "int" "int" */
#define	OSF1_SYS_shutdown	134

/* syscall: "mkdir" ret: "int" args: "char *" "int" */
#define	OSF1_SYS_mkdir	136

/* syscall: "rmdir" ret: "int" args: "char *" */
#define	OSF1_SYS_rmdir	137

/* syscall: "utimes" ret: "int" args: "char *" "struct timeval *" */
#define	OSF1_SYS_utimes	138

				/* 139 is obsolete 4.2 sigreturn */
/* syscall: "gethostid" ret: "int32_t" args: */
#define	OSF1_SYS_gethostid	142

/* syscall: "sethostid" ret: "int" args: "int32_t" */
#define	OSF1_SYS_sethostid	143

/* syscall: "getrlimit" ret: "int" args: "u_int" "struct rlimit *" */
#define	OSF1_SYS_getrlimit	144

/* syscall: "setrlimit" ret: "int" args: "u_int" "struct rlimit *" */
#define	OSF1_SYS_setrlimit	145

/* syscall: "setsid" ret: "int" args: */
#define	OSF1_SYS_setsid	147

/* syscall: "quota" ret: "int" args: */
#define	OSF1_SYS_quota	149

/* syscall: "sigaction" ret: "int" args: "int" "struct osf1_sigaction *" "struct osf1_sigaction *" */
#define	OSF1_SYS_sigaction	156

/* syscall: "getdirentries" ret: "int" args: "int" "char *" "u_int" "long *" */
#define	OSF1_SYS_getdirentries	159

/* syscall: "statfs" ret: "int" args: "char *" "struct osf1_statfs *" "int" */
#define	OSF1_SYS_statfs	160

/* syscall: "fstatfs" ret: "int" args: "int" "struct osf1_statfs *" "int" */
#define	OSF1_SYS_fstatfs	161

/* syscall: "lchown" ret: "int" args: "char *" "int" "int" */
#define	OSF1_SYS_lchown	208

/* syscall: "getsid" ret: "pid_t" args: "pid_t" */
#define	OSF1_SYS_getsid	234

/* syscall: "sigaltstack" ret: "int" args: "struct osf1_sigaltstack *" "struct osf1_sigaltstack *" */
#define	OSF1_SYS_sigaltstack	235

/* syscall: "usleep_thread" ret: "int" args: "struct timeval *" "struct timeval *" */
#define	OSF1_SYS_usleep_thread	251

/* syscall: "setsysinfo" ret: "int" args: "u_long" "caddr_t" "u_long" "caddr_t" "u_long" */
#define	OSF1_SYS_setsysinfo	257

#define	OSF1_SYS_MAXSYSCALL	261
