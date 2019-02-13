/* $Id: labels.h,v 1.5 2014/04/28 14:06:41 lyonsd Exp $
 * $Log: labels.h,v $
 * Revision 1.5  2014/04/28 14:06:41  lyonsd
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

#include "utils.h"
#include "xsection.h"

#ifndef LABELS_H
#define LABELS_H

/* A simple function to print the rcsid string */

void prtlabelsrcs(void);

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

string getlabelfn(xsection*);

/* This function accepts the addres of a string array, which is modified to
 * contain character strings of the cross section labels.
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

string *getnistlabels(string);

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

string *prtlabels(string*);
string *nlabels(string*);
string *sortnlabels(string*);
string *reverselabels(string*);
string *trimlabels(string*,int);

#endif
