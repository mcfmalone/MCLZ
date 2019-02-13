#!/bin/ksh
#
# $Id: mclz.sh,v 1.7 2014/05/31 22:13:24 lyonsd Exp $
# $Log: mclz.sh,v $
# Revision 1.7  2014/05/31 22:13:24  lyonsd
# Added command line option for bare ion, because on Darwin it wasn't being automatically detected.
#
# Revision 1.6  2014/05/31 19:29:00  lyonsd
# Added check option, to display lzdata's output and then quit.
#
# Revision 1.1  2014/04/30 17:14:32  lyonsd
# Enclosed name variable in quotes
# Works on Linux and Darwin
#
# Revision 1.0  2014/04/29 15:05:53  lyonsd
# Initial revision
#
#
# usage: mclz.sh [ options ]
#   -I, --ion=string
#	-A, --atom=string
#	-Z, --charge=int
#	-N, --name=string
#	-L, --initials=string
#	-s, --singlets
#	-t, --triplets
#	-n, --noedit
#	-b, --bareion
#	-c, --noedit

USAGE=$'[-?\n@(#)$Id: mclz.sh,v 1.7 2014/05/31 22:13:24 lyonsd Exp $\n]'
USAGE+="[-author?David A. Lyons <lyonsd@gmail.com>]"
USAGE+="[+NAME?mclz.sh - Multi Channel Landau Zener]"
USAGE+=$'[+DESCRIPTION?\amclz.sh\a is a wrapper script for \alzdata\a(1), \alzmcro\a(1), \amknlrescs\a(1), and \amknrescs\a(1). This script, utilizing the referenced programs, takes as input NIST enegy levels of a given ion, and produces the single electron capture cross sections from that ion\'s interaction with a neutral atom.\nBefore running this script, make sure that \alzdata\a(1), \alzmcro\a(1), \amknlrescs\a(1), and \amknrescs\a(1) are compiled and in your path.]'
USAGE+=$'[I:ion?Abbreviated name for ion.  If nothing is specified, then the envronment varaibles \aion\a or \aION\a will be used, if they are set.  Otherwise, script will terminate.]:[string]'
USAGE+=$'[A:atom?Abbreviated name for atom.  If nothing is specified, the the environment variables \aatom\a or \aATOM\a will be used, if they are set.  Otherwise, script will terminate.]:[string]'
USAGE+=$'[Z:charge]#[int?Specify charge of ion.  If nothing is specified, then the environment variables \az\a or \aZ\a wil be used, if they are set.  Otherwise, script will temrinate.]'
USAGE+=$'[N:name?Specify name of person performing this calculation.  If nothing is specified, then the environment variables \aname\a or \aNAME\a or \aLOGNAME\a will be used, if they are set. Otherwise script will terminate. ]:[string]'
USAGE+=$'[L:initials?Specify initials of person performing this calculation.  If nothing is specified, then \astring\a will be set to \"XXX\".]:[string]'
USAGE+="[s:singlets?Set this if singlet states are used.]"
USAGE+="[t:triplets?Set this if triplet states are used.]"
USAGE+="[n:noedit?Set this if you do NOT want to edit the avoided crossings data (output of lzdata).]"
USAGE+="[b:bareion?The script will attempt to detect if this is a bare ion, however, on some systems the detection isn't working.  This option overrides the automatic detection.]"
USAGE+="[c:check?Run display the output of lzdata and stop.]"
USAGE+=$'[+EXIT STATUS?0 upon successful completion.  Non-zero otherwise.]'
USAGE+=$'[+ENVIRONMENT?The following environment variables will be utilized if they are set.  The command-line options will override any value in these environment variables:\aatom\a\|\aATOM\a,\aion\a\|\aION\a, \az\a\|\aZ\a, \aname\a\|\aNAME\a|\aLOGNAME\a, \ainitials\a\|\aINITIALS\a.]'
USAGE+=$'[+FILES?INTPUT: 1) NIST energy levels, named as such: \a${ion}${z}.nist.in\a.\n2)\aelements.dat\a.\nOUTPUT: 1) avoided crossings: \a${ion}${z}+${atom}_sec_mclz.ax\a\n2) cross sections: \a${ion}${z}+${atom}_sec_mclz.xs\a\n3) formatted cross sections: \a${ion}${z}+${atom}_sec_mclz.cs\a and \a${ion}${z}+${atom}_sec_mclz_nres.cs\a.]'
USAGE+=$'[+BUGS?The built-in \aman\a(1) display may not work with some versions of \aksh\a(1), particularly non-standard variants.  Also, this script depends upon executables, mentioned above.  If these executables are not compiled or in the \aPATH\a, this this script cannot complete successfully.]'
USAGE+=$'[+EXAMPLE?To calculate the single electron capture cross sections for Ne^9+ + H (tripets):]'
USAGE+="{[mclz.sh -I Ne -Z 9 -A H -t -N \"John Q Public\" -L JQP]}"
USAGE+=$'[+SEE ALSO?\alzdata\a(1), \alzmcro\a(1), \amknlrescs\a(1), \amknrescs\a(1)]'

#initializations
sing=
trip=
sec=1
mclz=1
noedit=0
bareion=
check=
errorcode=0

#process command line options
while getopts "$USAGE" optchar ; do
    case $optchar in
	I)	ion=${OPTARG} ;;
	A)	atom=${OPTARG} ;;
	Z)	charge=${OPTARG} ;;
	N)	name="${OPTARG}" ;;
	L)	initials=${OPTARG} ;;
	b)  bareion=1 ;;
	c)  check=1 ;;
	s)  if ((${trip:-0}==1))
		then
			echo "Error: Cannot specify both singlets and triplets"
			exit 1
		else
			sing=1
			trip=
		fi
		;;
	t)  if ((${sing:-0}==1))
		then
			echo "Error: Cannot specify both singlets and triplets"
			exit 1
		else
			trip=1
			sing=
		fi
		;;
	n) noedit=1 ;;
    esac
done

#Print everything out so the user can see if it's correct or not
print ion=${ion:=${ION:?"Undefined!"}}
print charge=${charge:=${z:=${Z:?"Undefined!"}}}
atomicnumber=$(awk '$2 ~ /\<'${ion}'\>/ {print $1}' elements.dat)
print atom=${atom:=${ATOM:?"Undefined!"}}
print "${ion}^${charge}+ + ${atom} -> ${ion}^$((${charge}-1))+ + ${atom}^+" ${sing:+singlets} ${trip:+triplets}
if [[ ${charge} = ${atomicnumber} ]]
then
	bareion=1
fi
print ${bareion:+"bare ion -> hydrogen-like"}
print name=${name:=${NAME:=${LOGNAME}}}
print initials=${initials:=${INITIALS:="XXX"}}

# Set the "base" file name

fn=${ion}${charge}+${atom}${sec:+_sec}${mclz:+_mclz}${sing:+_sing}${trip:+_trip}
fn=$(echo ${fn}|tr '[:upper:]' '[:lower:]')

# set up our *ax (avoided crossings) filenames, using the variables above

axfile="${fn}.ax"

# set up our *xs (cross sections) filenames, using the variables above

xsfile="${fn}.xs"

# set up our *cs filenames, using the variables above

csfile1="${fn}.cs"
csfile2="${fn}_nres.cs"

# set up our xmgrace filenames, using the variables above

agrfile1="${fn}.agr"
agrfile2="${fn}_nres.agr"

# set up NIST file

nistfile="${ion}${charge}.nist.in"
nistfile=$(echo ${nistfile}|tr '[:upper:]' '[:lower:]')

# check to see if we're working with singlets or triplets
print "Executing...\n"
if [[ ${sing} ]]
then
	print "\tcp ${nistfile} ${nistfile}.bak\n"
	cp ${nistfile} ${nistfile}.bak
	print "\tdegen -d 1 < ${nistfile}.bak > ${nistfile}\n"
	degen -d 1 < ${nistfile}.bak > ${nistfile}
else if [[ ${trip} ]]
	then
		print "\tcp ${nistfile} ${nistfile}.bak\n"
		cp ${nistfile} ${nistfile}.bak
		print "\tdegen -d 3 < ${nistfile}.bak > ${nistfile}\n"
		degen -d 3 < ${nistfile}.bak > ${nistfile}
	fi
fi

# Run lzdata with the specified options
printf "\tlzdata < ${nistfile} -I ${ion} -Z ${charge} -A ${atom} -H ${sing:+-s} ${trip:+-t} > ${axfile}\n"
errorcode=$(lzdata < ${nistfile} -I ${ion} -Z ${charge} -A ${atom} ${sing:+"-s"} ${trip:+"-t"} -H > ${axfile})
if ((errorcode=0 ))
then
	if [[ -f ${nistfile}.bak ]]
	then
		print "\tmv ${nistfile}.bak ${nistfile}\n"
		mv ${nistfile}.bak ${nistfile}
	fi
	print "Error ${errorcode} lzdata did not complete successfully"
	exit 1
fi

# Are we checking only?  If so, display output of lzdata and exit
if ((check==1))
then
	less ${axfile}
	exit 0
fi

# Edit the output from lzdata, unless "noedit" is set
if ((noedit==0))
then
	vi ${axfile}
fi


# lzmcro
printf "\tlzmcro -t < ${axfile} > ${xsfile}\n"
errorcode=$(lzmcro -t < ${axfile} > ${xsfile})
if ((errorcode!=0))
then
	if [[ -f ${nistfile}.bak ]]
	then
		print "\tmv ${nistfile}.bak ${nistfile}\n"
		mv ${nistfile}.bak ${nistfile}
	fi
	print "Error ${errorcode}: lzmcro did not complete successfully"
	exit 1
fi

# mknlrescs
printf "\tmknlrescs < ${xsfile} -I ${ion} -Z ${charge} -A ${atom} -N \"${name}\" -L ${initials} > ${csfile1}\n"
errorcode=$(mknlrescs < ${xsfile} -I ${ion} -Z ${charge} -A ${atom} -N "${name}" -L ${initials} > ${csfile1})
if [[ -f ${nistfile}.bak ]]
then
	print "\tmv ${nistfile}.bak ${nistfile}\n"
	mv ${nistfile}.bak ${nistfile}
fi
if (( errorcode!=0 ))
then
	print "Error ${errorcode}: mknlrescs did not complete successfully"
	exit 1
fi

#mknrescs
printf "\tmknrescs < ${csfile1} > ${csfile2}\n"
errorcode=$(mknrescs < ${csfile1} > ${csfile2})
if (( errorcode!=0 ))
then
	print "Error ${errorcode}: mknrescs did not complete successfully"
	exit 1
fi

if [[ ${bareion} ]]
then
	# Find the minimum and maximum n values
	# because they're needed for mclzldist
	nmin=$(awk 'NR>3 {print $3}' ${axfile}|sort -n|head -1)
	nmax=$(awk 'NR>3 {print $3}' ${axfile}|sort -nr|head -1)
	# mclzldist
	print "\tmclzldist ${nmin} ${nmax} ${xsfile} ${fn}\n"
	mclzldist ${nmin} ${nmax} ${xsfile} ${fn}
fi

# if everything executed successfully, exit 0
exit 0
