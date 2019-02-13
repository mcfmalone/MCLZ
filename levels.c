/* $Id: levels.c,v 1.3 2014/05/23 16:53:30 lyonsd Exp $
 * $Log: levels.c,v $
 * Revision 1.3  2014/05/23 16:53:30  lyonsd
 * Fixed compiler error on Linux
 *
 * Revision 1.2  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.1  2014/05/01 04:22:17  Anyone
 * Added built-in man page.
 *
 * Revision 1.0  2014/04/30 17:13:28  lyonsd
 * Initial revision
 *
 *
 * Implements the energy levels from 1 to N using E_n=E_0·z²·(1-1/n²)
 * Useful for bare ion systems 
 */

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE	10000
#endif
#include <sys/ioctl.h>
#include <sys/types.h>
#include <ctype.h>
#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

static const char rcsid[]="$Id: levels.c,v 1.3 2014/05/23 16:53:30 lyonsd Exp $";
extern struct winsize ws;

void prtusage(char*);
void prtman(void);
unsigned int fpmans(FILE*,unsigned short int,const char*);
static struct option long_options[] =
{
    {"man", no_argument, NULL, 'm'},
    {"version", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {"ionization", required_argument, NULL, 'i'},
    {"n_max", required_argument, NULL, 'n'},
    {"charge", required_argument, NULL, 'z'},
    {NULL, 0, NULL, 0}
};

int main(int argc,char *argv[])
{

	unsigned short int no_n=1;
	unsigned short int no_i=1;
	unsigned short int nx=0;
	unsigned short int n=0;
	short int nopt;
	short int z=1;
	double q=1.0;
	double icm=109678.7717430710;		/* ground state, in 1/cm units */
/*	double icm=13.5984/1.23984193027e-4;    ground state energy of H, converted
                                          * to wave number (1/cm) units
										  */

/* Process the command line options */
	while((nopt=getopt_long(argc,argv,"hmvi:n:z:",long_options,NULL))!=-1)
		switch(nopt)
		{
			case 'm':
				prtman();
				exit(EXIT_SUCCESS);
				break;
			case 'v':
				(void)fputs(rcsid,stderr);
				(void)fputs("\n",stderr);
				exit(EXIT_SUCCESS);
				break;
			case 'h':
				prtusage(argv[0]);
				exit(EXIT_SUCCESS);
				break;
			case 'i':
				icm=atof(optarg);
				no_i=0;
				break;
			case 'n':
				n=atoi(optarg);
				no_n=0;
				break;
			case 'z':
				z=atoi(optarg);
				q=(double)z;
				break;
			default:
				prtusage(argv[0]);
				exit(EXIT_FAILURE);
				break;
		}

	if(no_n)
	{
		prtusage(argv[0]);
		exit(EXIT_FAILURE);
	}

/* if no ionization energy was specified on the command line, then we'll
 * calculate E_o=icm*q²
 */
	if(no_i)
	{
		icm*=pow(q,2.0);
	}

	for(nx=1;nx<=n;nx++)
	{
		(void)fprintf(stdout,"%2ds\tS\t0\t%14.3f\n",nx,icm*(1-1/pow(nx,2.0)));
	}	
	(void)fprintf(stdout,"---\t---\t0\t%14.3f\n",icm);
	
	return EXIT_SUCCESS;
}

void prtusage(char str[])
{
	(void)fprintf(stderr,"Usage: %s [-hmv] [-i float] [-n int] [-z int]\n",str);
}

void prtman()
{
	unsigned short int ix=0;		/* index counter */
	unsigned short int curpos=0;	/* cursor position, use to keep track of cusor's x-postion */

/* get the screen width */
	ioctl(0,TIOCGWINSZ, &ws);

	curpos=fprintf(stdout,"LEVELS(1)");
	for(ix=curpos;ix<(ws.ws_col-strlen("MCLZ Commands"))/2;ix++)
		curpos+=fprintf(stdout," ");
	curpos+=fprintf(stdout,"MCLZ Commands");
	for(ix=curpos;ix<(ws.ws_col-strlen("LEVELS(1)"));ix++)
		curpos+=fprintf(stdout," ");
	curpos+=fprintf(stdout,"LEVELS(1)\n\n");
	(void)fputs(BOLD"NAME"OFF"\n\n",stdout);
	(void)fpmans(stdout,1,BOLD"levels"OFF" — prints energies (1/cm) from 1 to "UND"n"OFF" for bare ion systems"); 
	(void)fputs("\n\n",stdout);
	(void)fputs(BOLD"SYNOPSIS"OFF"\n\n",stdout);
	(void)fputs("\tlevels ["UND"options"OFF"]\n\n",stdout); 
	(void)fputs(BOLD"AVAILABILITY"OFF"\n\n",stdout);
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
	(void)fputs("\tgcc\t-W \\\n\t\t-Wall \\\n\t\t-Werror \\\n\t\t-Wextra \\\n\t\t-Wshadow \\\n\t\t-Wcast-qual \\\n\t\t-Wcast-align \\\n\t\t-Wwrite-strings \\\n\t\t-Wpointer-arith \\\n\t\t-Wnested-externs \\\n\t\t-Wstrict-prototypes \\\n\t\t-Wmissing-prototypes \\\n\t\t-g \\\n\t\t-O2 \\\n\t\t-ansi \\\n\t\t-pedantic \\\n\t\t-Dinline= \\\n\t\t-fno-common \\\n\t\t-fshort-enums \\\n\t\t-o levels levels.c utils.c -lm\n\n",stdout);
	(void)fputs(BOLD"DESCRIPTION"OFF"\n\n",stdout);
	(void)fpmans(stdout,1,"The energy levels for a bare (hydrogen-like) ion can be approximated using E_n=E_o·q²(1-1/n²), where E_o is the ionizaton potential of hydrogen.");
	(void)fputs("\n\n",stdout);
  	(void)fpmans(stdout,1,BOLD"levels"OFF" computes these energies (1/cm) from 1 to "UND"n"OFF" using the preceding formula.");
	(void)fputs("\n\n",stdout);
	(void)fpmans(stdout,1,"This data can then be used by "BOLD"lzdata"OFF"(1) to compute the avoided crossings, which, in turn, are used by "BOLD"lzmcro"OFF"(1) to compute single electron capture cross sections.");
	(void)fputs("\n\n",stdout);
	(void)fputs(BOLD"OPTIONS"OFF"\n\n",stdout);
	(void)fpmans(stdout,1,"Mandatory arguments for long options are also mandatory for short options.");
	(void)fputs("\n\n",stdout);
	(void)fputs("\t"BOLD"-h"OFF", "BOLD"--help"OFF"\n\t\tdisplays usage and exits.\n\n",stdout);
	(void)fputs("\t"BOLD"-m"OFF", "BOLD"--man"OFF"\n\t\tdisplays man page and exits.\n\n",stdout);
	(void)fputs("\t"BOLD"-v"OFF", "BOLD"--version"OFF"\n\t\tdisplays rcsid string and exits.\n\n",stdout);
	(void)fputs("\t"BOLD"-n"OFF", "BOLD"--n_max="UND"int"OFF"\n\t\tprint energy levels up to "UND"n"OFF".\n\n",stdout);
	(void)fputs("\t"BOLD"-z"OFF", "BOLD"--charge="UND"int"OFF"\n\t\tIf this option is used, then "UND"z"OFF" will be used to calculate E_o.\n\n",stdout);
	(void)fputs("\t"BOLD"-i"OFF", "BOLD"--ionization="UND"float"OFF"\n\t\tIf this option is used, then E_o="UND"i"OFF".\n\n",stdout);
	(void)fputs(BOLD"EXIT STATUS"OFF"\n\n",stdout);
	(void)fpmans(stdout,1,"0 upon successful completion.  Otherwise "BOLD"exit"OFF"(3) will be called with EXIT_FAILURE or "UND"errno"OFF", depending upon the particular error that occurred.");
	(void)fputs("\n\n",stdout);
	(void)fputs(BOLD"EXAMPLES"OFF"\n\n",stdout);
	(void)fpmans(stdout,1,"Given a bare iron ion (which has a charge "UND"z"OFF"=26, and an ionization energy "UND"i"OFF"=74829550.20), compute the first 20 energy levels...");
	(void)fputs("\n\n",stdout);
	(void)fputs("\t\tlevels -n 20 -z 26 -i 74829550.20\n\n",stdout);
	(void)fpmans(stdout,1,"Given a bare neon ion (which has a charge "UND"z"OFF"=10, and an ionization energy "UND"i"OFF"=10986877.27), compute the first 20 energy levels...");
	(void)fputs("\n\n",stdout);
	(void)fputs("\t\tlevels -n 20 -z 10 -i 10986877.27\n\n",stdout);
	(void)fputs(BOLD"AUTHOR"OFF"\n\n",stdout);
	(void)fpmans(stdout,1,"David A Lyons <lyonsd@gmail.com>, Department of Physics and Astronomy, University of Georgia");
	(void)fputs("\n\n",stdout);
	(void)fputs(BOLD"BUGS"OFF"\n\n",stdout);
	(void)fpmans(stdout,1,"The energies produced are an "BOLD"approximation"OFF", which are useful when more accurate energies are unavailable.");
	(void)fputs("\n\n",stdout);
	(void)fputs(BOLD"SEE ALSO"OFF"\n\n",stdout);
	(void)fputs("\t"BOLD"mclz.sh"OFF"(1), "BOLD"lzdata"OFF"(1), "BOLD"lzmcro"OFF"(1), "BOLD"degen"OFF"(1)\n\n",stdout); 
	curpos=fprintf(stdout,rcsid);
	for(ix=curpos;ix<(ws.ws_col-strlen("LEVELS(1)"));ix++)
		curpos+=fprintf(stdout," ");
	curpos+=fprintf(stdout,"LEVELS(1)\n");
}

