/* This is a generated file. DO NOT EDIT. */
/*
 * Generated from:
 *
 *	OpenBSD: bcopy.m4
 */
/*
 * Copyright (c) 1999,2004 Michael Shalayeff
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#undef _LOCORE
#define _LOCORE
#include <machine/asm.h>
#include <machine/frame.h>


LEAF_ENTRY(bcopy)
	copy	%arg0, %ret0
	copy	%arg1, %arg0
	copy	%ret0, %arg1
ALTENTRY(memmove)
	cmpb,*>,n %arg0, %arg1, $bcopy.reverse
ALTENTRY(memcpy)
	copy	%arg0, %ret0
	
	cmpib,*>=,n 15, %arg2, $bcopy_f.byte

	extrd,u	%arg1, 63, 2, %r20
	extrd,u	%arg0, 63, 2, %r19
	add	%arg2, %r19, %arg2
	cmpb,*<> %r20, %r19, $bcopy_f.unaligned
	depd	%r0, 63, 2, %arg1
	
	addi	-16, %arg2, %arg2
$bcopy_f.loop16a
	ldw	 32(%sr0, %arg1), %r0
	ldw,ma	4(%sr0, %arg1), %r22
	ldw,ma	4(%sr0, %arg1), %r21
	ldw,ma	4(%sr0, %arg1), %r20
	ldw,ma	4(%sr0, %arg1), %r19
	stby,b,m %r22, 4(%sr0, %arg0)
	stw,ma	%r21, 4(%sr0, %arg0)
	stw,ma	%r20, 4(%sr0, %arg0)
	addib,*>= -16, %arg2, $bcopy_f.loop16a
	stw,ma	%r19, 4(%sr0, %arg0)

	addib,*<,n 12, %arg2, $bcopy_f.cleanup
$bcopy_f.word
	ldw,ma	4(%sr0, %arg1), %r22
	addib,*>= -4, %arg2, $bcopy_f.word
	stw,ma	%r22, 4(%sr0, %arg0)

$bcopy_f.cleanup
	addib,*=,n 4, %arg2, $bcopy_f.done
	ldw	0(%sr0, %arg1), %r22
	add	%arg0, %arg2, %arg0
	b	$bcopy_f.done
	stby,e	%r22, 0(%sr0, %arg0)

$bcopy_f.unaligned
	sub,*>=	%r19, %r20, %r21
	ldw,ma	4(%sr0, %arg1), %ret1
	depd,z	%r21, 60, 61, %r22
	mtsar	%r22
	
	addi	-16, %arg2, %arg2
$bcopy_f.loop16u
	ldw	 32(%sr0, %arg1), %r0
	ldw,ma	4(%sr0, %arg1), %r22
	ldw,ma	4(%sr0, %arg1), %r21
	ldw,ma	4(%sr0, %arg1), %r20
	ldw,ma	4(%sr0, %arg1), %r19
	shrpw	%ret1, %r22, %sar, %r31
	stby,b,m %r31, 4(%sr0, %arg0)
	shrpw	%r22, %r21, %sar, %r31
	stw,ma	%r31, 4(%sr0, %arg0)
	shrpw	%r21, %r20, %sar, %r31
	stw,ma	%r31, 4(%sr0, %arg0)
	shrpw	%r20, %r19, %sar, %r31
	stw,ma	%r31, 4(%sr0, %arg0)
	addib,*>= -16, %arg2, $bcopy_f.loop16u
	copy	%r19, %ret1

	addib,*<,n 12, %arg2, $bcopy_f.cleanup_un
$bcopy_f.word_un
	ldw,ma	4(%sr0, %arg1), %r22
	shrpw	%ret1, %r22, %sar, %r21
	addib,*< -4, %arg2, $bcopy_f.cleanup1_un
	stw,ma	%r21, 4(%sr0, %arg0)
	ldw,ma	4(%sr0, %arg1), %ret1
	shrpw	%r22, %ret1, %sar, %r21
	addib,*>= -4, %arg2, $bcopy_f.word_un
	stw,ma	%r21, 4(%sr0, %arg0)

$bcopy_f.cleanup_un
	addib,*<=,n 4, %arg2, $bcopy_f.done
	mfctl	%sar, %r19
	add	%arg0, %arg2, %arg0
	extrd,u	%r19, 60, 2, %r19
	sub,*<=	%arg2, %r19, %r0
	ldw,ma	4(%sr0, %arg1), %r22
	shrpw	%ret1, %r22, %sar, %r21
	b	$bcopy_f.done
	stby,e	%r21, 0(%sr0, %arg0)

$bcopy_f.cleanup1_un
	b	$bcopy_f.cleanup_un
	copy	%r22, %ret1

$bcopy_f.byte
	cmpb,*>=,n %r0, %arg2, $bcopy_f.done
$bcopy_f.byte_loop
	ldbs,ma	1(%sr0, %arg1), %r22
	addib,*<> -1, %arg2, $bcopy_f.byte_loop
	stbs,ma	%r22, 1(%sr0, %arg0)
$bcopy_f.done

	bv	%r0(%rp)
	nop
$bcopy.reverse
	copy	%arg0, %ret0
	add	%arg1, %arg2, %arg1
	add	%arg0, %arg2, %arg0


$bcopy_r.byte
	cmpb,*>=,n %r0, %arg2, $bcopy_r.done
$bcopy_r.byte_loop
	ldbs,mb	-1(%sr0, %arg1), %r22
	addib,*<> -1, %arg2, $bcopy_r.byte_loop
	stbs,mb	%r22, -1(%sr0, %arg0)
$bcopy_r.done

	bv	%r0(%rp)
	nop
EXIT(bcopy)

	.end
