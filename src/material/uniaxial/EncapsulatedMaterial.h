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

#ifndef EncapsulatedMaterial_h
#define EncapsulatedMaterial_h

#include <material/uniaxial/UniaxialMaterial.h>

namespace XC {
//! @ingroup MatUnx
//
//! @brief Encapsulates a copy to an uniaxial material.
class EncapsulatedMaterial: public UniaxialMaterial
  {
  protected:
    UniaxialMaterial *theMaterial;

    void free_mem(void);
    void copy(const UniaxialMaterial *);
  public:
    EncapsulatedMaterial(int tag, int classTag, const UniaxialMaterial &material); 
    EncapsulatedMaterial(int tag= 0,int classTag= 0);
    EncapsulatedMaterial(const EncapsulatedMaterial &);
    EncapsulatedMaterial &operator=(const EncapsulatedMaterial &);
    ~EncapsulatedMaterial(void);

    const UniaxialMaterial *getMaterial(void) const;
    UniaxialMaterial *getMaterial(void);
    virtual void setMaterial(const UniaxialMaterial &);
    void setMaterial(const std::string &);
    
    double getStrain(void) const;          
    double getStrainRate(void) const;
    
    int sendData(Communicator &);  
    int recvData(const Communicator &);
  };
} // end of XC namespace


#endif

