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

//================================================================================
// COPYRIGHT (C):     :-))                                                       |
// PROJECT:           Object Oriented Finite Element Program                     |
// PURPOSE:           General platform for elaso-plastic constitutive model      |
//                    implementation                                             |
//                                                                               |
// CLASS:             EvolutionLaw_L_Eij (linear tensorial Evolution law)        |
//                                                                               |
//                                                                               |
// VERSION:                                                                      |
// LANGUAGE:          C++.ver >= 2.0 ( Borland C++ ver=3.00, SUN C++ ver=2.1 )   |
// TARGET OS:         DOS || UNIX || . . .                                       |
// DESIGNER(S):       Boris Jeremic, Zhaohui Yang                                |
// PROGRAMMER(S):     Boris Jeremic, Zhaohui Yang                                |
//                                                                               |
//                                                                               |
// DATE:              09-13-2000                                                 |
// UPDATE HISTORY:                                                               |
//                                                                               |
//                                                                               |
//                                                                               |
//                                                                               |
// SHORT EXPLANATION: This is a linear evolution law for the evoltion of a       |
//                    tensorial variable alpha which depends on plastic strain   |
//                    i.e. dalpha = a * de_ij                                    |
//                                                                               |
//================================================================================

#ifndef EvolutionLaw_LEij_H
#define EvolutionLaw_LEij_H

#include <cmath>
#include "material/nD/Template3Dep/EvolutionLaw_T.h"

namespace XC {
//! @ingroup MatNDEL
//!
//! @brief Linear tensorial Evolution law.
//!
//! This is a linear evolution law for the evoltion of a tensorial
//! variable alpha which depends on plastic strain i.e.
//! \f$ dalpha = a * de_ij\f$.
class EvolutionLaw_L_Eij: public EvolutionLaw_T
  {
  // Private vars to define the evolution law
  private:
    double  a;  //coefficient to define the linear hardening rule of a scalar hardening var

  public:    
    EvolutionLaw_L_Eij( double ad = 10.0);                
    EvolutionLaw_L_Eij(const EvolutionLaw_L_Eij &LEL );   // Copy constructor
    
    EvolutionLaw_T *getCopy(void); // create a clone of itself
    
    //void InitVars(EPState *EPS);    // Initialize all hardening vars called only once 
    //                                // after material point is formed if necessary.
    
    //void setInitD(EPState  *EPS);   // set initial D once current stress hits the y.s.
    //                                // was primarily for Manzari-Dafalias model

    //double h( EPState *EPS,  double norm_dQods);     // Evaluating hardening function h
    BJtensor h_t( EPState *EPS, PotentialSurface *PS);    // Evaluating hardening function h
    
    //void UpdateVar( EPState *EPS, double dlamda );  // Evolve corresponding var linearly using de_eq_p
    //Moved to CDriver.cpp

    void print();

  private:
    // some accessor functions
    double geta() const;      // Linear coefficient used to evolve internal var
    void   seta(double ad);

    //================================================================================
    // Overloaded Insertion Operator	  Zhaohui Added Aug. 13, 2000
    // prints Linear EvolutionLaw's contents 
    //================================================================================
    friend std::ostream& operator<<(std::ostream &, const EvolutionLaw_L_Eij &);
  };
std::ostream& operator<<(std::ostream &, const EvolutionLaw_L_Eij &);
} // end of XC namespace


#endif




