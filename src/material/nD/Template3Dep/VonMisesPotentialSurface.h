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
///*
//################################################################################
//# COPYRIGHT (C):     :-))                                                      #
//# PROJECT:           Object Oriented Finite Element Program                    #
//# PURPOSE:           Von Mises         potential criterion                     #
//# CLASS:             VonMisesPotentialSurface                                        #
//#                                                                              #
//# VERSION:                                                                     #
//# LANGUAGE:          C++.ver >= 2.0 ( Borland C++ ver=3.00, SUN C++ ver=2.1 )  #
//# TARGET OS:         DOS || UNIX || . . .                                      #
//# PROGRAMMER(S):     Boris Jeremic, Zhaohui Yang                               #
//#                                                                              #
//#                                                                              #
//# DATE:              August 03 '93                                             #
//# UPDATE HISTORY:    September 01 '00                                          #
//#                                                                              #
//#                                                                              #
//#                                                                              #
//#                                                                              #
//# SHORT EXPLANATION: Von Mises potential surface                               #
//#                                                                              #
//#                                                                              #
//################################################################################
//*/

#ifndef VonMisesPotentialSurface_H
#define VonMisesPotentialSurface_H


#include "material/nD/Template3Dep/PotentialSurface.h"


namespace XC {
//! @ingroup MatPS
//!
//! @brief Von Mises model potential surface.
class VonMisesPotentialSurface: public PotentialSurface
{
  // Private vars to define the Von Mises Potential Surface
  private:		  
  
  public:
    PotentialSurface *getCopy(void); // create a clone of itself
    VonMisesPotentialSurface ( ) {} // Default constructor
    //VonMisesPotentialSurface (const VonMisesPotentialSurface & );   // copy constructor

    BJtensor dQods(const EPState *EPS) const;
    BJtensor d2Qods2(const EPState *EPS) const;
       
    BJtensor d2Qodsdt1(const EPState *EPS) const; // For Consistent Algorithm, Z Cheng, Jan 2004
    
    void print() { std::cerr << *this; }; 
  
    //================================================================================
    // Overloaded Insertion Operator
    // prints an VonMises PotentialSurface's contents 
    //================================================================================
    friend std::ostream& operator<<(std::ostream &, const VonMisesPotentialSurface &);
  };
std::ostream& operator<<(std::ostream &, const VonMisesPotentialSurface &);
} // end of XC namespace

#endif

