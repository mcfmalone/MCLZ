/* $Id: csheader.h,v 1.6 2014/05/23 09:22:20 lyonsd Exp $
 * $Log: csheader.h,v $
 * Revision 1.6  2014/05/23 09:22:20  lyonsd
 * Working version
 *
 * Revision 1.5  2014/04/28 14:06:00  lyonsd
 * Compiles under Linux with no warnings or errors
 *
 * Revision 1.4  2014/04/22 04:58:03  lyonsd
 * Updated for -ansi compiler flag.
 *
 * Revision 1.3  2014/03/22 01:47:40  lyonsd
 * Working for all systems
 *
 * Revision 1.2  2014/03/19 07:33:33  lyonsd
 * Works with mknlrescs on every system
 *
 * Revision 1.1  2014/03/12 22:40:00  lyonsd
 * Initial revision
 */

#ifndef CSHEADER_H
#define CSHEADER_H

#include "xsection.h"
#include "utils.h"

int prtcsheaderrcs(void);
xsection *prtcsheader(xsection*,const char[]);
xsection *getcsparams(string*);
string *readcsheader(void);
string *getcslabels(string*);

/* The fields are:
 * 
 * char *ION
 * char *ATOM
 * int Z (charge)
 * char *NAME
 * char *INITIALS
 */

#endif
