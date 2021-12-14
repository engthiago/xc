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
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/domain/spectrum/JonswapSpectrum.cpp,v $


//
// Written by Terje Haukaas (haukaas@ce.berkeley.edu)
//

#include "JonswapSpectrum.h"
#include <utility/matrix/Vector.h>
#include <classTags.h>
#include <cmath>


XC::JonswapSpectrum::JonswapSpectrum(int tag, double min, double max,
								 double alp, double w, double gam)
:Spectrum(tag,SPECTRUM_jonswap)
{
	minFreq = min;
	maxFreq = max;
	alpha = alp;
	wp = w;
	gamma = gam;
}

//! @brief Print stuff.
void XC::JonswapSpectrum::Print(std::ostream &s, int flag) const
  {}


double
XC::JonswapSpectrum::getMinFrequency()
{
	return minFreq;
}
double
XC::JonswapSpectrum::getMaxFrequency()
{
	return maxFreq;
}
double
XC::JonswapSpectrum::getAmplitude(double frequency)
{
	if (frequency < minFreq  ||  frequency > maxFreq) {
		return 0.0;
	}

	double sigma;
	if (frequency < wp) {
		sigma = 0.07;
	}
	else {
		sigma = 0.09;
	}

	double power = exp(-(frequency-wp)/(2.0*sigma*sigma*wp*wp));
	double GAMMA = pow(gamma,power);

	return GAMMA*alpha*pow(frequency,-5.0)*exp(-5.0/4.0*pow((frequency/wp),-4.0));
}
