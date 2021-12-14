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
//ShellLinearCrdTransf3d.h

#ifndef ShellLinearCrdTransf3d_h
#define ShellLinearCrdTransf3d_h

#include "ShellCrdTransf3dBase.h"

namespace XC {

class Matrix;

//! @ingroup PlaneElements
//
//! @brief Base class for small displacement 3D coordinate transformations.
class ShellLinearCrdTransf3d: public ShellCrdTransf3dBase
  {
  public:
    ShellLinearCrdTransf3d(void);
    ShellLinearCrdTransf3d(const Vector &,const Vector &,const Vector &);
    ShellLinearCrdTransf3d(const NodePtrs &t);
    virtual ShellCrdTransf3dBase *getCopy(void) const;

    virtual int initialize(const NodePtrs &);
    virtual int update(void);

    virtual int commitState(void);
    virtual int revertToLastCommit(void);        
    virtual int revertToStart(void);
    
    virtual Vector getBasicTrialDisp(const int &) const;
    virtual Vector getBasicTrialVel(const int &) const;
    virtual Vector getBasicTrialAccel(const int &) const;

    int sendSelf(Communicator &);
    int recvSelf(const Communicator &);
  };

} // end of XC namespace

#endif
