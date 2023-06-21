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
//ArcLengthBase.h

#ifndef ArcLengthBase_h
#define ArcLengthBase_h

#include "solution/analysis/integrator/static/arc-length/ProtoArcLength.h"

namespace XC {
class LinearSOE;
class AnalysisModel;
class FE_Element;
class Vector;

//! @ingroup ProtoArcLength
//
//! @brief Base class for the arc-length integrators family.
class ArcLengthBase: public ProtoArcLength
  {
  protected:
    double alpha2; //!< squared value of the scaling factor
                   //! on the reference loads.


    double getDLambdaNewStep(void) const;
    int sendData(Communicator &);
    int recvData(const Communicator &);

    ArcLengthBase(SolutionStrategy *,int classTag,double ArcLength, double alpha = 1.0);
  public:

    int sendSelf(Communicator &);
    int recvSelf(const Communicator &);
  };
} // end of XC namespace

#endif

