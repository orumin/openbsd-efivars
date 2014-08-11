/*
 * THIS FILE IS AUTOMATICALLY GENERATED
 * DONT EDIT THIS FILE
 */

/*	$OpenBSD: cn30xxbootbusreg.h,v 1.2 2014/08/11 18:29:56 miod Exp $	*/

/*
 * Copyright (c) 2007 Internet Initiative Japan, Inc.
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
 */

/*
 * Cavium Networks OCTEON CN30XX Hardware Reference Manual
 * CN30XX-HM-1.0
 * 12.8 Boot-Bus Registers
 */

#ifndef _CN30XXBOOTBUSREG_H_
#define _CN30XXBOOTBUSREG_H_

/* ---- register addresses */

#define	MIO_BOOT_REG_CFG0			0x0001180000000000ULL
#define	MIO_BOOT_REG_CFG1			0x0001180000000008ULL
#define	MIO_BOOT_REG_CFG2			0x0001180000000010ULL
#define	MIO_BOOT_REG_CFG3			0x0001180000000018ULL
#define	MIO_BOOT_REG_CFG4			0x0001180000000020ULL
#define	MIO_BOOT_REG_CFG5			0x0001180000000028ULL
#define	MIO_BOOT_REG_CFG6			0x0001180000000030ULL
#define	MIO_BOOT_REG_CFG7			0x0001180000000038ULL
#define	MIO_BOOT_REG_TIM0			0x0001180000000040ULL
#define	MIO_BOOT_REG_TIM1			0x0001180000000048ULL
#define	MIO_BOOT_REG_TIM2			0x0001180000000050ULL
#define	MIO_BOOT_REG_TIM3			0x0001180000000058ULL
#define	MIO_BOOT_REG_TIM4			0x0001180000000060ULL
#define	MIO_BOOT_REG_TIM5			0x0001180000000068ULL
#define	MIO_BOOT_REG_TIM6			0x0001180000000070ULL
#define	MIO_BOOT_REG_TIM7			0x0001180000000078ULL
#define	MIO_BOOT_LOC_CFG0			0x0001180000000080ULL
#define	MIO_BOOT_LOC_CFG1			0x0001180000000088ULL
#define	MIO_BOOT_LOC_ADR			0x0001180000000090ULL
#define	MIO_BOOT_LOC_DAT			0x0001180000000098ULL
#define	MIO_BOOT_ERR				0x00011800000000a0ULL
#define	MIO_BOOT_INT				0x00011800000000a8ULL
#define	MIO_BOOT_THR				0x00011800000000b0ULL
#define	MIO_BOOT_BIST_STAT			0x00011800000000f8ULL

/* ---- register bits */

#define	MIO_BOOT_REG_CFGN_XXX_63_37		0xffffffe000000000ULL
#define	MIO_BOOT_REG_CFGN_SAM			0x0000001000000000ULL
#define	MIO_BOOT_REG_CFGN_WE_EXT		0x0000000c00000000ULL
#define	MIO_BOOT_REG_CFGN_OE_EXT		0x0000000300000000ULL
#define	MIO_BOOT_REG_CFGN_EN			0x0000000080000000ULL
#define	MIO_BOOT_REG_CFGN_OR			0x0000000040000000ULL
#define	MIO_BOOT_REG_CFGN_ALE			0x0000000020000000ULL
#define	MIO_BOOT_REG_CFGN_WIDTH			0x0000000010000000ULL
#define	MIO_BOOT_REG_CFGN_SIZE			0x000000000fff0000ULL
#define	MIO_BOOT_REG_CFGN_BASE			0x000000000000ffffULL

#define	MIO_BOOT_REG_TIMN_PAGEM			0x8000000000000000ULL
#define	MIO_BOOT_REG_TIMN_WAITM			0x4000000000000000ULL
#define	MIO_BOOT_REG_TIMN_PAGES			0x3000000000000000ULL
#define	MIO_BOOT_REG_TIMN_ALE			0x0fc0000000000000ULL
#define	MIO_BOOT_REG_TIMN_PAGE			0x003f000000000000ULL
#define	MIO_BOOT_REG_TIMN_WAIT			0x0000fc0000000000ULL
#define	MIO_BOOT_REG_TIMN_PAUSE			0x000003f000000000ULL
#define	MIO_BOOT_REG_TIMN_WR_HLD		0x0000000fc0000000ULL
#define	MIO_BOOT_REG_TIMN_RD_HLD		0x000000003f000000ULL
#define	MIO_BOOT_REG_TIMN_WE			0x0000000000fc0000ULL
#define	MIO_BOOT_REG_TIMN_OE			0x000000000003f000ULL
#define	MIO_BOOT_REG_TIMN_CE			0x0000000000000fc0ULL
#define	MIO_BOOT_REG_TIMN_ADR			0x000000000000003fULL

#define	MIO_BOOT_LOC_CFGN_XXX_63_32		0xffffffff00000000ULL
#define	MIO_BOOT_LOC_CFGN_EN			0x0000000080000000ULL
#define	MIO_BOOT_LOC_CFGN_XXX_30_28		0x0000000070000000ULL
#define	MIO_BOOT_LOC_CFGN_BASE			0x000000000ffffff8ULL
#define	MIO_BOOT_LOC_CFGN_XXX_2_0		0x0000000000000007ULL

#define	MIO_BOOT_LOC_ADR_XXX_63_8		0xffffffffffffff00ULL
#define	MIO_BOOT_LOC_ADR_ADR			0x00000000000000f8ULL
#define	MIO_BOOT_LOC_ADR_XXX_2_0		0x0000000000000007ULL

#define	MIO_BOOT_ERR_XXX_63_2			0xfffffffffffffffcULL
#define	MIO_BOOT_ERR_WAIT_ERR			0x0000000000000002ULL
#define	MIO_BOOT_ERR_ADR_ERR			0x0000000000000001ULL

#define	MIO_BOOT_INT_XXX_63_2			0xfffffffffffffffcULL
#define	MIO_BOOT_INT_WAIT_INT			0x0000000000000002ULL
#define	MIO_BOOT_INT_ADR_INT			0x0000000000000001ULL

#define	MIO_BOOT_THR_XXX_63_14			0xffffffffffffc000ULL
#define	MIO_BOOT_THR_FIF_CNT			0x0000000000003f00ULL
#define	MIO_BOOT_THR_XXX_7_6			0x00000000000000c0ULL
#define	MIO_BOOT_THR_FIF_THR			0x000000000000003fULL

#define	MIO_BOOT_BIST_STAT_XXX_63_4		0xfffffffffffffff0ULL
#define	MIO_BOOT_BIST_STAT_NCBO_1		0x0000000000000008ULL
#define	MIO_BOOT_BIST_STAT_NCBO_0		0x0000000000000004ULL
#define	MIO_BOOT_BIST_STAT_LOC			0x0000000000000002ULL
#define	MIO_BOOT_BIST_STAT_NCBI			0x0000000000000001ULL

/* ---- bus_space */

#define	MIO_BOOT_REG_CFG0_OFFSET		0x0000
#define	MIO_BOOT_REG_CFG1_OFFSET		0x0008
#define	MIO_BOOT_REG_CFG2_OFFSET		0x0010
#define	MIO_BOOT_REG_CFG3_OFFSET		0x0018
#define	MIO_BOOT_REG_CFG4_OFFSET		0x0020
#define	MIO_BOOT_REG_CFG5_OFFSET		0x0028
#define	MIO_BOOT_REG_CFG6_OFFSET		0x0030
#define	MIO_BOOT_REG_CFG7_OFFSET		0x0038
#define	MIO_BOOT_REG_TIM0_OFFSET		0x0040
#define	MIO_BOOT_REG_TIM1_OFFSET		0x0048
#define	MIO_BOOT_REG_TIM2_OFFSET		0x0050
#define	MIO_BOOT_REG_TIM3_OFFSET		0x0058
#define	MIO_BOOT_REG_TIM4_OFFSET		0x0060
#define	MIO_BOOT_REG_TIM5_OFFSET		0x0068
#define	MIO_BOOT_REG_TIM6_OFFSET		0x0070
#define	MIO_BOOT_REG_TIM7_OFFSET		0x0078
#define	MIO_BOOT_LOC_CFG0_OFFSET		0x0080
#define	MIO_BOOT_LOC_CFG1_OFFSET		0x0088
#define	MIO_BOOT_LOC_ADR_OFFSET			0x0090
#define	MIO_BOOT_LOC_DAT_OFFSET			0x0098
#define	MIO_BOOT_ERR_OFFSET			0x00a0
#define	MIO_BOOT_INT_OFFSET			0x00a8
#define	MIO_BOOT_THR_OFFSET			0x00b0
#define	MIO_BOOT_BIST_STAT_OFFSET		0x00f8

#endif /* _CN30XXBOOTBUSREG_H_ */
