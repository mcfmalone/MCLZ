/* $Id: element.c,v 1.2 2014/04/29 14:48:43 lyonsd Exp $
 * $Log: element.c,v $
 * Revision 1.2  2014/04/29 14:48:43  lyonsd
 * Update to rcsid
 *
 * Revision 1.1  2014/04/22 04:58:03  lyonsd
 * Initial revision
 *
 *
 * This file contains routines that deak with the element_t and cs_t types
 */


#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE	10000
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "element.h"
#include "utils.h"

static const char rcsid[]="$Id: element.c,v 1.2 2014/04/29 14:48:43 lyonsd Exp $";

void prtelementrcs()
{
	(void)fprintf(stderr,"%s\n",rcsid);
}

element_t *initelement(element_t *particle)
{
	FILE *fp;
	short unsigned int a=0;
	char symbol[3];
	char name[16];
	float mass=0;
	double ie=0;
	float alpha=0.0;
	float zeta=0.0;
	bool found=false;
	char line[64];
	int llen=64;
	const char *datafile="elements.dat";

	fp=openfile(datafile,"r");
	while(NULL!=fgets(line,llen,fp))
	{
		(void)sscanf(line,"%hu\t%s\t%s\t%f\t%lf\t%f\t%f",&a,symbol,name,&mass,&ie,&alpha,&zeta);
		if(0==strcmp(particle->symbol,symbol))
		{
			found=true;
			particle->a=a;
			particle->m=mass;
			particle->name=(char*)malloc(sizeof(char)*(1+strlen(symbol)));
			strcpy(particle->name,name);
			particle->ita=(double*)malloc(sizeof(double));
			*(particle->ita)=ie;
			particle->alpha=alpha;
			if(zeta!=0.0)
				particle->zeta=zeta;
			else
				particle->zeta=(float)(-2.0*ie);
			break;
		}
	}
	if(found==false)
	{
		fprintf(stderr,"Error: Unknown atomic symbol %s\n",particle->symbol);
		exit(-1);
	}
	if(particle->a < particle->z)
	{
		fprintf(stderr,"Error: Charge %d cannot exceed atomic number %d\n",particle->z,particle->a);
		exit(-1);
	}
	return particle;
	
}

element_t *prtelement(element_t *particle)
{
	fprintf(stderr,"Name:              %-s\n",particle->name);
	fprintf(stderr,"Symbol:            %-s\n",particle->symbol);
	fprintf(stderr,"Atomic #:          %d\n",particle->a);
	fprintf(stderr,"Charge:            %d\n",particle->z);
	fprintf(stderr,"Mass:              %f (au)\n",particle->m);
	fprintf(stderr,"Polarizability:    %f\n",particle->alpha);
	fprintf(stderr,"Zeta:              %f\n",particle->zeta);
	fprintf(stderr,"Ionization energy: %14.8e (au)\n",particle->ita[0]);
	if(NULL!=particle->nist)
		prtnist(particle->nist);
	return particle;
}
