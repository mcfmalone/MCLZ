/* $Id: element.h,v 1.2 2014/04/29 14:48:43 lyonsd Exp $
 * $Log: element.h,v $
 * Revision 1.2  2014/04/29 14:48:43  lyonsd
 * Update to rcsid
 *
 * Revision 1.1  2014/04/22 04:58:03  lyonsd
 * Initial revision
 *
 *
 * Type declarations for elements (atoms and ions)
 */

#include "nist.h"

#ifndef ELEMENT_H
#define ELEMENT_H

typedef struct
{
	unsigned short int a;	/* atomic number */
	char *symbol;			/* element symbol */
	char *name;				/* element name */
	short int z;			/* charge, if this is an ion */
	float m;				/* atomic mass, in au */
	float alpha;			/* polarizability */
	float zeta;				/* -1 for H, -1.7 for He */
	double *ita;			/* ionization energy, in au */
	nist_t *nist;			/* energy levels */
} element_t;


void prtelementrcs(void);
element_t *initelement(element_t*);
element_t *prtelement(element_t*);

#endif

