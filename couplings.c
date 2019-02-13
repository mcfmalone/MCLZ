/* $Id: couplings.c,v 1.3 2014/05/23 09:22:20 lyonsd Exp $
 * $Log: couplings.c,v $
 * Revision 1.3  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.2  2014/04/30 17:14:32  lyonsd
 * Compiles without warnings on i686-apple-darwin10-gcc-4.2.1 (GCC) 4.2.1
 *
 * Revision 1.1  2014/04/22 04:58:03  lyonsd
 * Initial revision
 *
 *
 * Coupling routines used in Landau-Zener crossing estimator
 *
 * The couples provided in this file are:
 *
 * Butler & Dalgarno (1980)
 * Used for any ion with q<=4.
 *
 * Olson-Salop (1976)
 * Used for any q, but with bare ions.
 *
 * Olson-Salop-Taulberb (1986)
 * Used for partially stripped ions of any charge q
 *
 * Olson, Smith and Bauer
 *
 */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE	10000	/* Needed to suppress compiler warnings with -ansi flag */
#endif
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "couplings.h"
#include "element.h"
#include "utils.h"

static const char *rcsid="$Id: couplings.c,v 1.3 2014/05/23 09:22:20 lyonsd Exp $";
extern double lgamma(double);

void prtcouplingsrcs(void)
{
	(void)fprintf(stderr,"%s\n",rcsid);
}

/* Butler and Dalgarno */

double bd(double rx,double ita)
{
	return pow(rx,2.0)*exp(-rx*sqrt(2.0*ita));
}

/*Olson and Salop */

double os(double rx,double ita,short int charge)
{
	double a=0.0;
	double b=0.0;

	a=18.26/sqrt((double)charge);
	b=1.872*sqrt(ita/(double)charge);

	return a*exp(-b*rx);
}

/* Olson, Salop, and Taulberg */

double ost(double rx,double ita,short int charge,int PQN,int AQN)
{
	short int Z=charge;
	return os(rx,ita,Z)*exp(0.5*(log(2.0*AQN+1.0)-lgamma((double)PQN-AQN)-lgamma((double)(PQN+AQN+1)))+lgamma((double)PQN));
}

/* Olson, Smith and Bauer */

double osb(double rx,double ita,double ipa)
{
	/* ita - ionization energy of atom, in au) */
	/* ipa - difference between ionication energy and energy level of ion, in au */

	double a;
	double b;

	b=sqrt(ita/2)+sqrt(ipa/2);
	a=2.0*b*sqrt(ita*ipa);
	return a*rx*exp(-0.86*r*b);
}
