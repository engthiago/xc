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
                                                                        
// $Revision: 1.2 $
// $Date: 2002/06/26 23:00:12 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/uniaxial/fedeas/FedeasHyster1Material.h,v $
                                                                      
// Written: MHS
// Created: Jan 2001
//
// Description: This file contains the class definition for 
// FedeasHyster1Material. FedeasHyster1Material wraps the FEDEAS
// 1d material subroutine Hyster_1.

#ifndef FedeasHyster1Material_h
#define FedeasHyster1Material_h

#include <material/uniaxial/FedeasMaterial.h>

namespace XC {
//! @ingroup MatUnx
//
//! @brief FedeasConcr2Material wraps the FEDEAS
//! 1d material subroutine Concr_2.
class FedeasHyster1Material: public FedeasMaterial
  {
  public:
    FedeasHyster1Material(int tag,
		double mom1p, double rot1p, double mom2p, double rot2p,
		double mom1n, double rot1n, double mom2n, double rot2n,
		double pinchX, double pinchY, double damfc1 = 0.0, double damfc2 = 0.0);
    FedeasHyster1Material(int tag, const Vector &d);
    FedeasHyster1Material(int tag= 0);

    double getInitialTangent(void) const;
    UniaxialMaterial *getCopy(void) const;
  };
} // end of XC namespace


#endif

