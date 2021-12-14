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
                                                                        
// $Revision: 1.5 $
// $Date: 2003/02/14 23:01:26 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/nD/PlateFiberMaterial.h,v $

// Ed "C++" Love
//
// Generic Plate Fiber Material
//

#ifndef PLATE_FIBER_MATERIAL_H
#define PLATE_FIBER_MATERIAL_H

#include "NDAdaptorMaterial.h"

namespace XC{
//! @ingroup NDMatMatAdaptorGrp
//
//! @brief Builds a material that can be used in a plate fiber from
//! its definition as three-dimensional material.
class PlateFiberMaterial: public NDAdaptorMaterial
  {
  private :
    static Vector stress;
    static Matrix tangent;

    int indexMap(int i) const;
  public : 
    PlateFiberMaterial(int tag= 0);
    PlateFiberMaterial(int tag, NDMaterial &the3DMaterial );

    //make a clone of this material
    NDMaterial *getCopy(void) const;
    NDMaterial *getCopy(const std::string &) const;

    //send back order of strain in vector form
    int getOrder(void) const;

    //send back order of strain in vector form
    const std::string &getType(void) const ;

    //get the strain 
    int setTrialStrain( const Vector &strainFromElement );

    //send back the stress 
    const Vector& getStress(void) const;

    //send back the tangent 
    const Matrix& getTangent(void) const;
    const Matrix &getInitialTangent(void) const;

    //print out data
    void Print( std::ostream &s, int flag ) const;

    int sendSelf(Communicator &);
    int recvSelf(const Communicator &);

  };

} //end of XC namespace

#endif
