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

#include "EncapsulatedMaterial.h"
#include <utility/matrix/ID.h>
#include "utility/matrix/Vector.h"


void XC::EncapsulatedMaterial::free_mem(void)
  {
    if(theMaterial) delete theMaterial;
    theMaterial= nullptr;
  }

//! brief Copy object members.!
void XC::EncapsulatedMaterial::copy(const UniaxialMaterial *other)
  {
    free_mem();
    if(other)
      theMaterial= other->getCopy();
  }

XC::EncapsulatedMaterial::EncapsulatedMaterial(int tag, int classTag, UniaxialMaterial &material)
  :UniaxialMaterial(tag,classTag), theMaterial(nullptr)
  {
    copy(&material);
    if(!theMaterial)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
		  <<  "; failed to get copy of material\n";
        exit(-1);
      }
  }

XC::EncapsulatedMaterial::EncapsulatedMaterial(int tag, int classTag)
  :UniaxialMaterial(tag,classTag), theMaterial(nullptr) {}

XC::EncapsulatedMaterial::~EncapsulatedMaterial(void)
  { free_mem(); }

XC::EncapsulatedMaterial::EncapsulatedMaterial(const EncapsulatedMaterial &other)
  :UniaxialMaterial(other), theMaterial(nullptr)
  { copy(other.theMaterial); }

XC::EncapsulatedMaterial &XC::EncapsulatedMaterial::operator=(const EncapsulatedMaterial &other)
  {
    UniaxialMaterial::operator=(other);
    copy(other.theMaterial);
    return *this;
  }

double XC::EncapsulatedMaterial::getStrain(void) const
  { return theMaterial->getStrain(); }

double XC::EncapsulatedMaterial::getStrainRate(void) const
  { return theMaterial->getStrainRate(); }

int XC::EncapsulatedMaterial::sendData(Communicator &comm)
  {
    setDbTagDataPos(0,getTag());
    int res= sendMaterialPtr(theMaterial,getDbTagData(),comm,BrokedPtrCommMetaData(1,2,3));
    return res;
  }

int XC::EncapsulatedMaterial::recvData(const Communicator &comm)
  {
    int res= UniaxialMaterial::recvData(comm);
    theMaterial= dynamic_cast<UniaxialMaterial *>(receiveMaterialPtr(theMaterial,getDbTagData(),comm,BrokedPtrCommMetaData(1,2,3)));
    
    return res;
  }
