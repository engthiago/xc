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
//DistributedBase.cpp

#include "DistributedBase.h"
#include "DbTagData.h"
#include <iostream>
#include <typeinfo>

//! @brief Constructor.
XC::DistributedBase::DistributedBase(void)
  {}

//! @brief Destructor.
XC::DistributedBase::~DistributedBase(void)
  {}

//! @brief Returns a vector to store class dbTags.
XC::DbTagData &XC::DistributedBase::getDbTagData(void) const
  {
    static DbTagData retval(0);
    std::string nmb= typeid(*this).name();
    std::cerr << nmb << "; function getDbTagData must be overloaded."
              << std::endl;
    return retval;
  }

//! @brief Returns the data at the i-th position.
const int &XC::DistributedBase::getDbTagDataPos(const int &i) const
  { return getDbTagData().getDbTagDataPos(i); }

//! @brief Sets the data at the i-th position.
void XC::DistributedBase::setDbTagDataPos(const int &i,const int &v)
  { return getDbTagData().setDbTagDataPos(i,v); }

//! @brief Initializes communication.
void XC::DistributedBase::inicComm(const int &dataSize) const
  { return getDbTagData().inicComm(dataSize); }

