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
// CLASS:             EvolutionLaw_L_Eij (nonlinear tensorial Evolution law)     |
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
// SHORT EXPLANATION: This is a nonlinear evolution law for the evoltion of a    |
//                    tensorial variable alpha which depends on plastic strain   |
//                    i.e. dalpha = 2/3*ha*dE_ij -Cr*de_eq*alpha_ij(Amstrong-    |
//                    Frederick Model                                            |
//================================================================================

#ifndef EvolutionLaw_NLEij_H
#define EvolutionLaw_NLEij_H

#include <cmath>
#include "material/nD/template_3d_ep/EvolutionLaw_T.h"

namespace XC {
//! @ingroup MatNDEL
//!
//! @brief Nonlinear tensorial evolution law.
//! 
//! This is a nonlinear evolution law for the evoltion of a
//! tensorial variable alpha which depends on plastic strain
//! i.e. \f$dalpha = 2/3*ha*dE_ij -Cr*de_eq*alpha_ij\f$
//! (Amstrong-Frederick Model)
class EvolutionLaw_NL_Eij: public EvolutionLaw_T
  {
  // Private vars to define the evolution law

    //coefficient to define the A-F hardening rule of a scalar hardening var
    double  ha, Cr;  

  public:
    //EvolutionLaw_L_Eij( );    // default constructor---no parameters
    
    EvolutionLaw_NL_Eij( double had = 10.0, double Crd = 1.0) : ha(had),Cr(Crd) {}
                         
    EvolutionLaw_NL_Eij(const EvolutionLaw_NL_Eij &LEL );   // Copy constructor
    
    EvolutionLaw_T *getCopy(void) const; // create a clone of itself
    
    //void InitVars(EPState *EPS);    // Initialize all hardening vars called only once 
    //                                // after material point is formed if necessary.
    
    //void setInitD(EPState  *EPS);   // set initial D once current stress hits the y.s.
    //                                // was primarily for Manzari-Dafalias model

    //double h( EPState *EPS,  double norm_dQods);     // Evaluating hardening function h
    BJtensor h_t( EPState *EPS, PotentialSurface *PS);    // Evaluating hardening function h
    
    //void UpdateVar( EPState *EPS, double dlamda );  // Evolve corresponding var linearly using de_eq_p
    //Moved to CDriver.cpp

    virtual void print(std::ostream &) const;

    // some accessor functions
    double getha() const;      
    double getCr() const;      
    void   setha( double had);
    void   setCr( double Crd);

 };

} // end of XC namespace


#endif




