/* $Id: nist.h,v 1.3 2014/05/23 09:22:20 lyonsd Exp $
 * $Log: nist.h,v $
 * Revision 1.3  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.2  2014/04/22 04:58:03  lyonsd
 * Updated for -ansi compiler flag.
 *
 * Revision 1.0  2014/03/30 01:56:19  lyonsd
 * Initial revision
 *
 */

#ifndef NIST_H
#define NIST_H

typedef enum
{
	s=0,
	p=1,
	d=2,
	f=3,
	g=4,
	h=5,
	i=6,
	k=7,
	l=8,
	m=9,
	n=10,
	o=11,
	q=12,
	r=13,
	t=14,
	u=15,
	v=16,
	w=17,
	x=18,
	y=19,
	z=20
} aqn; /* azimuthal quantum number */

typedef struct
{
	char *config;
	char *term;
	int n;			/* principle quantum number */
	aqn l;			/* azimuthal quantum number */
	float j;
	double level; /* energy level, in cm^-1 */
} nist_t;	/* NIST data type */

void prtnistrcs(void);
nist_t *readnist(void);
nist_t *prtnist(nist_t*);
nist_t *prtnist_r(nist_t*,unsigned short int,unsigned short int, unsigned short int, unsigned short int, unsigned short int);
nist_t *reversenist(nist_t*);
aqn chartoaqn(char);

#endif
