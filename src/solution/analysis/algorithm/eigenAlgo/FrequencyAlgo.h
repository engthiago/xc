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
                                                                        
// $Revision: 1.2 $
// $Date: 2003/02/14 23:00:41 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/algorithm/eigenAlgo/FrequencyAlgo.h,v $
                                                                        
                                                                        
// File: ~/analysis/algorithm/eigenAlgo/FrequencyAlgo.h
//
// Written: Jun Peng
// Created: Wed Feb. 8, 1999
// Revision: A
//
// Description: This file contains the class definition of FrequencyAlgo.
// FrequencyAlgo is a class which performs a eigen solution algorithm
// to solve the Generalized eigen equations. It is not expected that 
// this class will have subclasses.
//
// This class is inherited from the base class of SolutionAlgorithm
// which was created by fmk (Frank).


#ifndef FrequencyAlgo_h
#define FrequencyAlgo_h

#include <solution/analysis/algorithm/eigenAlgo/EigenAlgorithm.h>

namespace XC {

//! @ingroup EigenAlgo
//
//! @brief Algorithm to obtain the natural frequencies of the model.
class FrequencyAlgo: public EigenAlgorithm
  {
    friend class SolutionStrategy;
    FrequencyAlgo(SolutionStrategy *);
    virtual SolutionAlgorithm *getCopy(void) const;
  public:
     
    virtual int solveCurrentStep(int numModes);
     
    virtual int sendSelf(Communicator &);
    virtual int recvSelf(const Communicator &);
     
    virtual void Print(std::ostream &s, int flag = 0) const;
  };
inline SolutionAlgorithm *FrequencyAlgo::getCopy(void) const
  { return new FrequencyAlgo(*this); }
} // end of XC namespace

#endif


