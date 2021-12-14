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
// $Date: 2002/12/05 22:49:09 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/nD/elastic_isotropic/ElasticIsotropicAxiSymm.h,v $

#ifndef ElasticIsotropicAxiSymm_h
#define ElasticIsotropicAxiSymm_h

// File: ~/material/ElasticIsotropicAxiSymm.h
//
// Written: MHS
// Created: Feb 2000
// Revision: A
//
// Description: 
//
// What: "@(#) ElasticIsotropicAxiSymm.h, revA"

#include <material/nD/elastic_isotropic/ElasticIsotropicMaterial.h>


namespace XC {
//! @ingroup NDMat
//
//! @brief Base class for elastic isotropic axisymmetric materials.
class ElasticIsotropicAxiSymm: public ElasticIsotropicMaterial
  {
  private:
    static Vector sigma;	// Stress vector ... class-wide for returns
    static Matrix D;	// Elastic constants
  public:
    ElasticIsotropicAxiSymm(int tag, double E, double nu, double rho);
    ElasticIsotropicAxiSymm(int tag);
    ElasticIsotropicAxiSymm();

    const Matrix &getTangent(void) const;
    const Matrix &getInitialTangent(void) const;
    const Vector &getStress(void) const;
    
    int commitState (void);
    int revertToLastCommit (void);
    
    NDMaterial *getCopy(void) const;
    const std::string &getType(void) const;
    int getOrder(void) const;
  };
} // end of XC namespace


#endif


