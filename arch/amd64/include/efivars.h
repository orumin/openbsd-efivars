/*	$OpenBSD$	*/

/*
 * Pentium performance counter driver for OpenBSD.
 * Copyright 2015 KANATSU Minoru <dev@orum.in>.
 *
 * Modification and redistribution in source and binary forms is
 * permitted provided that due credit is given to the author and the
 * OpenBSD project by leaving this copyright notice intact.
 */

#ifndef _MACHINE_EFIVARS_H_
#define _MACHINE_EFIVARS_H_

struct efivars_st {
};

#define _PATH_EFIVARS	"/dev/efivars"

#define EFI_SET_VARIABLE 0
#define EFI_GET_VARIABLE 1

void	efivarsattach(int);
int	efivarsopen(dev_t, int, int, struct proc *);
int	efivarsclose(dev_t, int, int, struct proc *);
int	efivarsioctl(dev_t, u_long, caddr_t, int, struct proc *);

#endif /* ! _MACHINE_EFIVARS_H_ */
