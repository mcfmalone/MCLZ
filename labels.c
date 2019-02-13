/* $Id: labels.c,v 1.8 2014/05/28 20:57:18 lyonsd Exp $
 * $Log: labels.c,v $
 * Revision 1.8  2014/05/28 20:57:18  lyonsd
 * Fixed label sorting issue
 *
 * Revision 1.7  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.6  2014/04/28 14:06:41  lyonsd
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

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <wchar.h>
#include "utils.h"
#include "xsection.h"
#include "labels.h"

static const char *rcsid="$Id: labels.c,v 1.8 2014/05/28 20:57:18 lyonsd Exp $";

void prtlabelsrcs(void)
{
	(void)fprintf(stderr,"%s\n",rcsid);
}

/* This function constructs the name of the file, which contains the ion
 * energy labels, which are used to identify the columns in the cross section
 * (*.cs) file.
 *
 * INPUT PARAMETERS
 * A character string containing the symbol for the ion
 * An integer containing the value of the charge
 *
 * RETURN
 * A character string containing the name of the file.  It typically has the
 * format {ion}{charge}.nist.in
 *
 * OUTPUT
 * It will prompt the user to specify a filename if the «ion» string is set to 
 * the default value.
 */

string getlabelfn(xsection *cs)
{
	const char suffix[]=".nist.in";	/* Default suffix */
	string filename;
	const char *chrg;
	int i;

/* allocate memory for the filename */
	filename=(char*)malloc(sizeof(char));

/* check to see if the value of «ion» is the default */
	if(0==strcmp(cs->ION,IOND))
	{
/* if it is the default, then we cannot use it for the name of the file which
 * contains the labels.
 */
		fprintf(stderr,"Error: No ion specified on the command line, or in an environment variable.\n");
		fprintf(stderr,"Specify an ion on the command line as follows:\n");
		fprintf(stderr,"\n\t-I «ion»\n");
		fprintf(stderr,"\n-or-\n\n");
		fprintf(stderr,"Set an environment variable with the desired ion as follows:\n");
		fprintf(stderr,"\n\texport ion=«ion» | export ION=«ion»\n");
		fprintf(stderr,"\nWhere «ion» is a one- or two-letter code, such as O, C, Fe, Mg, Ne, etc.\n");
		exit(-1);
	}	/* END if(0==strcmp(ion,IOND)) */
	else
	{
/* if it is not the default, that means it has been set either on the command
 * line, or via an environment variable.  So we'll use it, and the charge to
 * construct the name of the file which contains the labels.
 */
		chrg=itoa(cs->Z);
		filename=concat(3,cs->ION,chrg,suffix);
	}	/* END if(0==strcmp(ion,IOND)) else */

	for(i=0;filename[i];i++) filename[i]=tolower((unsigned char)filename[i]);
/* We'll return the filename.  Checks will be performed later whether this is
 * a valid file or not.
 */
	return filename;
} 	/* END char *getlabelfn(char *ion,int z) */

/* This function accepts the addres of a string array, which is modified to
 * contain character strings of the collision system labels.
 *
 * INPUT PARAMETERS
 * The address of the string array, which will be assigned the labels
 * A pointer to a file descriptor, which points to the file containing the labels
 *
 * RETURN
 * The address of a character, whose value contains the number of labels which 
 * were stored in the string array
 *
 * OUTPUT
 * none
 */

string *getnistlabels(string fn)
{
	unsigned short int n=0;	/* a control variable, to count the number of 
							   labels */
	string line=NULL;		/* the current line, which is read from the file,
							   which contains the label strings */
	string state=NULL;		/* a temporary string, which contains the most
							   recent collision system state, such as 1s, 2s,
							   2p, etc. */
	string term=NULL;		/* A temporary string, which contains the most
							   recent collision system config term, such as
							   ²S, ²P°, ²D, etc. */
	string label=NULL;		/* A temporary string, which contains the most
							   recently constructed label, which has the format
							   state(term), for example: 2s(²S), 2p(²P°), etc. */
	string *nistlabels=NULL;/* An array of strings. which contains the labels */
	const char fmt[]="%s(%s)";	/* A format string, which defines how we want the
							   labels to appear */
	int len=128;			/* The default length of the line to read from
							   the file containing the label data */
	FILE *fp;

	fp=openfile(fn,"r");
	line=(char*)malloc(sizeof(char)*(size_t)len);
/* read a line, of length «len», from the file pointed to by «fp» */
	while(fgets(line,len,fp))
	{
/* allocate memory for our temporary label variables */
		state=(string)malloc(sizeof(char)+20);
		term=(string)malloc(sizeof(char)+20);
/* find the label data in the line and store them into the temp variables */
		sscanf(line,"%s %s",state,term);
/* allocate memory for the label string */
		label=(string)malloc(sizeof(char)*(1+strlen(fmt)+strlen(state)+strlen(term)));
/* store the formated label into the label string */
		sprintf(label,fmt,state,term);
/* reallocate memory for another label entry */
		nistlabels=(string*)realloc(nistlabels,sizeof(string)*(size_t)(n+1));
/* allocate memory for the new label string in the array */
		nistlabels[n]=(string)malloc(sizeof(char)*(1+strlen(label)));
/* assign the formated label string to the label array */
		strcpy(nistlabels[n],trim(label));
/* free the temporary variables */
		free(label);
		free(state);
		free(term);
/* increment counter */
		n++;
	}	/* END whle */
/* free the line */
	free(line);

/* Allocate one more slot for the sting array and set it to NULL */

	nistlabels=(string*)realloc(nistlabels,sizeof(string)*(size_t)(n+1));
	nistlabels[n]=NULL;

/* return the number of labels read */
	return nistlabels;
}	/* END int *getnistlabels(string **labels) */

/* This function prints to STDERR the variable and its value.  This function
 * is mainly intended for troublshooting purposes.
 *
 * INPUT PARAMETERS
 * The address of a null-terminated array of strings containing the ion's state
 * labels.
 *
 * RETURN
 * A pointer to the same null-terminated string pointer that was passed as a
 * parameter to the function
 *
 * OUTPUT
 * The variable name and its value, written to STDERR
 */

string *prtlabels(string *labels)
{
	int i=0;

/*	for(i=0;NULL!=labels[i];i++) */
	while(NULL!=labels[i])
	{
		PRTSTR(labels[i]);
		PRTINT(i);
		++i;
	}	/* END for(i=0;i<n;i++) */
	PRTSTR(labels[i]);
	PRTINT(i);
	return labels; /* return the number of labels printed.  NULL if none */
}	/* END char *prtlabels(string *labels) */

/* This function takes a string array of nl-resolved labels and produces a
 * string array of n-resolved labels.
 */

/* This function constructs an array of strings where the elements of the
 * array contain a n-resolved cross section label (i.e., "n=x").
 */
string *nlabels(string* nllabels)
{
	unsigned short int i=0;
	unsigned short int j=0;
	unsigned short int k=0;
	unsigned short int n=0;
	int newlabel=0;

	string nlabel=NULL;
	string *labels=NULL;

	labels=(string*)malloc(sizeof(string));
	labels[0]=NULL;
	for(i=0;NULL!=nllabels[i];i++)
	{
			sscanf(nllabels[i],"%hu",&n);
			nlabel=(string)malloc(sizeof(char)*4);
			sprintf(nlabel,"n=%d",n);
			newlabel=1;
/* check to see if it's in the array */
		for(j=0;NULL!=labels[j];j++)
		{
			if(0==strcmp(labels[j],nlabel))
			{
				newlabel=0;
				break;
			}	/* END if(0==strcmp(labels[j],nlabel)) */
		}	/* END for(j=0;NULL!=labels[j];j++) */
		if(newlabel==1)
		{
			labels=(string*)realloc(labels,sizeof(string)*(size_t)(k+2));
			labels[k]=(string)malloc(sizeof(char)*(strlen(nlabel)+1));
			strcpy(labels[k],nlabel);
			labels[k+1]=NULL;
			++k;
		}	/* END if(newlabel==1) */
		free(nlabel);
	}	/* END for(i=0;NULL!=nllabels[i];i++) */
	return labels;
}

/* This function accepts an array of strings which contain label data, and
 * sorts the array, from high to low.  This ensures that the data columns
 * are written from the highest n-values to the lowest.  This function is
 * intended to sort n-resolved labels (i.e., those that are in the form
 * "n=x"), although it should work on nl-resolved labels as well.
 */
string *sortnlabels(string *labels)
{
	int x;
	int y;
	int n0;
	int n1;

	for(x=0;labels[x]!=NULL;x++)
	{
		for(y=0;labels[y+1]!=NULL;y++)
		{
			(void)sscanf(labels[y],"n=%2d",&n0);
			(void)sscanf(labels[y+1],"n=%2d",&n1);
/*			if(strcmp(labels[y],labels[y+1])<0) */
			if(n0<n1)
			{
				swapstr(&labels[y],&labels[y+1]);
			}
		}
	}
	return labels;
}

string *reverselabels(string *labels)
{
	unsigned short int i=0;		/* index */
	unsigned short int ne=0;	/* number of elements */

	string *rlabel=NULL;


	for(ne=0;labels[ne]!=NULL;ne++)
		;
	rlabel=(string*)malloc(sizeof(string));
	for(i=0;i<ne;i++)
	{
		rlabel=(string*)realloc(rlabel,sizeof(string)*(size_t)(i+1));
		rlabel[i]=(string)malloc(sizeof(char)*(strlen(labels[ne-i-1])+1));
		strcpy(rlabel[i],labels[ne-i-1]);
	}
	rlabel=(string*)realloc(rlabel,sizeof(string)*(size_t)(i+1));
	rlabel[i]=NULL;

	return rlabel;
}

/* This function removes some elements from the array of strings which contain
 * labels.
 */
string *trimlabels(string *labels,int cols)
{
	string *trimmed=NULL;
	unsigned short int n=0;	/* Number of elements in the array */
	unsigned short int i=0;	/* Index variable */
	int offset=0;

	while(labels[n]!=NULL)
	{
		++n;
	}

/*	for(n=0;labels[n]!=NULL;n++) */

	offset=n-(cols-2);

	if(offset==0)
	{
		return labels;
	}
	if(offset<0)
	{
		fprintf(stderr,"Error: There are %d cross section columns ",cols-2);
		fprintf(stderr,"and %d labels.\n",n);
		exit(1);
	}
	trimmed=(string*)malloc(sizeof(string)*(size_t)(cols-1));
	for(i=0;i<cols-2;i++)
	{
		trimmed[i]=(string)malloc(sizeof(char)*(strlen(labels[i+offset]+1)));
/*		trimmed[i]=(string)malloc(sizeof(char)*(wcslen((wchar_t*)(labels[i+offset])+1))); */
		strcpy(trimmed[i],labels[i+offset]);
/*		wcscpy((wchar_t*)trimmed[i],(wchar_t*)labels[i+offset]); */
	}
/* Set the final element to NULL */
	trimmed[i]=NULL;

	return trimmed;
}
