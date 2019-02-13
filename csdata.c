/* $Id: csdata.c,v 1.8 2014/05/23 16:53:30 lyonsd Exp $
 * $Log: csdata.c,v $
 * Revision 1.8  2014/05/23 16:53:30  lyonsd
 * Fixed compiler error on Linux
 *
 * Revision 1.7  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.6  2014/04/28 14:05:20  lyonsd
 * Compiles under Linux with no warnings or errors
 *
 * Revision 1.5  2014/04/22 04:58:03  lyonsd
 * Updated for -ansi compiler flag.
 *
 * Revision 1.4  2014/03/22 01:47:40  lyonsd
 * Working for all systems
 *
 * Revision 1.3  2014/03/20 22:53:02  lyonsd
 * Working for all systems
 *
 * Revision 1.2  2014/03/19 07:33:33  lyonsd
 * Works with mknlrescs on every system
 *
 * Revision 1.1  2014/03/12 22:40:00  lyonsd
 * Initial revision
 */

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 10000	/* Needed to suppress compiler warnings with -ansi flag */
#endif
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include "utils.h"
#include "xsection.h"
#include "csdata.h"

static const char *rcsid="$Id: csdata.c,v 1.8 2014/05/23 16:53:30 lyonsd Exp $";

/* Used for informational purposes only.  Will print the rcsid string when
 * called
 */ 
void prtcsdatarcs(void)
{
	(void)fprintf(stderr,"%s\n",rcsid);
}

/* Reading from STDIN, this function builds a 2-dimensional array of doubles
 * and stores the input data into the array.  It then returns the array, as
 * well as updating the passed variables, rows and cols with the numbers of
 * each that were stored.
 * The default line length may need to be increased if a large number of 
 * columns are read
 */

xsection *getcsdata(xsection *cs)
{
	double **csdata;
	unsigned short int row=0;
	unsigned short int col=0;
	unsigned short int len=2000;
	char *line;
	char *str;		/* Used in processing lines; extracting tokens */
	char *token;	/* A word (or number) extracted from a line */
	char *saveptr;	/* Used in extracting tokens from lines */

	line=(char*)malloc(sizeof(char)*len);
/* initialize the matrix */
	csdata=(double**)malloc(sizeof(double*));
	while(fgets(line,len,stdin))
	{
/*		allocate a row */
		csdata=(double**)realloc(csdata,sizeof(double*)*(size_t)(1+row));
/*		initialize a column */
		csdata[row]=(double*)malloc(sizeof(double));
/*		process the tokens */
		for(col=0,str=trim(line);;col++,str=NULL)
		{
			token=strtok_r(str," \t",&saveptr);
			if(token==NULL) 
			{
				col--;
				break;
			}
			else
			{
/*				allocate a column */
				csdata[row]=(double*)realloc(csdata[row],sizeof(double)*(size_t)(1+col));
/*				store the token as a double into the matrix at postion row:column */
				csdata[row][col]=atof(token);
			}
		}
		++row;
	}	/* END while(fgets(line,len,stdin)) */
	++col;
/*	*rows=row; */
/*	*cols=col; */
	cs->rows=row;
	cs->nlcols=col;
	cs->nldata=csdata;
	return cs;
}

/* This function accepts as parameters the address of the array which contains
 * the cross section data.  It also needs the number of rows and columns.  It
 * prints the data to STDOUT
 */

xsection *prtcsdata(xsection *cs,const char res[])
{
	int i=0;
	int j=0;
	int cols=0;
	int rows=0;
	double **csdata=NULL;

	if(0==strcmp(res,"nl"))
	{
		csdata=cs->nldata;
		cols=cs->nlcols;
	}
	else if(0==strcmp(res,"n"))
	{
		csdata=cs->ndata;
		cols=cs->ncols;
	}
	rows=cs->rows;

	for(i=0;i<rows;i++)
	{
		fprintf(stdout,"\n%-9.3e",csdata[i][0]);
		for(j=1;j<cols;j++)
			fprintf(stdout,"\t%-13.7e",csdata[i][j]);
	}

	return cs;
}

/* This function accepts as parameter the address of an array, which contains
 * cross section data.  It finds the maximum value of the first column (i.e,
 * the maximum "x", or "eV/u" value), and returns it.  The function also needs
 * to know the number of rows in the data.
 */

double csmaxx(double **csdata,int rows)
{
	double maxx=csdata[0][0];
	int i=0;

	for(i=0;i<rows;i++)
		if(csdata[i][0]>maxx)
			maxx=csdata[i][0];

	return maxx;
}	

/* This function accepts as parameter the address of an array, which contains
 * cross section data.  It finds the minimum value of the first column (i.e,
 * the minimum "x", or "eV/u" value), and returns it.  The function also needs
 * to know the number of rows in the data.
 */

double csminx(double **csdata,int rows)
{
	double minx=csdata[0][0];
	int i=0;

	for(i=0;i<rows;i++)
		if(csdata[i][0]<minx)
			minx=csdata[i][0];

	return minx;
}	

/* This function accepts as parameter the address of an array, which contains
 * cross section data.  It finds the maximum value in the last column (i.e,
 * the minimum "y", or cross section), and returns it.  The function also
 * needs to know the number of rows and columns in the data.
 */

double csmaxy(double **csdata,int rows,int cols)
{
	int i=0;
	double maxy=csdata[0][cols-1];

	for(i=0;i<rows;i++)
		if(csdata[i][cols-1]>maxy)
			maxy=csdata[i][cols-1];

	return maxy;
}	

/* This function accepts as parameter the address of an array, which contains
 * cross section data.  It finds the minimum value in all but the first column
 * (i.e, the minimum "y", or cross section), and returns it.  The function also
 * needs to know the number of rows and columns in the data.
 */

double csminy(double **csdata,int rows,int cols)
{
	double miny=csdata[0][1];
	int i=0;
	int j=0;

	for(i=0;i<rows;i++)
		for(j=1;j<cols;j++)
			if(csdata[i][j]<miny)
				miny=csdata[i][j];

	return miny;
}	

/* This function accepts as parameters the address of a 2-dimensional array
 * which contains the nl-resolved cross section data.  It also accepts the
 * address of an array of strings containing the labels associated with that
 * data, and the address of an array of strings containing the n-resolved
 * labels for the new n-resolved cross section data.  It returns the address
 * of a two-dimensional array which contains the n-resolved cross section data
 */

xsection *getnrescs(xsection *cs)
{
	unsigned short int i=0;
	unsigned short int r=0;		/* Rows counter */
	unsigned short int c=0;		/* Columns counter */
	unsigned short int nn=0;	/* Number of n-labels */
	unsigned short int n=0;		/* Used to hold the value of "n" read from the nl-resolved labels */
	unsigned short int ncs=0;	/* Used to hold the value of "n" read from the n-resolved labels */
	double **ndata=NULL;

/* We need to know how many n-resolved cross sections there are.
 * This loop counts them
 */
	for(nn=0;cs->nlab[nn]!=NULL;nn++)
		continue;

/* We need 1 more, because our array is null-terminated */
	++nn;

	ndata=(double**)malloc(sizeof(double*)*(size_t)(cs->rows));
	for(r=0;r<(cs->rows);r++)
		ndata[r]=(double*)malloc(sizeof(double)*(size_t)(1+nn));

	for(r=0;r<(cs->rows);r++)
	{
		memset(ndata[r],0.0,sizeof(double)*(size_t)(1+nn));
		for(c=0;c<(cs->nlcols-1);c++)
		{
			if(c==0)
			{
				ndata[r][0]=cs->nldata[r][0];
			}	/* END if(c==0) */
			else
			{
				sscanf(cs->nllab[c-1],"%hu",&n);
				ncs=0;
				i=0;
				/* Loop through the n-resolved labels to see where "n" is located */
				do
				{
					sscanf(cs->nlab[i],"n=%hu",&ncs);
					i++;
					if(i==nn)
						exit(1);

				} while(ncs!=n);
				ndata[r][i]+=cs->nldata[r][c];
/*				fprintf(stderr,"ndata[%d][%d] (%13.7e)+=nldata[%d][%d] (%13.7e)\n",r,i,ndata[r][i],r,c,nldata[r][c]); */
				ndata[r][nn]+=cs->nldata[r][c];
			}	/* END if(c==0) else */
		}	/* END for(c=0;c<cols;c++) */
	}	/* END for(r=0;r<rows;r++) */
	cs->ndata=ndata;
	cs->ncols=nn+1;
	return cs;
}
