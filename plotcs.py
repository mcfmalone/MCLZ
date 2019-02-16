#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
This script was written to read a "cs" (cross section) file, which was generated
by the Landau-Zener program, and plot it. It is intended to mimic the graphs produced
by xmgrace. But no labels/legend is displayed.

$Id: plotcs.py,v 1.1 2019/02/16 01:37:49 micha Exp micha $
$Author: micha $
$Revision: 1.1 $
$Log: plotcs.py,v $
Revision 1.1  2019/02/16 01:37:49  micha
Initial revision

"""
import sys
import os
import matplotlib.pyplot as plt
import numpy as np
import ntpath
import argparse

def MainMenu(argv):
    parser=argparse.ArgumentParser(description="Plot MCLZ cross-sections.",prog=sys.argv[0])
    parser.add_argument("-i",help="Input file.",type=argparse.FileType('r'),metavar="str",dest="InFile",default=sys.stdin)
    parser.add_argument("-s",help="Save image file.",type=str,metavar="str",dest="ImgFile")
    parser.add_argument("-q",help="Quiet; do not display plot.",action="store_true",dest="quiet")
    parser.add_argument("-v",help="Verbose. Intended for debugging.",action="store_true",dest="Verbose",default=False)
    parser._actions[0].help="Show help message and exit."
    args=parser.parse_args(None if argv else ["-h"])
    if(args.Verbose):
        for i in vars(args):
            print("%s=%s"%(i,getattr(args,i)))
    return args

def MakeFig(data,args):
    rows=len(data)
    cols=len(data[0])
    xmin=min(data[:,0])
    xmax=max(data[:,0])
    fig=plt.figure(args.InFile.name)
    fig.set_size_inches(16.0,9.0)
    fig.set_dpi(120)
    plt.style.use("default")
    plt.plot(data[:,0],data[:,-1],'-',color="Black")
    color_idx=np.linspace(0,1,cols-1)
    for i,j in zip(range(cols-2,0,-1),color_idx):
        plt.plot(data[:,0],data[:,i],'-',color=plt.cm.gist_rainbow(j))
#    plt.title(args.InFile.name)
    plt.ylim(ymin=1e-6)
    plt.xlim(xmin,xmax)
    plt.grid(which='major',linestyle='-',linewidth='0.3')
    plt.xlabel(r'energy ($\frac{eV}{u}$)')
    plt.ylabel(r'cross section (10$^{-16}$ cm$^{2}$)')
    plt.xscale('log',basex=10)
    plt.yscale('log',basey=10)
    return fig
    
def main(argv):

    args=MainMenu(argv)
#load the data from input
    data=np.loadtxt(args.InFile)

#    print('rows=%d',rows)
#    print('cols=%d',cols)

    fig=MakeFig(data,args)

    if(args.quiet==False):
        plt.show()
    if(args.ImgFile):
        fig.savefig(args.ImgFile,dpi=120)

if __name__=="__main__":
    main(sys.argv[1:])
