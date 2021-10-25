//----------------------------------------------------------------------------
//  XC program; finite element analysis code
//  for structural analysis and design.
//
//  Copyright (C)  Luis Claudio Pérez Tato
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
**   Frank McKenna(fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */
                                                                        
// $Revision: 1.5 $
// $Date: 2003/02/14 23:01:26 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/nD/PlateFiberMaterial.cpp,v $

//
// Ed "C++" Love
//
// Generic Plate fiber material
//


#include <material/nD/nd_adaptor/PlateFiberMaterial.h>
#include "utility/matrix/Vector.h"
#include "utility/matrix/Matrix.h"
#include <utility/matrix/ID.h> 
#include "material/nD/NDMaterialType.h"

//static vector and matrices
XC::Vector  XC::PlateFiberMaterial::stress(5);
XC::Matrix  XC::PlateFiberMaterial::tangent(5,5);

//! @brief Default constructor.
XC::PlateFiberMaterial::PlateFiberMaterial(int tag)
  : XC::NDAdaptorMaterial(ND_TAG_PlateFiberMaterial,tag,5)
  {}

//! @brief Full constructor
XC::PlateFiberMaterial::PlateFiberMaterial(int tag, NDMaterial &the3DMaterial) :
  XC::NDAdaptorMaterial(ND_TAG_PlateFiberMaterial,tag,the3DMaterial,5)
  {}

//make a clone of this material
XC::NDMaterial *XC::PlateFiberMaterial::getCopy(void) const 
  { return new PlateFiberMaterial(*this); }


//make a clone of this material
XC::NDMaterial *XC::PlateFiberMaterial::getCopy(const std::string &type) const
  { return this->getCopy(); }


//send back order of strain in vector form
int XC::PlateFiberMaterial::getOrder() const
  { return 5; }


const std::string &XC::PlateFiberMaterial::getType() const 
  { return strTypePlateFiber; }



//receive the strain
int XC::PlateFiberMaterial::setTrialStrain(const XC::Vector &strainFromElement)
{
  static const double tolerance = 1.0e-08;

  this->strain(0) = strainFromElement(0); //11
  this->strain(1) = strainFromElement(1); //22
  this->strain(2) = strainFromElement(2); //12
  this->strain(3) = strainFromElement(3); //23
  this->strain(4) = strainFromElement(4); //31

  double norm;
  static Vector outOfPlaneStress(1);
  static Vector strainIncrement(1);
  static Vector threeDstress(6);
  static Vector threeDstrain(6);
  static Matrix threeDtangent(6,6);
  static Vector threeDstressCopy(6); 

  static Matrix threeDtangentCopy(6,6);
  static Matrix dd22(1,1);

  int i, j;
  int ii, jj;

  int count = 0;
  //newton loop to solve for out-of-plane strains
  do {

    //set three dimensional strain
    threeDstrain(0) = this->strain(0);
    threeDstrain(1) = this->strain(1);

    threeDstrain(2) = this->Tstrain22;
  
    threeDstrain(3) = this->strain(2); 
    threeDstrain(4) = this->strain(3);
    threeDstrain(5) = this->strain(4);

    if(theMaterial->setTrialStrain(threeDstrain) < 0) {
      std::cerr << "PlateFiberMaterial::setTrialStrain - material failed in setTrialStrain() with strain " << threeDstrain;
      return -1;
    }

    //three dimensional stress
    threeDstress = theMaterial->getStress();

    //three dimensional tangent 
    threeDtangent = theMaterial->getTangent();

    //NDmaterial strain order          = 11, 22, 33, 12, 23, 31 
    //PlateFiberMaterial strain order =  11, 22, 12, 23, 31, 33 

    //swap matrix indices to sort out-of-plane components 
    for(i=0; i<6; i++) {

      ii = this->indexMap(i);

      threeDstressCopy(ii) = threeDstress(i);

      for(j=0; j<6; j++) {

    jj = this->indexMap(j);

    threeDtangentCopy(ii,jj) = threeDtangent(i,j);

      }//end for j

    }//end for i


    //partitioned stresses and tangent
    
    outOfPlaneStress(0) = threeDstress(2);

    dd22(0,0) = threeDtangentCopy(5,5);

    //set norm
    norm = outOfPlaneStress.Norm();

    //int Solve(const XC::Vector &V, XC::Vector &res) const;
    //int Solve(const XC::Matrix &M, XC::Matrix &res) const;
    //condensation 
    dd22.Solve(outOfPlaneStress, strainIncrement);

    //update out of plane strains
    this->Tstrain22 -= strainIncrement(0);

    count++;
  } while(norm > tolerance && count < 10);

  return 0;
}


//send back the stress 
const XC::Vector &XC::PlateFiberMaterial::getStress(void) const
{
  const XC::Vector &threeDstress = theMaterial->getStress();
  static Vector threeDstressCopy(6);

  //swap matrix indices to sort out-of-plane components 
  int i, ii;
  for(i=0; i<6; i++) {
    
    ii = this->indexMap(i);
    
    threeDstressCopy(ii) = threeDstress(i);
    
  }//end for i

  for(i=0; i<5; i++) 
    this->stress(i)     = threeDstressCopy(i);

  return this->stress;
}

//! @brief Return the tangent stiffness matrix.
const XC::Matrix  &XC::PlateFiberMaterial::getTangent(void) const
  {
    static Matrix dd11(5,5);
    static Matrix dd12(5,1);
    static Matrix dd21(1,5);
    static Matrix dd22(1,1);
    static Matrix dd22invdd21(1,5);

    static Matrix threeDtangentCopy(6,6);
    const Matrix &threeDtangent = theMaterial->getTangent();

    //swap matrix indices to sort out-of-plane components 
    int i,j, ii, jj;
    for(i=0; i<6; i++) {

      ii = this->indexMap(i);

      for(j=0; j<6; j++) {

	jj = this->indexMap(j);

	threeDtangentCopy(ii,jj) = threeDtangent(i,j);

      }//end for j

    }//end for i

    dd22(0,0) = threeDtangentCopy(5,5);

    for(i=0; i<5; i++) {

      dd12(i,0) = threeDtangentCopy(i,5);
      dd21(0,i) = threeDtangentCopy(5,i);

      for(int j=0; j<5; j++) 
	dd11(i,j) = threeDtangentCopy(i,j);

    }//end for i

    //int Solve(const XC::Vector &V, XC::Vector &res) const;
    //int Solve(const XC::Matrix &M, XC::Matrix &res) const;
    //condensation 
    dd22.Solve(dd21, dd22invdd21);
    this->tangent   = dd11; 
    this->tangent  -= (dd12*dd22invdd21);

    return this->tangent;
  }

//! @brief Return the material initial stiffness.
const XC::Matrix &XC::PlateFiberMaterial::getInitialTangent(void) const
  {
    std::cerr << getClassName() << "::" << __FUNCTION__
              << "; not yet implemented.\n";
    return this->getTangent();
  }


int XC::PlateFiberMaterial::indexMap(int i) const
  {
    int retval= 1;
    switch(i+1) //add 1 for standard vector indices
      { 
      case 1 :
	retval = 1; 
	break;

      case 2 :
	retval = 2;
	break;

      case 3 :
	retval = 6;
	break;

      case 4 :
	retval = 3;
	break;

      case 5 :
	retval = 4;
	break;

      case 6 :
	retval = 5;
	break;

      default :
	retval = 1;
	break;
      } //end switch
    retval--;
    return retval;
  }



//print out data
void XC::PlateFiberMaterial::Print(std::ostream &s, int flag) const
{
  s << "General Plate XC::Fiber XC::Material \n";
  s << " Tag: " << this->getTag() << "\n"; 
  s << "using the 3D material : \n";

  theMaterial->Print(s, flag);

  return;
}


//! @brief Sends object through the communicator argument.
int XC::PlateFiberMaterial::sendSelf(Communicator &comm) 
  {
    setDbTag(comm);
    const int dataTag= getDbTag();
    inicComm(6);
    int res= sendData(comm);

    res+= comm.sendIdData(getDbTagData(),dataTag);
    if(res < 0)
      std::cerr << getClassName() << "sendSelf() - failed to send data\n";
    return res;
  }

//! @brief Receives object through the communicator argument.
int XC::PlateFiberMaterial::recvSelf(const Communicator &comm)
  {
    inicComm(6);
    const int dataTag= getDbTag();
    int res= comm.receiveIdData(getDbTagData(),dataTag);

    if(res<0)
      std::cerr << getClassName() << "::recvSelf - failed to receive ids.\n";
    else
      {
        setTag(getDbTagDataPos(0));
        res+= recvData(comm);
        if(res<0)
          std::cerr << getClassName() << "::recvSelf - failed to receive data.\n";
      }
    return res;
  }
 


