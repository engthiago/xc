// -*-c++-*-
//----------------------------------------------------------------------------
//  XC program; finite element analysis code
//  for structural analysis and design.
//
//  Copyright (C)  Luis Claudio Pérez Tato
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
                                                                        
// $Revision: 1.5 $
// $Date: 2003/10/07 18:57:04 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/component/DomainComponent.h,v $
                                                                        
                                                                        
#ifndef DomainComponent_h
#define DomainComponent_h

// Written: fmk 
// Created: 11/96
// Revision: A
//
// Description: This file contains the class definition for DomainComponent.
// The DomainComponent class is an abstract class, example subclasses include
// Element, Node, SFreedom_Constraint, MFreedom_Constraint, NodalLoad, ElementalLoad. 
// Each of these objects forms part of Domain and has methods to set and obtain
// the associated Domain object.
//
// What: "@(#) DomainComponent.h, revA"

#include "utility/tagged/TaggedObject.h"
#include "utility/actor/actor/MovableObject.h"

namespace XC {
class Domain;
class Renderer;
class Information;
class Preprocessor;
class LoadPattern;

//! @brief Base class for domain components (nodes, elements, constraints,
//! loads,...).
//! @ingroup Dom
//!
//! Base class for the different components of a domain.
//! Each object of these types is a component of an enclosing Domain object.
//! The DomainComponent class provides methods to set and retrieve a pointer
//! to the enclosing domain object.
class DomainComponent: public TaggedObject, public MovableObject
  {
  private:    
    Domain *theDomain; //!< a pointer to the enclosing Domain object
    size_t idx; //!< Index of the object (to be used in VTK arrays).
  protected:
    DomainComponent(int tag, int classTag);
    int sendData(Communicator &);
    int recvData(const Communicator &);
  public:
    ~DomainComponent(void);
    //! @brief Returns the index of the object (to be used in VTK arrays).
    inline size_t getIdx(void) const
      { return idx; }
    virtual void setDomain(Domain *theDomain);
    virtual Domain *getDomain(void) const;
    const Preprocessor *getPreprocessor(void) const;
    Preprocessor *getPreprocessor(void);
    const LoadPattern *getCurrentLoadPattern(void) const;
    LoadPattern *getCurrentLoadPattern(void);
    
    void set_index(const size_t &i);

  };
} // end of XC namespace

#endif


