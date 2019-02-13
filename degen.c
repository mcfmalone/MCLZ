/* $Id: degen.c,v 1.1 2014/05/23 09:16:38 lyonsd Exp $
 * $Log: degen.c,v $
 * Revision 1.1  2014/05/23 09:16:38  lyonsd
 * Initial revision
 *
 *
 * This program will read a NIST input file (${ion}${z}.nist.in) and print out
 * only those energy levels specified by the command line criteria.
 *
 * On the command line, one can specify the minimum n (primary quantum number),
 * maximum n, minimum l (azimuthal quantum number), maximum l, and spin
 * multiplicity (2S+1), which indicates whether the state is a singlet,
 * triplet, doublet, etc.  If no command line options are specified, then the
 * entire NIST file will be printed to STDOUT, which can be fed directly to
 * lzdata(1) if desired.
 *
 * Also, this program makes use of long options, described below.
 */

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE	100000
#endif
#include <sys/ioctl.h>
#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nist.h"
#include "utils.h"

static const char rcsid[]="$Id: degen.c,v 1.1 2014/05/23 09:16:38 lyonsd Exp $";
extern struct winsize ws;
void prtusage(char*);
void prtman(void);
void prtdegenrcs(void);
static struct option long_options[] =
{
    {"man", no_argument, NULL, 'm'},
    {"version", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {"degeneracy", required_argument, NULL, 'd'},
    {"n_min", required_argument, NULL, 'n'},
    {"n_max", required_argument, NULL, 'N'},
    {"l_min", required_argument, NULL, 'l'},
    {"l_max", required_argument, NULL, 'L'},
    {NULL, 0, NULL, 0}
};

int main(int argc,char *argv[])
{
	short int nopt=0;
	unsigned short int degeneracy=0;
	unsigned short int n_min=0;
	unsigned short int n_max=USHRT_MAX;
	unsigned short int l_min=0;
	unsigned short int l_max=USHRT_MAX;
/*	nist_t *nist; */

	while((nopt=getopt_long(argc,argv,"mvhd:n:N:l:L:",long_options,NULL))!=-1)
		switch(nopt)
		{
			case 'm':
				prtman();
				exit(EXIT_SUCCESS);
				break;
			case 'v':
				(void)fprintf(stderr,"%s\n",rcsid);
				prtnistrcs();
				exit(EXIT_SUCCESS);
				break;
			case 'h':
				prtusage(argv[0]);
				exit(EXIT_SUCCESS);
				break;
			case 'd':
				degeneracy=atoi(optarg);
				break;
			case 'n':
				n_min=atoi(optarg);
				break;
			case 'N':
				n_max=atoi(optarg);
				break;
			case 'l':
				l_min=atoi(optarg);
				break;
			case 'L':
				l_max=atoi(optarg);
				break;
			default:
				prtusage(argv[0]);
				exit(EXIT_FAILURE);
				break;
		}
/*	nist=readnist(); */
	(void)prtnist_r(readnist(),n_min,n_max,l_min,l_max,degeneracy);
/*	free(nist); */
	return 0;
}

void prtdegenrcs()
{
	(void)fputs(rcsid,stderr);
}

void prtusage(char str[])
{
	(void)fprintf(stderr,"Usage: %s [-hmv] [-d int] [-n int] [-N int] [-l int] [-L int] < infile > outfile\n",str);
}

void prtman()
{
	unsigned short int ix=0;		/* index counter */
	unsigned short int curpos=0;	/* cursor position, use to keep track of cusor's x-postion */

	ioctl(0,TIOCGWINSZ, &ws);

	curpos=fprintf(stdout,"DEGEN(3)");
	for(ix=curpos;ix<(ws.ws_col-strlen("MCLZ Commands"))/2;ix++)
		curpos+=fprintf(stdout," ");
	curpos+=fprintf(stdout,"MCLZ Commands");
	for(ix=curpos;ix<(ws.ws_col-strlen("DEGEN(3)"));ix++)
		curpos+=fprintf(stdout," ");
	curpos+=fprintf(stdout,"DEGEN(3)\n\n");
	(void)fprintf(stdout,"\033[1mNAME\033[0m\n");
	(void)fprintf(stdout,"\t\033[1mdegen\033[0m — prints selected NIST energy levels for a given system\n\n"); 
	(void)fprintf(stdout,"\033[1mSYNOPSIS\033[0m\n");
	(void)fprintf(stdout,"\tdegen [ \033[4moptions\033[0m ]\n\n"); 
	(void)fprintf(stdout,"\033[1mAVAILABILITY\033[0m\n");
	(void)fpmans(stdout,1,"Development and testing was conducted on:");
	(void)fputs("\n",stdout);
    (void)fputs("\tDarwin (Darwin Kernel Version 10.8.0)\n",stdout);
    (void)fpmans(stdout,1,"i686-apple-darwin10-gcc-4.2.1 (GCC) 4.2.1 (Apple Inc. build 5666) (dot 3)");
	(void)fputs("\n",stdout);
    (void)fputs("\tLinux (3.2.0-60-generic #91-Ubuntu SMP)\n",stdout);
    (void)fputs("\tgcc (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3\n\n",stdout);
    (void)fputs("\tCygwin (CYGWIN_NT-6.1)\n",stdout);
    (void)fputs("\tgcc (GCC) 4.8.2\n\n",stdout);
	(void)fputs("\tCompiler flags:\n\n",stdout);
	(void)fputs("\tgcc\t-W \\\n\t\t-Wall \\\n\t\t-Werror \\\n\t\t-Wextra \\\n\t\t-Wshadow \\\n\t\t-Wcast-qual \\\n\t\t-Wcast-align \\\n\t\t-Wwrite-strings \\\n\t\t-Wpointer-arith \\\n\t\t-Wnested-externs \\\n\t\t-Wstrict-prototypes \\\n\t\t-Wmissing-prototypes \\\n\t\t-g \\\n\t\t-O2 \\\n\t\t-ansi \\\n\t\t-pedantic \\\n\t\t-Dinline= \\\n\t\t-fno-common \\\n\t\t-fshort-enums \\\n\t\t-o degen degen.c nist.c utils.c -lm\n\n",stdout);
	(void)fprintf(stdout,"\033[1mDESCRIPTION\033[0m\n");
	(void)fputs("\n",stdout);
	(void)fpmans(stdout,1,BOLD"degen"OFF" acts as a filter for NIST input files, which are used as input for "BOLD"lzdata"OFF"(1).  \033[1mdegen\033[0m reads a NIST input file (which has the form \033[4m${ion}${z}.nist.in\033[0m), and prints only those lines that conform to the selection criteria.  User can select a range of \033[4mn\033[0m values, \033[4ml\033[0m values, and spin multiplicity (2\033[4mS\033[0m+1), which indicate singlet, doublet, triplet, etc, states.  If no selection criteria are specified, the entire NIST file is printed.");
	(void)fputs("\n\n",stdout);
	(void)fprintf(stdout,"\033[1mOPTIONS\033[0m\n");
	(void)fputs("\n",stdout);
	(void)fprintf(stdout,"\t\033[1m-h\033[0m, \033[1m--help\033[0m\n\t\tdisplays usage and exits.\n\n");
	(void)fprintf(stdout,"\t\033[1m-m\033[0m, \033[1m--man\033[0m\n\t\tdisplays this man page and exits.\n\n");
	(void)fprintf(stdout,"\t\033[1m-v\033[0m, \033[1m--version\033[0m\n\t\tdisplays rcsid string and exits.\n\n");
	(void)fprintf(stdout,"\t\033[1m-n\033[0m \033[4mint\033[0m, \033[1m--n_min\033[0m=\033[4mint\033[0m\n");
	(void)fpmans(stdout,2,"sets the minumum principal quantum number \033[4mn\033[0m.  Any energies for \033[4mn\033[0m < \033[4mn_min\033[0m will not be displayed\n\n");
	(void)fprintf(stdout,"\t\033[1m-N\033[0m \033[4mint\033[0m, \033[1m--n_max\033[0m=\033[4mint\033[0m\n");
	(void)fpmans(stdout,2,"sets the maximum principal quantum number \033[4mn\033[0m.  Any energies for \033[4mn\033[0m > \033[4mn_max\033[0m will not be displayed\n\n");
	(void)fprintf(stdout,"\t\033[1m-l\033[0m \033[4mint\033[0m, \033[1m--l_min\033[0m=\033[4mint\033[0m\n");
	(void)fpmans(stdout,2,"sets the minumum azimuthal quantum number \033[4ml\033[0m.  Any energies for \033[4ml\033[0m < \033[4ml_min\033[0m will not be displayed\n\n");
	(void)fprintf(stdout,"\t\033[1m-L\033[0m \033[4mint\033[0m, \033[1m--l_max\033[0m=\033[4mint\033[0m\n");
	(void)fpmans(stdout,2,"sets the maximum azimuthal quantum number \033[4ml\033[0m.  Any energies for \033[4ml\033[0m > \033[4ml_max\033[0m will not be displayed\n\n");
	(void)fprintf(stdout,"\t\033[1m-d\033[0m \033[4mint\033[0m, \033[1m--degeneracy\033[0m=\033[4mint\033[0m\n");
	(void)fpmans(stdout,2,"sets the degeneracy (\033[4mspin multiplicity\033[0m=2\033[4mS\033[0m+1).  Only those energies which correspond to the degenerate state specified will be displayed.\n\n");
	(void)fprintf(stdout,"\033[1mEXIT STATUS\033[0m\n");
	(void)fputs("\n",stdout);
	(void)fpmans(stdout,1,"0 upon successful completion.  Otherwise "BOLD"exit"OFF"(3) will be called with EXIT_FAILURE or "UND"errno"OFF", depending upon the particular error that occurred.");
	(void)fputs("\n\n",stdout);
	(void)fprintf(stdout,"\033[1mEXAMPLES\033[0m\n");
	(void)fputs("\n",stdout);
	(void)fprintf(stdout,"\tPrint out only the singlet states for Ne^9+...\n\n");
	(void)fprintf(stdout,"\t\tdegen -d 1 < ne9.nist.in\n\n");
	(void)fprintf(stdout,"\tPrint out only the triplet states for Mg^11+...\n\n");
	(void)fprintf(stdout,"\t\tdegen --degeneracy=3 < mg11.nist.in\n\n");
	(void)fprintf(stdout,"\tPrint out only the states where \033[4ml\033[0m=0 for Fe^26+...\n\n");
	(void)fprintf(stdout,"\t\tdegen -l 0 -L 0 < fe26.nist.in\n\n");
	(void)fprintf(stdout,BOLD"AUTHOR"OFF);
	(void)fputs("\n",stdout);
	(void)fpmans(stdout,1,"David A Lyons <lyonsd@gmail.com>, Department of Physics and Astronomy, University of Georgia");
	(void)fputs("\n\n",stdout);
	(void)fprintf(stdout,"\033[1mBUGS\033[0m\n");
	(void)fprintf(stdout,"\tNone found.  If any are found, please report to author listed above.\n\n");
	(void)fprintf(stdout,"\033[1mSEE ALSO\033[0m\n");
	(void)fprintf(stdout,"\t\033[1mmclz.sh\033[0m(1), \033[1mlzdata\033[0m(1), \033[1mlzmcro\033[0m(1), \033[1mlevels\033[0m(1)\n\n"); 
	curpos=fprintf(stdout,rcsid);
	for(ix=curpos;ix<(ws.ws_col-strlen("DEGEN(3)"));ix++)
		curpos+=fprintf(stdout," ");
	curpos+=fprintf(stdout,"DEGEN(3)\n");
}
