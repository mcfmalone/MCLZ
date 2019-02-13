/* $Id: crossing.h,v 1.2 2014/04/29 14:56:39 lyonsd Exp $
 * $Log: crossing.h,v $
 * Revision 1.2  2014/04/29 14:56:39  lyonsd
 * Changes to rcsid
 *
 * Revision 1.1  2014/04/22 04:58:03  lyonsd
 * Initial revision
 *
 *
 * Landau-Zener crossing estimator
 *
 * code to estimate Rx and Delta U(R_x) using the
 * polarization potential and short-range repulsive
 * term of Butler & Dalgarno. Three forms of Delta U
 * are then determined: i) Butler-Dalgarno (1980),
 * ii) Olson-Salop (1976), and iii) Olson-Salop-Taulbjerg
 * (1986). The first is for any ion with q<=4, the second
 * is for any q, but for bare ions, while the latter
 * is for partially-stripped ions of any charge q.
 *
 * Rx occurs at point were second column changes sign
 * output in crossing.out
 *
 */

#ifndef CROSSING_H
#define CROSSING_H

#include "lzcxs.h"

void prtcrossingrcs(void);
cxs_t *crossing(cxs_t*);

#endif
