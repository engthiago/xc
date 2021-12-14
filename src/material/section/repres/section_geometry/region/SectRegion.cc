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
//SectRegion.cpp
// Written by Remo M. de Souza
// December 1998

#include "SectRegion.h"
#include "PolygonSectRegion.h"
#include "utility/geom/d2/2d_polygons/Polygon2d.h"
#include "utility/geom/pos_vec/Dir2d.h"

#include <utility/matrix/Vector.h>
#include <utility/matrix/Matrix.h>
#include "utility/geom/FT_matrix.h"

//! @brief Constructor.
XC::SectRegion::SectRegion(Material *mat)
  : DiscretBase(mat) {}

//! @brief Return the region contour.
Polygon2d XC::SectRegion::getPolygon(void) const
  {
    std::cerr << getClassName() << "::" << __FUNCTION__
              << "; not implemented." << std::endl;
    Polygon2d retval;
    return retval;
  }

//! @brief Return the intersection of the region with the half-plane
//!
//! @param sp: Half-plane to intersect width.
XC::PolygonSectRegion XC::SectRegion::Intersection(const HalfPlane2d &sp) const
  {
    std::list<Polygon2d> tmpList= getPolygon().getIntersection(sp);
    if(tmpList.size()>1)
      std::cerr << getClassName() << __FUNCTION__
	        << "; not a simply connected region."
                << std::endl;
    Polygon2d tmp= *tmpList.begin();
    return PolygonSectRegion(getMaterialPtr(),1,1,tmp);
  }

//! @brief Return the centroid coordinates.
const XC::Vector &XC::SectRegion::getCenterOfMass(void) const
  {
    const Pos2d p= getPolygon().getCenterOfMass();
    static Vector retval(2);
    retval[0]= p.x();
    retval[1]= p.y();
    return retval;
  }

//! @brief Return the region contour length.
double XC::SectRegion::getLength(void) const
  { return getPolygon().getLength(); }

//! @brief Return the region area.
double XC::SectRegion::getArea(void) const
  { return getPolygon().getArea(); }

//! @brief Return the moment of inertia with respect to the axis parallel to y axis through the centroid.
double XC::SectRegion::Iy(void) const
  { return getPolygon().Ix(); }

//! @brief Return the moment of inertia with respect to the axis parallel to z axis through the centroid.
double XC::SectRegion::Iz(void) const
  { return getPolygon().Iy(); }

//! @brief Return the product of inertia respecto a the parallel axes por el centroid.
double XC::SectRegion::Pyz(void) const
  { return getPolygon().Pxy(); }

//! @brief Return the moment of inertia polar with respect to centroid
//! in local coordinates.
double XC::SectRegion::Ix(void) const
  { return Iy()+Iz(); }

//! @brief Return the angle that defines a principal axis of inertia.
double XC::SectRegion::Theta_p(void) const
  { return getPolygon().Theta_p(); }

//! @brief Return the direction of a principal axis of inertia
//! (we don't know yet if it's the major one or the minor one).
const XC::Vector &XC::SectRegion::IAxisDir_a(void) const
  {
    const Dir2d p= getPolygon().IAxisDir_a();
    static Vector retval(2);
    retval[0]= p.dx();
    retval[1]= p.dy();
    return retval;
  }

//! @brief Return the direction of the other (with respect to IAxisDir_a)
//! principal axis of inertia (we don't know yet if it's the major
//! one or the minor one).
const XC::Vector &XC::SectRegion::IAxisDir_b(void) const
  {
    const Dir2d p= getPolygon().IAxisDir_b();
    static Vector retval(2);
    retval[0]= p.dx();
    retval[1]= p.dy();
    return retval;
  }

//     Line2d IAxis_a(void) const;
//     //Returns a principal axis of inertia (we don't know yet
//     //the major one or the minor one).
//     Line2d IAxis_b(void) const;
//     //Return the other principal axis of inertia (we don't know yet
//     //the major one or the minor one).
//     Ref2d2d principalAxesOfInertia(void) const;
//     //Return the principal axes of inertia.

//! @brief Return the principal major axis of inertia.
double XC::SectRegion::getI1(void) const
  { return getPolygon().I1(); }

//! @brief Return the principal minor axis of inertia.
double XC::SectRegion::getI2(void) const
  { return getPolygon().I2(); }

//     inline PrincipalAxesOfInertia2D Inertia(void)
//       { return PrincipalAxesOfInertia2D(getCenterOfMass(),Iy(),Iz(),Pyz()); }

//! @brief Return the i,j component of the tensor of inertia computed with respect to centroid.
double XC::SectRegion::getI(const unsigned short int &i,const unsigned short int &j) const
  { return getPolygon().I(i+1,j+1); }

//! @brief Return the moment of inertia with respect to the axis that passes through O with the direction of e.
double XC::SectRegion::getI(const Pos2d &O,const Vector &e) const
  { return getPolygon().I(O,Vector2d(e[0],e[1])); }

//     double I(const Line2d &r) const;
//       //Return the moment of inertia respecto a la recta being passed
//       //as parameter.

//! @brief Return the i,j component of the tensor of inertia computed with respecto to the point "o".
double XC::SectRegion::getI(const unsigned short int i,const unsigned short int j,const Pos2d &o) const
  { return getPolygon().I(i+1,j+1,o); }

//! @brief Return the polar moment of inertia with respect to the point o.
double XC::SectRegion::getIO(const Pos2d &o)
  { return getPolygon().IO(o); }

//! @brief Return the tensor of inertia computed with respect to the object centroid.
XC::Matrix &XC::SectRegion::getI(void) const
  {
    static Matrix i(2,2);
    const Polygon2d &pol= getPolygon();
    i(0,0)= pol.Ix(); i(0,1)= -pol.Pxy();
    i(1,0)= i(0,1);   i(1,1)= pol.Iy();
    return i;
  }

//! @brief Return the tensor of inertia with respect to the point o.
XC::Matrix &XC::SectRegion::getI(const Pos2d &o) const
  {
    FT_matrix tmp= getPolygon().I(o);
    static Matrix i(2,2);
    i(0,0)= tmp(1,1); i(0,1)= tmp(1,2);
    i(1,0)= tmp(1,2); i(1,1)= tmp(2,2);
    return i;
  }
