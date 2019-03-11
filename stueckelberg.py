#!/usr/bin/python
"""
$Id: stueckelberg.py,v 1.2 2019/03/09 20:27:26 micha Exp micha $
$Author: micha $
$Revision: 1.2 $
$Log: stueckelberg.py,v $
Revision 1.2  2019/03/09 20:27:26  micha
Updated to use ldist.py instead of mclzldist.exe.

Revision 1.1  2019/02/28 23:49:05  micha
Initial revision

Revision 1.1  2019/02/28 23:24:23  micha
Initial revision


This is a wrapper script for the Stueckleberg project.

In addition to prepping files, it runs the following applications:

    degen       - selects appropriate data from NIST file
    lzdata      - Creates ".ax" (avoided crossings) file used by lzmcro
    lzmcro      - Creates ".xs" (cross sections) file
    mknlrescs   - Creates ".cs" (cross sections) file with header info
    mknrescs    - Creates "_nres.cs" (n-resolved cross sections) file
    mclzldist   - Creates statisitcal and low-energy approximations files

Ernst Carl Gerlach Stueckelberg was a Swiss mathematician and physicist, regarded as one of the most eminent physicists of the 20th century. Despite making key advances in theoretical physics, including the exchange particle model of fundamental forces, causal S-matrix theory, and the renormalization group, his idiosyncratic style and publication in minor journals led to his work being unrecognized until the mid-1990s. (wikipedia)
"""

import argparse
import sys
import shutil as sh
import os

def MainMenu(argv):
    parser=argparse.ArgumentParser(description="Stueckleberg: find cross sections from single-electron capture collisions between ions and atoms.")
    parser.add_argument("-I","--ion",help="Ion (O, Ne, Mg, Fe, etc.)",type=str,metavar="ion",dest="ion")
    parser.add_argument("-A","--atom",help="Atom (H, He, Li, etc.), or molecule (CO2, H2O, etc.)",type=str,metavar="atom",dest="atom")
    parser.add_argument("-Z","--charge",help="Charge (integer)",type=int,metavar="charge",dest="charge")
    group=parser.add_mutually_exclusive_group()
    group.add_argument("-s","--singlets",help="Singlets",action="store_true",default=False)
    group.add_argument("-t","--triplets",help="Triplets",action="store_true",default=False)
    parser.add_argument("-b","--bare",help="Bare ion",action="store_true",dest="bareion",default=False)
    parser.add_argument("-c","--check",help="Check lzdata output and exit",action="store_true",default=False)
    parser.add_argument("-N","--name",help="Name (for the header of the cross section file)",type=str,metavar="name",dest="name",default="John Q. Public")
    parser.add_argument("-L","--initials",help="Initials (for the header of the cross section file)",type=str,metavar="initials",dest="initials",default="JQP")
    parser.add_argument("-e","--edit",help="Edit output of lzdata",action="store_true",default=False)
    parser.add_argument("-v",help="Verbose (intended for debugging)",action="store_true",dest="Verbose",default=False)
    args=parser.parse_args(None if argv else ["-h"])
    if(args.Verbose):
        for i in vars(args):
            print("%s=%s"%(i,getattr(args,i)))
    return args

# Build the names of all the files that are needed, and return them as a keyed list.
def Files(a):
    nistfile=a.ion.lower()+str(a.charge)
    basename=nistfile+"+"+a.atom.lower()+"_sec_mclz"
    files={"base":basename}             # basename = {ion}{Z}+{atom}_sec_mclz
    files["nist"]=nistfile+".nist.in"   # nist input file
    if(a.singlets):basename=basename+"_sing"
    elif(a.triplets):basename=basename+"_trip"
    files["ax"]=basename+".ax"          # avoided crossings
    files["xs"]=basename+".xs"          # cross sections
    files["cs1"]=basename+".cs"         # cross sections with header
    files["cs2"]=basename+"_nres.cs"    # n-resolved cross sections with header
    files["agr1"]=basename+".agr"       # xmgrace file
    files["agr2"]=basename+"_nres.agr"  # n-resolved xmgrace file
    if(a.Verbose):print(files)
    return files

def cpfile(f1,f2):
    if(os.path.exists(f1)):sh.copy(f1,f2)
    else:print("Error: 404: %s: Not found"%f1)

# If singlets|triplets is specified, then the nist file needs
# to be modified so only singlets|triplets are in it. A backup
# copy is made, then the external command "degen" is called.
def degen(a,f):
    if(a.singlets):d=1
    elif(a.triplets):d=3
    else:
        d=0
        rv=0
    if(d>0):
        f1=f["nist"]
        f2=f["nist"]+".bak"
        sh.copy(f1,f2)
        degencmd="degen -d %d < %s > %s"%(d,f2,f1)
        rv=Run_it(a,degencmd)
    return rv

# Execute external command "lzdata". The output of this
# command is used as input to the external "lzmcro" command.
def lzdata(a,f):
    if(a.singlets):dgen="-s"
    elif(a.triplets):dgen="-t"
    else:dgen=""
    lzdatacmd="lzdata < %s -I %s -Z %s -A %s -H %s > %s"%(f["nist"],a.ion,a.charge,a.atom,dgen,f["ax"])
    rv=Run_it(a,lzdatacmd)
    if(rv and (a.singlets or a.triplets)):cpfile(f["nist"]+".bak",f["nist"])
    if(a.check):
        os.system("less %s"%f["ax"])
        sys.exit()
    if(a.edit):os.system("vi %s"%f["ax"])
    return rv

# Execute external command "lzmcro", which computes the
# cross sections of the specified ion-atom collision.
# This command will produce a ".xs" (cross section) file
# which contains all the computed data in column form.
def lzmcro(a,f):
    lzmcrocmd="lzmcro -t < %s > %s"%(f["ax"],f["xs"])
    rv=Run_it(a,lzmcrocmd)
    if(a.singlets or a.triplets):cpfile(f["nist"]+".bak",f["nist"])
    return rv

# Execute external command "mknlrescs", which reads a
# cross section file (".xs") and creates a new file with 
# the same data, but preceded by header information.
def mknlrescs(a,f):
    mknlrescscmd="mknlrescs < %s -I %s -Z %s -A %s -N \"%s\" -L %s > %s"%(f["xs"],a.ion,a.charge,a.atom,a.name,a.initials,f["cs1"])
    rv=Run_it(a,mknlrescscmd)
    return rv

# Execute external command "mknrescs", which reads a
# nl-resolved cross section file with header data (".cs")
# and creates a new file with n-resolved cross section data
# along with header data.
def mknrescs(a,f):
    mknrescscmd="mknrescs < %s > %s"%(f["cs1"],f["cs2"])
    rv=Run_it(a,mknrescscmd)
    return rv

# Special consideration must be made if the ion is bare
# (hydrogen-like). In that case, the low-energy and statistical
# approximations must be made on the l=0 cross sections to
# get the remaining l-distributions l=1, 2, 3, etc. cross 
# sections, for each n. These calculations are performed in the
# external program "mclzldist".
def mclzldist(a,f):
    rv=0
    if(a.bareion):
        n=[l.split('\t')[2].strip() for l in open(f["ax"]).readlines()[3:]]
        nmax=int(max(n))
        nmin=int(min(n))
        mclzldistcmd="mclzldist %d %d %s %s"%(nmin,nmax,f["xs"],f["base"])
        rv=Run_it(a,mclzldistcmd)
    return rv

def ldist(a,f):
    ldistcmd="ldist.py -ls %s"%f["cs1"]
    rv=Run_it(a,ldistcmd)
    return rv

# Run it, a simple function to run a command, specified by string s,
# and exit if the return value is not 0.
def Run_it(a,s):
    if(a.Verbose):print(s)
    rv=os.system(s)
    if(a.Verbose):print("Return value: %d"%rv)
    if(rv):sys.exit()
    return rv

def uhoh(s):
    print(s)
    sys.exit(1)

def main(argv):
    args=MainMenu(argv)
    f=Files(args)
    degen(args,f)
    lzdata(args,f)
    lzmcro(args,f)
    mknlrescs(args,f)
    mknrescs(args,f)
#    mclzldist(args,f)
    ldist(args,f)
    return 0

if __name__=="__main__":
    main(sys.argv[1:])
