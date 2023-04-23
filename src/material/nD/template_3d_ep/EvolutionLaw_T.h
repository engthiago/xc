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

///=========================================================================
// COPYRIGHT (C): :-))                                                     |
// PROJECT:       Object Oriented Finite Element Program                   |
// PURPOSE:       General platform for elaso-plastic efficient and easy    |
//                constitutive model implementation                        |
//                                                                         |
// CLASS:         EvolutionLaw_T (the base class for tensorial             |
//                  evolution law)                                         |
//                                                                         |
// VERSION:                                                                |
// LANGUAGE:      C++.ver >= 2.0 ( Borland C++ ver=3.00, SUN C++ ver=2.1 ) |
// TARGET OS:     DOS || UNIX || . . .                                     |
// DESIGNER(S):   Boris Jeremic, Zhaohui Yang                              |
// PROGRAMMER(S): Boris Jeremic, Zhaohui Yang                              |
//                                                                         |
//                                                                         |
// DATE:          09-02-2000                                               |
// UPDATE HISTORY:                                                         |
//                                                                         |
//                                                                         |
//                                                                         |
//                                                                         |
// SHORT EXPLANATION: The base class is necessary for we need to use to    |
//                    runtime polymorphism of C++ to achieve the fexibility|
//                    of the platform.                                     |
//                                                                         |
//==========================================================================
//

#ifndef EL_T_H
#define EL_T_H

#include <iostream>
#include "utility/kernel/CommandEntity.h"

namespace XC {
class EPState;
class PotentialSurface;
 class BJtensor;

//! @ingroup MatNDEL
//!
//! @brief Base class for tensorial evolution laws.
class EvolutionLaw_T: public CommandEntity
  {
  public:
    // Constructor
    EvolutionLaw_T(void) {}	 
    virtual ~EvolutionLaw_T(void) {}
    //create a clone of itself
    virtual EvolutionLaw_T *getCopy(void) const;  

    // Not necessary since the increment of internal var can be evalulated in constitutive driver!
    //virtual void UpdateVar( EPState *EPS, double dlamda ) = 0; // Evolve only one internal variable
    
    //Print the contents of the evolution law
    virtual void print(std::ostream &) const; 	
    
    // updating E, e, D and m for Manzari-Dafalias model, 
    virtual int updateEeDm( EPState *EPS, double st_vol, double dLamda);  

    //virtual void setInitD( EPState *EPS) = 0;  // Initializing D once current st hits the y.s. for Manzari-Dafalias model , 
    //                                  // other model might not need it
    
    //Why can't i put const before EPState???????????????????????? Aug. 16, 2000
    //virtual double h( EPState *EPS, double d ) = 0;    // Evaluating hardening function h
    // Evaluating hardening function h_t
    virtual BJtensor h_t(EPState *EPS, PotentialSurface *PS);    

  };
  
std::ostream& operator<<(std::ostream &, const EvolutionLaw_T &);
} // end of XC namespace


#endif

