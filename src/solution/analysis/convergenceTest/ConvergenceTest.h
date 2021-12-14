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
                                                                        
// $Revision: 1.4 $
// $Date: 2005/12/15 00:19:28 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/convergenceTest/ConvergenceTest.h,v $
                                                                        
                                                                        
#ifndef ConvergenceTest_h
#define ConvergenceTest_h

// Written: fmk 
// Date: 09/98
// Revised:
//
// Purpose: This file contains the class definition for ConvergenceTest,
// which is an abstract class. Objects of concrete subclasses can be used 
// to test the convergence of an algorithm. 

#include <utility/actor/actor/MovableObject.h>
#include "utility/kernel/EntityWithOwner.h"
#include "utility/matrix/Vector.h"
#include "solution/system_of_eqn/linearSOE/LinearSOE.h"

namespace XC {
class EquiSolnAlgo;
class SolutionStrategy;

//! @ingroup CTest
//
//! @brief convergence test.
//!
//! A ConvergenceTest object is an object which can be used in an
//! algorithmic class to test if convergence has been achieved for an 
//! iteration. The ConvergenceTest class is an abstract class, defining
//! the interface that all subclasses must provide.
class ConvergenceTest: public MovableObject, public EntityWithOwner
  {
    SolutionStrategy *getSolutionStrategy(void);
    const SolutionStrategy *getSolutionStrategy(void) const;
  protected:
    int currentIter; //!< number of times test() has been invokes since last start()
    int maxNumIter;  //!< max number of iterations
    int printFlag; //!< a flag indicating if to print on test
    int nType; //!< type of norm to use (1-norm, 2-norm, p-norm, max-norm)
    Vector norms; //!< vector to hold the norms

    //Intermediate results:
    mutable double lastRatio; //!< Last calculated ratio (if any).
    mutable double calculatedNormX; //!< Last calculated |x|
    mutable double calculatedNormB; //!< Last calculated |b|
    mutable double calculatedEnergyProduct; //!< Last calculated |0.5*(x ^ b)|.

    bool hasLinearSOE(void) const;
    LinearSOE *getLinearSOEPtr(void);
    const LinearSOE *getLinearSOEPtr(void) const;
    const Vector &getX(void) const;
    double getNormX(void) const;
    const Vector &getB(void) const;
    double getNormB(void) const;
    double getEnergyProduct(void) const;

  protected:
    int sendData(Communicator &);
    int recvData(const Communicator &);

  public:
    ConvergenceTest(CommandEntity *owr,int classTag);	
    ConvergenceTest(CommandEntity *owr,int clasTag,int maxIter,int prtFlg, int normType,int sz_norms= 1);
    inline virtual ~ConvergenceTest(void) {}

    virtual ConvergenceTest *getCopy(void) const= 0;
    virtual ConvergenceTest *getCopy(int iterations) const;

    virtual std::string getStatusMsg(const int &flag= 1) const= 0;
    virtual int start(void);
    //! @brief Return a positive number if convergence is achieved.
    //! 
    //! To return a positive number if the convergence criteria defined for the
    //! object has been satisfied, the positibe number equal to the number of
    //! times since \p start that test() has been invoked. Otherwise a negative
    //! number is to be returned. A {\em -2} is returned if the test fails to
    //! meet the criteria and no more tests are to be 
    //! performed due to limits set, i.e. the maximum number of iterations,
    //! otherwise a {\em -1} is to be returned.
    virtual int test(void)= 0;

    int getMaxNumIter(void) const;
    void setMaxNumIter(const int &);
    int getCurrentIter(void) const;
    void setCurrentIter(const int &);
    int getPrintFlag(void) const;
    void setPrintFlag(const int &);
    int getNormType(void) const;
    void setNormType(const int &);
        
    virtual int getNumTests(void) const;
    virtual int getMaxNumTests(void) const;        
    virtual double getRatioNumToMax(void) const;            
    virtual const Vector &getNorms(void) const;
    int sendSelf(Communicator &);
    int recvSelf(const Communicator &);

    std::string getTestIterationMessage(void) const;
    std::string getFailedToConvergeMessage(void) const;
    std::string getDeltaXRMessage(void) const;
    std::string getDeltaXRNormsMessage(void) const;
  };
} // end of XC namespace


#endif

