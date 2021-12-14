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
** (C) Copyright 1999, The Regents of the University of California    **
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
** ****************************************************************** */
                                                                        
// $Revision: 1.2 $
// $Date: 2005/11/28 21:37:12 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/handler/LagrangeConstraintHandler.h,v $
                                                                        
                                                                        
// Written: fmk 
// Created: May 1998.
// Revision: A
//
// Description: This file contains the class definition for 
// LagrangeConstraintHandler. LagrangeConstraintHandler is a 
// constraint handler for handling constraints using the Lagrange method.
// for each element and degree-of-freedom at a node it constructs regular
// FE_Element and DOF_Groups; for each SFreedom_Constraint and MFreedom_Constraint
// LagrangeSFreedom_FE and LagrangeMFreedom_FE elements are created.
//
// What: "@(#) LagrangeConstraintHandler.h, revA"

#ifndef LagrangeConstraintHandler_h
#define LagrangeConstraintHandler_h

#include <solution/analysis/handler/FactorsConstraintHandler.h>

namespace XC {
class FE_Element;
class DOF_Group;

//
//! @ingroup AnalysisCH
//!
//! @brief Constraint handler for handling constraints using the Lagrange
//! multipliers method.
//! 
//! The LagrangeConstraintHandler class is a class which deals with
//! both single and multi freedom constraints using the Lagrange
//! method. This is done by, in addition to creating a DOF\_Group object
//! for each Node and an FE\_Element for each Element in the Domain,
//! creating a LagrangeDOF\_Group object and either a LagrangeSFreedom\_FE or a
//! LagrangeMP\_FE object for each constraint in the Domain. It is these
//! objects that enforce the constraints by modifying the tangent matrix
//! and residual vector.
class LagrangeConstraintHandler: public FactorsConstraintHandler
  {
  protected:
    friend class ModelWrapper;
    friend class FEM_ObjectBroker;
    LagrangeConstraintHandler(ModelWrapper *,const double &alphaSP= 1.0,const double &alphaMP= 1.0);
    ConstraintHandler *getCopy(void) const;
  public:
    int handle(const ID *nodesNumberedLast =0);
  };
} // end of XC namespace

#endif




