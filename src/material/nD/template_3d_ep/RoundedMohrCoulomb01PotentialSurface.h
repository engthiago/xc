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
// COPY LEFT and RIGHT:
// Commercial    use    of    this  program without express permission of the
// University, is strictly encouraged. Copyright and Copyleft are covered
// by the following clause:
//
// Woody's (Guthrie) license:																																		
// ``This    source    code is Copyrighted in U.S., by the The University and 
// by the Authors,  for  an indefinite period, and anybody caught
// using  it  without  our  permission,  will be mighty good friends of ourn,
// cause  we  don't give a darn. Hack it. Compile it. Debug it. Run it. Yodel
// it. Enjoy it. We wrote it, that's all we wanted to do.'' bj
//
//
//
// PROJECT:           Object Oriented Finite Element Program
// PURPOSE:           Rounded Mohr Coulomb Potential Surface
// CLASS:
//
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER(S):       Boris Jeremic jeremic@ucdavis.edu
//                    Zhao Cheng,
// PROGRAMMER(S):     Zhao Cheng, Boris Jeremic
//
//
// DATE:              12 Feb. 2003
// UPDATE HISTORY:
//
//
//
//
// SHORT EXPLANATION: Functions for rounded Mohr-Coulomb potential function
//
//================================================================================

#ifndef RoundedMohrCoulomb01PotentialSurface_H
#define RoundedMohrCoulomb01PotentialSurface_H

#include "material/nD/template_3d_ep/PotentialSurface.h"


namespace XC {
//! @ingroup MatPS
//!
//! @brief Rounded Mohr-Coulomb model potential surface.
class RoundedMohrCoulomb01PotentialSurface: public PotentialSurface
  {
  public:
    RoundedMohrCoulomb01PotentialSurface( ){ }; // Default constructor
    ~RoundedMohrCoulomb01PotentialSurface() { }; //Not all virtual functions  redefined
    PotentialSurface *getCopy(void) const; //create a clone of itself

    BJtensor dQods(const EPState *EPS  ) const;
    BJtensor d2Qods2(const EPState *EPS) const;

    BJtensor d2Qodsds1(const EPState *EPS) const; // For Consistent Algorithm, Z Cheng, Jan 2004

    virtual void print(std::ostream &os) const;  
  };

} // end of XC namespace

#endif

