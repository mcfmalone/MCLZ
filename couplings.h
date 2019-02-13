/* $Id: couplings.h,v 1.1 2014/04/22 04:58:03 lyonsd Exp $
 * $Log: couplings.h,v $
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

#ifndef COUPLINGS_H
#define COUPLINGS_H

void prtcouplingsrcs(void);

/* Butler and Dalgarno */
double bd(double,double);

/* Olson and Salop */
double os(double,double,short int);

/* Olson, Salop, and Taulberg */
double ost(double,double,short int,int,int);

/* Olson, Smith, and Bauer */
double osb(double,double,double);

#endif
