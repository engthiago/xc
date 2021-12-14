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
                                                                        
// $Revision$
// $Date$
// $Source$
                                                                        
#ifndef Beam2dPartialUniformLoad_h
#define Beam2dPartialUniformLoad_h


// Written: fmk 

// Purpose: This file contains the class definition for Beam2dPartialUniformLoad.
#include "domain/load/beam_loads/BeamUniformLoad.h"

namespace XC {

//! @ingroup ElemLoads
//
//! @brief Uniform load partially distributed
//! over 2D beam elements.
class Beam2dPartialUniformLoad: public BeamUniformLoad
  {
  private:
    double aOverL;
    double bOverL;
    static Vector data;

    int parameterID;
  protected:
    int sendData(Communicator &comm);
    int recvData(const Communicator &comm);    
  public:
    Beam2dPartialUniformLoad(int tag, double wTrans, double wAxial, double aL, double bL, const ID &theElementTags);
    Beam2dPartialUniformLoad(int tag= 0);    

    const Vector &getData(int &type,const double &loadFactor) const;

    size_t getForceVectorDimension(void) const;
    size_t getMomentVectorDimension(void) const;

    int sendSelf(Communicator &comm);  
    int recvSelf(const Communicator &comm);
    
    void Print(std::ostream &s, int flag =0) const;       

    int setParameter(const std::vector<std::string> &argv, Parameter &param);
    int updateParameter(int parameterID, Information &info);
    int activateParameter(int paramID);

    const Vector &getSensitivityData(int gradNumber);
  
  };
} // end of XC namespace

#endif
