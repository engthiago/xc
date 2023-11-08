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
//DeformationPlane

#ifndef DEFORMATION_PLANE_H
#define DEFORMATION_PLANE_H

#include <iostream>
#include "utility/geom/d2/Plane.h"
#include "utility/actor/actor/MovableObject.h"

class Pos2d;
class Pos3d;
class Line2d;
class HalfPlane2d;

namespace XC {

class Vector;
class ResponseId;

//! @ingroup MATSCC
//!
//! @defgroup MATSCCDiagInt Iteraction diagram for a cross-section.
//
//! @ingroup MATSCCDiagInt
//
//! @brief Deformation plane for a cross-section.
class DeformationPlane: public Plane, public MovableObject
  {
  protected:
    bool check_positions(const Pos2d &,const Pos2d &, const Pos2d &);
    bool check_positions(const Pos3d &,const Pos3d &, const Pos3d &);
    int sendData(Communicator &);
    int recvData(const Communicator &);
    friend class SectionForceDeformation;
    friend class FiberPtrDeque;
    friend class FiberSectionBase; //Erase this line.
  public:
    DeformationPlane( const Pos3d &p1,const Pos3d &p2, const Pos3d &p3);
    explicit DeformationPlane(const Plane &);
    DeformationPlane( const Pos2d &yz1, const double &e_1, //Strains at three fiber sections.
                      const Pos2d &yz2, const double &e_2,
                      const Pos2d &yz3, const double &e_3);
    explicit DeformationPlane(const double &eps= 0.0);
    DeformationPlane(const Vector &);

    double Strain(const Pos2d &p) const;
    const Vector &getDeformation(void) const;
    void setDeformation(const Vector &);
    const Vector &getDeformation(const size_t &,const ResponseId &) const;
    boost::python::list getGeneralizedStrainsPy(const boost::python::list &) const;

    void ConstantStrain(const double &);

    Line2d getNeutralAxis(void) const;
    Pos2d getPointOnTensionedHalfPlane(void) const;
    Pos2d getPointOnCompressedHalfPlane(void) const;
    HalfPlane2d getTensionedHalfPlane(void) const;
    HalfPlane2d getTensionedHalfPlane(const Line2d &) const;
    HalfPlane2d getCompressedHalfPlane(void) const;
    HalfPlane2d getCompressedHalfPlane(const Line2d &) const;


    int sendSelf(Communicator &);
    int recvSelf(const Communicator &);
  };

} // end of XC namespace

#endif
