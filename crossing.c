/* $Id: crossing.c,v 1.5 2014/05/23 09:22:20 lyonsd Exp $
 * $Log: crossing.c,v $
 * Revision 1.5  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.4  2014/04/29 15:04:43  lyonsd
 * Minor change to rcsid
 *
 * Revision 1.3  2014/04/29 14:56:30  lyonsd
 * Changes to rcsid
 *
 * Revision 1.1  2014/04/22 04:58:03  lyonsd
 * Initial revision
 *
 *
 * Landau-Zener crossing estimator
 *
 * code to estimate Rx and Delta U(R_x) using the
 * polarization potential and short-range repulsive
 * term of Butler & Dalgarno. Three forms of Delta U
 * are then determined: i) Butler-Dalgarno (1980),
 * ii) Olson-Salop (1976), and iii) Olson-Salop-Taulbjerg
 * (1986). The first is for any ion with q<=4, the second
 * is for any q, but for bare ions, while the latter
 * is for partially-stripped ions of any charge q.
 *
 * Rx occurs at point were second column changes sign
 * output in crossing.out
 */

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE	10000	/* Needed to suppress compiler warnings with -ansi flag */
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "element.h"
#include "couplings.h"
#include "crossing.h"
#include "lzcxs.h"
#include "utils.h"

static const char rcsid[]="$Id: crossing.c,v 1.5 2014/05/23 09:22:20 lyonsd Exp $";

void prtcrossingrcs()
{
	(void)fprintf(stderr,"%s\n",rcsid);
}

cxs_t *crossing(cxs_t *cxs)
{

	cx_t *cx=NULL;
	cx_t *cxi=NULL;
	bool bareion=false;
	unsigned short int nx=0;
	unsigned short int ncx=0;
	unsigned int ix=0;
	float a=0.0;
	float b=0.0;
	float alpha=0.0;
	double rx=0.0;
	double du=0.0;

	ncx=cxs->ncx;

/* if the charge and atomic number of ion are the same,
 * then it's hydrogen-like */
	if(cxs->ion->z == cxs->ion->a)
		bareion=true;

	cx=cxs->cx;
	a=25.0f*cxs->ion->z;
	b=-0.8f+cxs->atom->zeta;
	alpha=cxs->atom->alpha;

	for(nx=1;nx<ncx;nx++)
	{
		ix=0;
		do
		{
			rx=cx[nx].rx+(ix-1.0)*1e-6;
			du=a*exp(b*rx)-0.5*alpha*pow(cxs->ion->z,2)/pow(rx,4)-(cxs->ion->z-1.0)/rx+cx[nx].dele;
			++ix;
		} while(du<0.0);
  
		cxi=(cx_t*)realloc(cxi,sizeof(cx_t)*(size_t)(1+nx));
		cxi[nx].rx=rx;
		if(bareion==true)
		{
			cxi[nx].dele=os(rx,cxs->atom->ita[0],cxs->z);
		}
		else
		{
			cxi[nx].dele=ost(rx,cxs->atom->ita[0],cxs->z,cxs->ion->nist[nx].n,cxs->ion->nist[nx].l);
		}
	}	

	cxs->cxi=cxi;

	return cxs;
}
