/* $Id: mknlrescs.c,v 1.6 2014/05/23 09:22:20 lyonsd Exp $
 * $Log: mknlrescs.c,v $
 * Revision 1.6  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.5  2014/04/22 04:58:03  lyonsd
 * Will not compile.  Needs fixed.
 *
 * Revision 1.4  2014/03/22 01:47:40  lyonsd
 * Working for all systems
 *
 * Revision 1.3  2014/03/20 22:53:02  lyonsd
 * Working for all systems.
 *
 * Revision 1.2  2014/03/19 07:33:33  lyonsd
 * This has been tested on every system, and it works successfully.
 *
 * Revision 1.1  2014/03/12 22:40:00  lyonsd
 * Initial revision
 */
static const char *rcsid="$Id: mknlrescs.c,v 1.6 2014/05/23 09:22:20 lyonsd Exp $";

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 10000
#endif
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <locale.h>
#include "utils.h"
#include "xsection.h"
#include "csheader.h"
#include "labels.h"
#include "csdata.h"

int main(int argc,char *argv[])
{
	xsection *xs=NULL;
	const char nl[]="nl";

/* getopt variables */
	int nopt;

/* Locale */
	char *locale;

    locale = setlocale(LC_ALL, "");

/* Initialize the xsection variable */
	xs=initcs();

/* Process command-line arguments, and overwrite default values with
 * any command line options which were specified.
 */
	while((nopt=getopt(argc,argv,"hvA:I:Z:N:L:i:o:"))!=-1)
	{
		switch(nopt)
		{
			case 'h':
				fputs(LBLUE,stderr);
				fprintf(stderr,"Usage: %s [-h] [-v] [-A «atom»] [-I «ion»] [-Z «charge»] [-N «name»] [-L «initials»] [-i «infile»] [-o «outfile»]",argv[0]);
				fprintf(stderr,"\n\n  -h\t\tprints this help message");
				fprintf(stderr,"\n  -v\t\tprints the rcsid");
				fprintf(stderr,"\n  -A «atom»\tspecify the abbreviation for the target atom,\n\t\tsuch as \"H\", or \"He\", etc");
				fprintf(stderr,"\n  -I «ion»\tspecify the abbreviation for the ion,\n\t\tsuch as \"Mg\", or \"Ne\", etc");
				fprintf(stderr,"\n  -Z «charge»\tspecify an integer for the charge,\n\t\tsuch as \"6\", or \"10\", etc");
				fprintf(stderr,"\n  -N «name»\tspecify name, such as \"John Q Public\"\n\t\t(quotes required if there are blankspaces)");
				fprintf(stderr,"\n  -L «initials»\tspecify initials, such as \"JQP\"");
				fprintf(stderr,"\n  -i «infile»\tspecify the name of the file\n\t\twhich contains the cross section data");
				fprintf(stderr,"\n  -o «outfile»\tspecify the name of the file\n\t\tto which to send the output");
				fprintf(stderr,"\n\nThis program takes as STDIN (unles the [-i] option is used) cross section data, and writes to STDOUT (unless the [-o] option is used) the cross section data with a header for a *.cs file.  The cross section labels are read from another file, typically the NIST input file to lzdata.  The data label filename is constructed from the ion and the charge, for example: mg10.nist.in.");
				fprintf(stderr,"\n\nIf the atom, ion, charge, name and initials are not specified on the command line, these variables will be set by the following environment variables:");
				fprintf(stderr,"\n\n\tatom|ATOM\n\tion|ION\n\tz|Z\n\tname|NAME|LOGNAME\n\tinitials|INITIALS");
				fprintf(stderr,"\n\nAnything set on the command line will override any environment variable setting.");
				fprintf(stderr,"\n\nIf the variable cannot be set via command line options, or via environment variables, then they will default to the following values:");
				fprintf(stderr,"\n\n\t«atom»\n\t«ion»\n\t0\n\t«NAME»\n\t«XXX»\n");
				fputs(RESET,stderr);
				exit(1);
				break;
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
				exit(1);
                break;
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
			case 'A':	/* ATOM */
				xs->ATOM=(char*)malloc(strlen(optarg)+1);
				strcpy(xs->ATOM,optarg);
				break;
			case 'I':	/* ION */
				xs->ION=(char*)malloc(strlen(optarg)+1);
				strcpy(xs->ION,optarg);
				break;
			case 'Z':	/* CHARGE */
				xs->Z=atoi(optarg);
				break;
			case 'N':	/* NAME */
				xs->NAME=(char*)malloc(strlen(optarg)+1);
				strcpy(xs->NAME,optarg);
				break;
			case 'L':	/* INITIALS */
				xs->INITIALS=(char*)malloc(strlen(optarg)+1);
				strcpy(xs->INITIALS,optarg);
				break;
			default:
				break;
		}	/* END switch(nopt) */
	}	/* END while((nopt=getopt(argc,argv,"A:I:Z:N:L:"))!=-1) */

/*	Get the nl-resolved labels */
	xs->nllab=reverselabels(getnistlabels(getlabelfn(xs)));

/*	Get the cross section data, as well as the number of rows and columns */
	xs=getcsdata(xs);

/*	Trim the nl labels to make sure we have the correct number of them, based on
 *	the number of columns of cross section data
 */
	xs->nllab=trimlabels(xs->nllab,xs->nlcols);

/*	Print out the cross section header
 *	Since the xs variable can contain both nl- and n-resolved data and labels,
 *	we need to let the function know which set to process.
 */
	prtcsheader(xs,nl);

/*	Print out the cross section data
 *	Since the xs variable can contain both nl- and n-resolved data and labels,
 *	we need to let the function know which set to process.
 */
	prtcsdata(xs,nl);

	return 0;
}	/* END int main(int argc,char *argv[]) */
