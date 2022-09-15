/* pltsub.f -- translated by f2c (version 20160102).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "paving.h"

/* Copyright(C) 1999-2020 National Technology & Engineering Solutions */
/* of Sandia, LLC (NTESS).  Under the terms of Contract DE-NA0003525 with */
/* NTESS, the U.S. Government retains certain rights in this software. */

/* See packages/seacas/LICENSE for details */
/* ======================================================================= */
/* Subroutine */ int pltsub_(xc_float *ybump, xc_float *ychrsz)
{
    static integer j;

    *ybump = -text_1.textp[0] * text_1.textp[33];
    text_1.textp[17] -= *ybump * text_1.textp[28];
    text_1.textp[18] += *ybump * text_1.textp[27];
    for (j = 14; j <= 17; ++j) {
	text_1.textp[j - 1] *= text_1.textp[31];
/* L2130: */
    }
    *ychrsz = text_1.textp[0] * text_1.textp[31];
    return 0;
} /* pltsub_ */

#ifdef __cplusplus
	}
#endif
