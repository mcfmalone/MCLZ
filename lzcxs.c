/* $Id: lzcxs.c,v 1.3 2014/05/26 23:30:47 lyonsd Exp micha $
 * $Log: lzcxs.c,v $
 * Revision 1.3  2014/05/26 23:30:47  lyonsd
 * Fixed bareion check
 *
 * Revision 1.2  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.1  2014/04/22 04:58:03  lyonsd
 * Initial revision
 *
 *
 * This file contains routines that deak with the element_t and cs_t types
 */

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <limits.h>
#include "element.h"
#include "lzcxs.h"
#include "utils.h"

static const char *rcsid="$Id: lzcxs.c,v 1.3 2014/05/26 23:30:47 lyonsd Exp micha $";

void prtlzcxs()
{
	(void)fprintf(stderr,"%s\n",rcsid);
}

cxs_t *initcxs(element_t *atom,element_t *ion)
{
	cxs_t *cxs=NULL;
	int errnum=0;

	if(NULL==(cxs=(cxs_t*)malloc(sizeof(cxs_t))))
	{
		errnum=errno;
		fprintf(stderr,"Error %d: memory allocation failure\n",errnum);
		fprintf(stderr,"File: %s\nFunction: %s\nLine: %d\n",__FILE__,__FUNCTION__,__LINE__-4);
		exit(errnum);
	}
	cxs->mu=(atom->m)*(ion->m)/((atom->m)+(ion->m));
	cxs->atom=atom;
	ion->ita[0]=ion->nist[0].level;
	cxs->ion=ion;
	cxs->z=(short int)((atom->z)+(ion->z));
	cxs->gp=1.0;
	cxs->mu=(atom->m)*(ion->m)/((atom->m)+(ion->m));
	cxs->deli=(ion->ita[0])-(atom->ita[0]);	/* This is the difference between the
											   ion and that of the atom */
	cxs->ncx=0;
	cxs->n_min=0;
	cxs->n_max=USHRT_MAX;
	cxs->du_min=0.0;
	cxs->header=0;
	cxs->cx=NULL;

	return cxs;
}

cxs_t *lzcxs(cxs_t *cxs)
{
		unsigned short int nx=0;
		nist_t *tnist=NULL;
		cx_t *cx=NULL;
		int errnum=0;

		tnist=cxs->ion->nist;

		for(nx=1;NULL!=tnist[nx].config;nx++)
		{
			if(NULL==(cx=(cx_t*)realloc(cx,sizeof(cx_t)*(size_t)(1+nx))))
			{
				errnum=errno;
				fprintf(stderr,"Error %d: Memory allocation failure\n",errnum);
				fprintf(stderr,"File: %s\nFunction: %s\nLine: %d\n",__FILE__,__FUNCTION__,__LINE__-4);
				exit(errnum);
			}
			else
			{
				cx[nx].dele=(cxs->deli)-(cxs->ion->nist[nx].level);
				cx[nx].rx=(cxs->z-1.0)/cx[nx].dele;
			}
		}
		cxs->cx=cx;
		cxs->ncx=nx;
		return cxs;
}

/* print crossing system: atom data, ion data, system string, reduced mass,
 * ionization difference, crossing data
 *
 * Useful for debugging */

cxs_t *prtcxs(cxs_t *cxs)
{
	(void)fprintf(stderr,"## ATOM ##\n");
	(void)prtelement(cxs->atom);
	(void)fprintf(stderr,"## ION ##\n");
	(void)prtelement(cxs->ion);
	fprintf(stderr,"System:          %s^%hu+ + %s\n",cxs->ion->symbol,cxs->ion->z,cxs->atom->symbol);
	fprintf(stderr,"Reduced mass:    %f (au)\n",cxs->mu);
	fprintf(stderr,"Ionization diff: %14.8e (au)\n",cxs->deli);
	(void)fprintf(stderr,"##############################\n");
	(void)fprintf(stderr,"#     Crossing data (au)     #\n");
	(void)fprintf(stderr,"# rx\t\t  du         #\n");
	(void)fprintf(stderr,"##############################\n");
	(void)prtcx(cxs->cx,cxs->ncx);
	(void)fprintf(stderr,"##############################\n");
	(void)fprintf(stderr,"#     Crossing data (au)     #\n");
	(void)fprintf(stderr,"#       with coupling        #\n");
	(void)fprintf(stderr,"# rx\t\t  du         #\n");
	(void)fprintf(stderr,"##############################\n");
	(void)prtcx(cxs->cxi,cxs->ncx);
	return cxs;
}

/* print crossing data: energy difference, rx
 *
 * Useful for debugging */

cx_t *prtcx(cx_t *cx,unsigned short int ncx)
{
	unsigned short int nx;

	for(nx=1;nx<ncx;nx++)
	{
		(void)fprintf(stderr,"%14.8e\t%14.8e\n",cx[nx].rx,cx[nx].dele);
	}
	return cx;
}

/* print crossing system with three lines of data for input to lzmcro.
 * The initial three lines are:
 * start energy, step size, and qon (for rotational coupling)
 * reduced mass, charge
 * alpha, zeta, gp (degeneracy factor of incoming channel)
 *
 * Then, the avoided crossings will be printed:
 *
 * rx, du, n (principle quantum number)
 */
cxs_t *prtoldcxs(cxs_t *cxs)
{
	unsigned short int qon=0;
	unsigned short int nx=0;
	float estart=1.0e-3f;
	float de=5.0e-3f;

	if(cxs->ion->z==cxs->ion->a)
		qon=1;
	if(cxs->header==1)
	{
		(void)fprintf(stdout,"%14.3e\t%14.3e\t%5d\n",estart,de,qon);
		(void)fprintf(stdout,"%14.8e\t%14d\n",cxs->mu,cxs->z);
		(void)fprintf(stdout,"%14.8e\t%14.2f\t%5.2f\n",cxs->atom->alpha,cxs->atom->zeta,cxs->gp);
	}
	for(nx=1;nx<cxs->ncx;nx++)
	{
		if(cxs->cxi[nx].rx>0.0 && cxs->cxi[nx].dele>cxs->du_min && cxs->ion->nist[nx].n>=cxs->n_min && cxs->ion->nist[nx].n<=cxs->n_max)
			(void)fprintf(stdout,"%14.8e\t%14.8e\t%5d\n",cxs->cxi[nx].rx,cxs->cxi[nx].dele,cxs->ion->nist[nx].n);
		/* Prints in the form:
		 *
		 * 0.00000000e+00  0.00000000e-00      0
		 */
	}

	return cxs;
}
