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
                                                                        
// $Revision: 1.1.1.1 $
// $Date: 2000/09/15 08:23:23 $
// $Source: /usr/local/cvs/OpenSees/SRC/modelbuilder/ModelBuilder.h,v $
                                                                        
                                                                        
// File: ~/modelbuilder/ModelBuilder.h
// 
// Written: fmk 
// Created: Mon Sept 15 14:47:47: 1996
// Revision: A
//
// Description: This file contains the class definition for ModelBuilder.
// ModelBuilder is an abstract base class, i.e. no objects of it's
// type can be created. A ModelBuilder creates the discritization of 
// the structure.
//
// What: "@(#) ModelBuilder.h, revA"

#ifndef ModelBuilder_h
#define ModelBuilder_h

#include "utility/kernel/CommandEntity.h"

namespace XC {
class Domain;

//! @ingroup SubDom
//
//! 
//! @brief Base class for model builders.
//! 
//! The ModelBuilder class is an abstract base class. A subclass
//! of ModelBuilder is a class which creates the finite element
//! discretization of a structure: that is it discretizes the structure to
//! be modeled into Elements, Nodes, Constraints, etc. and adds these
//! components to the Domain.
 class ModelBuilder: public CommandEntity
  {
  private:
    Domain *myDomain;
  protected:
    Domain *getDomainPtr(void) const;
  public:
    ModelBuilder(Domain &theDomain);
    
    //! @brief The ModelBuilder will construct the Element, Node, Load and
    //! Constraint objects and add them to the Domain object associated with
    //! the ModelBuilder.
    virtual int buildFE_Model(void) = 0;    
  };
} // end of XC namespace

#endif

