/* $Id: csdata.h,v 1.6 2014/05/23 09:22:20 lyonsd Exp $
 * $Log: csdata.h,v $
 * Revision 1.6  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.5  2014/04/28 14:05:20  lyonsd
 * Compiles under Linux with no warnings or errors
 *
 * Revision 1.4  2014/04/22 04:58:03  lyonsd
 * Updated for -ansi compiler flag.
 *
 * Revision 1.3  2014/03/22 01:47:40  lyonsd
 * Working for all systems
 *
 * Revision 1.2  2014/03/19 07:33:33  lyonsd
 * Works with mknlrescs on every system
 *
 * Revision 1.1  2014/03/12 22:40:00  lyonsd
 * Initial revision
 */

#ifndef CSDATA_H
#define CSDATA_H

#include "xsection.h"

/* This function prints the rcsid for csdata.c */
void prtcsdatarcs(void);

/* This function reads the data, row by row, breaks the line into tokens,
 * converts those tokens into doubles, and stores them into a 2-dimentional
 * array of doubles.  It keeps track of the number of rows and columns read,
 * and modifies the parameter addresses with the addresses of the integers
 * containing the number of rows and columns
 *
 * INPUT
 * Two integer pointers (addresses to integers)
 *
 * RETURN
 * A 2-dimensional array (the address of an array of addresses of doubles,
 * where the values are stored
 *
 * OUTPUT
 * None
 */
xsection *getcsdata(xsection*);
xsection *prtcsdata(xsection*,const char[]);
double csmaxx(double**,int);
double csminx(double**,int);
double csmaxy(double**,int,int);
double csminy(double**,int,int);
xsection *getnrescs(xsection*);

#endif
