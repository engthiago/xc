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
// $Date: 2005/11/28 21:38:40 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/model/fe_ele/penalty/PenaltyMFreedom_FE.h,v $
                                                                        
                                                                        
#ifndef PenaltyMFreedom_FE_h
#define PenaltyMFreedom_FE_h

// Written: fmk 
// Created: 11/96
// Revision: A
//
// Description: This file contains the class definition for PenaltyMFreedom_FE.
// PenaltyMFreedom_FE is a subclass of FE_Element which handles MFreedom_Constraints
// using the penalty method.
//
// What: "@(#) PenaltyMFreedom_FE.h, revA"

#include "solution/analysis/model/fe_ele/MFreedom_FE.h"
#include <utility/matrix/ID.h>

namespace XC {
class Element;
class Integrator;
class AnalysisModel;
class Domain;

//! @ingroup AnalysisFE
//
//! @brief PenaltyMFreedom_FE is a subclass of FE_Element which
//! handles MFreedom_Constraints using the penalty method.
//!
//! PenaltyMP\_FE is a subclass of FE\_Element used to enforce a
//! multi point constraint, of the form \f$U_c = C_{cr} U_r\f$,
//! where \f$U_c\f$ are the constrained degrees-of-freedom at the
//! constrained node, \f$U_r\f$ are the retained degrees-of-freedom
//! at the retained node and \f$C_{cr}\f$ a matrix defining the relationship
//! between these degrees-of-freedom. 
//!
//! To enforce the constraint a matrix \f$\alpha C^T C\f$ is added to the
//! tangent for the degrees-of-freedom \f$[U_c\f$ \f$U_r]\f$, where
//! \f$C = [-I\f$ \f$C_{cr}]\f$. Nothing is added to the residual.  
class PenaltyMFreedom_FE: public MFreedom_FE
  {
  private:
    void determineTangent(void);
    
    Matrix C; //!< to hold the C matrix

    friend class AnalysisModel;
    friend class AutoConstraintHandler;
    PenaltyMFreedom_FE(int tag, Domain &, MFreedom_Constraint &theMFreedom, double alpha);
  public:

    // public methods
    virtual int  setID(void);
    virtual const Matrix &getTangent(Integrator *theIntegrator);
    virtual const Vector &getResidual(Integrator *theIntegrator);
    virtual const Vector &getTangForce(const Vector &x, double fact = 1.0);

    virtual const Vector &getK_Force(const Vector &x, double fact = 1.0);
    virtual const Vector &getKi_Force(const Vector &x, double fact = 1.0);
    virtual const Vector &getC_Force(const Vector &x, double fact = 1.0);
    virtual const Vector &getM_Force(const Vector &x, double fact = 1.0);
  };
} // end of XC namespace

#endif


