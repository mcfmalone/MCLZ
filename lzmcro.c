/* $Id: lzmcro.c,v 1.7 2014/05/31 19:14:39 lyonsd Exp $
 *
 * SUMMARY
 *
 * $Log: lzmcro.c,v $
 * Revision 1.7  2014/05/31 19:14:39  lyonsd
 * Added command line options 'e' for start energy and 'E' for stop energy
 * Default will go from 0.001 to 100000
 *
 * Revision 1.7  2014/05/31 19:10:55  lyonsd
 * Standardized start/stop energies.
 * Added command line options 'e' for start energy and 'E' for stop energy
 *
 * Revision 1.6  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.5  2014/04/28 14:07:25  lyonsd
 * Compiles under Linux with no warnings or errors
 *
 * Revision 11.6  2014/03/29 16:36:46  lyonsd
 * *** empty log message ***
 *
 * Revision 11.5  2014/03/07 19:26:38  lyonsd
 * Fe^26+ + H
 *
 * Revision 10.5  2014/03/06 19:47:18  lyonsd
 * O^6+ + H
 *
 * Revision 10.0  2014/03/06 19:31:32  lyonsd
 * O^6+ + He
 *
 * Revision 9.5  2014/03/06 18:47:51  lyonsd
 * Ne^8+ + H
 *
 * Revision 9.1  2014/03/06 18:46:55  lyonsd
 * Ne^8+ + H
 *
 * Revision 9.0  2014/03/06 18:22:04  lyonsd
 * Ne^8+ + He
 *
 * Revision 8.7  2014/03/06 18:06:32  lyonsd
 * Mg^9+ + H (triplets)
 *
 * Revision 8.4  2014/03/06 17:41:48  lyonsd
 * Ne^9+ + H (singlets)
 *
 * Revision 8.0  2014/03/06 00:13:40  lyonsd
 * Ne^9+ + He
 *
 * Revision 7.5  2014/03/05 23:42:45  lyonsd
 * Ne^10+ + H
 *
 * Revision 7.0  2014/03/05 22:51:16  lyonsd
 * Ne^10+ + He
 *
 * Revision 6.5  2014/03/05 21:29:43  lyonsd
 * Mg^8+ + H
 *
 * Revision 6.0  2014/03/05 19:54:51  lyonsd
 * Mg^8+ + He
 *
 * Revision 5.7  2014/03/05 19:24:46  lyonsd
 * Mg^9+ + H (triplets)
 *
 * Revision 5.4  2014/03/05 18:39:06  lyonsd
 * Mg^9+ + H (singlets)
 *
 * Revision 5.0  2014/03/05 00:32:33  lyonsd
 * Mg^9+ + He
 *
 * Revision 4.6  2014/03/04 23:57:07  lyonsd
 * Mg^10 + H
 *
 * Revision 4.5  2014/03/04 23:54:49  lyonsd
 * Mg^10+ + H
 *
 * Revision 4.0  2014/03/04 23:32:59  lyonsd
 * Mg^10+ + He
 *
 * Revision 3.9  2014/03/04 23:06:44  lyonsd
 * Mg^11+ + H (triplets)
 *
 * Revision 3.8  2014/03/04 22:23:57  lyonsd
 * Mg^11+ + H (singlets)
 *
 * Revision 3.7  2014/03/04 21:40:12  lyonsd
 * Mg^11 + + He
 *
 * Revision 1.4  2014/03/04 19:22:05  lyonsd
 * Added comments.
 * Added more error checking.
 *
 * Revision 1.3  2014/03/04 00:49:42  lyonsd
 * Added options to specify input/output files on command line.
 * Also added options for -t and -v
 * -t prints time of execution when program completes.
 * -v prints the rcsid string.
 * Cleaned up some unused variables, and writes to files.
 * Still need to look at why nothing prints to c.16
 *
 * Revision 1.2  2014/03/03 20:32:31  lyonsd
 * Changed the program to accept data from STDIN and output to STDOUT,
 * so it works more like a typical Unix filter.
 *
 * Revision 1.1  2014/03/03 20:21:34  lyonsd
 * Initial revision
 *
 ******************************************************************** 
 * Multichannel Landau-Zener Charge Transfer with Rotational Coupling
 *
 * Based on:
 * Janev, Belic', and Bransden, PRA 28, 1293 (1983); multichannel
 *      probability with rotational coupling
 * Butler and Dalgarno, ApJ 241, 838 (1980); empirical potentials
 *      H11 (called u11 in code), difference in potential slope
 *      (du in code)
 ******************************************************************** 
 * This is a C language version of the Fortran language version.
 * Unlike the Fortran language version, this version does not require
 * any changes to the code and recompilations.  In the Fortran
 * verson, one must change the parameter ‹ncx› for each system.  In
 * this version, ‹ncx› is automatically determined.
 *
 * Additionally, this version can be used as a "filter', where data
 * is taken from STDIN and output goes to STDOUT.  Optionally, one
 * may specify input and output files on the command line.  See below
 * for details.
 ******************************************************************** 
 * TO COMPILE:
 *
 * cc -O2 -Wall -o lzmcro lzmcro.c -lm
 *
 * TO COMPILE with gnu99 standard:
 *
 * cc -Wall -W -O2 -std=gnu99 -o lzmcro lzmcro.c -lm
 *
 * TO COMPILE with ANSI standard:
 *
 * cc -Wall -W -O2 -ansi -pedantic -o lzmcro lzmcro.c -lm
 ******************************************************************** 
 * INPUT:
 *
 * By default, input will come from STDIN.  So one will need to
 * redirect a file and pipe it to the program, or pipe the output of
 * another program to this one.  For example:
 *
 * cat fort.3 | ./lzmcro
 *
 * or
 *
 * ./lzmcro < fort.3 
 *
 * or
 *
 * ./lzdata.pl | ./lzmcro
 *
 * Optionally, once can specify an input file on the command line, 
 * using a "-i" flag.  For example:
 *
 * ./lzmcro -i fort.3
 ******************************************************************** 
 * OUTPUT:
 *
 * By default, output will go to STDOUT.  So one will need to redirect
 * the output to a file.  for example:
 *
 * cat fort.3 | ./lzmcro > fort.15
 *
 * Optionally, one may specify an output file on the command line, 
 * using a "-o" flag.  For example:
 *
 * cat fort.3 | ./lzmcro -o fort.15
 *
 * or
 *
 * ./lzmcro -i fort.3 -o fort.15
 ******************************************************************** 
 * OTHER OPTIONS
 *
 * -t  This will print the execution time to STDERR
 * -v  This will print the rcsid string to STDERR
 * -m  This will print the man page and exit (assuming the man page
 *     is located in the current directory).
 ******************************************************************** 
 */

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE  10000	/* Needed to suppress compiler warnings when compiling with -ansi flag */
#endif						 			
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#ifndef M_PI
#define M_PI	3.14159265358979323846264338327
#endif

static char rcsid[]="$Id: lzmcro.c,v 1.7 2014/05/31 19:14:39 lyonsd Exp $";
const char manfile[]="lzmcro.1";

typedef struct
{
	double a;
	double b;
	float c;
} fort3;

int main(int argc,char *argv[])
{
	int k;			/* Counter for LOOP 0 */
	int i;			/* Counter for LOOP 1 and array indexes */
	int lval;		/* Counter for LOOP 2 */
	int ii;			/* Counter for LOOP 3 */
	int j;			/* Counter for LOOP 4 */
	int im;			/* Counter for LOOP 5 */
	int lmaxx;
	int nopt;
	unsigned short int ncx;		/* Number of avoided crossings */
	unsigned short int tflag=0;	/* Timing flag */
	unsigned short int llen=45;	/* Length of input line */
	unsigned short int x1=1;
	unsigned short int x2=360;
	char *sysstr=NULL;

/* Input parameters */
	unsigned short int qon;	/* Rotational coupling (on|off) */
	float z;				/* charge of ion */
	float estart=0.001;		/* start energy in eV/u */
	float estop=100000.0;	/* stop energy in eV/u */
	float h;				/* energy step (not used) */
	float amu;				/* collision system reduced mass (amu) */
	float alpha;			/* polarization of target (a0^3) */
							/* (see CRC handbk) */
	float zeta;				/* exponent (-0.8 + zeta) of initial channel, */
							/* short range repulsive potential (zeta=-1 */
							/* for H target, zeta=-1.7 for He target; */
							/* Butler and Dalgarno) */
	float gp;				/* degeneracy factor of incoming channel */
							/* (obtained from symmetry of molecular */
							/* potentials) */

/* Used in calculations */

	float *nv;
	double mu;
	double k1;
	double l;
	double lmax;
	double a;
	double b;
	double vrl;
	double ev;
	double e;
	double Gamma;
	double p1;
	double psigma;
	double eva=27.2113962e+0;
	double camu=1822.888484514e+0;
	double p2=0;
	double p3=0;
	double tsigma=0.0; /* Total sigma */
	double *rx;
	double *u11;
	double *du;
	double *delta;
	double *a12;
	double *sigma;
	double *p;
	double varg;
	double *qr;		/* Probability of rotational decay */
	double bi;		/* Impact parameter, used only in Rotational Coupling */
	double dki2;	/* ½ angle of rotation, used only in Rotational Coupling */
	double alpha1;	/* Angle; used only in Rotational Coupling */
	double beta1;	/* Angle; used only in Rotational Coupling */
	double pq;
	double bor;		/* Used only in Rotational Coupling */
	double lbar;
	double m=5.37497215e-02;
	double y_0=9.47669323e-04;

	char *ifile=NULL;
	char *ofile=NULL;
	char *line;
	clock_t tstart=0;
	clock_t tstop=0;

	fort3 fin;


/* Process command-line arguments */

	while((nopt=getopt(argc,argv,"mvti:o:e:E:"))!=-1)
		switch(nopt)
		{
			case 'v':
				fprintf(stderr,"%s\n",rcsid);
				return 0;
			case 'i':
				ifile=(char*)malloc(strlen(optarg)+1);
				strcpy(ifile,optarg);
				if(!access(ifile,R_OK))
				{
					if(!freopen(ifile,"r",stdin))
						exit(errno);
				}
				else
				{
					fprintf(stderr,"Error %d: %s does not exist for reading\n",errno,ifile);
					exit(errno);
				}
				break;
			case 'o':
				ofile=(char*)malloc(strlen(optarg)+1);
				strcpy(ofile,optarg);
				if(!freopen(ofile,"w",stdout))
					exit(errno);
				break;
			case 'e':
				estart=atof(optarg);
				break;
			case 'E':
				estop=atof(optarg);
				break;
			case 't':
				tflag=1;
				tstart=clock();
				break;
			case 'm':
				if(-1!=access(manfile,F_OK))
				{
					sysstr=(char*)malloc(sizeof(char)*(16+strlen(manfile)));
					sprintf(sysstr,"/usr/bin/man ./%s",manfile);
					if(0!=system(sysstr))
						fprintf(stderr,"Error: Unable to dislpay man\n");
					free(sysstr);
					exit(EXIT_FAILURE);
				}
				else
				{
					fprintf(stderr,"Error: 404: %s: Not found\n",manfile);
					exit(404);
				}
				break;
			default:
				break;
		}

/* Allocate memory for line buffer */

	line=(char*)malloc(sizeof(char)*(size_t)(llen+1));

/* Read first input line */
	if(fgets(line,llen,stdin))
		sscanf(line,"%f %f %hu",&h,&h,&qon);
/* Read second input line */
	if(fgets(line,llen,stdin))
		sscanf(line,"%f %f",&amu,&z);
/* Read third input line */
	if(fgets(line,llen,stdin))
		sscanf(line,"%f %f %f",&alpha,&zeta,&gp);

/* Set up the start and stop energies in eV/u
 * If nothing specified on command line, the defaults will be:
 * estart=0.001 eV/u
 * estop=500000 eV/u
 */
	estart*=amu;
	estop*=amu;

	m=log(estart/estop)/(x1-x2);
	y_0=exp((x1*log(estop)-x2*log(estart))/(x1-x2));

/* Allocate memory for our arrays */
	if(NULL==(rx=(double*)malloc(sizeof(double))))
	{
		fprintf(stderr,"Error %d: failed to malloc %lu bytes for rx\n",errno,sizeof(double));
		exit(errno);
	}
	if(NULL==(delta=(double*)malloc(sizeof(double))))
	{
		fprintf(stderr,"Error %d: failed to malloc %lu bytes for delta\n",errno,sizeof(double));
		exit(errno);
	}
	if(NULL==(nv=(float*)malloc(sizeof(float))))
	{
		fprintf(stderr,"Error %d: failed to malloc %lu bytes for nv\n",errno,sizeof(float));
		exit(errno);
	}

/* read the rest of the file */
	ncx=0;
	while(fgets(line,llen,stdin))
	{
		sscanf(line,"%lf %lf %f",&fin.a,&fin.b,&fin.c);
/* reallocate memory for the arrays, which will be used to */
/* store the date that will be read */
		if(NULL==(rx=(double*)realloc(rx,sizeof(double)*(size_t)(ncx+1))))
		{
			fprintf(stderr,"Error %d: failed to realloc %lu bytes for rx\n",errno,sizeof(double)*(size_t)(ncx+1));
			exit(errno);
		}
		rx[ncx]=fin.a;
		
		if(NULL==(delta=(double*)realloc(delta,sizeof(double)*(size_t)(ncx+1))))
		{
			fprintf(stderr,"Error %d: failed to realloc %lu bytes for delta\n",errno,sizeof(double)*(size_t)(ncx+1));
			exit(errno);
		}
		delta[ncx]=fin.b;

		if(NULL==(nv=(float*)realloc(nv,sizeof(float)*(size_t)(ncx+1))))
		{
			fprintf(stderr,"Error %d: failed to realloc %lu bytes for nv\n",errno,sizeof(float)*(size_t)(ncx+1));
			exit(errno);
		}
		nv[ncx]=fin.c;
		++ncx;
	} /* end while(fgets(line,llen,stdin)) */
/* ‹ncx› will now contain the number of avoided crossings */
/* We'll use it to allocate memory for our arrays. */

	if(NULL==(u11=(double*)malloc(sizeof(double)*(size_t)ncx)))
	{
		fprintf(stderr,"Error %d: failed to malloc %lu bytes for u11\n",errno,sizeof(double)*(size_t)ncx);
		exit(errno);
	}
	if(NULL==(du=(double*)malloc(sizeof(double)*(size_t)ncx)))
	{
		fprintf(stderr,"Error %d: failed to malloc %lu bytes for du\n",errno,sizeof(double)*(size_t)ncx);
		exit(errno);
	}
	if(NULL==(a12=(double*)malloc(sizeof(double)*(size_t)ncx)))
	{
		fprintf(stderr,"Error %d: failed to malloc %lu bytes for a12\n",errno,sizeof(double)*(size_t)ncx);
		exit(errno);
	}

	mu=camu*amu;

/* Calculate initial potential energy (u11), slope of potential
 * differences (du), and radial coupling matrix element (a12) all at
 * avoided crossing distance *rx).
 * See Butler and Dalgarno, ApJ 241, 838 (1980).
 */

	a=25.0*z;
	b=-0.8+zeta;

	for(i=0;i<ncx;i++)
	{
		u11[i]=0.5*-alpha*z*z/(rx[i]*rx[i]*rx[i]*rx[i])+a*exp(b*rx[i]);
		du[i]=fabs(2.0*alpha*z*z/pow(rx[i],5.0)+(z-1.0)/(rx[i]*rx[i])+a*b*exp(b*rx[i]));
		a12[i]=fabs(du[i]/(2.0*delta[i]));
	}

	if(NULL==(p=(double*)malloc(sizeof(double)*(size_t)(ncx+2))))
	{
		fprintf(stderr,"Error %d: failed to malloc %lu bytes for p\n",errno,sizeof(double)*(size_t)(ncx+2));
		exit(errno);
	}
	if(NULL==(qr=(double*)malloc(sizeof(double)*(size_t)(ncx+2))))
	{
		fprintf(stderr,"Error %d: failed to malloc %lu bytes for qr\n",errno,sizeof(double)*(size_t)(ncx+2));
		exit(errno);
	}
	if(NULL==(sigma=(double*)malloc(sizeof(double)*(size_t)ncx)))
	{
		fprintf(stderr,"Error %d: failed to malloc %lu bytes for sigma\n",errno,sizeof(double)*(size_t)ncx);
		exit(1);
	}


	for(k=x1;k<=x2;k++) /* Do 200 */
	{
/* LOOP 0 */
		ev=y_0*exp(m*k);

		tsigma=0.0;
/* Set the last two elements of these arrays to 0.0 */
		p[ncx]=0.0;
		p[ncx+1]=0.0;
		qr[ncx]=0.0;
		qr[ncx+1]=0.0;
/* Convert eV to au */
		e=ev/eva;
/* k1 is neutral channel */
		k1=sqrt(2.00*mu*e);

		for(i=0;i<ncx;i++) /* Do 14 */
		{
/* LOOP 1 */
			if((k1*k1)-2.0*mu*u11[i]<=0.0)
			{
				lmax=0.0;
			} /* end if((k1*k1)-2.0*mu*u11[i]<=0.0) */
			else
			{
				lmax=sqrt((rx[i]*rx[i])*((k1*k1)-2.0*mu*u11[i]))-0.5;
			} /* end if(pow(k1,2)-2.0*mu*u11[i]<=0.0) else */
			lmaxx=(int)lmax;
			lbar=rx[i]*k1*pow(4.0*fabs(u11[i])/e,0.25)-0.5;

			sigma[i]=0.0;
			pq=0.0;
			if(i>0)
			{
				pq=1.0;
			} /* end if(i>0) */
			for(lval=0;lval<=lmaxx;lval++) /* Do 10 */
			{
/* LOOP 2 */
				l=lval+0.0;
				if(!((e<fabs(u11[i])) && (l>lbar)))
				{
					for(ii=0;ii<ncx;ii++)
					{
/* LOOP 3 */
						varg=(k1*k1)-2.0*mu*u11[ii]-(l*l+l+0.25)/(rx[ii]*rx[ii]);
/* What if ‹varg› is negative? ‹vrl› will be ‹nan›. */
/* Therefore, make it absolute value.  Cannot make it 0, becasue */
/* it's a denominator in calculating ‹Gamma› */
						vrl=sqrt(fabs(varg))/mu;
						Gamma=0.125*M_PI*(delta[ii]/a12[ii])/vrl;
						p[ii]=exp(-2.0*Gamma);
/* Rotational coupling */
						if(qon==1)
						{
							bi=(l+0.5)/mu/vrl;
							bor=bi/rx[ii];
							if(bor>1.0)
							{
								bor=1.0;
							} /* end if(bor>1.0) */
							dki2=cos(bor);
							alpha1=dki2*sqrt(1.0+(3.0*nv[ii])/pow(2.0*z*bi*vrl,2));
							beta1=atan(2.0*z*bi*vrl/3.0/nv[ii]);
							qr[ii]=1.0-pow((1.0-pow((sin(beta1)*sin(alpha1)),2)),(2.0*(nv[ii]-1.0)));
						} /* end if(qon==1) */
						else
						{
							qr[ii]=0.0;
						} /* end if(qon==1) else */
						if(varg<0.0)
						{
							p[ii]=0.0;
							qr[ii]=0.0;
						} /* end if(varg<0.0) */
/* END LOOP 3 */
					} /* end for(ii=0;ii<ncx;ii++) */

					p1=1.0;
					for(j=0;j<=i;j++)
					{
/* LOOP 4 */
						p1*=p[j];
						if((i+1)<ncx)
						{
							p2=1.0;
							p3=0.0;
							for(im=i+1;im<ncx;im++)
							{
/* LOOP 5 */
								p2=p2*p[im]*p[im];
								if(im<(ncx-1))
								{
									p3=p3+p2*pow(1.0-p[im+1],2)*(1.0-qr[im+1]);
								} /* end if(im<(ncx-1)) */
/* END LOOP 5 */
							} /* end for(im=i+1;im<ncx;im++) */
						}
						else
						{
							p2=0.0;
							p3=0.0;
						} /* end if((i+1)<ncx) */
/* END LOOP 4 */
					} /* end for(j=0;j<=i;j++) */

					if(p[i]==0.0)
					{
						p1=0.0;
					} /* end if(p[i]==0.0) */
					else
					{
						p1=p1*(1.0-p[i])*(1.0+p2+p3+pow((1.0-p[i+1]),2)*(1.0-qr[i+1]))+pq*p1/p[i]*pow(1.0-p[i],2)*qr[i];
					} /* end if(p[i]==0.0) else */
/* 90 */

/* approximate (sin of phase)**2 by 1/2 */
					psigma=2.0*M_PI*(2.0*l+1.0)*p1/(k1*k1)*0.14001414866;
					sigma[i]+=psigma*gp;
				} /* end if(!((e<fabs(u11[i])) && (l>lbar))) */
/* 10 */
/* END LOOP 2 */
			} /* end for(lval=0;lval<=lmaxx;lval++) */
			tsigma+=sigma[i];
/* END LOOP 1 */
		} /* end for(i=0;i<ncx;i++) */
/* 14 */
		fprintf(stdout,"%9.3e",ev/amu);
		for(j=0;j<ncx;j++)
		{
			fprintf(stdout,"\t%13.7e",sigma[j]);
		} /* end for(j=0;j<ncx;j++) */
		fprintf(stdout,"\t%13.7e\n",tsigma);
/* 200 */
/* END LOOP 0 */
	}	/* end for(k=1;k<360;k++) */

	if(tflag==1)
	{
		tstop=clock();
		fprintf(stderr,"Execution time: %ld seconds\n",(tstop-tstart)/CLOCKS_PER_SEC);
	}
	return 0;
}	/* END main */
