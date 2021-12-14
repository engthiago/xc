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
//VectorReinfBar.cc

#include "VectorReinfBar.h"
#include <material/section/repres/section_geometry/reinfBar/ReinfBar.h> 
#include "utility/matrix/Vector.h"
#include "utility/matrix/Matrix.h"
#include "utility/geom/pos_vec/Dir2d.h"
#include "utility/geom/pos_vec/Vector2d.h"
#include "utility/geom/pos_vec/Pos2d.h"
#include "utility/geom/ref_sys/Ref2d2d.h"
#include "material/uniaxial/UniaxialMaterial.h" 




//! @brief Frees memory for all the bars.
void XC::VectorReinfBar::free_mem(void)
  {
    const size_t sz= size();
    for(size_t i= 0;i<sz;i++)
      free_mem(i);
  }

//! @brief Frees memory for the i-th bar.
void XC::VectorReinfBar::free_mem(const size_t i)
  {
    if((*this)[i])
      delete (*this)[i];
    (*this)[i]= nullptr;
  }

//! @brief Stores a copy of the bar at the i-th position.
void XC::VectorReinfBar::alloc(const size_t i,const ReinfBar &c)
  {
    free_mem(i);
    (*this)[i]= c.getCopy();
  }

//! @brief Constructor.
XC::VectorReinfBar::VectorReinfBar(const size_t &sz)
  : v_rfBar(sz,nullptr) {}

//! @brief Copy constructor.
XC::VectorReinfBar::VectorReinfBar(const VectorReinfBar  &other)
  : v_rfBar(other.size(),nullptr)
  {
    const size_t sz= size();
    for(size_t i=0;i<sz;i++)
      {
        const ReinfBar *tmp= other[i];
        if(tmp)
          (*this)[i]= tmp->getCopy();
      }
  }

//! @brief Assignment operator.
XC::VectorReinfBar &XC::VectorReinfBar::operator=(const VectorReinfBar &other)
  {
    resize(other.size());
    const size_t sz= size();
    for(size_t i=0;i<sz;i++)
      {
        const ReinfBar *tmp= other[i];
        if(tmp)
          (*this)[i]= tmp->getCopy();
      }
    return *this;
  }

//! @brief Destructor.
XC::VectorReinfBar::~VectorReinfBar(void)
  { free_mem(); }

//! @brief Returns the homogenized area of the regions.
double XC::VectorReinfBar::getAreaHomogenizedSection(const double &E0) const
  {
    if(fabs(E0)<1e-6)
      std::clog << getClassName() << __FUNCTION__
		<< "; homogenization reference modulus too small; E0= "
		<< E0 << std::endl; 

    double retval= 0.0;
    double n= 0.0;
    for(const_iterator i= begin();i!=end();i++)
      {
        const UniaxialMaterial *mat= dynamic_cast<const UniaxialMaterial *>((*i)->getMaterialPtr());
        if(mat)
          {
            n= mat->getTangent()/E0;
            retval+= n*(*i)->getArea();
          }
        else
	  std::cerr << getClassName() << "::" << __FUNCTION__
		    << "; material not found." << std::endl; 
      }
    return retval;
  }

XC::Vector XC::VectorReinfBar::getCenterOfMassHomogenizedSection(const double &E0) const
  {
    if(fabs(E0)<1e-6)
      std::clog << getClassName() << __FUNCTION__
		<< "; homogenization reference modulus too small; E0= "
		<< E0 << std::endl; 

    Vector retval(2);
    double weight= 0.0;
    double divisor= 0.0;
    for(const_iterator i= begin();i!=end();i++)
      {
        const UniaxialMaterial *mat= dynamic_cast<const UniaxialMaterial *>((*i)->getMaterialPtr());
        if(mat)
          {
            weight= mat->getTangent()/E0*(*i)->getArea();
            retval+= weight*(*i)->getPosition();
            divisor+= weight;
          }
        else
	  std::cerr << getClassName() << "::" << __FUNCTION__
		    << "; region material undefined." << std::endl;
      }
    retval/= divisor;
    return retval;
  }

//! @brief Returns homogenized moment of inertia of the cross-section with respecto to the axis parallel to y passing through the centroid.
//! @param E0: Reference elastic modulus.
double XC::VectorReinfBar::getIyHomogenizedSection(const double &E0) const
  {
    double retval= 0.0;
    double n= 0.0;
    double d= 0.0;
    const double zCenterOfMass= getCenterOfMassHomogenizedSection(E0)[1];
    for(const_iterator i= begin();i!=end();i++)
      {
        const UniaxialMaterial *mat= dynamic_cast<const UniaxialMaterial *>((*i)->getMaterialPtr());
        if(mat)
          {
            n= mat->getTangent()/E0;
            d= (*i)->getPosition()[1]-zCenterOfMass;
            retval+= n*((*i)->Iy()+(*i)->getArea()*sqr(d));
          }
        else
	  std::cerr << getClassName() << "::" << __FUNCTION__
		    << "; region material undefined." << std::endl; 
      }
    return retval;
  }

//! @brief Returns homogenized moment of inertia of the cross-section with respecto to the axis parallel to z passing through the centroid.
//! @param E0: Reference elastic modulus.
double XC::VectorReinfBar::getIzHomogenizedSection(const double &E0) const
  {
    double retval= 0.0;
    double n= 0.0;
    double d= 0.0;
    const double yCenterOfMass= getCenterOfMassHomogenizedSection(E0)[0];
    for(const_iterator i= begin();i!=end();i++)
      {
        const UniaxialMaterial *mat= dynamic_cast<const UniaxialMaterial *>((*i)->getMaterialPtr());
        if(mat)
          {
            n= mat->getTangent()/E0;
            d= (*i)->getPosition()[0]-yCenterOfMass;
            retval+= n*((*i)->Iz()+(*i)->getArea()*sqr(d));
          }
        else
	  std::cerr << getClassName() << "::" << __FUNCTION__
		    << "; region material undefined." << std::endl; 
      }
    return retval;
  }

//! @brief Returns homogenized product of inertia of the cross-section with respecto to the axis parallel to y and z passing through the centroid.
//! @param E0: Reference elastic modulus.
double XC::VectorReinfBar::getPyzHomogenizedSection(const double &E0) const
  {
    double retval= 0.0;
    double n= 0.0;
    double d2= 0.0;
    const double zCenterOfMass= getCenterOfMassHomogenizedSection(E0)[0];
    const double yCenterOfMass= getCenterOfMassHomogenizedSection(E0)[0];
    for(const_iterator i= begin();i!=end();i++)
      {
        const UniaxialMaterial *mat= dynamic_cast<const UniaxialMaterial *>((*i)->getMaterialPtr());
        if(mat)
          {
            n= mat->getTangent()/E0;
            d2= ((*i)->getPosition()[0]-yCenterOfMass)*((*i)->getPosition()[1]-zCenterOfMass);
            retval+= n*((*i)->Pyz()+(*i)->getArea()*d2);
          }
        else
	  std::cerr << getClassName() << "::" << __FUNCTION__
		    << "; region material not found." << std::endl; 
      }
    return retval;
  }


//! @brief Return gross area of rebars.
double XC::VectorReinfBar::getAreaGrossSection(void) const
  {
    double retval= 0.0;
    for(const_iterator i= begin();i!=end();i++)
      retval+= (*i)->getArea();
    return retval;
  }

//! @brief Returns gross section centroid.
XC::Vector XC::VectorReinfBar::getCenterOfMassGrossSection(void) const
  {
    Vector retval(2);
    double weight= 0.0;
    double divisor= 0.0;
    for(const_iterator i= begin();i!=end();i++)
      {
        weight= (*i)->getArea();
        retval+= weight*(*i)->getPosition();
        divisor+= weight;
      }
    retval/= divisor;
    return retval;
  }

//! @brief Inertia of the gross section about an axis parallel to y through his center of gravity.
double XC::VectorReinfBar::getIyGrossSection(void) const
  {
    double retval= 0.0;
    double d= 0.0;
    const double zCenterOfMass= getCenterOfMassGrossSection()[1];
    for(const_iterator i= begin();i!=end();i++)
      {
        d= (*i)->getPosition()[1]-zCenterOfMass;
        retval+= (*i)->Iy()+(*i)->getArea()*sqr(d);
      }
    return retval;
  }

//! @brief Inertia of the gross section about an axis parallel to z through his center of gravity.
double XC::VectorReinfBar::getIzGrossSection(void) const
  {
    double retval= 0.0;
    double d= 0.0;
    const double yCenterOfMass= getCenterOfMassGrossSection()[0];
    for(const_iterator i= begin();i!=end();i++)
      {
        d= (*i)->getPosition()[0]-yCenterOfMass;
        retval+= (*i)->Iz()+(*i)->getArea()*sqr(d);
      }
    return retval;
  }

//! @brief Product of inertia of the gross section about y and z axis through his center of gravity.
double XC::VectorReinfBar::getPyzGrossSection(void) const
  {
    double retval= 0.0;
    double d2= 0.0;
    const Vector centerOfMassPosition= getCenterOfMassGrossSection();
    const double zCenterOfMass= centerOfMassPosition[1];
    const double yCenterOfMass= centerOfMassPosition[0];
    for(const_iterator i= begin();i!=end();i++)
      {
        d2= ((*i)->getPosition()[0]-yCenterOfMass)*((*i)->getPosition()[1]-zCenterOfMass);
        retval+= (*i)->Pyz()+(*i)->getArea()*d2;
      }
    return retval;
  }

//! @brief Resizes vector.
void XC::VectorReinfBar::resize(const size_t sz)
  {
    free_mem();
    v_rfBar::resize(sz);
    for(size_t i= 0;i<sz;i++)
      (*this)[i]= nullptr;
  }

void XC::VectorReinfBar::put(const size_t i,const ReinfBar &c)
  { alloc(i,c); }

void XC::VectorReinfBar::Print(std::ostream &os) const
  {
    for(const_iterator i= begin();i!=end();i++)
      (*i)->Print(os);
  }
