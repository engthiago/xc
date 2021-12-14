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
//PYBase.h

#ifndef PYBASE_H
#define PYBASE_H

#include "material/uniaxial/UniaxialMaterial.h"
#include "InternalParamsIn.h"
#include "InternalParamsLRIn.h"

namespace XC {
//! @ingroup MatUnx
//
//! @degroup PYMat
//! @ingroup PYMat
//
//! @brief Base class for the materials that represent a p-y curve that
//! simulates the soil resistance as predefined nonlinear springs, where
//! p is the soil pressure per unit length and y is the deflection.
class PYBase: public UniaxialMaterial
  {
  protected:
    InternalParamsA C; //!< Committed history variables for entire Q-z material.
    InternalParamsA T; //!< Trial history variables for entire p-y material.
    double TvRate; //!< Trial velocity
    InternalParamsA CFar; //!< Committed internal parameters for the Far Field component
    InternalParamsA TFar; //!< Trial internal parameters for the Far Field component

    // Material parameters
    double initialTangent;
    int soilType; //!< Soil type.
    double matCapacity; //!< Material capacity.
    double v50; //!< y (or z) at 50% of matCapacity.
    double vRef; //!< (y or z) reference point for Near Field component
    double np; //!< exponent for hardening shape of Near Field component
    double dashpot; //!< dashpot on the far-field (elastic) component

    void getFarField(const double &y);

    int sendData(Communicator &);
    int recvData(const Communicator &);


  public:
    PYBase(int tag, int classtag, int soilType, double mCap, double v50, double dashpot);
    PYBase(int tag, int classtag);
    PYBase(void);

    double getTangent(void) const;
    double getInitialTangent(void) const;
    void setInitialTangent(const double &);
    double getStrain(void) const;
    double getStrainRate(void) const;

    void setSoilType(const int &);
    int getSoilType(void) const;
    void setUltimateCapacity(const double &);
    double getUltimateCapacity(void) const;
    void setY50(const double &);
    double getY50(void) const;
    void setDashPot(const double &);
    double getDashPot(void) const;

    int commitState(void);
    int revertToLastCommit(void);

    void Print(std::ostream &s, int flag =0) const;

  };
} // end of XC namespace


#endif
