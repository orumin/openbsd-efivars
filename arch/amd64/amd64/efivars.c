/*	$OpenBSD$ */

/*
 * Copyright (c) 2015 KANATSU Minoru <dev@orum.in>
 * All rights reserved.
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

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/fcntl.h>
#include <sys/conf.h>

#include <machine/biosvar.h>
#include <machine/efi/efi.h>
#include <machine/efi/eficall.h>
#include <machine/efivars.h>

EFI_GUID EfiGlobalVariableGuid = EFI_GLOBAL_VARIABLE;

static int efivars_initialized;

	void
efivarsattach(int num)
{
	if (num > 1)
		return;

	if (efivars_initialized || bios_efiinfo != NULL) {
#ifdef EFIVARS_DEBUG
		printf("efivars: initialized\n");
#endif
		efivars_initialized = 1;
	} else
		printf("efivars: invalid EFI SYSTEM TABLE\n");
}

	int
efivarsopen(dev_t dev, int flag, int mode, struct proc *p)
{
	if ((minor(dev) != 0) || (!efivars_initialized))
		return (ENXIO);

	return (0);
}

	int
efivarsclose(dev_t dev, int flag, int mode, struct proc *p)
{
	return (0);
}

	int
efivarsioctl(dev_t dev, u_long cmd, caddr_t data, int flags, struct proc *p)
{
	printf("enter efivars ioctl\n");
	switch(cmd) {
		case EFI_SET_VARIABLE:
			printf("GetVariable: 0x%llx\n", (long long unsigned)bios_efiinfo->efi_get_variable);
			return (0);
		case EFI_GET_VARIABLE:
		{
			UINT8 data[128];
			UINTN data_size = sizeof(data);
			printf("call EFI_GET_VARIABLE\n");
			EFI_CALL(bios_efiinfo->efi_get_variable, (CHAR16 *)L"Boot0000", &EfiGlobalVariableGuid, NULL, &data_size, (void *)data);
			printf("%s\n", data);
			return (0);
		}
		default:
			return (EINVAL);
	}
}

