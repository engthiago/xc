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
// $Date: 2006/01/17 21:24:00 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/forceBeamColumn/BeamIntegration.cpp,v $

#include "BeamIntegration.h"
#include "utility/matrix/Matrix.h"
#include "domain/mesh/element/utils/coordTransformation/CrdTransf.h"
#include "IntegrationPointsCoords.h"

#include "utility/matrix/ID.h"
#include <utility/actor/objectBroker/FEM_ObjectBroker.h>

XC::BeamIntegration::BeamIntegration(int classTag)
  : MovableObject(classTag), CommandEntity() {}

int XC::BeamIntegration::setParameter(const std::vector<std::string> &argv, Parameter &param)
  { return 0; }

int XC::BeamIntegration::updateParameter(int parameterID, Information &info)
  { return 0; }

int XC::BeamIntegration::activateParameter(int parameterID)
  { return 0; }

void XC::BeamIntegration::getLocationsDeriv(int nIP, double L, double dLdh,double *dptsdh)
  {
    for (int i = 0; i < nIP; i++)
      dptsdh[i] = 0.0;
  }

void XC::BeamIntegration::getWeightsDeriv(int nIP, double L, double dLdh,double *dwtsdh)
  {
    for (int i = 0; i < nIP; i++)
      dwtsdh[i] = 0.0;
  }

//! @brief Returns the weights (between 0 and 1).
const XC::Vector &XC::BeamIntegration::getIntegrPointWeights(int numSections, double L) const
  {
    static Vector retval;
    std::vector<double> wi(numSections);
    getSectionWeights(numSections,L,&wi[0]);
    retval= Vector(&wi[0],numSections);
    return retval;
  }

//! @brief Return the weights corresponding to each section.
boost::python::list XC::BeamIntegration::getSectionWeightsPy(int numSections, double L) const
  {
    std::vector<double> wi(numSections);
    this->getSectionWeights(numSections,L,&wi[0]);
    boost::python::list retval;
    for(std::vector<double>::const_iterator i= wi.begin(); i!= wi.end(); i++)
      { retval.append(*i); }
    return retval;
  }

//! @brief Returns the location of the sections along the element.
boost::python::list XC::BeamIntegration::getSectionLocationsPy(int numSections, double L) const
  {
    std::vector<double> xi(numSections);
    this->getSectionLocations(numSections,L,&xi[0]);
    boost::python::list retval;
    for(std::vector<double>::const_iterator i= xi.begin(); i!= xi.end(); i++)
      { retval.append(*i); }
    return retval;
  }

//! @brief Returns the normalized coordinates (entre 0 y 1).
const XC::Matrix &XC::BeamIntegration::getIntegrPointCoords(int numSections, double L) const
  {
    static Matrix retval;
    std::vector<double> xi(numSections);
    getSectionLocations(numSections,L,&xi[0]);
    retval= Matrix(&xi[0],numSections,1);
    return retval;
  }

//! @brief Returns the natural coordinates (between -1 and 1) from the
//! normalized ones.
const XC::Matrix &XC::BeamIntegration::getIntegrPointNaturalCoords(int numSections, double L) const
  {
    static Matrix retval;
    retval= getIntegrPointCoords(numSections,L); //normalized coordinates.
    for(int i = 0;i<numSections; i++)
      retval(i,1)= 2.0*retval(i,1) - 1.0;
    return retval;
  }

//! @brief Returns the local coordinates (between 0 and L) from the
//! normalized ones.
const XC::Matrix &XC::BeamIntegration::getIntegrPointLocalCoords(int numSections, double L) const
  {
    static Matrix retval;
    retval= getIntegrPointCoords(numSections,L); //normalized coordinates.
    for(int i = 0;i<numSections; i++)
      retval(i,1)*= L;
    return retval;
  }

//! @brief Returns a matrix with the natural coordinates for each integration point.
const XC::Matrix &XC::BeamIntegration::getIntegrPointNaturalCoords(int nIP,const CrdTransf &trf) const
  { return getIntegrPointNaturalCoords(nIP,trf.getInitialLength()); }

//! @brief Returns a matrix with the normalized coordinates for each integration point.
const XC::Matrix &XC::BeamIntegration::getIntegrPointNormalizedCoords(int nIP,const CrdTransf &trf) const
  { return getIntegrPointCoords(nIP,trf.getInitialLength()); }

//! @brief Returns a matrix with the local coordinates for each integration point.
const XC::Matrix &XC::BeamIntegration::getIntegrPointLocalCoords(int nIP,const CrdTransf &trf) const
  {
    const Matrix tmp= getIntegrPointLocalCoords(nIP,trf.getInitialLength());
    static Matrix retval;
    retval.resize(nIP,3);
    retval.Zero();
    for(int i= 0;i<nIP;i++)
      retval(i,1)= tmp(i,1);
    return retval;
  }

//! @brief Returns a matrix with the global coordinates for each integration point.
const XC::Matrix &XC::BeamIntegration::getIntegrPointGlobalCoords(int nIP,const CrdTransf &trf) const
  { return trf.getPointsGlobalCoordFromLocal(getIntegrPointLocalCoords(nIP,trf)); }

//! @brief Returns the values of the expression being passed as parameter on each integration point.
const XC::Vector &XC::BeamIntegration::evalInIntegrPoints(const ExprAlgebra &expr,int nIP,const CrdTransf &trf) const
  {
    const IntegrationPointsCoords ipCoords(*this,nIP,trf);
    return ipCoords.eval(expr);
  }

//! @brief Returns the integral of the expression.
double XC::BeamIntegration::getIntegral(const ExprAlgebra &expr,int nIP,const CrdTransf &trf) const
  {
    const double L= trf.getInitialLength();
    std::vector<double> wt(nIP);
    getSectionWeights(nIP,L,&wt[0]);
    const Vector f= evalInIntegrPoints(expr,nIP,trf);
    double retval= 0;
    for(int i= 0;i<nIP;i++)
      retval+= wt[i]*f[i];
    retval*= L;
    return retval;
  }

//! @brief Send a pointer to material through the communicator argument.
//! 
//! @param posClassTag: index of the class tags in the data vector
//! @param posDbTag: index of the dbTag in the data vector
int XC::sendBeamIntegrationPtr(BeamIntegration *ptr,int posClassTag, int posDbTag,DbTagData &dt,Communicator &comm)
  {
    int res= 0;
    if(ptr)
      {
        dt.setDbTagDataPos(posClassTag,ptr->getClassTag());
        res= comm.sendMovable(*ptr,dt,CommMetaData(posDbTag));
      }
    if(res < 0)
      std::cerr << __FUNCTION__ << "; WARNING "
                << "failed to send material.\n";
    return res;
  }

//! @brief Receive a pointer to beam integration through the communicator argument.
//! @param posClassTag: index of the class tags in the data vector
//! @param posDbTag: index of the dbTag in the data vector
XC::BeamIntegration *XC::receiveBeamIntegrationPtr(BeamIntegration* ptr,int posClassTag, int posDbTag,DbTagData &dt,const Communicator &comm)
  {
    BeamIntegration *retval= nullptr;
    const int matClass= dt.getDbTagDataPos(posClassTag);
    if(ptr && (ptr->getClassTag() == matClass))
      retval= ptr;
    else 
      {

        // check if we have a material object already & if we do if of right type
        // if old one .. delete it
        if(ptr)
	  {
            delete ptr;
	    ptr= nullptr;
	  }
        // create a new_ material object
        retval= comm.getNewBeamIntegration(matClass);
      }
    if(retval)
      {
        int res= comm.receiveMovable(*retval,dt,CommMetaData(posDbTag));
        if(res<0)
          std::cerr << __FUNCTION__ << "; WARNING " 
                    << "failed to receive material.\n";
      }
    else
      std::cerr << __FUNCTION__ << "; WARNING "
                << " failed to get a blank material of type: "
                << matClass << std::endl; 
    return retval;
  }
