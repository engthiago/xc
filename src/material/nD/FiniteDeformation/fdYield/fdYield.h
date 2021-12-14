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
//===============================================================================
//# COPYRIGHT (C): Woody's license (by BJ):
//                 ``This    source  code is Copyrighted in
//                 U.S.,  for  an  indefinite  period,  and anybody
//                 caught  using it without our permission, will be
//                 mighty good friends of ourn, cause we don't give
//                 a  darn.  Hack it. Compile it. Debug it. Run it.
//                 Yodel  it.  Enjoy it. We wrote it, that's all we
//                 wanted to do.''
//
//# PROJECT:           Object Oriented Finite Element Program
//# PURPOSE:           Finite Deformation Hyper-Elastic classes
//# CLASS:
//#
//# VERSION:           0.6_(1803398874989) (golden section)
//# LANGUAGE:          C++
//# TARGET OS:         all...
//# DESIGN:            Zhao Cheng, Boris Jeremic (jeremic@ucdavis.edu)
//# PROGRAMMER(S):     Zhao Cheng, Boris Jeremic
//#
//#
//# DATE:              July 2004
//# UPDATE HISTORY:
//#
//===============================================================================

#ifndef fdYield_H
#define fdYield_H

#include <iostream>

namespace XC {
  class stresstensor;
  class FDEPState;

//! @ingroup FDNDMat
//!
//! @defgroup FDYieldNDMat Yield surface.
//
//! @ingroup FDYieldNDMat
//
//! @brief Base class for large deformation hyperelastic yield.
class fdYield
  {
  public:
    fdYield();
    virtual ~fdYield() {}; 
    
    virtual fdYield *getCopy(void) = 0;   

    virtual int getNumRank();
    virtual double getTolerance();
    virtual double Yd(const stresstensor &sts, const FDEPState &fdepstate ) const = 0;	
    
    virtual stresstensor dYods(const stresstensor &sts, const FDEPState &fdepstate ) const = 0; 
    
    virtual double dYodq(const stresstensor &sts, const FDEPState &fdepstate ) const;	 
    virtual stresstensor dYoda(const stresstensor &sts, const FDEPState &fdepstate ) const; 
    
    virtual void print() = 0; 

    friend std::ostream& operator<<(std::ostream &, const fdYield &);
  };
std::ostream &operator<<(std::ostream &, const fdYield &);
} // end of XC namespace


#endif
