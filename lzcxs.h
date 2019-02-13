/* $Id: lzcxs.h,v 1.2 2014/05/23 09:22:20 lyonsd Exp $
 * $Log: lzcxs.h,v $
 * Revision 1.2  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.1  2014/04/22 04:58:03  lyonsd
 * Initial revision
 *
 *
 * This file contains routines that deak with the element_t and cs_t types
 */

#ifndef LZCSX_H
#define LZCSX_H
#include "element.h"

typedef struct
{
	double rx;				/* value of r at avoided crossing */
	double dele;			/* energy difference at avoided crossing */
} cx_t;						/* crossing data */

typedef struct
{
	element_t *atom;
	element_t *ion;
	short int z;				/* charge of system */
	unsigned short int ncx;		/* number of avoided crossings */
	unsigned short int n_min;	/* minimum n to display */
	unsigned short int n_max;	/* maximum n to display */
	float gp;					/* degeneracy factor of incoming channel
								   (singlets: 0.25, triplets: 0.75)
								   otherwise 1.0 */
	float mu;					/* reduced mass */
	double deli;				/* difference in ionization energies */
	double du_min;				/* Minimum dele to display; any values of dele smaller than
								   this will not be printed */
	unsigned char header;		/* this is used as a flag, whether or not to print the
								 * first three lines conforming to the old fort.3
								 * output
								 */
	cx_t *cx;					/* crossing data */
	cx_t *cxi;					/* crossing data (improved) */
} cxs_t;						/* crossing system type */

void prtlzcxs(void);
cxs_t *initcxs(element_t*,element_t*);
cxs_t *prtcxs(cxs_t*);
cxs_t *prtoldcxs(cxs_t*);
cx_t *prtcx(cx_t*,unsigned short int);
cxs_t *lzcxs(cxs_t*);

#endif
