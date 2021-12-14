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
** (C) Copyright 2001, The Regents of the University of California    **
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
** Reliability module developed by:                                   **
**   Terje Haukaas (haukaas@ce.berkeley.edu)                          **
**   Armen Der Kiureghian (adk@ce.berkeley.edu)                       **
**                                                                    **
** ****************************************************************** */
                                                                        
// $Revision: 1.3 $
// $Date: 2003/10/27 23:04:40 $
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/domain/filter/StandardLinearOscillatorVelocityFilter.cpp,v $


//
// Written by Terje Haukaas (haukaas@ce.berkeley.edu)
//

#include <reliability/domain/filter/StandardLinearOscillatorVelocityFilter.h>
#include <reliability/domain/filter/Filter.h>
#include <classTags.h>
#include <cmath>


XC::StandardLinearOscillatorVelocityFilter::StandardLinearOscillatorVelocityFilter(int tag, double period, double dampingRatio)
:Filter(tag,FILTER_standardLinearOscillator)
  {
    double pi= M_PI;
    wn = 2*pi/period;
    xi = dampingRatio;
  }

double XC::StandardLinearOscillatorVelocityFilter::getAmplitude(double time) const
  {
    if(time<0.0)
      { return 0.0; }
    else
      {
	const double wd = wn * sqrt(1.0-pow(xi,2.0));
	return (  ( wd*cos(wd*time) - xi*wn*sin(wd*time) ) * exp(-xi*wn*time)  );
	    // Should maybe include the 1/mwd factor too
      }
  }

double XC::StandardLinearOscillatorVelocityFilter::getMaxAmplitude(void) const
  {
    //double wd = wn * sqrt(1.0-pow(xi,2.0));
    std::cerr << getClassName() << "::" << __FUNCTION__
              << "; ERROR: method not implemented for velocity filter."
	      << std::endl;
    const double result = 0.0;
    return result;
  }

double XC::StandardLinearOscillatorVelocityFilter::getTimeOfMaxAmplitude(void) const
  {
    //double wd = wn * sqrt(1.0-pow(xi,2.0));
    std::cerr << getClassName() << "::" << __FUNCTION__
              << "; ERROR: method not implemented for velocity filter."
	      << std::endl;
    return 0.0;
  }

//! @brief Print stuff.
void XC::StandardLinearOscillatorVelocityFilter::Print(std::ostream &s, int flag) const
  {}
