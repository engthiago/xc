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
                                                                        
// $Revision: 1.6 $
// $Date: 2003/02/14 23:01:33 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/section/fiber_section/ElasticSection1d.h,v $
                                                                        
                                                                        
///////////////////////////////////////////////////////
// File:  ~/Src/element/hinge/ElasticSection1d.h
//
// Written by Matthew Peavy
//
// Written:  Feb 13, 2000
// Debugged: Feb 14, 2000
// Revised:  May 2000 -- MHS
//
//
// Purpose:  This header file contains the prototype
// for the ElasticSection1d class.

#ifndef ElasticSection1d_h
#define ElasticSection1d_h

#include <material/section/elastic_section/BaseElasticSection1d.h>

namespace XC {
class Channel;
class FEM_ObjectBroker;
class Information;

//! @ingroup MATSCCElastica
//
//! @brief Cross section with linear elastic material
//! for bi-dimensional problems (3 degrees of freedom in each section).
//!
//! Provides the implementation of a section which exhibits
//! uncoupled elastic behavior in axial, moment, and shear response.
class ElasticSection1d: public BaseElasticSection1d
  {
  private:
    static Vector s;
  public:
    ElasticSection1d(int tag= 0, double E= 0.0, double A=0.0);
    ElasticSection1d(int tag, double EA);
    ElasticSection1d(int tag, MaterialHandler *mat_ldr= nullptr);    

    double getStress(void) const;
    const Vector &getStressResultant(void) const;
    const Matrix &getSectionTangent(void) const;
    const Matrix &getInitialTangent(void) const;
    const Matrix &getSectionFlexibility(void) const;
    const Matrix &getInitialFlexibility(void) const;

    SectionForceDeformation *getCopy(void) const;
    const ResponseId &getResponseType(void) const;
    int getOrder(void) const;
    
    int sendSelf(Communicator &);
    int recvSelf(const Communicator &);
    

    void Print(std::ostream &s, int flag =0) const;
  };
} // end of XC namespace

#endif
