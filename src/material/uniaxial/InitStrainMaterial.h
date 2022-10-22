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
                                                                        
// $Revision: 1.1 $
// $Date: 2010-09-11 00:45:21 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/uniaxial/InitStrainMaterial.h,v $
                                                      
// Written: fmk
// Created: Sep 2010
//
// Description: This file contains the class definition for 
// InitStrainMaterial.  InitStrainMaterial wraps a UniaxialMaterial
// and imposes an initial strain

#ifndef InitStrainMaterial_h
#define InitStrainMaterial_h

#include <material/uniaxial/InitStrainBaseMaterial.h>

namespace XC {
//! @ingroup MatUnx
//
//! @brief InitStrainMaterial wraps a UniaxialMaterial
//! and imposes an initial strain
class InitStrainMaterial: public InitStrainBaseMaterial
  {
  protected:
    double localStrain;

    DbTagData &getDbTagData(void) const;
    int sendData(Communicator &);  
    int recvData(const Communicator &);
  public:
    InitStrainMaterial(int tag= 0);
    InitStrainMaterial(int tag, const UniaxialMaterial &material, double epsInit); 
    // Virtual constructor.
    UniaxialMaterial *getCopy(void) const;
    
    virtual void setMaterial(const UniaxialMaterial &);
    
    virtual int setInitialStrain(const double &);
    
    int setTrialStrain(double strain, double strainRate = 0.0); 
    double getStrain(void) const;          

    int sendSelf(Communicator &);  
    int recvSelf(const Communicator &);
    
    Response *setResponse(const std::vector<std::string> &, Information &);
    int getResponse(int responseID, Information &);
    
    void Print(std::ostream &s, int flag =0) const;
    
    // AddingSensitivity:BEGIN //////////////////////////////////////////
    int setParameter(const std::vector<std::string> &, Parameter &);
    int updateParameter(int parameterID, Information &info);
    // AddingSensitivity:END ///////////////////////////////////////////
  };
} // end of XC namespace


#endif

