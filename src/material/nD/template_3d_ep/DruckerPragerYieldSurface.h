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
//# PURPOSE:           Drucker - Prager  yield criterion                         #
//# CLASS:             DruckerPragerYieldSurface                                            #
//#                                                                              #
//# VERSION:                                                                     #
//# LANGUAGE:          C++.ver >= 2.0 ( Borland C++ ver=3.00, SUN C++ ver=2.1 )  #
//# TARGET OS:         DOS || UNIX || . . .                                      #
//# PROGRAMMER(S):     Boris Jeremic, Zhaohui Yang                               #
//#                                                                              #
//#                                                                              #
//# DATE:              August 03 '93                                             #
//# UPDATE HISTORY:    August 08 '00                                                          #
//#                                                                              #
//#                                                                              #
//#                                                                              #
//#                                                                              #
//# SHORT EXPLANATION:                                                           #
//#                                                                              #
//# if alfa1#=0 && alfa2#=0 && alfa1#=alfa2 =>                                   #
//#              Drucker-Prager with non-associated flow rule                    #
//#                                                                              #
//# if alfa1#=0 && alfa2#=0 && alfa1==alfa2 =>                                   #
//#              Drucker-Prager with associated flow rule                        #
//#                                                                              #
//# if alfa1==0 && alfa2#=0 =>                                                   #
//#              Von Mises with non-associated Drucker-Prager flow rule          #
//#                                                                              #
//# if alfa1#=0 && alfa2==0 =>                                                   #
//#              Drucker-Prager with non-associated Von Mises flow rule          #
//#                                                                              #
//# if alfa1==0 && alfa2==0 =>                                                   #
//#              Von Mises with associated flow rule                             #
//#                                                                              #
//################################################################################
//*/

#ifndef DruckerPragerYieldSurface_H
#define DruckerPragerYieldSurface_H

#include "material/nD/template_3d_ep/YieldSurface.h"


namespace XC {
//! @ingroup MatNDYS
//!
//! @brief Drucker-Prager model yield surface.
class DruckerPragerYieldSurface: public YieldSurface
  {
  public:
    DruckerPragerYieldSurface(void) {}    // Default constructor
    YieldSurface *getCopy(void) const; //create a clone of itself
    

    double f(const EPState *EPS) const;
    BJtensor dFods(const EPState *EPS) const;

    // Redefine 1st derivative of F over scalar internal variables
    double xi_s1( const EPState *EPS ) const;
    double xi_s2( const EPState *EPS ) const;

    // Redefine 1st derivative of F over tensorial internal variables
    BJtensor xi_t1(const EPState *EPS) const;

    void print() { std::cerr << *this; }; 
  
    friend std::ostream& operator<<(std::ostream &, const DruckerPragerYieldSurface &);
  };
std::ostream& operator<<(std::ostream &, const DruckerPragerYieldSurface &);
} // end of XC namespace

#endif

