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
//PrismaticBarCrossSection.h
                                                                        
                                                                        
#ifndef PrismaticBarCrossSection_h
#define PrismaticBarCrossSection_h

class PrincipalAxes2D;

#include "material/section/SectionForceDeformation.h"

class HalfPlane2d;
class Line2d;
class Pos2d;
class Vector2d;

namespace XC {

class DeformationPlane;

//! @brief Base class for beam-column cross sections.
//! Constitutive equations of the section.
//! @ingroup MATSCC
class PrismaticBarCrossSection: public SectionForceDeformation
  {
    
  public:
    PrismaticBarCrossSection(int tag,int classTag,MaterialHandler *mat_ldr= nullptr);
    
    int setTrialDeformationPlane(const DeformationPlane &);
    int setInitialDeformationPlane(const DeformationPlane &);
    virtual const Vector &getGeneralizedStrainVector(const DeformationPlane &) const;
    DeformationPlane getDeformationPlane(void) const;
    DeformationPlane getInitialDeformationPlane(void) const;
    virtual double getStrain(const double &y,const double &z= 0) const;

    double getN(void) const;
    double getMy(void) const;
    double getMz(void) const;

    virtual double getCenterOfMassY(void) const;
    virtual double getCenterOfMassZ(void) const;
    Pos2d getCenterOfMass(void) const;
    Line2d getNeutralAxis(void) const;
    Line2d getInternalForcesAxis(void) const;
    Pos2d getPointOnTensionedHalfPlane(void) const;
    Pos2d getPointOnCompressedHalfPlane(void) const;
    HalfPlane2d getTensionedHalfPlane(void) const;
    HalfPlane2d getTensionedHalfPlane(const Line2d &) const;
    HalfPlane2d getCompressedHalfPlane(void) const;
    HalfPlane2d getCompressedHalfPlane(const Line2d &) const;

    virtual const double &EA(void) const;
    virtual const double &EIz(void) const;
    virtual const double &EIy(void) const;
    virtual const double &EIyz(void) const;
    virtual const double &GAy(void) const;
    virtual const double &GAz(void) const;
    virtual const double &GJ(void) const;
    double getTheta(void) const;
    double getEI1(void) const;
    double getEI2(void) const;
    PrincipalAxes2D getInertiaAxes(void) const;
    Vector2d getAxis1VDir(void) const;
    Vector2d getVDirStrongAxis(void) const;
    Vector2d getAxis2VDir(void) const;
    Vector2d getVDirWeakAxis(void) const;

    bool hayAxil(const double &tol=1e-4) const;
    bool isSubjectedToShear(const double &tol=1e-4) const;
    bool isSubjectedToBending(const double &tol=1e-4) const;
    bool hayTorsor(const double &tol=1e-4) const;

  };

} // end of XC namespace


#endif
