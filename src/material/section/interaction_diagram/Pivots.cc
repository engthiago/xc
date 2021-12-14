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
//Pivots.cc

#include "Pivots.h"
#include "ComputePivots.h"


bool XC::Pivots::checkPositions(void) const
  {
    bool retval= true;
    const double dMin= 1e-4;
    const Pos2d pA= getAPivotPosition();
    const Pos2d pB= getBPivotPosition();
    const Pos2d pC= getCPivotPosition();
    const Pos2d pD= getPointDPosition();
    const double dAB= dist(pA,pB);
    if(dAB<dMin)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
		  << "; pivots A= " << pA << " and B= " 
                  << pB << " too close; d= " << dAB << std::endl;
        retval= false;
      }
    const double dAC= dist(pA,pC);
    if(dAC<dMin)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
		  << "; pivots A= " << pA << " and C= " 
                  << pC << " too close; d= " << dAC << std::endl;
        retval= false;
      }
    // const double dAD= dist(pA,pD); 

    const double dBC= dist(pB,pC);
    if(dBC<dMin)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
		  << "; pivots B= " << pB << " and C= " 
                  << pC << " too close; d= " << dBC << std::endl;
        retval= false;
      }
    const double dBD= dist(pB,pD);
    if(dBD<dMin)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
		  << "; pivot B= " << pB << " and point D= " 
                  << pD << " too close; d= " << dBD << std::endl;
        retval= false;
      }
    const double dCD= dist(pC,pD);
    if(dCD<dMin)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
		  << "; pivot C= " << pC << " and point D= " 
                  << pD << " too close; d= " << dCD << std::endl;
        retval= false;
      }
    return retval;
  }

//! @brief Constructor.
XC::Pivots::Pivots(const Pos3d &a,const Pos3d &b,const Pos3d &c,const Pos3d &d)
  : A(a), B(b), C(c), D(d) 
  { ok= checkPositions(); }

//! @brief Constructor.
XC::Pivots::Pivots(const ComputePivots &cp)
  : A(cp.calcPositionPivotA()), B(cp.calcPositionPivotB()), C(cp.calcPositionPivotC()), D(cp.getDPoint())
  { ok= checkPositions(); }

//! @brief Print pivots definition
void XC::Pivots::print(std::ostream &os) const
  {
    os << "pos(A): " << getAPivotPosition() << " epsilon(A)= "
       << getEpsilonA() << std::endl;
    os << "pos(B): " << getBPivotPosition() << " epsilon(B)= "
       << getEpsilonB() << std::endl;
    os << "pos(C): " << getCPivotPosition() << " epsilon(C)= "
       << getEpsilonC() << std::endl;
    os << "pos(D): " << getPointDPosition() << " epsilon(D)= "
       << getEpsilonD() << std::endl;
  }
