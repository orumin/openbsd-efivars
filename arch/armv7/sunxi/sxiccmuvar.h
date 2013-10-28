/*	$OpenBSD: sxiccmuvar.h,v 1.1 2013/10/23 17:08:48 jasper Exp $	*/
/*
 * Copyright (c) 2007,2009 Dale Rahn <drahn@openbsd.org>
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

void sxiccmu_enablemodule(int);

enum CCMU_MODULES {
	CCMU_EHCI0,
	CCMU_EHCI1,
	CCMU_OHCI,
	CCMU_AHCI,
	CCMU_EMAC,
	CCMU_DMA,
	CCMU_UART0,
	CCMU_UART1,
	CCMU_UART2,
	CCMU_UART3,
	CCMU_UART4,
	CCMU_UART5,
	CCMU_UART6,
	CCMU_UART7,
	CCMU_TWI0,
	CCMU_TWI1,
	CCMU_TWI2,
	CCMU_TWI3
};