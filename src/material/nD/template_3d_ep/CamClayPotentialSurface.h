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
//
//=============================================================================
//# COPYRIGHT (C):     :-))                                                   #
//# PROJECT:           Object Oriented Finite Element Program                 #
//# PURPOSE:           Cam-Clay potential criterion                           #
//# CLASS:             CamClayPotentialSurface                                    #
//#                                                                           #
//# VERSION:                                                                  #
//# LANGUAGE:          C++.ver >= 2.0 ( Borland C++ ver=3.00, SUN C++ ver=2.1)#
//# TARGET OS:         DOS || UNIX || . . .                                   #
//# PROGRAMMER(S):     Boris Jeremic, ZHaohui Yang                            #
//#                                                                           #
//#                                                                           #
//# DATE:              Mar. 28, 2001                                          #
//# UPDATE HISTORY:                                                           #
//#                                                                           #
//#                                                                           #
//#                                                                           #
//#                                                                           #
//# SHORT EXPLANATION:                                                        #
//#                                                                           #
//#                                                                           #
//=============================================================================


#ifndef CamClayPotentialSurface_H
#define CamClayPotentialSurface_H

#include "material/nD/template_3d_ep/PotentialSurface.h"

namespace XC {
  class BJtensor;
//! @ingroup MatPS
//!
//! @brief Cam-Clay model potential surface.
class CamClayPotentialSurface: public PotentialSurface
  {
  // Private vars to define the Manzari-Dafalias Potential Surface
  private:
    double M;//!< the slope of critical state line

  public:
    CamClayPotentialSurface(double Mp = 1.2); // Default constructor
    PotentialSurface *getCopy(void) const; // create a clone of itself

    BJtensor dQods(const EPState *EPS) const;
    BJtensor d2Qods2(const EPState *EPS) const;
    
    BJtensor d2Qodsds1(const EPState *EPS) const; // For Consistent Algorithm, Z Cheng, Jan 2004

    double getM() const;
    void setM(const double &);
    
    void print(std::ostream &os) const;

  };
} // end of XC namespace

#endif

