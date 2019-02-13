/* $Id: nist.c,v 1.3 2014/05/29 03:40:46 lyonsd Exp $
 * $Log: nist.c,v $
 * Revision 1.3  2014/05/29 03:40:46  lyonsd
 * Fixed NIST data sorting issue
 *
 * Revision 1.2  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.1  2014/04/22 04:58:03  lyonsd
 * Updated for -ansi compiler flag.
 *
 * Revision 1.0  2014/03/30 01:56:19  lyonsd
 * Initial revision
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "nist.h"
#include "utils.h"

static const char *rcsid="$Id: nist.c,v 1.3 2014/05/29 03:40:46 lyonsd Exp $";

void prtnistrcs()
{
	fprintf(stderr,"%s\n",rcsid);
}

nist_t *readnist()
{
	nist_t *nist=NULL;	/* NIST data */
	char line[50];		/* A single line of NIST data */
	char config[10];	/* nl quantum numbers */
	char term[10];		/* Rotational term */
	float j;
	double level;		/* energy level */
	int len=50;			/* default length of string */
	size_t a=0;			/* counter/index variable */
	int N;				/* principal quantum number */
	char c;				/* azimuthal quantum number */
	int en=0;			/* error number */

	while(fgets(line,len,stdin))
	{
		(void)sscanf(line,"%s\t%s\t%f\t%lf",config,term,&j,&level);
		if(NULL==(nist=(nist_t*)realloc(nist,sizeof(nist_t)*(a+1))))
		{
			en=errno;
			fprintf(stderr,"Error %d: mamory allocation failure\n",en);
			fprintf(stderr,"File:\t\t%s\n",__FILE__);
			fprintf(stderr,"Function:\t%s\n",__FUNCTION__);
			fprintf(stderr,"Line:\t\t%d\n",__LINE__-6);
			exit(en);
		};
		nist[a].config=(char*)malloc(sizeof(char)*(strlen(config)+1));
		nist[a].term=(char*)malloc(sizeof(char)*(strlen(term)+1));
		strcpy(nist[a].config,config);
		sscanf(config,"%d%c",&N,&c);
		nist[a].n=N;
		nist[a].l=chartoaqn(c);
		strcpy(nist[a].term,term);
		nist[a].j=j;
		if(level==0.0)
		{
			nist[a].level=0.0;	/* to eliminate any rounding errors */
		}
		else
		{
			nist[a].level=cmtohartree(level); /* convert levels from cm^-1 to hartrees (au) */
		}
		++a;
	}
	if(NULL==(nist=(nist_t*)realloc(nist,sizeof(nist_t)*(a+1))))
	{
		en=errno;
		fprintf(stderr,"Error %d: mamory allocation failure\n",en);
		fprintf(stderr,"File:\t\t%s\n",__FILE__);
		fprintf(stderr,"Function:\t%s\n",__FUNCTION__);
		fprintf(stderr,"Line:\t\t%d\n",__LINE__-6);
		exit(en);
	};
	nist[a].config=NULL;
	nist[a].term=NULL;
	nist[a].j=0.0;
	nist[a].level=0.0;

	return nist;
}

/* This simply prints the nist_t variable to stderr.  Intended to be used for
 * debugging purposes.
 */
nist_t *prtnist(nist_t *nist)
{
	unsigned short int a=0;

	while((NULL!=nist[a].config) && (NULL!=nist[a].term))
	{
		if(nist[a].config[0]!='-')
			(void)fprintf(stderr,"%s\t%d\t%d\t%s\t%14.8e\n",nist[a].config,nist[a].n,nist[a].l,nist[a].term,nist[a].level);
		else
			(void)fprintf(stderr,"%s\t%s\t%s\t%s\t%14.8e\n","Limit","--","--","--",nist[a].level);
		++a;
	}
	return nist;
}		

/* Prints the nist_t variable to stdout, with restrictions: only n-values
 * between n_low and n_high will be printed; only l-values between l_low
 * and l_high will be printed; only certain degeneracies (singlets, doublets,
 * triplets, etc) will be printed.
 */
nist_t *prtnist_r(nist_t *nist,unsigned short int n_low,unsigned short int n_high, unsigned short int l_low, unsigned short int l_high, unsigned short int degeneracy)
{
	unsigned short int a=0;	/* used as index counter */
	unsigned short int spin_multiplicity=0; /* spin_multiplitiy=2S+1 */

	while((NULL!=nist[a].config) && (NULL!=nist[a].term))
	{
		if((nist[a].term[0]==-71 || nist[a].term[1]==-71) || nist[a].term[0]==49)
			spin_multiplicity=1;
		if((nist[a].term[0]==-78 || nist[a].term[1]==-78) || nist[a].term[0]==50)
			spin_multiplicity=2;
		if((nist[a].term[0]==-77 || nist[a].term[1]==-77) || nist[a].term[0]==51)
			spin_multiplicity=3;
		if(nist[a].term[0]==52)
			spin_multiplicity=4;
		if(nist[a].term[0]==53)
			spin_multiplicity=5;

		if(nist[a].config[0]!='-')
		{
			if(nist[a].n>=n_low && nist[a].n<=n_high && nist[a].l>=l_low && nist[a].l<=l_high)
			{
				if(degeneracy==0 || degeneracy==spin_multiplicity)
				{
					(void)fprintf(stdout,"%s\t%s\t%3.1f\t%14.6f\n",nist[a].config,nist[a].term,nist[a].j,hartreetocm(nist[a].level));
				}
			}
		}
		else
			(void)fprintf(stdout,"%s\t%s\t%3.1f\t%14.6f\n","---","---",nist[a].j,hartreetocm(nist[a].level));
		++a;
	}
	return nist;
}		

/* character to aqn (azimuthal quantum number) */
aqn chartoaqn(char c)
{
	switch(c)
	{
		case 's': return s;
		case 'p': return p;
		case 'd': return d;
		case 'f': return f;
		case 'g': return g;
		case 'h': return h;
		case 'i': return i;
		case 'k': return k;
		case 'l': return l;
		case 'm': return m;
		case 'n': return n;
		case 'o': return o;
		case 'q': return q;
		case 'r': return r;
		case 't': return t;
		case 'u': return u;
		case 'v': return v;
		case 'w': return w;
		case 'x': return x;
		case 'y': return y;
		case 'z': return z;
		default:
			  fprintf(stderr,"Error: invalid azimuthal quantum number: %c\n",c);
			  exit(EXIT_FAILURE);
			  break;
	}
}

nist_t *reversenist(nist_t *nist)
{

	nist_t *newnist=NULL;
	unsigned short int a=0;
	unsigned short int j=0;
	unsigned short int b=0;

	for(a=0;(NULL!=nist[a].config) && (NULL!=nist[a].term);a++)
	{
		newnist=(nist_t*)realloc(newnist,sizeof(nist_t)*(unsigned)(1+a));
	}
	for(j=0;j<a;j++)
	{
		b=(unsigned short)(a-1-j);
		newnist[j]=nist[b];
		newnist[j].config=(char*)malloc(sizeof(char)*(1+strlen(nist[b].config)));
		strcpy(newnist[j].config,nist[b].config);
		newnist[j].term=(char*)malloc(sizeof(char)*(1+strlen(nist[b].term)));
		strcpy(newnist[j].term,nist[b].term);
		newnist[j].n=nist[b].n;
		newnist[j].l=nist[b].l;
		newnist[j].j=nist[b].j;
		newnist[j].level=nist[b].level;
	}
	newnist[a].config=NULL;
	newnist[a].term=NULL;
	newnist[a].n=0;
	newnist[a].l=0;
	newnist[a].j=0;
	newnist[a].level=0;

	return newnist;
}
