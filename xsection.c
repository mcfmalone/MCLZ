/* $Id: xsection.c,v 1.6 2014/04/28 14:09:32 lyonsd Exp $
 * $Log: xsection.c,v $
 * Revision 1.6  2014/04/28 14:09:32  lyonsd
 * Compiles under Linux with no warnings or errors
 *
 * Revision 1.5  2014/04/22 04:58:03  lyonsd
 * Updated for -ansi compiler flag
 *
 * Revision 1.4  2014/03/22 01:47:40  lyonsd
 * Working for all systems
 *
 * Revision 1.3  2014/03/21 17:59:19  lyonsd
 * Renaming to xsection
 *
 * Revision 1.2  2014/03/19 07:33:33  lyonsd
 * This fails on some systems.  Further debugging needed.
 *
 * Revision 1.1  2014/03/12 22:40:00  lyonsd
 * Initial revision
 */

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 10000
#endif
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "utils.h"
#include "xsection.h"

static const char rcsid[]="$Id: xsection.c,v 1.6 2014/04/28 14:09:32 lyonsd Exp $";

void prtxsectionrcs()
{
	(void)fprintf(stderr,"%s\n",rcsid);
}

xsection *initcs()
{
	xsection *cs=NULL;
	char *errmsg;
	const char fmtstr[]="Error: %d: memory allocation failure in\nfile\t\t%s\nfunction\t%s\nline\t\t%d\n";
	int lineno=0;

	cs=(xsection*)malloc(sizeof(xsection));
	if(!cs)
	{
		lineno=__LINE__-2;
		errmsg=(char*)malloc(strlen(fmtstr)+strlen(__FILE__)+strlen(__FUNCTION__));
		sprintf(errmsg,fmtstr,errno,__FILE__,__FUNCTION__,lineno);
		perror(errmsg);
		free(errmsg);
		exit(errno);
	}
	memset(cs,'\0',sizeof(xsection));
	if(!(cs->ATOM=getenv("atom")) && !(cs->ATOM=getenv("ATOM")))
	{
		cs->ATOM=(char*)malloc(sizeof(ATOMD)+1);
		strcpy(cs->ATOM,ATOMD);
	}
	if(!(cs->ION=getenv("ion")) && !(cs->ION=getenv("ION")))
	{
		cs->ION=(char*)malloc(sizeof(IOND)+1);
		strcpy(cs->ION,IOND);
	}
	if(getenv("z"))
	{
		cs->Z=atoi(getenv("z"));
	}
	else if(getenv("Z"))
	{
		cs->Z=atoi(getenv("Z"));
	}
	else
	{
		cs->Z=0;
	}
	if(!(cs->NAME=getenv("name")) && !(cs->NAME=getenv("NAME")) && !(cs->NAME=getenv("LOGNAME")))
	{
		cs->NAME=(char*)malloc(sizeof(NAMED)+1);
		strcpy(cs->NAME,NAMED);
	}
	if(!(cs->INITIALS=getenv("initials")) && !(cs->INITIALS=getenv("INITIALS")))
	{
		cs->INITIALS=(char*)malloc(sizeof(INITIALSD)+1);
		strcpy(cs->INITIALS,INITIALSD);
	}

	cs->nllab=NULL;
	cs->nlab=NULL;
	cs->rows=0;
	cs->nlcols=0;
	cs->ncols=0;
	cs->nldata=NULL;
	cs->ndata=NULL;

	return cs;
}

/* This function is useful for error checking, or verification.
 * It accepts the address of a xsection structure, and prints the values
 * of the fields within that structure.  The output goes to STDERR.
 *
 * INPUT:
 * The address of (a pointer to) a xsection structure.
 *
 * RETURN:
 * The address the xsection structure, if it was successfully printed.
 *
 * OUTPUT:
 * The names of the elements in the structure, and their values, to STDERR
 */

xsection *prtcs(xsection *cs)
{

	if(cs)
	{
		PRTSTR(cs->ION);
		PRTSTR(cs->ATOM);
		PRTINT(cs->Z);
		PRTSTR(cs->NAME);
		PRTSTR(cs->INITIALS);
	}

	return cs;
}
