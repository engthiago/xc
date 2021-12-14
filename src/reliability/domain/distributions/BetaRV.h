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
                                                                        
// $Revision: 1.5 $
// $Date: 2003/03/04 00:44:33 $
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/domain/distributions/BetaRV.h,v $


//
// Written by Terje Haukaas (haukaas@ce.berkeley.edu) 
//

#ifndef BetaRV_h
#define BetaRV_h

#include <reliability/domain/components/RandomVariable.h>

namespace XC {
//! @ingroup ProbDistributions
// 
//! @brief Beta distribution random variable.
//
//! In probability theory and statistics, the beta distribution is a
//! family of continuous probability distributions defined on the
//! interval [0, 1] parametrized by two positive shape parameters,
//! denoted by α and β, that appear as exponents of the random variable
//! and control the shape of the distribution.
class BetaRV: public RandomVariable
  {
  private:
    double a;
    double b;
    double q;
    double r;
  public:
    BetaRV(int tag, 
		     double parameter1,
		     double parameter2,
		     double parameter3,
		     double parameter4,
		     double startValue);
    BetaRV(int tag, 
		     double parameter1,
		     double parameter2,
		     double parameter3,
		     double parameter4);
    void Print(std::ostream &s, int flag =0) const;
    double getPDFvalue(double rvValue);
    double getCDFvalue(double rvValue);
    double getInverseCDFvalue(double probValue);
    const std::string getType(void);
    double getMean();
    double getStdv();
    double getParameter1();
    double getParameter2();
    double getParameter3();
    double getParameter4();
    double getStartValue();
    double betaFunction(double passed_q, double passed_r);
  };
} // end of XC namespace

#endif

