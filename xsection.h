/* $Id: xsection.h,v 1.7 2014/04/28 14:09:32 lyonsd Exp $
 * $Log: xsection.h,v $
 * Revision 1.7  2014/04/28 14:09:32  lyonsd
 * Compiles under Linux with no warnings or errors
 *
 * Revision 1.6  2014/04/22 04:58:03  lyonsd
 * Updated for -ansi compiler flag.
 *
 * Revision 1.5  2014/03/22 01:47:40  lyonsd
 * Working for all systems
 *
 * Revision 1.4  2014/03/21 17:59:19  lyonsd
 * Renaming to xsection
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

#ifndef XSECTION_H
#define XSECTION_H

/* These are the default values which will be assigned to our variables, in
 * the event nothing is specified on the command line, and the appropriate
 * environment variables are not set.
 */

#define ATOMD "«ATOM»"
#define IOND "«ION»"
#define NAMED "«NAME»"
#define INITIALSD "«XXX»"

typedef struct
{
	int Z;
	char *ATOM;
	char *ION;
	char *NAME;
	char *INITIALS;
	char **nllab;
	char **nlab;
	int rows;
	int nlcols;
	int ncols;
	double **nldata;
	double **ndata;
} xsection;

void prtxsectionrcs(void);
xsection *initcs(void);
xsection *prtcs(xsection*);

#endif
