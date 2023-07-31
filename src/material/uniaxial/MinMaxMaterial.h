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
                                                                        
// $Revision: 1.3 $
// $Date: 2003/02/14 23:01:39 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/uniaxial/MinMaxMaterial.h,v $
                                                      
// Written: MHS
// Created: Aug 2001
//
// Description: This file contains the class definition for 
// MinMaxMaterial.  MinMaxMaterial wraps a UniaxialMaterial
// and imposes min and max strain limits.

#ifndef MinMaxMaterial_h
#define MinMaxMaterial_h

#include "material/uniaxial/EncapsulatedMaterial.h"

namespace XC {
//! @ingroup MatUnx
//
//! @brief Stores max and min strain values.
class MinMaxMaterial: public EncapsulatedMaterial
  {
  private:
    double minStrain;
    double maxStrain;

    bool Tfailed;
    bool Cfailed;
    
    DbTagData &getDbTagData(void) const;
    int sendData(Communicator &);  
    int recvData(const Communicator &);
  public:
    MinMaxMaterial(int tag, UniaxialMaterial &material, double min, double max); 
    MinMaxMaterial(int tag= 0);
    
    UniaxialMaterial *getCopy(void) const;

    int setTrialStrain(double strain, double strainRate = 0.0); 
    double getStress(void) const;
    double getTangent(void) const;
    double getDampTangent(void) const;
    inline double getInitialTangent(void) const
        {return this->getMaterial()->getInitialTangent();}

    int commitState(void);
    int revertToLastCommit(void);    
    int revertToStart(void);        

    
    int sendSelf(Communicator &);  
    int recvSelf(const Communicator &);
    
    void Print(std::ostream &s, int flag =0) const;
  };
} // end of XC namespace


#endif

