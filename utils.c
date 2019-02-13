/* $Id: utils.c,v 1.5 2014/05/23 16:53:30 lyonsd Exp $
 * $Log: utils.c,v $
 * Revision 1.5  2014/05/23 16:53:30  lyonsd
 * Fixed compiler error on Linux
 *
 * Revision 1.4  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.3  2014/04/22 04:58:03  lyonsd
 * Updated for -ansi compiler flag.
 *
 * Revision 1.2  2014/03/19 07:33:33  lyonsd
 * Works with mknlrescs on every system
 *
 * Revision 1.1  2014/03/12 22:40:00  lyonsd
 * Initial revision
 */

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE	100000
#endif
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE  200112L
#endif
#include <sys/ioctl.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

static const char *rcsid="$Id: utils.c,v 1.5 2014/05/23 16:53:30 lyonsd Exp $";
/* int snprintf(char *str, size_t size, const char *format, ...);*/
struct winsize ws;

int prtutilsrcs()
{
	return fprintf(stderr,"%s\n",rcsid);
}

const char *itoabuf(char *buf,size_t len,int num)
{
	static char loc_buf[sizeof(int)*4]; /* not thread safe */

	if(!buf)
	{
		buf=loc_buf;
		len=sizeof(loc_buf);
	}

	if(snprintf(buf,len,"%d",num)==-1)
		return ""; /* or whatever */

	return buf;
}

const char *itoa(int num)
{
	return itoabuf(NULL, 0, num);
}

char* concat(int count, ...)
	{
	va_list ap;
	int i;
	size_t null_pos=0;
	char *merged=NULL;

	/* Find required length to store merged string */
	size_t len=1; /* room for NULL */
	va_start(ap, count);
	for(i=0;i<count;i++)
		len+=strlen(va_arg(ap,char*));
	va_end(ap);
	
	/* Allocate memory to concat strings */
	merged=calloc(sizeof(char),len);
	null_pos=0;
	
	/* Actually concatenate strings */
	va_start(ap,count);
	for(i=0;i<count;i++)
	{
		char *s=va_arg(ap,char*);
		strcpy(merged+null_pos,s);
		null_pos+=strlen(s);
	}
	va_end(ap);
	return merged;
}

FILE *openfile(const char *filename,const char *mode)
{
    FILE *fp;
    char *errmsg;
    const char *fmtstr="Error %d: %s";
    int lineno=0;
	int errnum=0;

    if(NULL==(fp=fopen(filename,mode)))
    {
		errnum=errno;
        lineno=__LINE__-3;
		errmsg=(char*)malloc(strlen(filename)+strlen(fmtstr));
        sprintf(errmsg,fmtstr,errnum,filename);
        fprintf(stderr,"File:\t\t%s\n",__FILE__);
        fprintf(stderr,"Function:\t%s\n",__FUNCTION__);
        fprintf(stderr,"Line:\t\t%d\n",lineno);
        perror(errmsg);
        exit(errnum);
    }

    return fp;
}

char *trim(char *s)
{
	if(s)
	{ /* Don't forget to check for NULL! */
		while(*s && isspace((unsigned char)*s))
			++s;
		if(*s)
		{
			register char *p = s;
			while(*p)
				++p;
			do
			{
				--p;
			} while((p != s) && isspace((unsigned char)*p));
			*(p + 1) = '\0';
		}
	}
	return(s);
}

void swapstr(string *i, string *d)
{
    string t;
	
	t=*d;
    *d=*i;
    *i=t;
}

double cmtohartree(double x)
{
	return x/219474.631370811;
}

double hartreetocm(double x)
{
	return x*219474.631370811;
}

unsigned int fpmans(FILE *fp,unsigned short int tabs,const char *str)
{
	unsigned short int tabsize=8;
	unsigned short int maxstrlen=0;
	unsigned short int instrlen=0;
	unsigned short int breaks=0;
	unsigned short int index=0;
	unsigned short int jndex=0;
	char tab='\t';
	char newline='\n';
	char *strbreak=NULL;
	const char *tstr=str;
	char **mstr;
/*	char *newstr=NULL; */



/* "strbreak" is a string which contains a newline character '\n' and a number
 * of tab characters '\t' equal to the variable "tab".  So, for example, if
 * "tab"==3, then "strbreak"="\n\t\t\t". It will then be inserted into the
 * string contained in the variable "str" such that the string will be printed
 * on multiple lines, indented by "tab" tab spaces, depending on the width of
 * the screen.
 */
	strbreak=(char*)malloc(sizeof(char)*(size_t)(1+tabs));
	strbreak[0]=newline;
	for(index=1;index<=tabs;index++)
		strbreak[index]=tab;

/* Here we need to find out how long out string can be.  So we start with the
 * screen width (i.e., the number of columns, and subtract off 8 spaces for
 * each tab.  The number of tabs tells us how far the text will be indented
 */
	maxstrlen=ws.ws_col-(tabs*tabsize)-2;
	
/* We need to find the length of the input string, and then subtract off the
 * number of non-printable characters.  Since every control sequences will be
 * in the form \033[Nm, that's 4 characters: \033, [, N, and m.  None of these
 * will use any spaces on the output.
 */
	instrlen=strlen(str);
	for(index=0;index<strlen(str);index++)
	{
		if(iscntrl((int)tstr[index]))
			instrlen-=4;
	}

/* Find out how many breaks in the line will be needed */
	breaks=(unsigned short int)(instrlen/maxstrlen);

/* Print out N tabs */
	for(index=0;index<tabs;index++)
		(void)fputs("\t",fp);

/* Print out the input string if it's not too long */
	if(instrlen<=maxstrlen)
		(void)fputs(tstr,fp);
	else
	{
		mstr=(char**)malloc(sizeof(char*)*(size_t)(breaks+1));
		for(index=0;index<=breaks;index++)
		{
			maxstrlen=ws.ws_col-(tabs*tabsize)-2;
			for(jndex=0;jndex<maxstrlen;jndex++)
			{
				if(iscntrl((int)tstr[jndex]))
						maxstrlen+=4;
			}	
			mstr[index]=(char*)malloc(sizeof(char)*(size_t)(maxstrlen+1));
			strncpy(mstr[index],tstr,maxstrlen);
			jndex=maxstrlen;
			tstr+=maxstrlen;
			if(index<breaks)
			{
				while(!isblank(mstr[index][jndex]))
				{
					mstr[index][jndex]='\0';
					tstr--;
					jndex--;
				}
				(void)fputs(mstr[index],fp);
				(void)fputs(strbreak,fp);
			}
			else
			{
				(void)fputs(mstr[index],fp);
			}
			free(mstr[index]);
		}
		free(mstr);
	}
	free(strbreak);
	return 0;
}
