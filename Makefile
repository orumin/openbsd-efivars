#	$OpenBSD: Makefile,v 1.26 2005/04/27 19:57:37 deraadt Exp $
#	$NetBSD: Makefile,v 1.5 1995/09/15 21:05:21 pk Exp $

SUBDIR=	dev/microcode \
	arch/alpha arch/amd64 arch/cats arch/hp300 \
	arch/hppa arch/hppa64 arch/i386 arch/luna88k \
	arch/m68k arch/mac68k arch/macppc arch/mvme68k \
	arch/mvme88k arch/mvmeppc arch/sgi arch/solbourne \
	arch/sparc arch/sparc64 arch/vax arch/zaurus

.include <bsd.subdir.mk>
