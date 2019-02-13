/* $Id: mknrescs.c,v 1.9 2014/05/23 09:22:20 lyonsd Exp $
 * $Log: mknrescs.c,v $
 * Revision 1.9  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.8  2014/04/28 14:08:17  lyonsd
 * Compiles under Linux with no warnings or errors
 *
 * Revision 1.7  2014/03/22 01:47:40  lyonsd
 * Working for all systems
 *
 * Revision 1.6  2014/03/20 22:53:02  lyonsd
 * Crashes in csheader.c with one system.  Further debugging needed.
 *
 * Revision 1.5  2014/03/09 19:48:00  lyonsd
 * Got rid of the free() statements at the end, which, in come cases, resulted
 * in a segmentation fault and crashed.
 * This is now working for all systems except the following:
 * Fe^26+ + He
 * Ne^10+ + H
 * I don't know why.  Maybe there are non-standard characters in the input files.
 *
 * Revision 1.4  2014/03/09 17:40:44  lyonsd
 * Tracked down another "segmentation fault"
 * This is now working for all but two systems.
 *
 * Revision 1.3  2014/03/08 22:39:46  lyonsd
 * Addressed an issue resulting in a segmentation fault.
 *
 * Revision 1.2  2014/03/08 21:41:21  lyonsd
 * Increase line length from 1000 to 2000.  1000 was not long enough for
 * systems with more than 50 avoided crossings.
 *
 * Revision 1.1  2014/03/08 21:19:01  lyonsd
 * Initial revision
 *
 * 
 * WARNING:
 * Do not compile with any optimizations (-O, -O1, -O2, -O3).
 * I don't know why. but if you use optimizations when you compile, the program
 * will not run correctly.
 *
 * Compiles without warnings using:
 *
 * cc -Wall -std=gnu99 -o mknrescs mknrescs.c
 */

#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "csdata.h"
#include "csheader.h"
#include "labels.h"
#include "utils.h"
#include "xsection.h"
#define DBUG

static const char rcsid[]="$Id: mknrescs.c,v 1.9 2014/05/23 09:22:20 lyonsd Exp $";

/* getops variables */
int nopt;

int main(int argc,char *argv[])
{
	string *csheader=NULL;
	xsection *cs=NULL;


/* Locale */
	char *locale;

    locale = setlocale(LC_ALL, "");

/* Process command-line arguments */

	while((nopt=getopt(argc,argv,"dvti:o:"))!=-1)
		switch(nopt)
		{
			case 'v':
				fputs(LGREEN,stderr);
				fputs(locale,stderr);
                fprintf(stderr,"\n%s\n",rcsid);
				prtxsectionrcs();
				prtcsheaderrcs();
				prtcsdatarcs();
				prtlabelsrcs();
				prtutilsrcs();
				fputs(RESET,stderr);
				return 0;
			case 'i':
				if(!access(optarg,R_OK))
				{
					if(!freopen(optarg,"r",stdin))
					{
						fprintf(stderr,"Error %d: cannot open %s\n",errno,optarg);
						exit(errno);
					}
				}
				else
				{
					fprintf(stderr,"Error %d: %s does not exist for reading\n",errno,optarg);
					exit(errno);
				}
				break;
			case 'o':
				if(!freopen(optarg,"w",stdout))
				{
					fprintf(stderr,"Error %d: cannot open %s\n",errno,optarg);
					exit(errno);
				}
				break;
			default:
				break;
		}

	csheader=readcsheader();
	cs=getcsparams(csheader);
	cs->nllab=getcslabels(csheader);
	cs->nlab=sortnlabels(nlabels(cs->nllab));
	cs=getcsdata(cs);
	cs=getnrescs(cs);
	prtcsheader(cs,"n");
	prtcsdata(cs,"n");

	return 0;
}
