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
// $Date: 2004/10/12 21:52:25 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/model/dof_grp/LagrangeDOF_Group.cpp,v $
                                                                        
// Written: fmk 
// Created: 02/99
// Revision: A
//
// Purpose: This file contains the code for implementing the methods
// of the XC::LagrangeDOF_Group class interface.
//
// What: "@(#) LagrangeDOF_Group.C, revA"


#include <solution/analysis/model/dof_grp/LagrangeDOF_Group.h>
#include <cstdlib>

#include <domain/mesh/node/Node.h>
#include <utility/matrix/Matrix.h>
#include <solution/analysis/integrator/TransientIntegrator.h>
#include <domain/constraints/SFreedom_Constraint.h>
#include <domain/constraints/MFreedom_Constraint.h>
#include <domain/constraints/MRMFreedom_Constraint.h>
#include "utility/utils/misc_utils/colormod.h"

//! @brief Constructor.
//!
//! @param tag: object identifier.
//! @param spPtr: pointer to a single DOF constraint.
XC::LagrangeDOF_Group::LagrangeDOF_Group(int tag, SFreedom_Constraint &spPtr)
  :DOF_Group(tag, 1), m_lagrange_variable(1) {}

//! @brief Constructor.
//!
//! @param tag: object identifier.
//! @param spPtr: pointer to a multiple DOF constraint.
XC::LagrangeDOF_Group::LagrangeDOF_Group(int tag, MFreedom_Constraint &mpPtr)
  : DOF_Group(tag, (mpPtr.getRetainedDOFs()).Size()),
    m_lagrange_variable(mpPtr.getConstrainedDOFs().Size())
  {}

//! @brief Constructor.
//!
//! @param tag: object identifier.
//! @param spPtr: pointer to a multiple row multiple DOF constraint.
XC::LagrangeDOF_Group::LagrangeDOF_Group(int tag, MRMFreedom_Constraint &mrmpPtr)
  :DOF_Group(tag, mrmpPtr.getRetainedDOFs().Size()),
   m_lagrange_variable(mrmpPtr.getConstrainedDOFs().Size())
  {}

//! @brief Does nothing; the Lagrange FE_Elements provide coeffs to tangent.
const XC::Matrix &XC::LagrangeDOF_Group::getTangent(Integrator *theIntegrator)
  {
    // does nothing - the Lagrange FE_Elements provide coeffs to tangent
    unbalAndTangent.getTangent().Zero();
    return unbalAndTangent.getTangent();
  }

//! @brief Does nothing; the Lagrange FE_Elements provide residual.
const XC::Vector &XC::LagrangeDOF_Group::getUnbalance(Integrator *theIntegrator)
  {
    // does nothing - the Lagrange FE_Elements provide residual 
    unbalAndTangent.getResidual().Zero();
    return unbalAndTangent.getResidual();
  }

// void setNodeDisp(const Vector &u);
//	Method to set the corresponding nodes displacements to the
//	values in u, components identified by myID;

//! @brief Does nothing. The lagrange multipliers are associated with no Nodes
//! in the Domain.
void XC::LagrangeDOF_Group::setNodeDisp(const Vector &u)
  {
    m_lagrange_variable.Zero();
    const ID &ids= this->getID();
    for (int i = 0; i < this->getNumDOF(); i++)
      {
        int loc = ids(i);
        if (loc >= 0)
	  m_lagrange_variable(i) = u(loc);
      }
#ifdef LAG_DOF_VERBOSE
    std::cerr << "LAG DOF: set U (total) = " << u;
    std::cerr << "   -> LM = " << m_lagrange_variable;
#endif // LAG_DOF_VERBOSE
  }
	
	
//! @brief Does nothing. The lagrange multipliers are associated with no Nodes
//! in the Domain.
void XC::LagrangeDOF_Group::setNodeVel(const Vector &udot)
  { return; }


//! @brief Does nothing. The lagrange multipliers are associated with no Nodes
//! in the Domain.
void XC::LagrangeDOF_Group::setNodeAccel(const Vector &udotdot)
  { return; }

//! @brief Does nothing. The lagrange multipliers are associated with no Nodes
//! in the Domain.
void XC::LagrangeDOF_Group::incrNodeDisp(const Vector &u)
  {
    const ID &ids = this->getID();
    for(int i = 0; i < this->getNumDOF(); i++)
      {
        int loc = ids(i);
        if (loc >= 0)
	  m_lagrange_variable(i) += u(loc);
      }
#ifdef LAG_DOF_VERBOSE
    opserr << "LAG DOF: set U (increment) = " << u;
    opserr << "   -> LM = " << m_lagrange_variable;
#endif // LAG_DOF_VERBOSE

  }
	
//! @brief Does nothing. The lagrange multipliers are associated with no Nodes
//! in the Domain.
void XC::LagrangeDOF_Group::incrNodeVel(const Vector &udot)
  { return; }

//! @brief Does nothing. The lagrange multipliers are associated with no Nodes
//! in the Domain.
void XC::LagrangeDOF_Group::incrNodeAccel(const Vector &udotdot)
  { return; }


const XC::Vector &XC::LagrangeDOF_Group::getCommittedDisp(void) const
  { 
    // note: this is actually the trial one. but this method is only
    // called by traansient integrators during the domainChanged method
    // (trial and committed should be the same)
    return m_lagrange_variable;
  }

const XC::Vector &XC::LagrangeDOF_Group::getCommittedVel(void) const
  {
    LagrangeDOF_Group *this_no_const= const_cast<LagrangeDOF_Group *>(this);
    this_no_const->unbalAndTangent.getResidual().Zero();
    return unbalAndTangent.getResidual();
  }

const XC::Vector &XC::LagrangeDOF_Group::getCommittedAccel(void) const
  {
    LagrangeDOF_Group *this_no_const= const_cast<LagrangeDOF_Group *>(this);
    this_no_const->unbalAndTangent.getResidual().Zero();
    return unbalAndTangent.getResidual();
  }
const XC::Vector &XC::LagrangeDOF_Group::getTrialDisp(void) const
  {
    return m_lagrange_variable;
  }

const XC::Vector& XC::LagrangeDOF_Group::getTrialVel(void) const
  {
    LagrangeDOF_Group *this_no_const= const_cast<LagrangeDOF_Group *>(this);
    this_no_const->unbalAndTangent.getResidual().Zero();
    return unbalAndTangent.getResidual();
  }

const XC::Vector& XC::LagrangeDOF_Group::getTrialAccel() const
  {
    LagrangeDOF_Group *this_no_const= const_cast<LagrangeDOF_Group *>(this);
    this_no_const->unbalAndTangent.getResidual().Zero();
    return unbalAndTangent.getResidual();
  }

void XC::LagrangeDOF_Group::addMtoTang(double fact)
  {}

void XC::LagrangeDOF_Group::zeroUnbalance(void)
  {}

void XC::LagrangeDOF_Group::zeroTangent(void)
  {}

void XC::LagrangeDOF_Group::addPtoUnbalance(double fact)
  {}

void XC::LagrangeDOF_Group::addPIncInertiaToUnbalance(double fact)
  {}

void XC::LagrangeDOF_Group::addM_Force(const Vector &Udotdot, double fact)
  {}

const XC::Vector &XC::LagrangeDOF_Group::getTangForce(const Vector &disp, double fact)
  {
    std::cerr << Color::red << getClassName() << "::" << __FUNCTION__
              << "; not yet implemented."
	      << Color::def << std::endl;
    unbalAndTangent.getResidual().Zero();
    return unbalAndTangent.getResidual();
  }

const XC::Vector &XC::LagrangeDOF_Group::getC_Force(const Vector &disp, double fact)
  {
    unbalAndTangent.getResidual().Zero();
    return unbalAndTangent.getResidual();
  }

const XC::Vector &XC::LagrangeDOF_Group::getM_Force(const Vector &disp, double fact)
  {
    unbalAndTangent.getResidual().Zero();
    return unbalAndTangent.getResidual();
  }


