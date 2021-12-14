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
                                                                        
// $Revision: 1.8 $
// $Date: 2003/05/08 22:56:41 $
// $Source: SRC/material/section/fiber_section/fiber/UniaxialFiber.cpp,v $
                                                                        
                                                                        
// File: ~/section/UniaxialFiber.C
//
// Written: Remo Magalhaes de Souza
// Created: 10/98
// Revision: 
//
// Description: This file contains the implementation for the
// UniaxialFiber class. UniaxialFiber provides the abstraction of a
// uniaXial XC::Fiber whose position  is defined with only one coordinate.
// The XC::UniaxialFiber is subjected to a stress state with 
// only one nonzero axial stress and corresponding axial strain.
//
// What: "@(#) UniaxialFiber.h, revA"

#include "UniaxialFiber.h"
#include <material/uniaxial/UniaxialMaterial.h>
#include "preprocessor/prep_handlers/MaterialHandler.h"
#include "utility/actor/actor/MovableVector.h"

void XC::UniaxialFiber::free_mem(void)
  {
    if(theMaterial)
      {
        delete theMaterial;
        theMaterial= nullptr;
      }
  }

void XC::UniaxialFiber::alloc(const UniaxialMaterial &theMat)
  {
    free_mem();
    theMaterial = theMat.getCopy();// get a copy of the MaterialModel
    if(!theMaterial)
      {
        std::cerr <<"XC::UniaxialFiber::UniaxialFiber  -- failed to get copy of XC::UniaxialMaterial\n";
        exit(-1);
      }    
  }

//! @brief Constructor for blank object that recvSelf needs to be invoked upon
XC::UniaxialFiber::UniaxialFiber(int classTag)
  : Fiber(0, classTag), theMaterial(nullptr), area(0.0) {}

//! @brief Constructor.
XC::UniaxialFiber::UniaxialFiber(int tag, int classTag, double Area)
  : Fiber(tag, classTag), theMaterial(nullptr), area(Area) {}

//! @brief Constructor.
XC::UniaxialFiber::UniaxialFiber(int tag, int classTag,const UniaxialMaterial &theMat,const double &Area)
  : Fiber(tag, classTag), theMaterial(nullptr), area(Area)
  { alloc(theMat); }

//! @brief Constructor.
XC::UniaxialFiber::UniaxialFiber(int tag, int classTag,const MaterialHandler &ldr,const std::string &nmbMat,const double &Area)
  : Fiber(tag, classTag), theMaterial(nullptr), area(Area)
  { setMaterial(ldr,nmbMat); }

//! @brief Copy constructor.
XC::UniaxialFiber::UniaxialFiber(const UniaxialFiber &other)
  : Fiber(other),theMaterial(nullptr), area(other.area)
  { setMaterial(other.theMaterial); }

//! @brief Assignment operator.
XC::UniaxialFiber &XC::UniaxialFiber::operator=(const UniaxialFiber &other)
  {
    Fiber::operator=(other);
    area= other.area;
    setMaterial(other.theMaterial);
    return *this;
  }

// Destructor: 
XC::UniaxialFiber::~UniaxialFiber(void)
  { free_mem(); }

//! @brief Set the fiber material.
void XC::UniaxialFiber::setMaterial(const UniaxialMaterial *theMat)
  {
    if(theMat)
      alloc(*theMat);
    else
      free_mem();
  }

//! @brief Sets the fiber material (identified by name).
void XC::UniaxialFiber::setMaterial(const MaterialHandler &ldr,const std::string &nmbMat)
  {
    const UniaxialMaterial *theMat= dynamic_cast<const UniaxialMaterial *>(ldr.find_ptr(nmbMat));
    setMaterial(theMat);
  }

//! @brief Commits material state.
int XC::UniaxialFiber::commitState(void)
  { return theMaterial->commitState(); }

//! @brief Returns to its last committed state.
int XC::UniaxialFiber::revertToLastCommit(void)
  { return theMaterial->revertToLastCommit(); }

//! @brief Returns to the initial state.
int XC::UniaxialFiber::revertToStart(void)
  { return theMaterial->revertToStart(); }


//! @brief Send data through the communicator argument.
int XC::UniaxialFiber::sendData(Communicator &comm)
  {
    int res= Fiber::sendData(comm);
    res+= comm.sendBrokedPtr(theMaterial,getDbTagData(),BrokedPtrCommMetaData(2,3,4));
    res+= comm.sendDouble(area,getDbTagData(),CommMetaData(5));

    return res;
  }


//! @brief Receive data through the communicator argument.
int XC::UniaxialFiber::recvData(const Communicator &comm)
  {    
    int res= Fiber::recvData(comm);
    theMaterial= comm.getBrokedMaterial(theMaterial,getDbTagData(),BrokedPtrCommMetaData(2,3,4));
    res+= comm.receiveDouble(area,getDbTagData(),CommMetaData(5));
    return res;
  }
