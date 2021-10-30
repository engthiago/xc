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
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */
                                                                        
// $Revision: 1.2 $
// $Date: 2003/02/14 23:01:37 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/section/repres/section_geometry/reinfBar/ReinfBar.cpp,v $
                                                                        
                                                                        
// File: ReinfBar.C
// Written by Remo M. de Souza
// December 1998

#include "material/section/repres/section_geometry/reinfBar/ReinfBar.h"
#include <cmath>
#include "utility/matrix/Vector.h"
#include "utility/matrix/Matrix.h"
#include "utility/geom/pos_vec/Dir2d.h"
#include "utility/geom/pos_vec/Vector2d.h"
#include "utility/geom/pos_vec/Pos2d.h"
#include "utility/geom/ref_sys/Ref2d2d.h"


//! @brief Constructor.
XC::ReinfBar::ReinfBar(Material *mat)
  : DiscretBase(mat), diameter(0.0), area(0.0), posit(2) {}


//! @brief Constructor.
XC::ReinfBar::ReinfBar(const double &barArea, Material *mat, const Vector &position)
  : DiscretBase(mat), diameter(2*sqrt(barArea/M_PI)), area(barArea), posit(position){}

//! @brief Constructor.
XC::ReinfBar::ReinfBar(const double &barArea,const double &barDiameter, Material *mat, const Vector &position)
  : DiscretBase(mat), diameter(barDiameter), area(barArea), posit(position){}


//! @brief Copy constructor.
XC::ReinfBar *XC::ReinfBar::getCopy(void) const
  { return new ReinfBar(*this); }

void XC::ReinfBar::setDiameter(const double &barDiameter)
  {
    diameter = barDiameter;
    area = M_PI * diameter*diameter/4.0;     
  }

void XC::ReinfBar::setArea(const double &barArea)
  { area = barArea; }

void XC::ReinfBar::setPosition (const Vector &position)
  { posit = position; }

//! @brief Return the bar diameter.
double XC::ReinfBar::getDiameter(void) const
  { return diameter; }

//! @brief Return the bar area.
double XC::ReinfBar::getArea(void) const
  { return area; }

//! @brief Return the moment of inertia with respect to the axis parallel to y axis through the centroid.
double XC::ReinfBar::Iy(void) const
  { return M_PI/4.0*pow((diameter/2.0),4.0); }

//! @brief Return the moment of inertia with respect to the axis parallel to z axis through the centroid.
double XC::ReinfBar::Iz(void) const
  { return Iy(); }

//! @brief Return the product of inertia with respect to the parallel axes through the centroid.
double XC::ReinfBar::Pyz(void) const
  { return 0.0; }

//! @brief Return the moment of inertia polar with respect to centroid in local coordinates.
double XC::ReinfBar::Ix(void) const
  { return Iy()+Iz(); }

//! @brief Return the angle that defines a principal axis of inertia.
double XC::ReinfBar::Theta_p(void) const
  { return 0.0; }

//! @brief Return the direcction of one of the principal axis of inertia
//! (we don't know yet if it's the major one or the minor one).
const XC::Vector &XC::ReinfBar::IAxisDir_a(void) const
  {
    static Vector retval(2);
    retval[0]= 1.0;
    retval[1]= 0.0;
    return retval;
  }

//! @brief Return the direcction of the other (with respect to IAxisDir_a)
//! principal axis of inertia (we don't know yet if it's the major
//! one or the minor one).
const XC::Vector &XC::ReinfBar::IAxisDir_b(void) const
  {
    static Vector retval(2);
    retval[0]= 0.0;
    retval[1]= 1.0;
    return retval;
  }

//     Line2d IAxis_a(void) const;
//     //Returns a principal axis of inertia (we don't know yet if it's
//     //the major one or the minor one).
//     Line2d IAxis_b(void) const;
//     //Return the other principal axis of inertia (we don't know yet if it's
//     //the major one or the minor one).
//     Ref2d2d principalAxesOfInertia(void) const;
//     //Return the principal axes of inertia.

//! @brief Return the principal major axis of inertia.
double XC::ReinfBar::getI1(void) const
  { return Iy(); }

//! @brief Return the principal minor axis of inertia.
double XC::ReinfBar::getI2(void) const
  { return Iz(); }

//     inline PrincipalAxesOfInertia2D Inertia(void)
//       { return PrincipalAxesOfInertia2D(getCenterOfMass(),Iy(),Iz(),Pyz()); }

//! @brief Return the (i,j) components of the inertia tensor computed with respect to the centroid.
double XC::ReinfBar::getI(const unsigned short int &i,const unsigned short int &j) const
  {
    if(i!=j)
      return 0.0;
    else
      return Iy();
  }

//! @brief Return the moment of inertia with respect to the axis
//! that passes through O with the direction of e.
double XC::ReinfBar::getI(const Pos2d &O,const Vector &e) const
  {
    const Matrix Io(getI(O));
    return dot(e,Io*e)/e.Norm2();
  }

//! @brief Return the (i,j) components of the inertia tensor computed with respect to the point "o".
double XC::ReinfBar::getI(const unsigned short int i,const unsigned short int j,const Pos2d &o) const
  {
    const double Iij= getI(i,j);
    const Vector center_of_mass= getPosition();

    Pos2d pp(center_of_mass[0],center_of_mass[1]);
    Ref2d2d axes(pp);
    Pos2d pos_local= axes.getLocalPosition(o);
    return Iij + getArea() * pos_local(i) * pos_local(j);
  }

//! @brief Return the polar moment of inertia with respect to the point o.
double XC::ReinfBar::getIO(const Pos2d &o) const
  { return (getI(1,1,o)+getI(2,2,o)+getI(3,3,o))/2; }

//! @brief Return the inertia tensor computed with respect to the object centroid.
XC::Matrix &XC::ReinfBar::getI(void) const
  {
    static Matrix i(2,2);
    i(0,0)= Iy(); i(0,1)= -Pyz();
    i(1,0)= i(0,1);   i(1,1)= Iz();
    return i;
  }

//! @brief Return the inertia tensor with respect to the point o.
XC::Matrix &XC::ReinfBar::getI(const Pos2d &o) const
  {
    static Matrix retval(2,2);
    Matrix Ig= getI();
    Vector O(2); O[0]= o.x(); O[1]= o.y();
    Vector og= getPosition() - O;
    double m= getArea();
    retval= Ig+m*(og.Norm2()*identity(Ig)-(og & og));
    return retval;
  }

const XC::Vector &XC::ReinfBar::getPosition(void) const
  { return posit; }

Pos2d XC::ReinfBar::getPos2d(void) const
  { return Pos2d(posit[0],posit[1]); }

double XC::ReinfBar::getMaxY(void) const
  { return posit(0)+(diameter/2); }
double XC::ReinfBar::getMaxZ(void) const
  { return posit(1)+(diameter/2); }
double XC::ReinfBar::getMinY(void) const
  { return posit(0)-(diameter/2); }
double XC::ReinfBar::getMinZ(void) const
  { return posit(1)-(diameter/2); }

void XC::ReinfBar::Print(std::ostream &s, int flag) const
  {
    s << "\nReinforcing Bar area: " << area;
    //s << "\nMaterial ID: " << getMaterialID();
    s << "\nDiameter: " << diameter;
    s << "\nArea: " << area;
    s << "\nPosition: " << posit;
  }


