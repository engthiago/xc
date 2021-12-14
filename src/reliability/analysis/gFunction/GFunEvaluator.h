// -*-c++-*-
//----------------------------------------------------------------------------
//  XC program; finite element analysis code
//  for structural analysis and design.
//
//  Copyright (C)  Luis C. Pérez Tato
//
//  This program derives from OpenSees <http://opensees.berkeley.edu>
//  developed by the  «Pacific earthquake engineering research center».
//
//  Except for the restrictions that may arise from the copyright
//  of the original program (see copyright_opensees.txt)
//  XC is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or 
//  (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful, but 
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details. 
//
//
// You should have received a copy of the GNU General Public License 
// along with this program.
// If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------
/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 2001, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** Reliability module developed by:                                   **
**   Terje Haukaas (haukaas@ce.berkeley.edu)                          **
**   Armen Der Kiureghian (adk@ce.berkeley.edu)                       **
**                                                                    **
** ****************************************************************** */
                                                                        
// $Revision: 1.4 $
// $Date: 2003/10/27 23:45:43 $
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/analysis/gFunction/GFunEvaluator.h,v $


//
// Written by Terje Haukaas (haukaas@ce.berkeley.edu)
//

#ifndef GFunEvaluator_h
#define GFunEvaluator_h

#include <utility/matrix/Vector.h>
#include <reliability/domain/components/ReliabilityDomain.h>
#include <tcl.h>

#include <fstream>
using std::ofstream;

namespace XC {

//! @ingroup ReliabilityAnalysis
//
//! @brief Performance function evaluator.
class GFunEvaluator
  {
  protected:
    Tcl_Interp *theTclInterp;
    ReliabilityDomain *theReliabilityDomain;
    double g;
    int numberOfEvaluations;

  public:
    GFunEvaluator(Tcl_Interp *theTclInterp, ReliabilityDomain *theReliabilityDomain);

    // Methods provided by base class
    int evaluateG(Vector x);
    double getG(void);
    int initializeNumberOfEvaluations();
    int getNumberOfEvaluations();

    // Methods to be implemented by specific classes
    virtual int runGFunAnalysis(const Vector &x)= 0;
    virtual int tokenizeSpecials(const std::string &theExpression)	=0;

    // Methods implemented by SOME specific classes (random vibrations stuff)
    virtual void setNsteps(int nsteps);
    virtual double getDt(void);
  };
} // end of XC namespace

#endif

