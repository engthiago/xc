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
//SectionMassProperties.cpp

#include "SectionMassProperties.h"
#include "utility/utils/misc_utils/inertia.h"
#include "utility/matrix/Vector.h"
#include "utility/matrix/Matrix.h"
#include "utility/geom/pos_vec/Pos2d.h"
#include "utility/geom/ref_sys/Ref2d2d.h"
#include "utility/geom/ref_sys/PrincipalAxesOfInertia2D.h"
#include "material/section/repres/CrossSectionProperties3d.h"
#include "material/section/repres/CrossSectionProperties2d.h"
#include "material/section/repres/CrossSectionProperties1d.h"
#include "utility/geom/d1/Segment2d.h"

//! @brief Constructor.
XC::SectionMassProperties::SectionMassProperties(CommandEntity *owr)
  : CommandEntity(owr) {}

//! @brief Inertia of the homogenized section about an axis parallel to x
//! through his center of gravity.
//! @param E0: Elastic modulus used as reference.
double XC::SectionMassProperties::getIxHomogenizedSection(const double &E0) const
  { return getIyHomogenizedSection(E0)+getIzHomogenizedSection(E0); }



//! @brief Agle of the principal axis of inertia of the homogenized section
//! with respect of current axis.
double XC::SectionMassProperties::getTheta_pHomogenizedSection(void) const
  { return theta_inertia(getIyHomogenizedSection(1.0),getIzHomogenizedSection(1.0),getPyzHomogenizedSection(1.0)); }

//! @brief Direction of a principal axis of inertia of the homogenized section
//! with respect of current axis.
//! @param E0: Elastic modulus used as reference.
XC::Vector XC::SectionMassProperties::getIAxisDir_aHomogenizedSection(void) const
  {
    const Dir2d p(getTheta_pHomogenizedSection());
    Vector retval(2);
    retval[0]= p.dx();
    retval[1]= p.dy();
    return retval;
  }

//! @brief Direction of the other axis of inertia of the homogenized section
//! with respect of current axis.
//! @param E0: Elastic modulus used as reference.
XC::Vector XC::SectionMassProperties::getIAxisDir_bHomogenizedSection(void) const
  {
    const Dir2d p= Dir2d(getTheta_pHomogenizedSection()).Perpendicular();
    Vector retval(2);
    retval[0]= p.dx();
    retval[1]= p.dy();
    return retval;
  }

//! @brief Major principal moment of inertia of the homogenized section.
double XC::SectionMassProperties::getI1HomogenizedSection(const double &E0) const
  { 
    const double iy= getIyHomogenizedSection(E0);
    const double iz= getIzHomogenizedSection(E0);
    const double pyz= getPyzHomogenizedSection(E0);
    return I1_inertia(iy,iz,pyz);
  }

//! @brief Minor principal moment of inertia of the homogenized section.
double XC::SectionMassProperties::getI2HomogenizedSection(const double &E0) const
  {
    const double iy= getIyHomogenizedSection(E0);
    const double iz= getIzHomogenizedSection(E0);
    const double pyz= getPyzHomogenizedSection(E0);
    return I2_inertia(iy,iz,pyz);
  }

//! @brief Principal axis of inertia of the homogenized section.
PrincipalAxesOfInertia2D XC::SectionMassProperties::getInertiaAxesHomogenizedSection(const double &E0) const
  {
    const Vector v= getCenterOfMassHomogenizedSection(E0);
    const Pos2d center_of_mass(v[0],v[1]);
    return PrincipalAxesOfInertia2D(center_of_mass,getIyHomogenizedSection(E0),getIzHomogenizedSection(E0),getPyzHomogenizedSection(E0));
  }
//! @brief Direction of the major principal axis of inertia of the homogenized
//! section.
Vector2d XC::SectionMassProperties::getAxis1VDirHomogenizedSection(const double &E0) const
  { return getInertiaAxesHomogenizedSection(E0).getAxis1VDir(); }
//! @brief Direction of the major principal axis of inertia of the homogenized
//! section.
Vector2d XC::SectionMassProperties::getVDirStrongAxisHomogenizedSection(const double &E0) const
  { return getInertiaAxesHomogenizedSection(E0).getAxis1VDir(); }
//! @brief Direction of the minor principal axis of inertia of the homogenized
//! section.
Vector2d XC::SectionMassProperties::getAxis2VDirHomogenizedSection(const double &E0) const
  { return getInertiaAxesHomogenizedSection(E0).getAxis2VDir(); }
//! @brief Direction of the minor principal axis of inertia of the homogenized
//! section.
Vector2d XC::SectionMassProperties::getVDirWeakAxisHomogenizedSection(const double &E0) const
  { return getInertiaAxesHomogenizedSection(E0).getAxis2VDir(); }

//! @brief tensor of inertia (i,j) component of the homogenized section.
double XC::SectionMassProperties::getIHomogenizedSection(const double &E0,const unsigned short int &i,const unsigned short int &j) const
  {
    unsigned short int k= i + (j-1)*2;
    double retval= 0.0;
    switch(k)
      {
        case 1: //(1,1)
          retval= getIyHomogenizedSection(E0);
          break;
        case 2: //(2,1)
        case 3: //(1,2)
          retval= -getPyzHomogenizedSection(E0);
          break;
	case 4:
          retval= getIzHomogenizedSection(E0);
          break;
      }
    return retval;
  }

//! @brief Return the moment of inertia with respect to the axis that passes
//! through O with the direction of e.
double XC::SectionMassProperties::getIHomogenizedSection(const double &E0,const Pos2d &O,const Vector &e) const
  {
    const Matrix Io(getIHomogenizedSection(E0,O));
    return dot(e,Io*e)/e.Norm2();
  }

//! @brief Return the moment of inertia respecto a la recta being passed
//! as parameter.
double XC::SectionMassProperties::getIHomogenizedSection(const double &E0,const Line2d &r) const
  { return getIHomogenizedSection(E0,r.Point(),Vector(r.VDir()));  }

//! @brief Return the i,j component of the tensor of inertia computed with
//! respect to the point "o".
double XC::SectionMassProperties::getIHomogenizedSection(const double &E0,const unsigned short int &i,const unsigned short int &j,const Pos2d &o) const
  {
    const double Iij= getIHomogenizedSection(E0,i,j);
    const Vector center_of_mass= getCenterOfMassHomogenizedSection(E0);

    Pos2d pp(center_of_mass[0],center_of_mass[1]);
    Ref2d2d axes(pp);
    Pos2d pos_local= axes.getLocalPosition(o);
    return Iij + getAreaHomogenizedSection(E0) * pos_local(i) * pos_local(j);
  }

//! @brief Return the polar moment of inertia with respect to the point o.
double XC::SectionMassProperties::getIOHomogenizedSection(const double &E0,const Pos2d &o) const
  { return (getIHomogenizedSection(E0,1,1,o)+getIHomogenizedSection(E0,2,2,o)+getIHomogenizedSection(E0,3,3,o))/2; }


//! @brief Return the tensor of inertia computed with respect to the object
//! centroid.
XC::Matrix XC::SectionMassProperties::getIHomogenizedSection(const double &E0) const
  {
    Matrix i(2,2);
    i(0,0)= getIyHomogenizedSection(E0); i(0,1)= -getPyzHomogenizedSection(E0);
    i(1,0)= i(0,1);   i(1,1)= getIzHomogenizedSection(E0);
    return i;
  }

//! @brief Return the tensor of inertia with respect to the point o.
XC::Matrix XC::SectionMassProperties::getIHomogenizedSection(const double &E0,const Pos2d &o) const
  {
    Matrix retval(2,2);
    const Matrix Ig= getIHomogenizedSection(E0);
    Vector O(2); O[0]= o.x(); O[1]= o.y();
    const Vector og= getCenterOfMassHomogenizedSection(E0) - O;
    const double m= getAreaHomogenizedSection(E0);
    retval= Ig+m*(og.Norm2()*identity(Ig)-(og & og));
    return retval;
  }

//! @brief Return the 3D mass properties of the homogenized section.
XC::CrossSectionProperties3d XC::SectionMassProperties::getCrossSectionProperties3d(const CrossSectionProperties3d &base) const
  {
    CrossSectionProperties3d retval;
    retval= base; //Assign E, G y alpha.
    retval.A()= getAreaHomogenizedSection(base.E()); 
    retval.Iy()= getIyHomogenizedSection(base.E());
    retval.Iz()= getIzHomogenizedSection(base.E());
    retval.J()= getIxHomogenizedSection(base.E());
    return retval;
  }

//! @brief Return the 2D mass properties of the homogenized section.
XC::CrossSectionProperties2d XC::SectionMassProperties::getCrossSectionProperties2d(const CrossSectionProperties2d &base) const
  {
    static CrossSectionProperties2d retval;
    retval= base; //Assign E, G y alpha.
    retval.A()= getAreaHomogenizedSection(base.E()); 
    retval.I()= getIzHomogenizedSection(base.E());
    return retval;
  }

//! @brief Return the 1D mass properties of the homogenized section.
XC::CrossSectionProperties1d XC::SectionMassProperties::getCrossSectionProperties1d(const CrossSectionProperties1d &base) const
  {
    static CrossSectionProperties1d retval;
    retval= base; //Assign E and rho.
    retval.A()= getAreaHomogenizedSection(base.E()); 
    return retval;
  }

//! @brief Inertia of the gross section about an axis parallel to x through
//! his center of gravity.
double XC::SectionMassProperties::getIxGrossSection(void) const
  { return getIyGrossSection()+getIzGrossSection(); }

//! @brief Agle of the principal axis of inertia of the gross section with
//! respect of current axis.
double XC::SectionMassProperties::getTheta_pGrossSection(void) const
  { return theta_inertia(getIyGrossSection(),getIzGrossSection(),getPyzGrossSection()); }

//! @brief Direction of a principal axis of inertia of the gross section with
//! respect of current axis.
XC::Vector XC::SectionMassProperties::getIAxisDir_aGrossSection(void) const
  {
    const Dir2d p(getTheta_pGrossSection());
    Vector retval(2);
    retval[0]= p.dx();
    retval[1]= p.dy();
    return retval;
  }

//! @brief Direction of the other principal axis of inertia of the gross section
//! with respect of current axis.
XC::Vector XC::SectionMassProperties::getIAxisDir_bGrossSection(void) const
  {
    const Dir2d p= Dir2d(getTheta_pGrossSection()).Perpendicular();
    Vector retval(2);
    retval[0]= p.dx();
    retval[1]= p.dy();
    return retval;
  }

//! @brief Major principal moment of inertia of the gross section.
double XC::SectionMassProperties::getI1GrossSection(void) const
  {
    const double iy= getIyGrossSection();
    const double iz= getIzGrossSection();
    const double pyz= getPyzGrossSection();
    return I1_inertia(iy,iz,pyz);
  }

//! @brief Minor principal moment of inertia of the gross section.
double XC::SectionMassProperties::getI2GrossSection(void) const
  {
    const double iy= getIyGrossSection();
    const double iz= getIzGrossSection();
    const double pyz= getPyzGrossSection();
    return I2_inertia(iy,iz,pyz);
  }

//! @brief Principal axis of inertia of the gross section.
PrincipalAxesOfInertia2D XC::SectionMassProperties::getInertiaAxesGrossSection(void) const
  {
    const Vector v= getCenterOfMassGrossSection();
    const Pos2d center_of_mass(v[0],v[1]);
    return PrincipalAxesOfInertia2D(center_of_mass,getIyGrossSection(),getIzGrossSection(),getPyzGrossSection());
  }
//! @brief Direction of the major principal axis of inertia of the gross
//! section.
Vector2d XC::SectionMassProperties::getAxis1VDirGrossSection(void) const
  { return getInertiaAxesGrossSection().getAxis1VDir(); }
//! @brief Direction of the major principal axis of inertia of the gross
//! section.
Vector2d XC::SectionMassProperties::getVDirStrongAxisGrossSection(void) const
  { return getInertiaAxesGrossSection().getAxis1VDir(); }
//! @brief Direction of the minor principal axis of inertia of the gross
//! section.
Vector2d XC::SectionMassProperties::getAxis2VDirGrossSection(void) const
  { return getInertiaAxesGrossSection().getAxis2VDir(); }
//! @brief Direction of the minor principal axis of inertia of the gross
//! section.
Vector2d XC::SectionMassProperties::getVDirWeakAxisGrossSection(void) const
  { return getInertiaAxesGrossSection().getAxis2VDir(); }

//! @brief Return the i,j component of the tensor of inertia computed with
//! respect to centroid.
double XC::SectionMassProperties::getIGrossSection(const unsigned short int &i,const unsigned short int &j) const
  {
    unsigned short int k= i + (j-1)*2;
    double retval= 0.0;
    switch(k)
      {
        case 1: //(1,1)
          retval= getIyGrossSection();
          break;
        case 2: //(2,1)
        case 3: //(1,2)
          retval= -getPyzGrossSection();
          break;
	case 4:
          retval= getIzGrossSection();
          break;
      }
    return retval;
  }

//! @brief Return the moment of inertia with respect to the axis that passes
//! through O with the direction of e.
double XC::SectionMassProperties::getIGrossSection(const Pos2d &O,const Vector &e) const
  {
    const Matrix Io(getIGrossSection(O));
    return dot(e,Io*e)/e.Norm2();
  }

//! @brief Return the moment of inertia respecto a la recta being passed
//! as parameter.
double XC::SectionMassProperties::getIGrossSection(const Line2d &r) const
  { return getIGrossSection(r.Point(),Vector(r.VDir()));  }

//! @brief Return the i,j component of the tensor of inertia computed with
//! respect to the point "o".
double XC::SectionMassProperties::getIGrossSection(const unsigned short int &i,const unsigned short int &j,const Pos2d &o) const
  {
    const double Iij= getIGrossSection(i,j);
    const Vector center_of_mass= getCenterOfMassGrossSection();

    Pos2d pp(center_of_mass[0],center_of_mass[1]);
    Ref2d2d axes(pp);
    Pos2d pos_local= axes.getLocalPosition(o);
    return Iij + getAreaGrossSection() * pos_local(i) * pos_local(j);
  }

//! @brief Return the polar moment of inertia with respect to the point o.
double XC::SectionMassProperties::getIOGrossSection(const Pos2d &o) const
  { return (getIGrossSection(1,1,o)+getIGrossSection(2,2,o)+getIGrossSection(3,3,o))/2; }


//! @brief Return the tensor of inertia computed with respect to the object
//! centroid.
XC::Matrix XC::SectionMassProperties::getIGrossSection(void) const
  {
    Matrix i(2,2);
    i(0,0)= getIyGrossSection(); i(0,1)= -getPyzGrossSection();
    i(1,0)= i(0,1);   i(1,1)= getIzGrossSection();
    return i;
  }

//! @brief Return the tensor of inertia with respect to the point o.
XC::Matrix XC::SectionMassProperties::getIGrossSection(const Pos2d &o) const
  {
    Matrix retval(2,2);
    const Matrix Ig= getIGrossSection();
    Vector O(2); O[0]= o.x(); O[1]= o.y();
    const Vector og= getCenterOfMassGrossSection() - O;
    const double m= getAreaGrossSection();
    retval= Ig+m*(og.Norm2()*identity(Ig)-(og & og));
    return retval;
  }

