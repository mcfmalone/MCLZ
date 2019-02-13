/* $Id: csheader.c,v 1.8 2014/05/23 16:53:30 lyonsd Exp micha $
 * $Log: csheader.c,v $
 * Revision 1.8  2014/05/23 16:53:30  lyonsd
 * Fixed compiler error on Linux
 *
 * Revision 1.7  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.6  2014/04/28 14:06:00  lyonsd
 * Compiles under Linux with no warnings or errors
 *
 * Revision 1.5  2014/04/22 04:58:03  lyonsd
 * Updated for -ansi compiler flag
 *
 * Revision 1.4  2014/03/22 01:47:40  lyonsd
 * Working for all systems
 *
 * Revision 1.3  2014/03/20 22:53:02  lyonsd
 * Crashes in csheader.c with one system.  Further debugging needed.
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
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "utils.h"
#include "xsection.h"
#include "csheader.h"

static const char rcsid[]="$Id: csheader.c,v 1.8 2014/05/23 16:53:30 lyonsd Exp micha $";
/* extern char *strtok_r(char *str, const char *delim, char **saveptr); */

int prtcsheaderrcs(void)
{
	return fprintf(stderr,"%s\n",rcsid);
}

xsection *prtcsheader(xsection *cs,const char res[])
{
	int i=0;
	time_t *now=NULL;
	struct tm *mytime=NULL;
	char yyyy[5];
	char mm[3];
	char dd[3];
	char mname[10];
	char *errmsg;
	const char *fmtstr="Error %d:";
	int errnum=0;
	int lineno=0;

	mytime=(struct tm*)malloc(sizeof(struct tm));
	now=(time_t*)malloc(sizeof(time_t));

	if(time(now)==(time_t)(-1))
	{
		errnum=errno;
		lineno=__LINE__-3;
		errmsg=(char*)malloc(strlen(fmtstr)+2);
        sprintf(errmsg,fmtstr,errnum);
        fprintf(stderr,"File:\t\t%s\n",__FILE__);
        fprintf(stderr,"Function:\t%s\n",__FUNCTION__);
        fprintf(stderr,"Line:\t\t%d\n",lineno);
        perror(errmsg);
        exit(errnum);
	}
	else
	{
		mytime=localtime_r(now,mytime);
		strftime(yyyy, sizeof(yyyy), "%Y", mytime);
		strftime(mm, sizeof(mm), "%m", mytime);
		strftime(dd, sizeof(dd), "%d", mytime);
		strftime(mname, sizeof(mname), "%B", mytime);
	}

	fprintf(stdout,"# Magic number=%s%s%s, %s %s %s, %s\n",dd,mm,yyyy,dd,mname,yyyy,cs->INITIALS);
	fprintf(stdout,"# Single electron capture cross sections, %s state-selective\n",res);
	fprintf(stdout,"# %s^%d+ + %s -> %s^%d+ + %s^+\n",cs->ION,cs->Z,cs->ATOM,cs->ION,cs->Z-1,cs->ATOM);
	fprintf(stdout,"# Method=MCLZ\n");
	fprintf(stdout,"# %s et al. %s, to be submitted\n",cs->NAME,yyyy);
	fprintf(stdout,"# ----------------------------------------------------------------------------\n");
	fprintf(stdout,"# Energy\t\tCross sections (10^-16 cm^2)\n");
	fprintf(stdout,"%-15s","# (eV/u)");
	if(0==strcmp(res,"nl"))
	{
		for(i=0;cs->nllab[i]!=NULL;i++)
			fprintf(stdout,"\t%-14s",cs->nllab[i]);
	}
	else if(0==strcmp(res,"n"))
	{
		for(i=0;cs->nlab[i]!=NULL;i++)
			fprintf(stdout,"\t%-14s",cs->nlab[i]);
	}
	fprintf(stdout,"\t%s","Total");

	return cs;
}

string *readcsheader()
{
	int lineno=0;
	unsigned int linelen=2000;
	string tline;
	string *line;

/* allocate memory for 8 lines */
	line=(string*)malloc(sizeof(string)*8);
/* read in those 8 lines */
	rewind(stdin);
	for(lineno=0;lineno<8;lineno++)
	{
		tline=(string)malloc(sizeof(char)*linelen);
		if(NULL==fgets(tline,(int)linelen,stdin))
			exit(errno);
		line[lineno]=(string)malloc(strlen(tline)+1);
		strcpy(line[lineno],tline);
		free(tline);
	}
	return line;
}

xsection *getcsparams(string* csheader)
{
	unsigned int namelen=40;
	string tstr1=(string)malloc(sizeof(char)*namelen);
	string tstr2=(string)malloc(sizeof(char)*namelen);
	int junk;

	xsection *cs=initcs();

	sscanf(csheader[0],"# Magic number=%d, %d %s %d, %s",&junk,&junk,tstr1,&junk,cs->INITIALS);
	tstr1=&csheader[2][2];
	sscanf(tstr1,"%2[A-Za-z]s^",cs->ION);
	tstr1=strstr(tstr1,"^");
	++tstr1;
	sscanf(tstr1,"%d+",&(cs->Z));
	tstr1=strstr(tstr1,"+ + ");
	tstr1+=4;
	sscanf(tstr1,"%2[A-Za-z]s^",cs->ATOM);
	cs->NAME=(string)malloc(strlen(csheader[4]));
	tstr1=strstr(csheader[4],"#");
	tstr1+=strlen("#");
	while(isspace((unsigned char)*tstr1))
		++tstr1;
	tstr2=strstr(csheader[4],"et al");
	strncpy(cs->NAME,tstr1,strlen(tstr1)-strlen(tstr2));
	cs->NAME[strlen(tstr1)-strlen(tstr2)-1]='\0';
	return cs;
}

string *getcslabels(string *csheader)
{
	int i;
	int j;
	char *saveptr;
	string token=NULL;
	string line=NULL;
	string tline1=NULL;
	string tline2=NULL;
	string str=NULL;
	const char l1[]="# (eV/u)";
	const char l2[]="Total";
	string *labels=NULL;

/* In this section, we'll strip off the leading "# (eV/U)" and the trailing "Total"
 * from the line that contains the labels.
 */
	tline1=(string)malloc(sizeof(char)*(1+strlen(csheader[7])));
	strcpy(tline1,csheader[7]);
	tline1+=strlen(l1);
	while(isspace((unsigned char)*tline1))
		++tline1;
	tline2=(string)malloc(sizeof(char)*(strlen(tline1)-strlen(l2)));
	tline2=strstr(tline1,l2);
	line=(string)malloc(sizeof(char)*(1+strlen(tline1)-strlen(tline2)));
	strncpy(line,tline1,strlen(tline1)-strlen(tline2));
/* Now line should contain nothing but labels. */

/* Initialize the variable, but at this time we do not know how many labels
 * there are.
 */
	labels=(string*)malloc(sizeof(string));

	i=0;
	for(j=0,str=line;;j++,str=NULL)
	{
		token=strtok_r(str," \t",&saveptr);
		if(token==NULL)
			break;
/*		while(isspace(*token))
  			++token; */
		token=trim(token);
		if(0==strlen(token))
			break;
		labels=(string*)realloc(labels,sizeof(string)*(size_t)(i+1));
		labels[i]=(string)malloc(sizeof(char)*(1+strlen(token)));
		strcpy(labels[i],token);
		++i;
	} /* END for(j=0,str=line;;j++,str=NULL) */
	labels=(string*)realloc(labels,sizeof(string)*(size_t)(i+1));
	labels[i]=(string)malloc(sizeof(char));
	labels[i]=NULL;
	return labels;
}
