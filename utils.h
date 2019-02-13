/* $Id: utils.h,v 1.5 2014/05/23 09:22:20 lyonsd Exp $
 * $Log: utils.h,v $
 * Revision 1.5  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.4  2014/04/22 04:58:03  lyonsd
 * Updated for -ansi compiler flag.
 *
 * Revision 1.3  2014/03/30 01:56:19  lyonsd
 * *** empty log message ***
 *
 * Revision 1.2  2014/03/19 07:33:33  lyonsd
 * Works with mknlrescs on every system
 *
 * Revision 1.1  2014/03/12 22:40:00  lyonsd
 * Initial revision
 */

#ifndef UTILS_H
#define UTILS_H

#define BLACK		"\033[22;30m"
#define RED			"\033[22;31m"
#define GREEN		"\033[22;32m"
#define BROWN		"\033[22;33m"
#define BLUE		"\033[22;34m"
#define MAGENTA		"\033[22;35m"
#define CYAN		"\033[22;36m"
#define GRAY		"\033[22;37m"
#define DGRAY		"\033[01;30m"
#define LRED		"\033[01;31m"
#define LGREEN		"\033[01;32m"
#define YELLOW		"\033[01;33m"
#define LBLUE		"\033[01;34m"
#define LMAGENTA	"\033[01;35m"
#define LCYAN		"\033[01;36m"
#define WHITE		"\033[01;37m"
#define BOLD		"\033[1m"
#define UND			"\033[4m"
#define OFF			"\033[0m"
#define RESET		"\x1b[0m"
#define PDEBUG fprintf(stderr,RED"%s\t%s\t%d"RESET"\n",__FILE__,__FUNCTION__,__LINE__+1)
#define PRTINT(x) fprintf(stderr,#x"="LRED"%d"RESET"\n",x)
#define PRTDBL(x) fprintf(stderr,#x"="LRED"%13.7e"RESET"\n",x)
#define PRTSTR(x) fprintf(stderr,#x"="LBLUE"%s"RESET"\n",x)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b)) 

typedef enum
{
    false = ( 1 == 0 ),
    true = ( ! false )
} bool;

typedef char *string;
int prtutilsrcs(void);
const char *itoabuf(char*,size_t,int);
const char *itoa(int);
char* concat(int, ...);
FILE *openfile(const char*,const char*);
char *trim(char*);
void swapstr(string*,string*);
double cmtohartree(double);
double hartreetocm(double);
unsigned int fpmans(FILE*,unsigned short int,const char*);

#endif
