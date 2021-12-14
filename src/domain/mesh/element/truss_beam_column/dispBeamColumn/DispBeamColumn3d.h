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
                                                                        
// $Revision: 1.11 $
// $Date: 2003/02/25 23:32:50 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/dispBeamColumn/DispBeamColumn3d.h,v $

// Written: MHS
// Created: Feb 2001
//
// Description: This file contains the class definition for DispBeamColumn3d.
// The element displacement field gives rise to constant axial strain,
// linear curvature, and constant twist angle.

#ifndef DispBeamColumn3d_h
#define DispBeamColumn3d_h

#include <domain/mesh/element/truss_beam_column/BeamColumnWithSectionFDTrf3d.h>
#include <utility/matrix/Matrix.h>
#include <utility/matrix/Vector.h>
#include "domain/mesh/element/utils/fvectors/FVectorBeamColumn3d.h"

namespace XC {
class Node;
class PrismaticBarCrossSection;
class CrdTransf3d;
class Response;
class BeamIntegration;

//! @ingroup DispBeamColumnGrp
//
//! @brief 3D beam element with PrismaticBarCrossSection type material.
class DispBeamColumn3d: public BeamColumnWithSectionFDTrf3d
  {
  private:
    void free_mem(void);
    void alloc(const BeamIntegration &);
    BeamIntegration *beamIntegration; //!< Integration along the beam length.
    mutable Vector q; //!< Basic force
    FVectorBeamColumn3d q0;  // Fixed end forces in basic system (no torsion)
    FVectorBeamColumn3d p0;  // Reactions in basic system (no torsion)

    const Matrix &getInitialBasicStiff(void) const;

    // AddingSensitivity:BEGIN //////////////////////////////////////////
    int parameterID;
    // AddingSensitivity:END ///////////////////////////////////////////
    
    static Matrix K;		// Element stiffness, damping, and mass Matrix
    static Vector P;		// Element resisting force vector


    static double workArea[];

  protected:
    int sendData(Communicator &comm);
    int recvData(const Communicator &comm);

  public:
    DispBeamColumn3d(int tag= 0);
    DispBeamColumn3d(int tag, int numSec, const Material *theSection, const CrdTransf *trf, const BeamIntegration *bi);
    DispBeamColumn3d(int tag, int nd1, int nd2, int numSections, const std::vector <PrismaticBarCrossSection *> &, const CrdTransf3d &coordTransf, const BeamIntegration &bi, double rho = 0.0);
    DispBeamColumn3d(const DispBeamColumn3d &);
    DispBeamColumn3d &operator=(const DispBeamColumn3d &);
    ~DispBeamColumn3d(void);
    Element *getCopy(void) const;

    int getNumDOF(void) const;
    void setDomain(Domain *theDomain);

    // public methods to set the state of the element    
    int commitState(void);
    int revertToLastCommit(void);
    int revertToStart(void);

    // public methods to obtain stiffness, mass, damping and residual information    
    int update(void);
    const Matrix &getTangentStiff(void) const;
    const Matrix &getInitialStiff(void) const;
    const Matrix &getMass(void) const;

    void zeroLoad(void);
    int addLoad(ElementalLoad *, double loadFactor);
    int addInertiaLoadToUnbalance(const Vector &accel);

    const Vector &getResistingForce(void) const;
    const Vector &getResistingForceIncInertia(void) const;            

    // public methods for element output
    int sendSelf(Communicator &);
    int recvSelf(const Communicator &);
    void Print(std::ostream &s, int flag =0) const;

    Response *setResponse(const std::vector<std::string> &argv, Information &eleInfo);
    int getResponse(int responseID, Information &eleInfo);
    // AddingSensitivity:BEGIN //////////////////////////////////////////
    int setParameter(const std::vector<std::string> &argv, Parameter &param);
    int updateParameter(int parameterID, Information &info);
    int activateParameter(int parameterID);
    const Vector & getResistingForceSensitivity(int gradNumber);
    const Matrix & getKiSensitivity(int gradNumber);
    const Matrix & getMassSensitivity(int gradNumber);
    int commitSensitivity(int gradNumber, int numGrads);
    // AddingSensitivity:END ///////////////////////////////////////////
  };
} // end of XC namespace

#endif

