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
// $Date: 2003/02/14 23:00:57 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/load/beam_loads/Beam2dUniformLoad.cpp,v $
                                                                        

// Written: fmk 
//
// Purpose: This file contains the class implementation of XC::Beam2dUniformLoad.

#include <domain/load/beam_loads/Beam2dUniformLoad.h>
#include <utility/matrix/Vector.h>
#include <utility/matrix/Matrix.h>
#include <utility/actor/objectBroker/FEM_ObjectBroker.h>
#include "utility/matrix/ID.h"

#include "domain/mesh/element/utils/fvectors/FVector.h"
#include "material/section/repres/CrossSectionProperties3d.h"

XC::Vector XC::Beam2dUniformLoad::data(2);

//! @brief Constructor.
XC::Beam2dUniformLoad::Beam2dUniformLoad(int tag, double wt, double wa,const ID &theElementTags)
  :BeamUniformLoad(tag, LOAD_TAG_Beam2dUniformLoad,wt,wa,theElementTags) {}

//! @brief Constructor.
XC::Beam2dUniformLoad::Beam2dUniformLoad(int tag)
  :BeamUniformLoad(tag, LOAD_TAG_Beam2dUniformLoad) {}


const XC::Vector &XC::Beam2dUniformLoad::getData(int &type, const double &loadFactor) const
  {
    type = getClassTag();
    data(0)= WTrans();
    data(1)= WAxial();
    return data;
  }

//! @brief Return the dimension of the force vector.
size_t XC::Beam2dUniformLoad::getForceVectorDimension(void) const
  { return 2; }

//! @brief Return the dimension of the vector moment.
size_t XC::Beam2dUniformLoad::getMomentVectorDimension(void) const
  { return 1; }

//! @brief Returns force expressed in local coordinates.
XC::Vector XC::Beam2dUniformLoad::getLocalForce(void) const
  {
    Vector retval(2);
    retval(0)= WAxial();
    retval(1)= WTrans();
    return retval;
  }

//! @brief Returns moment expressed in local coordinates.
XC::Vector XC::Beam2dUniformLoad::getLocalMoment(void) const
  {
    Vector retval(1);
    retval(0)= 0.0;
    return retval;
  }

//! @brief Returns distributed force vectors (one for each element) expressed in local coordinates.
const XC::Matrix &XC::Beam2dUniformLoad::getLocalForces(void) const
  {
    static Matrix retval;
    const size_t sz= numElements();
    retval= Matrix(sz,2);
    for(size_t i=0; i<sz; i++)
      {
        retval(i,0)= WAxial();
        retval(i,1)= WTrans();
      }
    return retval;
  }

//! @brief Returns distributed force moments (one for each element) expressed in local coordinates.
const XC::Matrix &XC::Beam2dUniformLoad::getLocalMoments(void) const
  {
    static Matrix retval;
    const size_t sz= numElements();
    retval= Matrix(sz,1);
    for(size_t i=0; i<sz; i++)
      retval(i,0)= 0.0;
    return retval;
  }

//! @brief Applied section forces due to element uniform load
const XC::Matrix &XC::Beam2dUniformLoad::getAppliedSectionForces(const double &L,const Matrix &xi_pt,const double &loadFactor) const
  {
    const size_t nSections= xi_pt.noRows();
    static Matrix retval(3,1); //Compile time definition.
    retval.resize(3,nSections); //Resize.
    retval.Zero();
    const double wa= WAxial()*loadFactor;  // Axial
    const double wy= WTrans()*loadFactor;  // Transverse

    //Computes effect of the loads over the sections.
    for(size_t i = 0; i < nSections; i++)
      {
        const double x= xi_pt(i,0)*L;

        retval(0,i)= wa*(L-x); //Integral of the axial force between x and L.
        retval(1,i)= wy*0.5*x*(x-L); //Bending moment on the section at x.
        retval(2,i)= wy*(x-0.5*L); //Shear force on the section at x.
      }
    return retval;
  }

//! @brief Returns the consistent load vector (see page 108 libro Eugenio Oñate).
//! @param Li: Length of the element edge.
//! @param loadFactor Load factor.
//! @param p0 element Load vector.
void XC::Beam2dUniformLoad::addReactionsInBasicSystem(const std::vector<double> &Li,const double &loadFactor,FVector &p0) const
  {
    const double wa= WAxial()*loadFactor;  // Axial
    const double wy= WTrans()*loadFactor;  // Transverse

    //Forces over element.
    const double L= Li[0];
    const double V = 0.5*wy*L; // Shear force in the element ends.
    const double P= wa*L; //Axil.

    // Reactions in basic system
    p0[0]-= P; // Substracts the axial force. 
    p0[1]-= V; //Subtracts the reaction at the back end.
    p0[2]-= V; //Subtracts the reaction at the front end.
  }

//! @brief Returns the consistent load vector (see page 108 Eugenio Oñate book).
//! @param L Length of the element edge.
//! @param loadFactor Load factor.
//! @param q0 Consistent load vector.
//! @param release Moment release: 0=none, 1=I, 2=J, 3=I,J
void XC::Beam2dUniformLoad::addFixedEndForcesInBasicSystem(const std::vector<double> &Li,const double &loadFactor, FVector &q0, int releasey, int releasez) const
  {
    const double wy = WTrans()*loadFactor;  // Transverse
    const double wx = WAxial()*loadFactor;  // Axial (+ve from node I to J)

    const double L= Li[0];
    //Forces over element.
    const double Mz = wy*L*L/12.0; // z bending moment at the ends of the element.
    const double P = wx*L; //Axial force at the back end.

    // Fixed end forces in basic system
    q0[0]-= 0.5*P;
    if(releasez == 0)
      {
        q0[1]-= Mz;
        q0[2]+= Mz;
      }
    else if(releasez == 1)
      { q0[2]+= wy*L*L/8;  }
    else if(releasez == 2)
      { q0[1]-= wy*L*L/8; }
    else if(releasez == 3)
      {
        // Nothing to do
      }
    else
      std::cerr << getClassName() << "::" << __FUNCTION__
	        << " z-axis release value (" << releasez
	        << ") not valid. Must be between 0 and 3."
	        << std::endl;
    if(releasey!=0)
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< " y-axis release value (" << releasey
		<< ") not valid. Must be 0 for 2D loads."
		<< std::endl;
  }

void XC::Beam2dUniformLoad::addElasticDeformations(const double &L,const CrossSectionProperties2d &ctes_scc,const double &lpI,const double &lpJ,const double &loadFactor,FVector &v0)
  {
    // Length of elastic interior
    const double Le = L-lpI-lpJ;
    if(Le == 0.0)
      return;
    else
      {
        const double oneOverL = 1.0/L;
    
        const double wa= WAxial()*loadFactor;  // Axial
        const double wy= WTrans()*loadFactor;  // Transverse
    
        // Accumulate basic deformations due to uniform load on interior
        // Midpoint rule for axial
        v0[0] += wa*0.5*(L-lpI+lpJ)/(ctes_scc.EA())*Le;
    
        // Two point Gauss for bending ... will not be exact when
        // hinge lengths are not equal, but this is not a big deal!!!
        double x1 = lpI + 0.5*Le*(1.0-1.0/sqrt(3.0));
        double x2 = lpI + 0.5*Le*(1.0+1.0/sqrt(3.0));
    
        double M1 = 0.5*wy*x1*(x1-L);
        double M2 = 0.5*wy*x2*(x2-L);
    
        double b1, b2;
        double Le2EI = Le/(2*ctes_scc.EI());
        b1= x1*oneOverL;
        b2= x2*oneOverL;
        v0[2]+= Le2EI*(b1*M1+b2*M2);
   
        b1-= 1.0;
        b2-= 1.0;
        v0[1]+= Le2EI*(b1*M1+b2*M2);
      }
  }

//! @brief Sends object through the communicator argument.
int XC::Beam2dUniformLoad::sendSelf(Communicator &comm)
  {
    inicComm(6);
    int result= sendData(comm);
    const int dbTag= getDbTag();
    result+= comm.sendIdData(getDbTagData(),dbTag);
    if(result < 0)
      std::cerr << getClassName() << "::" << __FUNCTION__
	        << "; failed to send extra data\n";
    return result;
  }

//! @brief Receives object through the communicator argument.
int XC::Beam2dUniformLoad::recvSelf(const Communicator &comm)
  {
    inicComm(6);
    const int dataTag= getDbTag();
    int res= comm.receiveIdData(getDbTagData(),dataTag);
    if(res<0)
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; data could not be received\n" ;
    else
      res+= recvData(comm);
    return res;
  }
