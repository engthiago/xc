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
                                                                        
// $Revision: 1.7 $
// $Date: 2003/03/12 19:20:46 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/truss/CorotTruss.h,v $

#ifndef CorotTruss_h
#define CorotTruss_h

// Written: MHS 
// Created: May 2001


#include "CorotTrussBase.h"
#include <utility/matrix/Matrix.h>
#include <utility/matrix/Vector.h>

namespace XC {
class Channel;
class UniaxialMaterial;

//! @ingroup TrussElem
//
//! @brief Truss element with corotational formulation. A small strain,
//! large displacement corotational space truss element,
//! as described by Crisfield in "Nonlinear Finite Element Analysis of
//! Solids and Structures", Vol. 1, 1991, J.T. Wiley.
class CorotTruss: public CorotTrussBase
  {
  private:
    UniaxialMaterial *theMaterial; //!< pointer to a material

    double A;//!< area of CorotTruss element.
    double persistentInitialDeformation; //!< Persistent initial strain at element level. Used to store the deformation during the inactive phase of the element (if any).
    
  protected:
    void free_material(void);
    void free_mem(void);
    void set_material(const UniaxialMaterial &);
    
    int sendData(Communicator &comm);
    int recvData(const Communicator &comm);

  public:
    CorotTruss(int tag= 0);
    CorotTruss(int tag, int dim,int Nd1, int Nd2, const UniaxialMaterial &theMaterial,double A);
    CorotTruss(int tag,int dimension,const Material *ptr_mat);
    CorotTruss(const CorotTruss &);
    CorotTruss &operator=(const CorotTruss &);
    Element *getCopy(void) const;
    ~CorotTruss(void);

    // public methods to obtain information about dof & connectivity    
    void setDomain(Domain *theDomain);

    // Element birth and death stuff.
    const double &getPersistentInitialSectionDeformation(void) const;
    void incrementPersistentInitialDeformationWithCurrentDeformation(void);

    // public methods to set the state of the element    
    int commitState(void);
    int revertToLastCommit(void);        
    int revertToStart(void);        
    int update(void);

    const Material *getMaterial(void) const;
    Material *getMaterial(void);
    virtual double getRho(void) const;
    double getLinearRho(void) const;
    inline const double &getSectionArea(void) const
      { return A; }
    inline void setSectionArea(const double &a)
      { A= a; }

    // public methods to obtain stiffness, mass, damping and residual information    
    const Matrix &getTangentStiff(void) const;
    const Matrix &getInitialStiff(void) const;
    const Matrix &getMass(void) const;

    void alive(void);
    void zeroLoad(void);	
    int addLoad(ElementalLoad *, double loadFactor);
    int addInertiaLoadToUnbalance(const Vector &accel);

    double getAxialForce(void) const;
    const Vector &getResistingForce(void) const;
    const Vector &getResistingForceIncInertia(void) const;            
    double getInitialStrain(void) const;

    // public methods for element output
    int sendSelf(Communicator &);
    int recvSelf(const Communicator &);
    void Print(std::ostream &s, int flag =0) const;    

    Response *setResponse(const std::vector<std::string> &argv, Information &eleInfo);
    int getResponse(int responseID, Information &eleInformation);

  };
} // end of XC namespace

#endif




