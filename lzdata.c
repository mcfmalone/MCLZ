/* $Id: lzdata.c,v 1.3 2014/05/23 09:22:20 lyonsd Exp $
 * $Log: lzdata.c,v $
 * Revision 1.3  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.2  2014/04/29 15:03:49  lyonsd
 * Minor updates to version printing and fprintf statements
 *
 * Revision 1.1  2014/04/22 04:58:03  lyonsd
 * Fixed all compiler warnings.
 *
 * Revision 1.0  2014/03/30 01:56:19  lyonsd
 * Initial revision
 */

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE	10000	/* Needed to suppress compiler warnings with -ansi flag */
#endif
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "crossing.h"
#include "couplings.h"
#include "element.h"
#include "lzcxs.h"
#include "nist.h"
#include "utils.h"

static const char rcsid[]="$Id: lzdata.c,v 1.3 2014/05/23 09:22:20 lyonsd Exp $";
static const char manfile[]="lzdata.1";
void lzdatahelp(const char[]);
static struct option long_options[] =
{
    {"man", no_argument, NULL, 'm'},
    {"version", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {"input", required_argument, NULL, 'i'},
    {"output", required_argument, NULL, 'o'},
    {"ion", required_argument, NULL, 'I'},
    {"atom", required_argument, NULL, 'A'},
    {"charge", required_argument, NULL, 'Z'},
    {"n_min", required_argument, NULL, 'n'},
    {"n_max", required_argument, NULL, 'N'},
    {"du_min", required_argument, NULL, 'u'},
    {"header", no_argument, NULL, 'H'},
    {"singlets", no_argument, NULL, 's'},
    {"triplets", no_argument, NULL, 't'},
    {NULL, 0, NULL, 0}
};

int main(int argc,char *argv[])
{

	char *ionstr=NULL;
	char *atomstr=NULL;
    char *sysstr=NULL;
	short int Q=0;
	unsigned short int n_min=0;
	unsigned short int n_max=USHRT_MAX;
	unsigned char header=0;
	bool noatom=true;
	bool noion=true;
	bool nocharge=true;
	bool singlets=false;
	bool triplets=false;
	element_t *atom=NULL;
	element_t *ion=NULL;
	double du_min=0.0;
	cxs_t *cxs=NULL;

/* getopt variables */
	int nopt;

/* Locale */
	char *locale;

    locale=setlocale(LC_ALL, "");

	while((nopt=getopt_long(argc,argv,"mA:I:Z:Hhvsti:o:n:N:u:",long_options,NULL))!=-1)
		switch(nopt)
		{
			case 'h':
				lzdatahelp(argv[0]);
				exit(EXIT_SUCCESS);
			case '?':
				lzdatahelp(argv[0]);
				exit(EXIT_SUCCESS);
			case 'v':
				(void)fprintf(stderr,"%s\n",locale);
				(void)fprintf(stderr,"%s\n",rcsid);
				prtcouplingsrcs();
				prtcrossingrcs();
				prtelementrcs();
				prtutilsrcs();
				prtnistrcs();
				prtlzcxs();
				exit(EXIT_SUCCESS);
			case 'i':
				if(!access(optarg,R_OK))
				{
					if(!freopen(optarg,"r",stdin))
						exit(EXIT_FAILURE);
				}
				else
				{
					(void)fprintf(stderr,"Error %d: %s does not exist for reading\n",errno,optarg);
					exit(EXIT_FAILURE);
				}
				break;
			case 'o':
				if(!freopen(optarg,"w",stdout))
					exit(EXIT_FAILURE);
				break;
			case 'A':	/* ATOM */
				noatom=false;
				atomstr=(char*)malloc(strlen(optarg)+1);
				strcpy(atomstr,optarg);
				break;
			case 'I':	/* ION */
				noion=false;
				ionstr=(char*)malloc(strlen(optarg)+1);
				strcpy(ionstr,optarg);
				break;
			case 'Z':	/* CHARGE */
				nocharge=false;
				Q=(short int)atoi(optarg);
				break;
			case 'n':
				n_min=(unsigned short int)atoi(optarg);
				break;
			case 'N':
				n_max=(unsigned short int)atoi(optarg);
				break;
			case 'u':
				du_min=atof(optarg);
				break;
			case 'H':
				header=1;
				break;
            case 'm':
                if(-1!=access(manfile,F_OK))
                {
                    sysstr=(char*)malloc(sizeof(char)*(16+strlen(manfile)));
                    sprintf(sysstr,"/usr/bin/man ./%s",manfile);
                    if(0!=system(sysstr))
                        (void)fprintf(stderr,"Error: Unable to dislpay man\n");
                    free(sysstr);
                    exit(EXIT_FAILURE);
                }
                else
                {
                    (void)fprintf(stderr,"Error: 404: %s: Not found\n",manfile);
                    exit(EXIT_FAILURE);
                }
                break;
			case 't':
				if(singlets)
				{
					(void)fprintf(stderr,"Error: cannot specify both singlets and triplets\n");
					exit(EXIT_FAILURE);
				}
				triplets=true;
				break;
			case 's':
				if(triplets)
				{
					(void)fprintf(stderr,"Error: cannot specify both singlets and triplets\n");
					exit(EXIT_FAILURE);
				}
				singlets=true;
				break;
			default:
				lzdatahelp(argv[0]);
				exit(EXIT_SUCCESS);
				break;
		}

	if(!ion)	/* check to see if the structure has been allocated */
		ion=(element_t*)malloc(sizeof(element_t));
	if(noion) /* If no ion was specified on command line ... */
	{
		/* We'll check the environment */
		if(!(ion->symbol=getenv("ion")) && !(ion->symbol=getenv("ION")))
		{ 
			(void)fprintf(stderr,"Error %d: no ion specified on command line or environment variables: $ion; $ION\n",EXIT_FAILURE);
			exit(EXIT_FAILURE);
		}	/* if no ion is found in the environment, we can't continue */
	}
	else	/* We'll get here if an ion was specified on the command line */
	{
		ion->symbol=(char*)malloc(strlen(ionstr)+1);
		strcpy(ion->symbol,ionstr);
	}

	if(nocharge)
	{
		if(getenv("z"))
		{ 
			ion->z=(short int)atoi(getenv("z"));
		}
		else if(getenv("Z"))
			{
				ion->z=(short int)atoi(getenv("Z"));
			}
			else
			{
				(void)fprintf(stderr,"Error: no charge specified on command line or environment variables: $z; $Z\n");
				exit(EXIT_FAILURE);
			}
	}
	else
	{
		ion->z=Q;
	}

	if(!atom)	/* check to see if the structure has been allocated */
		atom=(element_t*)malloc(sizeof(element_t));
	if(noatom) /* If no atom was specified on command line ... */
	{
		/* We'll check the environment */
		if(!(atom->symbol=getenv("atom")) && !(atom->symbol=getenv("ATOM")))
		{ 
			(void)fprintf(stderr,"Error: no atom specified on command line or environment variables: $atom; $ATOM\n");
			exit(EXIT_FAILURE);
		}	/* if no atom is found in the environment, we can't continue */
	}
	else	/* We'll get here if an atom was specified on the command line */
	{
		atom->symbol=(char*)malloc(strlen(atomstr)+1);
		strcpy(atom->symbol,atomstr);
	}
	atom->z=0;

/* Initialize the ion and element
 * These two calls read the elements.dat file to extract the appropriate
 * information, and assign the proper values to the field in the data
 * structure
 */
	ion=initelement(ion);
	atom=initelement(atom);
			
/* Get the NIST data of the ion
 * The NIST data will either be read via STDIN, or via the file specified on
 * the command line.  We'll reverse the data, and assign it to the ion's
 * data structure.
 */
	ion->nist=reversenist(readnist());

/*	(void)prtelement(ion);
	(void)prtelement(atom);
 */

/* Initialize the crossing system
 * This assigns the appropriate values to the variables in the cxs structure
 */
	cxs=initcxs(atom,ion);
	cxs->n_min=n_min;
	cxs->n_max=n_max;
	cxs->du_min=du_min;
	cxs->header=header;
	if(singlets)
		cxs->gp=0.25;
	else if(triplets)
		cxs->gp=0.75;
	cxs=lzcxs(cxs);
	cxs=crossing(cxs);
/*	(void)prtcxs(cxs); */
	(void)prtoldcxs(cxs);

	free(ion);
	free(atom);
	free(cxs);
	return 0;
}

void lzdatahelp(const char arg[])
{
	(void)fprintf(stderr,"Usage: %s [ \033[4moptions\033[0m ]\n\n",arg);
	(void)fprintf(stderr,"\t-h, --help\n\t\tprints this help message.\n\n");
	(void)fprintf(stderr,"\t-v, --version\n\t\tprints the rcsid.\n\n");
	(void)fprintf(stderr,"\t-m, --man\n\t\tprints the man page.\n\n");
	(void)fprintf(stderr,"\t-i, --input=\033[4minfile\033[0m\n\t\tName of the file which contains the NIST data.\n\n");
	(void)fprintf(stderr,"\t-o, --output=\033[4moutfile\033[0m\n\t\tName of the file to which to send the output.\n\n");
	(void)fprintf(stderr,"\t-A, --atom=\033[4mstring\033[0m\n\t\tAtomic symbol for the atom.\n\n");
	(void)fprintf(stderr,"\t-I, --ion=\033[4mstring\033[0m\n\t\tAtomic symbol for the ion.\n\n");
	(void)fprintf(stderr,"\t-Z, --charge=\033[4mint\033[0m\n\t\tCharge of the ion.\n\n");
	(void)fprintf(stderr,"\t-n, --n_min=\033[4mint\033[0m\n\t\tMinimum value for \033[4mn\033[0m.\n\n");
	(void)fprintf(stderr,"\t-N, --n_max=\033[4mint\033[0m\n\t\tMaximum value for \033[4mn\033[0m.\n\n");
	(void)fprintf(stderr,"\t-u, --du_min=\033[4mfloat\033[0m\n\t\tMinimum value for \033[4mdU\033[0m at avoided crossing \033[4mrx\033[0m.\n\n");
	(void)fprintf(stderr,"\t-s, --singlets\n\t\tSinglet states of the ion.\n\n");
	(void)fprintf(stderr,"\t-t, --triplets\n\t\tTriplet states of the ion.\n\n");
	(void)fprintf(stderr,"\t-H, --header\n\t\tPrint the 3-line \"header\" data for compatibility with the old-style \033[4mfort.3\033[0m.\n\n");
	(void)fprintf(stderr,"\nThis program takes as STDIN (unles the [-i] option is used) NIST data, and writes to STDOUT (unless the [-o] option is used) the crossing data.\n");
}
