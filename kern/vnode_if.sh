#!/bin/sh -
copyright="\
/*
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
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
"
SCRIPT_ID='$OpenBSD: vnode_if.sh,v 1.12 2003/05/01 21:41:35 tedu Exp $'
# SCRIPT_ID='$NetBSD: vnode_if.sh,v 1.9 1996/02/29 20:58:22 cgd Exp $'

# Script to produce VFS front-end sugar.
#
# usage: vnode_if.sh srcfile
#	(where srcfile is currently /sys/kern/vnode_if.src)
#

if [ $# -ne 1 ] ; then
	echo 'usage: vnode_if.sh srcfile'
	exit 1
fi

# Name and revision of the source file.
src=$1
SRC_ID=`head -1 $src | sed -e 's/.*\$\(.*\)\$.*/\1/'`

# Names of the created files.
out_c=vnode_if.c
out_h=../sys/vnode_if.h

# Awk program (must support nawk extensions)
# Use "awk" at Berkeley, "nawk" or "gawk" elsewhere.
awk=${AWK:-awk}

# Does this awk have a "toupper" function? (i.e. is it GNU awk)
isgawk=`$awk 'BEGIN { print toupper("true"); exit; }' 2>/dev/null`

# If this awk does not define "toupper" then define our own.
if [ "$isgawk" = TRUE ] ; then
	# GNU awk provides it.
	toupper=
else
	# Provide our own toupper()
	toupper='
function toupper(str) {
	_toupper_cmd = "echo "str" |tr a-z A-Z"
	_toupper_cmd | getline _toupper_str;
	close(_toupper_cmd);
	return _toupper_str;
}'
fi

#
# This is the common part of all awk programs that read $src
# This parses the input for one function into the arrays:
#	argdir, argtype, argname, willrele
# and calls "doit()" to generate output for the function.
#
# Input to this parser is pre-processed slightly by sed
# so this awk parser doesn't have to work so hard.  The
# changes done by the sed pre-processing step are:
#	insert a space beween * and pointer name
#	replace semicolons with spaces
#
sed_prep='s:\*\([^\*/]\):\* \1:g
s/;/ /'
awk_parser='
# Comment line
/^#/	{ next; }
# First line of description
/^vop_/	{
	name=$1;
	argc=0;
	next;
}
# Last line of description
/^}/	{
	doit();
	next;
}
# Middle lines of description
{
	argdir[argc] = $1; i=2;
	if ($2 == "WILLRELE" ||
	    $3 == "WILLRELE") {
		willrele[argc] = 1;
		i++;
	} else if ($2 == "WILLUNLOCK" ||
	    $3 == "WILLUNLOCK") {
		willrele[argc] = 2;
		i++;
	} else if ($2 == "WILLPUT" ||
	    $3 == "WILLPUT") {
		willrele[argc] = 3;
		i++;
	} else
		willrele[argc] = 0;

	if ($2 == "SHOULDBELOCKED") {
	   shouldbelocked[argc] = 1;
	   i++;
	} else
	   shouldbelocked[argc] = 0;

	argtype[argc] = $i; i++;
	while (i < NF) {
		argtype[argc] = argtype[argc]" "$i;
		i++;
	}
	argname[argc] = $i;
	argc++;
	next;
}
'

# This is put after the copyright on each generated file.
warning="\
/*
 * Warning: This file is generated automatically.
 * (Modifications made here may easily be lost!)
 *
 * Created from the file:
 *	${SRC_ID}
 * by the script:
 *	${SCRIPT_ID}
 */
" 

# This is to satisfy McKusick (get rid of evil spaces 8^)
anal_retentive='s:\([^/]\*\) :\1:g'

#
# Redirect stdout to the H file.
#
echo "$0: Creating $out_h" 1>&2
exec > $out_h

# Begin stuff
echo -n "$warning" | sed -e 's/\$//g'
echo ""
echo -n "$copyright"
echo '
extern struct vnodeop_desc vop_default_desc;
'

echo '#include "systm.h"'

# Body stuff
# This awk program needs toupper() so define it if necessary.
sed -e "$sed_prep" $src | $awk "$toupper"'
function doit() {
	# Declare arg struct, descriptor.
	printf("\nstruct %s_args {\n", name);
	printf("\tstruct vnodeop_desc * a_desc;\n");
	for (i=0; i<argc; i++) {
		printf("\t%s a_%s;\n", argtype[i], argname[i]);
	}
	printf("};\n");
	printf("extern struct vnodeop_desc %s_desc;\n", name);
	# Prototype it.
	protoarg = sprintf("int %s(", toupper(name));
	protolen = length(protoarg);
	printf("%s", protoarg);
	for (i=0; i<argc; i++) {
		protoarg = sprintf("%s", argtype[i]);
		if (i < (argc-1)) protoarg = (protoarg ", ");
		arglen = length(protoarg);
		if ((protolen + arglen) > 77) {
			protoarg = ("\n    " protoarg);
			arglen += 4;
			protolen = 0;
		}
		printf("%s", protoarg);
		protolen += arglen;
	}
	printf(");\n");
}
BEGIN	{
	arg0special="";
}
END	{
	printf("\n/* Special cases: */\n#include <sys/buf.h>\n");
	argc=1;
	argtype[0]="struct buf *";
	argname[0]="bp";
	shouldbelocked[0] = 0;
	arg0special="->b_vp";
	name="vop_strategy";
	doit();
	name="vop_bwrite";
	doit();
}
'"$awk_parser" | sed -e "$anal_retentive"

# End stuff
echo '
/* End of special cases. */'


#
# Redirect stdout to the C file.
#
echo "$0: Creating $out_c" 1>&2
exec > $out_c

# Begin stuff
echo -n "$warning" | sed -e 's/\$//g'
echo ""
echo -n "$copyright"
echo '
#include <sys/param.h>
#include <sys/mount.h>
#include <sys/vnode.h>

struct vnodeop_desc vop_default_desc = {
	0,
	"default",
	0,
	NULL,
	VDESC_NO_OFFSET,
	VDESC_NO_OFFSET,
	VDESC_NO_OFFSET,
	VDESC_NO_OFFSET,
	NULL,
};
'

# Body stuff
sed -e "$sed_prep" $src | $awk '
function do_offset(typematch) {
	for (i=0; i<argc; i++) {
		if (argtype[i] == typematch) {
			printf("\tVOPARG_OFFSETOF(struct %s_args, a_%s),\n",
				name, argname[i]);
			return i;
		};
	};
	print "\tVDESC_NO_OFFSET,";
	return -1;
}

function doit() {
	# Define offsets array
	printf("\nint %s_vp_offsets[] = {\n", name);
	for (i=0; i<argc; i++) {
		if (argtype[i] == "struct vnode *") {
			printf ("\tVOPARG_OFFSETOF(struct %s_args,a_%s),\n",
				name, argname[i]);
		}
	}
	print "\tVDESC_NO_OFFSET";
	print "};";
	# Define F_desc
	printf("struct vnodeop_desc %s_desc = {\n", name);
	# offset
	printf ("\t0,\n");
	# printable name
	printf ("\t\"%s\",\n", name);
	# flags
	printf("\t0");
	vpnum = 0;
	for (i=0; i<argc; i++) {
		if (willrele[i]) {
			if (willrele[i] == 2) {
				word = "UNLOCK";
			} else if (willrele[i] == 3) {
				word = "PUT";
			} else {
				word = "RELE";
			}
			if (argdir[i] ~ /OUT/) {
				printf(" | VDESC_VPP_WILL%s", word);
			} else {
				printf(" | VDESC_VP%s_WILL%s", vpnum, word);
			};
			vpnum++;
		}
	}
	print ",";
	# vp offsets
	printf ("\t%s_vp_offsets,\n", name);
	# vpp (if any)
	do_offset("struct vnode **");
	# cred (if any)
	do_offset("struct ucred *");
	# proc (if any)
	do_offset("struct proc *");
	# componentname
	do_offset("struct componentname *");
	# transport layer information
	printf ("\tNULL,\n};\n");

	# Define inline function.
	printf("\nint %s(", toupper(name));
	for (i=0; i<argc; i++) {
		printf("%s", argname[i]);
		if (i < (argc-1)) printf(", ");
	}
	printf(")\n");
	for (i=0; i<argc; i++) {
		printf("\t%s %s;\n", argtype[i], argname[i]);
	}
	printf("{\n\tstruct %s_args a;\n", name);
	printf("\ta.a_desc = VDESC(%s);\n", name);
	for (i=0; i<argc; i++) {
		printf("\ta.a_%s = %s;\n", argname[i], argname[i]);
		if (shouldbelocked[i]) {
			printf ("#ifdef LOCKDEBUG\n");
			printf ("\tif ((%s->v_flag & VLOCKSWORK) && !VOP_ISLOCKED(%s))\n", argname[i], argname[i]);
			printf ("\t\tpanic(\"%s: %s\");\n", name, argname[i]);
			printf ("#endif\n");
		}
	}
	printf("\treturn (VCALL(%s%s, VOFFSET(%s), &a));\n}\n",
		argname[0], arg0special, name);

}
BEGIN	{
	arg0special="";
}
END	{
	printf("\n/* Special cases: */\n");
	argc=1;
	argtype[0]="struct buf *";
	argdir[0]="IN";
	argname[0]="bp";
	shouldbelocked[0] = 0;
	arg0special="->b_vp";
	willrele[0]=0;
	name="vop_strategy";
	doit();
	name="vop_bwrite";
	doit();
}
'"$awk_parser" | sed -e "$anal_retentive"

# End stuff
echo '
/* End of special cases. */'

# Add the vfs_op_descs array to the C file.
# Begin stuff
echo '
struct vnodeop_desc *vfs_op_descs[] = {
	&vop_default_desc,	/* MUST BE FIRST */
	&vop_strategy_desc,	/* XXX: SPECIAL CASE */
	&vop_bwrite_desc,	/* XXX: SPECIAL CASE */
'

# Body stuff
sed -e "$sed_prep" $src | $awk '
function doit() {
	printf("\t&%s_desc,\n", name);
}
'"$awk_parser"

# End stuff
echo '	NULL
};
'

exit 0

# Local Variables:
# tab-width: 4
# End:
