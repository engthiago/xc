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
                                                                        
// $Revision: 1.16 $
// $Date: 2004/06/07 23:42:46 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/nonlinearBeamColumn/element/NLBeamColumn2d.h,v $
                                                                        
                                                                        
// File: ~/model/element/NLBeamColumn2d.h
//
// Written by Remo Magalhaes de Souza on  09/98 
// Revised: rms 10/98 (uses section class)
//          rms 01/99 (with distributed loads)
//          rms 07/99 (using setDomain)
//          rms 08/99 (included P-Delta effect)
//	    fmk 10/99 setResponse() & getResponse()
//          rms 11/99 (included rigid joint offsets)
//          rms 04/00 (using transformation class w/ linear or corotational transf)
//          rms 04/00 (generalized to iterative/non-iterative algorithm)
//          mhs 06/00 (using new section class w/ variable dimensions)
//          rms 06/00 (torsional stiffness considered at the section level)
//          rms 06/00 (making copy of the sections)
//          rms 06/00 (storing section history variables at the element level)
//
//
// Purpose: This file contains the class definition for NLBeamColumn2d.
// NLBeamColumn2d is a materially nonlinear flexibility based frame element.

#ifndef NLBeamColumn2d_h
#define NLBeamColumn2d_h

#include <domain/mesh/element/truss_beam_column/NLForceBeamColumn2dBase.h>
#include <utility/matrix/Matrix.h>
#include <utility/matrix/Vector.h>
#include <material/section/PrismaticBarCrossSection.h>
#include <domain/mesh/element/truss_beam_column/nonlinearBeamColumn/quadrule/GaussLobattoQuadRule1d01.h>

namespace XC {
class Response;
class CrdTransf2d;

//! @ingroup BeamColumnElemGrp
//!
//! @defgroup OneDimensionalElemNL Nonlinear beam-column bidimensional elements.
//
//! @ingroup BeamColumnElemGrpNL
//! @brief Nonlinear beam-column bidimensional elements.
class NLBeamColumn2d: public NLForceBeamColumn2dBase
  {
    int maxSubdivisions;
    // internal data
    // THESE SHOULD BE REMOVED!!! -- MHS
    double cosTheta, sinTheta;     // cossine directors
  private:
    void getGlobalDispls(Vector &dg) const;
    void getGlobalAccels(Vector &ag) const;      
    void getForceInterpolatMatrix(double xi, Matrix &b, const ID &code);
    void getDistrLoadInterpolatMatrix(double xi, Matrix &bp, const ID &code);
    void compSectionDisplacements(std::vector<Vector> &,std::vector<Vector> &) const;
    
    static GaussLobattoQuadRule1d01 quadRule;

  protected:
    int sendData(Communicator &);
    int recvData(const Communicator &);
  public:
    NLBeamColumn2d(int tag= 0);
    NLBeamColumn2d(int tag,int numSec,const Material *theSection,const CrdTransf *coordTransf);
    NLBeamColumn2d(int tag, int nodeI, int nodeJ, 
		    int numSections,const std::vector<PrismaticBarCrossSection *> &, 
		    CrdTransf2d &coordTransf, double massDensPerUnitLength = 0.0, 
		    int maxNumIters = 10, double tolerance = 1e-12, int maxSub = 10);
    Element *getCopy(void) const;

    void setDomain(Domain *theDomain);

    int commitState(void);
    int revertToLastCommit(void);        
    int revertToStart(void);
    int update(void);    
    
    const Matrix &getInitialStiff(void) const;
    const Matrix &getMass(void) const;

    void zeroLoad(void);	
    int addLoad(ElementalLoad *, double loadFactor);
    int addInertiaLoadToUnbalance(const Vector &accel);


    const Vector &getResistingForceIncInertia(void) const;            
    
    bool isSubdomain(void);

    int sendSelf(Communicator &);
    int recvSelf(const Communicator &);

    friend std::ostream &operator<<(std::ostream &, const NLBeamColumn2d &);        
    void Print(std::ostream &s, int flag =0) const;    

    Response *setResponse(const std::vector<std::string> &argv, Information &eleInformation);
    int getResponse(int responseID, Information &eleInformation);
    
    int setParameter(const std::vector<std::string> &argv, Parameter &param);
    int updateParameter(int parameterID, Information &info);
  };

 std::ostream &operator<<(std::ostream &, const NLBeamColumn2d &);
} // end of XC namespace

#endif


