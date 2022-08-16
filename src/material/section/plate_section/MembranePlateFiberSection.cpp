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
                                                                        
// $Revision: 1.6 $
// $Date: 2003/02/14 23:01:34 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/section/MembranePlateFiberSection.cpp,v $

// Ed "C++" Love
//
//  Generic Plate Section with membrane
//


#include <material/section/plate_section/MembranePlateFiberSection.h>
#include <material/nD/NDMaterial.h>
#include "material/section/ResponseId.h"
#include "domain/mesh/element/utils/Information.h"

const double XC::MembranePlateFiberSection::root56= sqrt(5.0/6.0); //shear correction

//static vector and matrices
XC::Vector XC::MembranePlateFiberSection::stressResultant(XC::MembranePlateFiberSection::order);
XC::Matrix XC::MembranePlateFiberSection::tangent(XC::MembranePlateFiberSection::order, XC::MembranePlateFiberSection::order);

const std::string XC::MembranePlateFiberSection::lobattoLabel= "Lobatto";
const double XC::MembranePlateFiberSection::sgLobatto[] = { -1, 
                                                  -0.65465367, 
                                                   0, 
                                                   0.65465367, 
                                                   1 };
 
const double XC::MembranePlateFiberSection::wgLobatto[] = { 0.1, 
                                                  0.5444444444, 
                                                  0.7111111111, 
                                                  0.5444444444, 
                                                  0.1  };
const std::string XC::MembranePlateFiberSection::gaussLabel= "Gauss";
const double XC::MembranePlateFiberSection::sgGauss[] = { -0.906179845938664,
						      -0.538469310105683,
						      0.0,
						      0.538469310105683,
						      0.906179845938664 };

const double XC::MembranePlateFiberSection::wgGauss[] = { 0.236926885056189,
						      0.478628670499366,
						      0.568888888888889,
						      0.478628670499366,
						      0.236926885056189};

/*      from Ham-O
        case 5:
         xi(0,0) = -1.;
         xi(1,0) = -0.65465367;
         xi(2,0) =  0.;
         xi(3,0) =  0.65465367;
         xi(4,0) =  1.;
      
         w(0) =  0.1;
         w(1) =  0.5444444444;
         w(2) =  0.7111111111;
         w(3) =  0.5444444444;
         w(4) =  0.1;
      break;
*/

//! @brief Initializes material pointers.
void XC::MembranePlateFiberSection::init(void)
  { 
    for(int i= 0;i<numFibers;i++ )
      theFibers[i]= nullptr;
  }

//! @brief Allocates material pointers.
void XC::MembranePlateFiberSection::alloc(const NDMaterial &templ)
  {
    free();
    for(int i= 0;i<numFibers;i++ )
      theFibers[i]= templ.getCopy("PlateFiber");
  }

//! @brief Copy the material pointers.
void XC::MembranePlateFiberSection::copy_fibers(const MembranePlateFiberSection &other)
  
  {
    free();
    for(int i= 0;i<numFibers;i++ )
      {
	const NDMaterial *ndMatPtr= other.theFibers[i];
	if(ndMatPtr)
          theFibers[i]= ndMatPtr->getCopy();
      }
  }

//! @brief Releases material pointers.
void XC::MembranePlateFiberSection::free(void)
  { 
    for(int i= 0;i<numFibers;i++ )
      if(theFibers[i])
	{
	  delete theFibers[i];
          theFibers[i]= nullptr;
	}
  }

//! @brief Default constructor.
XC::MembranePlateFiberSection::MembranePlateFiberSection(int tag)
  : PlateBase( tag, SEC_TAG_MembranePlateFiberSection ),
    strainResultant(order), initialStrain(order), integrationType(0)
  { init(); }

//! @brief full constructor
XC::MembranePlateFiberSection::MembranePlateFiberSection(int tag, double thickness, NDMaterial &Afiber, const std::string &integrType)
  : PlateBase( tag, SEC_TAG_MembranePlateFiberSection,thickness, Afiber.getRho()),
    strainResultant(order), initialStrain(order), integrationType(0)
  {
    alloc(Afiber);
    setIntegrationType(integrType);
  }

//! @brief Copy constructor.
XC::MembranePlateFiberSection::MembranePlateFiberSection(const MembranePlateFiberSection &other)
  : PlateBase(other),
    strainResultant(other.strainResultant), initialStrain(other.initialStrain),
    integrationType(other.integrationType)
  {
    init();
    copy_fibers(other);
  }

//! @brief Assignment operator.
XC::MembranePlateFiberSection &XC::MembranePlateFiberSection::operator=(const MembranePlateFiberSection &other)
  {
    PlateBase::operator=(other);
    strainResultant= other.strainResultant;
    initialStrain= other.initialStrain;
    integrationType= other.integrationType;
    copy_fibers(other);
    return *this;
  }


//! @brief Destructor
XC::MembranePlateFiberSection::~MembranePlateFiberSection(void) 
  { free(); }


//! @brief make a clone of this material
XC::SectionForceDeformation  *XC::MembranePlateFiberSection::getCopy(void) const 
  { return new MembranePlateFiberSection(*this); }

//! @brief Return the z coordinate for each fiber (layer if you prefer).
std::vector<double> XC::MembranePlateFiberSection::getFiberZs(void) const
  {
    std::vector<double> retval(numFibers,0.0);
    const double *sg= (integrationType == 0) ? sgLobatto : sgGauss;
      sg= sgGauss;
    for(int i = 0; i < numFibers; i++ )
      { retval[i]= ( 0.5*h ) * sg[i]; }
    return retval;
  }

//! @brief Return the weight coordinate for each fiber (layer if you prefer).
std::vector<double> XC::MembranePlateFiberSection::getFiberWeights(void) const
  {
    std::vector<double> retval(numFibers,0.0);
    const double *wg= (integrationType == 0) ? wgLobatto : wgGauss;
    for(int i = 0; i < numFibers; i++)
      { retval[i]= (0.5*h) * wg[i];}
    return retval;
  }
  
//! @brief Return the weight coordinate for each fiber (layer if you prefer).
std::vector<std::pair<double, double> > XC::MembranePlateFiberSection::getFiberZsAndWeights(void) const
  {
    std::vector<std::pair<double, double> > retval(numFibers,std::pair<double,double>(0.0,0.0));
    const double *sg= (integrationType == 0) ? sgLobatto : sgGauss;
    const double *wg= (integrationType == 0) ? wgLobatto : wgGauss;
    for(int i = 0; i < numFibers; i++)
      {
	retval[i].first= (0.5*h) * sg[i];
	retval[i].second= (0.5*h) * wg[i];
      }
    return retval;
  }


//! @brief send back order of strainResultant in vector form
int XC::MembranePlateFiberSection::getOrder(void) const
  { return order; }


//! @brief Returns the labels of the DOFs for which the element
//! adds (assembles) stiffness.
const XC::ResponseId &XC::MembranePlateFiberSection::getType(void) const 
  { return RespShellMat; }



//! @brief Swap history variables.
int XC::MembranePlateFiberSection::commitState(void) 
  {
    int success = 0;
    for(int i= 0; i < numFibers; i++ )
      success += theFibers[i]->commitState( );
    return success;
  }

//! @brief Revert to last committed state.
int XC::MembranePlateFiberSection::revertToLastCommit(void)
  {
    int success = 0;
    for(int i= 0; i < numFibers; i++ )
      success+= theFibers[i]->revertToLastCommit();
    return success;
  }

//! @brief Revert to start.
int XC::MembranePlateFiberSection::revertToStart(void)
  {
    int success = 0;
    for(int i= 0;i<numFibers;i++ )
      success += theFibers[i]->revertToStart( );
    return success;
  }

void XC::MembranePlateFiberSection::setIntegrationType(const std::string &integrType)
  {
    if(integrType== lobattoLabel)
      integrationType= 0;
    else if(integrType== gaussLabel)
      integrationType= 0;
    else
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
	          << "; integration type: " << integrType
	   	  << " unknown. Assuming Lobatto." << std::endl;
	integrationType= 0;
      }
  }

const std::string &XC::MembranePlateFiberSection::getIntegrationType(void) const
  {
    if(integrationType == 0)
      return lobattoLabel;
    else if(integrationType == 1)
      return gaussLabel;
    else
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
	          << "; unrecognized integration type value: " << integrationType
	   	  << ". Assuming Lobatto." << std::endl;
	return lobattoLabel;
      }
  }
  

//! @brief Return mass per unit area.
double XC::MembranePlateFiberSection::getRho(void) const
  {
    double rhoH = 0.0;
    const std::vector<double> weights= getFiberWeights();
    for(int i = 0; i < numFibers; i++ )
      { rhoH+= ( theFibers[i]->getRho() ) * weights[i]; }
    return rhoH;
  }

//! @brief Asigns density per unit area
void XC::MembranePlateFiberSection::setRho(const double &r)
  {
    const std::vector<double> weights= getFiberWeights();
    for(int i = 0; i < numFibers; i++ )
      { theFibers[i]->setRho(r/weights[i]); }
  }

//! @brief Return mass per unit area.
double XC::MembranePlateFiberSection::getArealRho(void) const
  { return getRho(); }
    
//! @brief Return mass per unit area.
void XC::MembranePlateFiberSection::setArealRho(const double &r)
  { setRho(r); }

//! @brief Set initial values for deformation.
int XC::MembranePlateFiberSection::setInitialSectionDeformation(const Vector &initialStrain_from_element)
  {
    this->initialStrain = initialStrain_from_element;

    static Vector strain(numFibers);
    int success= 0;
    const std::vector<double> fiberZ= getFiberZs();
    for(int i = 0; i < numFibers; i++ )
      {
        const double &z= fiberZ[i];
        strain(0)= initialStrain(0)-z*initialStrain(3);
        strain(1)= initialStrain(1)-z*initialStrain(4);
        strain(2)= initialStrain(2)-z*initialStrain(5);
        strain(3)= root56*initialStrain(6);
        strain(4)= root56*initialStrain(7);
        success+= theFibers[i]->setInitialStrain(strain);
      } //end for i
    return success;
  }

//! @brief Zero initial deformation.
void XC::MembranePlateFiberSection::zeroInitialSectionDeformation(void)
  {
    std::cerr << getClassName() << "::" << __FUNCTION__
	      << "; not implemented." << std::endl;
  }

//! @brief Return initial deformation.
const XC::Vector &XC::MembranePlateFiberSection::getInitialSectionDeformation(void) const
  {
    return initialStrain;
  }

//! @brief Set trial deformation. 
int XC::MembranePlateFiberSection::setTrialSectionDeformation(const Vector &strainResultant_from_element)
  {
    this->strainResultant = strainResultant_from_element;

    static Vector strain(numFibers);
    int success= 0;
    const std::vector<double> fiberZ= getFiberZs();
    for(int i = 0; i < numFibers; i++ )
      {
        const double &z= fiberZ[i];
        strain(0)=  strainResultant(0)  - z*strainResultant(3);
        strain(1)=  strainResultant(1)  - z*strainResultant(4);
        strain(2)=  strainResultant(2)  - z*strainResultant(5);
        strain(3)=  root56*strainResultant(6);
        strain(4)=  root56*strainResultant(7);
        success+= theFibers[i]->setTrialStrain(strain);
      } //end for i
    return success;
  }


//! @brief Returns section deformation.
const XC::Vector &XC::MembranePlateFiberSection::getSectionDeformation(void) const
  {
    static Vector retval;
    retval= strainResultant-initialStrain;
    return retval;
  }

//! @brief Return stress resultant.
const XC::Vector &XC::MembranePlateFiberSection::getStressResultant(void) const
  {
    static Vector stress(numFibers);
    stressResultant.Zero( );

    const std::vector< std::pair<double, double> > zsAndWeights= getFiberZsAndWeights();
    for(int i = 0; i < numFibers; i++ )
      {
        const double &z= zsAndWeights[i].first;
        const double &weight= zsAndWeights[i].second;
        stress= theFibers[i]->getStress();
        //membrane
        stressResultant(0)+= stress(0)*weight;
        stressResultant(1)+= stress(1)*weight;
        stressResultant(2)+= stress(2)*weight;
        //bending moments
        stressResultant(3)+= ( z*stress(0) ) * weight;
        stressResultant(4)+= ( z*stress(1) ) * weight;
        stressResultant(5)+= ( z*stress(2) ) * weight;
        //shear
        stressResultant(6)+= stress(3)*weight;
        stressResultant(7)+= stress(4)*weight;
      } //end for i
    //modify shear 
    stressResultant(6)*= root56;  
    stressResultant(7)*= root56;
    return this->stressResultant;
  }


//! @brief Return the tangent stiffness matrix.
const XC::Matrix &XC::MembranePlateFiberSection::getSectionTangent(void) const
  {
    static Matrix dd(numFibers,numFibers);
    static Matrix Aeps(numFibers,order);
    static Matrix Asig(order,numFibers);

    tangent.Zero( );

    const std::vector< std::pair<double, double> > zsAndWeights= getFiberZsAndWeights();
    for(int i = 0; i < numFibers; i++)
      {
        const double &z= zsAndWeights[i].first;
        const double &weight= zsAndWeights[i].second;
/*      //compute Aeps

      Aeps.Zero( );

      Aeps(0,0)= 1.0;
      Aeps(0,3)= -z;

      Aeps(1,1)= 1.0;
      Aeps(1,4)= -z;

      Aeps(2,2)= 1.0;
      Aeps(2,5)= -z;

      Aeps(3,6)= root56;
      Aeps(4,7)= root56;

      //compute Asig

      Asig.Zero( );

      Asig(0,0)= 1.0;
      Asig(3,0)= z;

      Asig(1,1)= 1.0;
      Asig(4,1)= z;

      Asig(2,2)= 1.0;
      Asig(5,2)= z;

      Asig(6,3)= root56;
      Asig(7,4)= root56;
*/
      //compute the tangent
      dd= theFibers[i]->getTangent( );
      dd*= weight;

      //tangent +=  ( Asig * dd * Aeps );   

//from MATLAB : tangent = 
//[      d11,           d12,           d13,        -z*d11,        -z*d12,        -z*d13,    d14*root56,    d15*root56]
//[      d21,           d22,           d23,        -z*d21,        -z*d22,        -z*d23,    d24*root56,    d25*root56]
//[      d31,           d32,           d33,        -z*d31,        -z*d32,        -z*d33,    d34*root56,    d35*root56]
//[     z*d11,         z*d12,         z*d13,      -z^2*d11,      -z^2*d12,      -z^2*d13,  z*d14*root56,  z*d15*root56]
//[     z*d21,         z*d22,         z*d23,      -z^2*d21,      -z^2*d22,      -z^2*d23,  z*d24*root56,  z*d25*root56]
//[     z*d31,         z*d32,         z*d33,      -z^2*d31,      -z^2*d32,      -z^2*d33,  z*d34*root56,  z*d35*root56]
//[  root56*d41,    root56*d42,    root56*d43, -root56*d41*z, -root56*d42*z, -root56*d43*z,  root56^2*d44,  root56^2*d45]
//[  root56*d51,    root56*d52,    root56*d53, -root56*d51*z, -root56*d52*z, -root56*d53*z,  root56^2*d54,  root56^2*d55]
 
      //row 1
//[      d11,           d12,           d13,        -z*d11,        -z*d12,        -z*d13,    d14*root56,    d15*root56]
      tangent(0,0) +=  dd(0,0);
      tangent(0,1) +=  dd(0,1);
      tangent(0,2) +=  dd(0,2);      
      tangent(0,3) +=  -z*dd(0,0);      
      tangent(0,3) +=  -z*dd(0,1);
      tangent(0,5) +=  -z*dd(0,2);
      tangent(0,6) +=  root56*dd(0,3);
      tangent(0,7) +=  root56*dd(0,4);

      //row 2
//[      d21,           d22,           d23,        -z*d21,        -z*d22,        -z*d23,    d24*root56,    d25*root56]
      tangent(1,0) +=  dd(1,0);
      tangent(1,1) +=  dd(1,1);
      tangent(1,2) +=  dd(1,2);      
      tangent(1,3) +=  -z*dd(1,0);      
      tangent(1,4) +=  -z*dd(1,1);
      tangent(1,5) +=  -z*dd(1,2);
      tangent(1,6) +=  root56*dd(1,3);
      tangent(1,7) +=  root56*dd(1,4);

      //row 3
//[      d31,           d32,           d33,        -z*d31,        -z*d32,        -z*d33,    d34*root56,    d35*root56]
      tangent(2,0) +=  dd(2,0);
      tangent(2,1) +=  dd(2,1);
      tangent(2,2) +=  dd(2,2);      
      tangent(2,3) +=  -z*dd(2,0);      
      tangent(2,4) +=  -z*dd(2,1);
      tangent(2,5) +=  -z*dd(2,2);
      tangent(2,6) +=  root56*dd(2,3);
      tangent(2,7) +=  root56*dd(2,4);

      //row 4
//[     z*d11,         z*d12,         z*d13,      -z^2*d11,      -z^2*d12,      -z^2*d13,  z*d14*root56,  z*d15*root56]
      tangent(3,0) +=  z*dd(0,0);
      tangent(3,1) +=  z*dd(0,1);
      tangent(3,2) +=  z*dd(0,2);      
      tangent(3,3) +=  -z*z*dd(0,0);      
      tangent(3,4) +=  -z*z*dd(0,1);
      tangent(3,5) +=  -z*z*dd(0,2);
      tangent(3,6) +=  z*root56*dd(0,3);
      tangent(3,7) +=  z*root56*dd(0,4);

      //row 5
//[     z*d21,         z*d22,         z*d23,      -z^2*d21,      -z^2*d22,      -z^2*d23,  z*d24*root56,  z*d25*root56]
      tangent(4,0) +=  z*dd(1,0);
      tangent(4,1) +=  z*dd(1,1);
      tangent(4,2) +=  z*dd(1,2);      
      tangent(4,3) +=  -z*z*dd(1,0);      
      tangent(4,4) +=  -z*z*dd(1,1);
      tangent(4,5) +=  -z*z*dd(1,2);
      tangent(4,6) +=  z*root56*dd(1,3);
      tangent(4,7) +=  z*root56*dd(1,4);

      //row 6
//[     z*d31,         z*d32,         z*d33,      -z^2*d31,      -z^2*d32,      -z^2*d33,  z*d34*root56,  z*d35*root56]
      tangent(5,0) +=  z*dd(2,0);
      tangent(5,1) +=  z*dd(2,1);
      tangent(5,2) +=  z*dd(2,2);      
      tangent(5,3) +=  -z*z*dd(2,0);      
      tangent(5,4) +=  -z*z*dd(2,1);
      tangent(5,5) +=  -z*z*dd(2,2);
      tangent(5,6) +=  z*root56*dd(2,3);
      tangent(5,7) +=  z*root56*dd(2,4);

      //row 7
//[  root56*d41,    root56*d42,    root56*d43, -root56*d41*z, -root56*d42*z, -root56*d43*z,  root56^2*d44,  root56^2*d45]
      tangent(6,0) +=  root56*dd(3,0);
      tangent(6,1) +=  root56*dd(3,1);
      tangent(6,2) +=  root56*dd(3,2);      
      tangent(6,3) +=  -root56*z*dd(3,0);      
      tangent(6,4) +=  -root56*z*dd(3,1);
      tangent(6,5) +=  -root56*z*dd(3,2);
      tangent(6,6) +=  root56*root56*dd(3,3);
      tangent(6,7) +=  root56*root56*dd(3,4);

      //row 8 
//[  root56*d51,    root56*d52,    root56*d53, -root56*d51*z, -root56*d52*z, -root56*d53*z,  root56^2*d54,  root56^2*d55]
      tangent(7,0) +=  root56*dd(4,0);
      tangent(7,1) +=  root56*dd(4,1);
      tangent(7,2) +=  root56*dd(4,2);      
      tangent(7,3) +=  -root56*z*dd(4,0);      
      tangent(7,4) +=  -root56*z*dd(4,1);
      tangent(7,5) +=  -root56*z*dd(4,2);
      tangent(7,6) +=  root56*root56*dd(4,3);
      tangent(7,7) +=  root56*root56*dd(4,4);

  } //end for i

  return this->tangent;
  }

//! @brief Return the Von Mises stress at each fiber.
XC::Vector XC::MembranePlateFiberSection::getVonMisesStressAtFibers(void) const
  {
    Vector retval(numFibers);
    for(int i= 0;i<numFibers; i++)
      retval[i]= theFibers[i]->getVonMisesStress();
    return retval;
  }

//! @brief Return the minimum Von Mises stress at fibers.
double XC::MembranePlateFiberSection::getMinVonMisesStress(void) const
  {
    const Vector tmp= getVonMisesStressAtFibers();
    double retval= tmp[0];
    for(int i= 1;i<numFibers; i++)
      retval= std::min(retval, tmp[i]);
    return retval;
  }

//! @brief Return the maximum Von Mises stress at fibers.
double XC::MembranePlateFiberSection::getMaxVonMisesStress(void) const
  {
    const Vector tmp= getVonMisesStressAtFibers();
    double retval= tmp[0];
    for(int i= 1;i<numFibers; i++)
      retval= std::max(retval, tmp[i]);
    return retval;
  }
  
//! @brief Return the maximum Von Mises stress at fibers.
double XC::MembranePlateFiberSection::getAvgVonMisesStress(void) const
  {
    const Vector tmp= getVonMisesStressAtFibers();
    double retval= 0.0;
    for(int i= 0;i<numFibers; i++)
      retval+= tmp[i];
    retval/=numFibers;
    return retval;
  }

//! @brief Return values of internal forces, deformations...
//! @param cod: name of the requested value.
//! @param silent: if true don't complain about non-existen property.
XC::Matrix XC::MembranePlateFiberSection::getValues(const std::string &cod, bool silent) const
  {
    Matrix retval;
    if(cod == "max_von_mises_stress")
      {
	retval.resize(1,1);
	retval(0,0)= getMaxVonMisesStress();
      }
    else if(cod == "min_von_mises_stress")
      {
	retval.resize(1,1);
	retval(0,0)= getMinVonMisesStress();
      }
    else if((cod == "avg_von_mises_stress") || (cod == "mean_von_mises_stress"))
      {
	retval.resize(1,1);
	retval(0,0)= getAvgVonMisesStress();
      }
    else if((cod == "von_mises_stresses") || (cod == "Von_Mises_stresses"))
      {
	retval.resize(5,1);
	const Vector vm= getVonMisesStressAtFibers();
	retval.putCol(0,vm);
      }
    else
      retval= PlateBase::getValues(cod, silent);
    return retval;
  }

//! @brief Print out data
void  XC::MembranePlateFiberSection::Print( std::ostream &s, int flag ) const
  {
    s << "MembranePlateFiberSection: \n ";
    s <<  "  Thickness h = "        <<  h  <<  std::endl;
    for(int i = 0; i < numFibers; i++)
      { theFibers[i]->Print( s, flag ) ; }
    return;
  }

//! @brief Send object members through the communicator argument.
int XC::MembranePlateFiberSection::sendData(Communicator &comm)
  {
    int res= PlateBase::sendData(comm);
    res+= comm.sendBrokedPtr(theFibers[0],getDbTagData(),BrokedPtrCommMetaData(7,8,9));
    res+= comm.sendBrokedPtr(theFibers[1],getDbTagData(),BrokedPtrCommMetaData(10,11,12));
    res+= comm.sendBrokedPtr(theFibers[2],getDbTagData(),BrokedPtrCommMetaData(13,14,15));
    res+= comm.sendBrokedPtr(theFibers[3],getDbTagData(),BrokedPtrCommMetaData(16,17,18));
    res+= comm.sendBrokedPtr(theFibers[4],getDbTagData(),BrokedPtrCommMetaData(19,20,21));
    res+= comm.sendVector(strainResultant,getDbTagData(),CommMetaData(22));
    res+= comm.sendVector(initialStrain,getDbTagData(),CommMetaData(23));
    res+= comm.sendInt(integrationType,getDbTagData(),CommMetaData(24));
    return res;
  }

//! @brief Receives object members through the communicator argument.
int XC::MembranePlateFiberSection::recvData(const Communicator &comm)
  {
    int res= PlateBase::recvData(comm);
    theFibers[0]= comm.getBrokedMaterial(theFibers[0],getDbTagData(),BrokedPtrCommMetaData(7,8,9));
    theFibers[1]= comm.getBrokedMaterial(theFibers[1],getDbTagData(),BrokedPtrCommMetaData(10,11,12));
    theFibers[2]= comm.getBrokedMaterial(theFibers[2],getDbTagData(),BrokedPtrCommMetaData(13,14,15));
    theFibers[3]= comm.getBrokedMaterial(theFibers[3],getDbTagData(),BrokedPtrCommMetaData(16,17,18));
    theFibers[4]= comm.getBrokedMaterial(theFibers[4],getDbTagData(),BrokedPtrCommMetaData(19,20,21));
    res+= comm.receiveVector(strainResultant,getDbTagData(),CommMetaData(22));
    res+= comm.receiveVector(initialStrain,getDbTagData(),CommMetaData(23));
    res+= comm.receiveInt(integrationType,getDbTagData(),CommMetaData(24));
    return res;
  }

//! @brief Send object itself through the communicator argument.
int XC::MembranePlateFiberSection::sendSelf(Communicator &comm)
  {
    setDbTag(comm);
    const int dataTag= getDbTag();
    inicComm(25);
    int res= sendData(comm);

    res+= comm.sendIdData(getDbTagData(),dataTag);
    if(res < 0)
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; failed to send data.\n";
    return res;
  }


//! @brief Receive object itself through the communicator argument.
int XC::MembranePlateFiberSection::recvSelf(const Communicator &comm)
  {
    inicComm(25);
    const int dataTag= getDbTag();
    int res= comm.receiveIdData(getDbTagData(),dataTag);

    if(res<0)
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; failed to receive ids.\n";
    else
      {
        setTag(getDbTagDataPos(0));
        res+= recvData(comm);
        if(res<0)
          std::cerr << getClassName() << "::" << __FUNCTION__
		    << "; failed to receive data.\n";
      }
    return res;
  }
 
XC::Response *XC::MembranePlateFiberSection::setResponse(const std::vector<std::string> &argv, Information &info)
  {
    Response *theResponse= nullptr;
    const int argc= argv.size();
    if(argc > 2 && (argv[0]=="fiber"))
      {
        const int passarg = 2;
        const int key = atoi(argv[1]);    

	if(key > 0 && key <= numFibers)
	  {
	    //info.tag("FiberOutput");
	    //info.attr("number", key);
	    //const double *sg= (integrationType == 0) ? sgLobatto : sgGauss;
	    //const double *wg= (integrationType == 0) ? wgLobatto : wgGauss;      
	    //info.attr("zLoc", 0.5 * h * sg[key - 1]);
	    //info.attr("thickness", 0.5 * h * wg[key - 1]);
            // Slice arguments.
	    const std::vector<std::string> argv2= std::vector<std::string>(argv.begin()+passarg, argv.end());
	    theResponse= theFibers[key-1]->setResponse(argv2, info);
	    //info.endTag();
	  }
      }
    if(!theResponse)
      return SectionForceDeformation::setResponse(argv, info);
    return theResponse;
  }

int XC::MembranePlateFiberSection::setParameter(const std::vector<std::string> &argv, Parameter &param)
  {
    // if the user explicitly wants to update a material in this section...
    const int argc= argv.size();
    if(argc > 1)
      {
        // case 1: fiber value (all fibers)
        // case 2: fiber id value (one specific fiber)
        if((argv[0]=="fiber") || (argv[0]=="Fiber"))
	  {
            // test case 2 (one specific fiber) ...
            if(argc > 2)
	      {
                const int pointNum = atoi(argv[1]);
                if(pointNum > 0 && pointNum <= numFibers)
		  {
		    // Slice arguments.
		    const std::vector<std::string> argv2= std::vector<std::string>(argv.begin()+2, argv.end());
                    return theFibers[pointNum - 1]->setParameter(argv2, param);
                  }
              }
            // ... otherwise case 1 (all fibers), if the argv[1] is not a valid id
            int mixed_result = -1;
            for(int i = 0; i < numFibers; ++i)
	      {
		// Slice arguments.
		const std::vector<std::string> argv1= std::vector<std::string>(argv.begin()+1, argv.end());
                if(theFibers[i]->setParameter(argv1, param) == 0)
                    mixed_result = 0; // if at least one fiber handles the param, make it successful
              }
            return mixed_result;
	  }
      }
    // if we are here, the first keyword is not "fiber", so we can check for parameters
    // specific to this section (if any) or forward the request to all fibers.
    if(argc > 0)
      {
        // we don't have parameters for this section, so we directly forward it to all fibers.
        // placeholder for future implementations: if we will have parameters for this class, check them here
        // before forwarding to all fibers
        int mixed_result = -1;
        for(int i = 0; i < numFibers; ++i)
	  {
            if (theFibers[i]->setParameter(argv, param) == 0)
                mixed_result = 0; // if at least one fiber handles the param, make it successful
          }
        return mixed_result;
      }
    // fallback to base class implementation
    return SectionForceDeformation::setParameter(argv, param);
  }


