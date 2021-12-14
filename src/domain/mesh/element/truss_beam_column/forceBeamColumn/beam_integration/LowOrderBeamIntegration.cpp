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
// $Date: 2008-12-03 23:42:33 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/forceBeamColumn/LowOrderBeamIntegration.cpp,v $

#include "LowOrderBeamIntegration.h"

#include "utility/matrix/ID.h"
#include "utility/matrix/Matrix.h"
#include "utility/matrix/Vector.h"
#include "domain/mesh/element/utils/Information.h"
#include "domain/component/Parameter.h"

XC::LowOrderBeamIntegration::LowOrderBeamIntegration(int nIP, const Vector &pt, int nc, const Vector &wc)
  : ParameterIDBeamIntegration(BEAM_INTEGRATION_TAG_LowOrder,pt,wc), Nc(nc), computed(false)
  {

    const int nf = nIP-nc;

    if(nf > 0)
      {
        Vector R(nf);
        for(int i = 0; i < nf; i++)
          {
            double sum = 0.0;
            for(int j = 0; j < nc; j++)
	      sum += pow(pts(j),i)*wc(j);
            R(i) = 1.0/(i+1) - sum;
          }
    
        Matrix J(nf,nf);
        for(int i = 0; i < nf; i++)
          for(int j = 0; j < nf; j++)
	    J(i,j) = pow(pts(nc+j),i);
    
        Vector wf(nf);
    
        J.Solve(R, wf);
    
        for(int i = 0; i < nf; i++)
          wts(nc+i) = wf(i);
    
        for(int i = 0; i < nc; i++)
          wts(i) = wc(i);
      }
    else
      wts = wc;

    computed = true;
  }

XC::LowOrderBeamIntegration::LowOrderBeamIntegration()
  : ParameterIDBeamIntegration(BEAM_INTEGRATION_TAG_LowOrder)
  {}

void XC::LowOrderBeamIntegration::getSectionLocations(int numSections, double L, double *xi)
  {
    const int nIP = pts.Size();

    int i;
    for(i = 0; i < nIP; i++)
      xi[i] = pts(i);
    for( ; i < numSections; i++)
      xi[i] = 0.0;
  }

void XC::LowOrderBeamIntegration::getSectionWeights(int numSections, double L, double *wt)
  {
    const int nIP = wts.Size();

    const int Nf = nIP-Nc;

    if(!computed && Nf > 0)
      {
        Vector R(Nf);
        for(int i = 0; i < Nf; i++)
          {
            double sum = 0.0;
            for(int j = 0; j < Nc; j++)
	      sum += pow(pts(j),i)*wts(j);
            R(i) = 1.0/(i+1) - sum;
          }
    
        Matrix J(Nf,Nf);
        for(int i = 0; i < Nf; i++)
          for(int j = 0; j < Nf; j++)
	    J(i,j) = pow(pts(Nc+j),i);
    
        Vector wf(Nf);
    
        J.Solve(R, wf);
    
        for(int i = 0; i < Nf; i++)
          wts(Nc+i) = wf(i);
      }

    int i;
    for(i = 0; i < nIP; i++)
      wt[i] = wts(i);
    for( ; i < numSections; i++)
      wt[i] = 1.0;
  }

XC::BeamIntegration *XC::LowOrderBeamIntegration::getCopy(void) const
  { return new LowOrderBeamIntegration(*this); }

//! @brief Send object members through the communicator argument.
int XC::LowOrderBeamIntegration::sendData(Communicator &comm)
  {
    int res= ParameterIDBeamIntegration::sendData(comm);
    res+= comm.sendInt(Nc,getDbTagData(),CommMetaData(6));
    res+= comm.sendBool(computed,getDbTagData(),CommMetaData(7));
    return res;
  }

//! @brief Receives object members through the communicator argument.
int XC::LowOrderBeamIntegration::recvData(const Communicator &comm)
  {
    int res= ParameterIDBeamIntegration::recvData(comm);
    res+= comm.receiveInt(Nc,getDbTagData(),CommMetaData(6));
    res+= comm.receiveBool(computed,getDbTagData(),CommMetaData(7));
    return res;
  }

//! @brief Sends object through the communicator argument.
int XC::LowOrderBeamIntegration::sendSelf(Communicator &comm)
  {
    setDbTag(comm);
    const int dataTag= getDbTag();
    inicComm(8);
    int res= sendData(comm);

    res+= comm.sendIdData(getDbTagData(),dataTag);
    if(res < 0)
      std::cerr << getClassName() << "sendSelf() - failed to send data\n";
    return res;
  }

//! @brief Receives object through the communicator argument.
int XC::LowOrderBeamIntegration::recvSelf(const Communicator &comm)
  {
    inicComm(8);
    const int dataTag= getDbTag();
    int res= comm.receiveIdData(getDbTagData(),dataTag);

    if(res<0)
      std::cerr << getClassName() << "::recvSelf - failed to receive ids.\n";
    else
      {
        //setTag(getDbTagDataPos(0));
        res+= recvData(comm);
        if(res<0)
          std::cerr << getClassName() << "::recvSelf - failed to receive data.\n";
      }
    return res;
  }

//! @brief Set parameter value.
int XC::LowOrderBeamIntegration::setParameter(const std::vector<std::string> &argv, Parameter &param)
  {
    const int argc= argv.size();
    if(argc < 2)
      return -1;

    const int point = atoi(argv[1]);
    if(point < 1)
      return -1;

    const int N = pts.Size();
    const int Nf = N-Nc;

    if( (argv[0]=="xf") && point <= Nf)
      {
	param.setValue(pts(Nc+(point-1)));
        return param.addObject(point, this);
      }
    else if( (argv[0] == "xc") && point <= Nc)
      {
	param.setValue(pts(point-1));
        return param.addObject(10+point, this);
      }
    else if( (argv[0] == "wc") && point <= Nc)
      {
	param.setValue(wts(point-1));
        return param.addObject(20+point, this);
      }
    else
      return -1;
  }

//! @brief Update parameter value.
int XC::LowOrderBeamIntegration::updateParameter(int parameterID, Information &info)
  {
    if(parameterID <= 10)
      { // xf
        pts(Nc+(parameterID-1)) = info.theDouble;
        computed = false;
       return 0;
      }
    else if(parameterID <= 20)
      { // xc
        pts(parameterID-10-1) = info.theDouble;
        computed = false;
        return 0;
      }
    else if(parameterID <= 30)
      { // wc
        wts(parameterID-20-1) = info.theDouble;
        computed = false;
        return 0;
      }
    else
      return -1;
  }

void XC::LowOrderBeamIntegration::Print(std::ostream &s, int flag) const
  {
    s << "LowOrder" << std::endl;
    s << " Points: " << pts;
    s << " Weights: " << wts;
    double sum = 0.0;
    int N = wts.Size();
    for(int i = 0; i < N; i++)
      sum += fabs(wts(i));
    s << " Condition Number: " << sum << std::endl;
  }

void XC::LowOrderBeamIntegration::getLocationsDeriv(int numSections, double L, double dLdh, double *dptsdh)
  {
    for(int i = 0; i < numSections; i++)
      dptsdh[i] = 0.0;

    if(parameterID == 0)
      return;

    if(parameterID < 10) // xf
      dptsdh[Nc+(parameterID-1)] = 1.0;
    else if(parameterID < 20) // xc
      dptsdh[parameterID-10-1] = 1.0;

    //for(int i = 0; i < numSections; i++)
    //  std::cerr << dptsdh[i] << ' ';
    //std::cerr << std::endl;
    return;
  }

void XC::LowOrderBeamIntegration::getWeightsDeriv(int numSections, double L, double dLdh, double *dwtsdh)
  {
    for(int i = 0; i < numSections; i++)
      dwtsdh[i] = 0.0;

    if(parameterID == 0)
      return;

    double dxcdh[10];
    double dxfdh[10];
    for(int i = 0; i < 10; i++)
      {
        dxcdh[i] = 0.0;
        dxfdh[i] = 0.0;
      }
 
    if(parameterID < 10) // xf
      dxfdh[parameterID-1] = 1.0;
    else if(parameterID < 20) // xc
      dxcdh[parameterID-10-1] = 1.0;
    else if(parameterID < 30) // wc
      dwtsdh[parameterID-20-1] = 1.0;

    int N = pts.Size();
    int Nf = N-Nc;

    if(Nf > 0)
      {

        Vector R(Nf);

        double sum = 0.0;
        for(int j = 0; j < Nc; j++)
          sum += dwtsdh[j];
        R(0) = -sum;

        for(int i = 1; i < Nf; i++)
          {
            sum = 0.0;
            for(int j = 0; j < Nf; j++)
              sum+= i*pow(pts(Nc+j),i-1)*dxfdh[j]*wts(Nc+j);
            for(int j = 0; j < Nc; j++)
	      sum+= i*pow(pts(j),i-1)*dxcdh[j]*wts(j);
            for(int j = 0; j < Nc; j++)
	      sum+= pow(pts(j),i)*dwtsdh[j];
            R(i) = -sum;
          }

        Matrix J(Nf,Nf);
        for(int i = 0; i < Nf; i++)
          for(int j = 0; j < Nf; j++)
	    J(i,j) = pow(pts(Nc+j),i);
    
        Vector dwfdh(Nf);

        J.Solve(R,dwfdh);

        for(int i = 0; i < Nf; i++)
          dwtsdh[Nc+i] = dwfdh(i);    
      }

    //for(int i = 0; i < numSections; i++)
    //  std::cerr << dwtsdh[i] << ' ';
    //std::cerr << std::endl;

    return;
  }
