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
                                                                        
// $Revision: 1.10 $
// $Date: 2003/03/11 20:42:39 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/elasticBeamColumn/ElasticBeam2d.h,v $
                                                                        
                                                                        
// File: ~/model/ElasticBeam2d.h
//
// Written: fmk 11/95
// Revised:
//
// Purpose: This file contains the class definition for ElasticBeam2d.
// ElasticBeam2d is a plane frame member.

#ifndef ElasticBeam2d_h
#define ElasticBeam2d_h

#include "domain/mesh/element/truss_beam_column/elasticBeamColumn/ElasticBeam2dBase.h"
#include "domain/mesh/element/utils/fvectors/FVectorBeamColumn2d.h"
#include "domain/mesh/element/utils/coordTransformation/CrdTransf2d.h"

namespace XC {
class Channel;
class Information;
class Response;
class CrossSectionProperties3d;

//! @brief 2D elastic beam element.
//! @ingroup BeamColumnElemGrp
class ElasticBeam2d: public ElasticBeam2dBase
  {
  private:
    double d; //!< Section depth.

    static Matrix K;
    static Vector P;
    
    static Matrix kb;
    mutable Vector q; //!< forces in local system
    FVectorBeamColumn2d q0;  //!< Fixed end forces in basic system
    FVectorBeamColumn2d p0;  //!< Reactions in basic system
    
    int release; //!< moment release 0=none, 1=I, 2=J, 3=I,J

    void init(void);
  protected:
    DbTagData &getDbTagData(void) const;
    int sendData(Communicator &comm);
    int recvData(const Communicator &comm);

  public:
    ElasticBeam2d(int tag= 0);
    ElasticBeam2d(int tag,const Material *m,const CrdTransf *trf);
    ElasticBeam2d(int tag, double A, double E, double I, 
		  int Nd1, int Nd2, CrdTransf2d &theTransf, 
		  double d = 0.0, double rho = 0.0, int release= 0);
    Element *getCopy(void) const;

    const Vector &computeCurrentStrain(void) const;
    
    inline int getReleaseCode(void) const
      { return release; }
    inline void setReleaseCode(const int &rc)
      { release= rc; }

    virtual int update(void);
    int commitState(void);
    int revertToLastCommit(void);        
    int revertToStart(void);

    inline double getDepth(void) const
      { return d; }
    void setDepth(const double &h)
      { d= h; } 

    const Matrix &getTangentStiff(void) const;
    const Matrix &getInitialStiff(void) const;
    const Matrix &getMass(void) const;

    void zeroLoad(void);	
    int addLoad(ElementalLoad *, double loadFactor);
    int addInertiaLoadToUnbalance(const Vector &accel);

    const Vector &getResistingForce(void) const;
    const Vector &getResistingForceIncInertia(void) const;            
    
    int sendSelf(Communicator &);
    int recvSelf(const Communicator &);
    
    void Print(std::ostream &s, int flag = 0) const;
    
    //! @brief Internal shear force in the middle of the element.
    inline double getV(void) const
      { return (q(1)+q(2))/theCoordTransf->getInitialLength(); }
    //! @brief Internal shear force at the back end.   
    inline double getV1(void) const
      { return -getV()-p0[1]; }
    //! @brief Internal shear force at the front end.   
    inline double getV2(void) const
      { return -getV()+p0[2]; }
    
    //! @brief Internal axial force at the back end.   
    inline double getN1(void) const
      { return q(0)-p0[0]; }
    //! @brief Internal axial force at the front end.   
    inline double getN2(void) const
      { return q(0); }
    
    //! @brief Internal bending moment at the back end.   
    inline double getM1(void) const
      { return -q(1); }
    //! @brief Internal bending moment at the front end.   
    inline double getM2(void) const
      { return q(2); }


    Response *setResponse(const std::vector<std::string> &argv, Parameter &param);
    int getResponse(int responseID, Information &info);
 
    int setParameter(const std::vector<std::string> &argv, Parameter &param);
    int updateParameter(int parameterID, Information &info);

  };
} // end of XC namespace

#endif


