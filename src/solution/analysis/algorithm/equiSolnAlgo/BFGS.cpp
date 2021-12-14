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
                                                                        
// $Revision: 1.5 $
// $Date: 2005/11/29 22:42:41 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/algorithm/equiSolnAlgo/BFGS.cpp,v $
                                                                        
// Written: Ed Love
// Created: 06/01

// What: "@(#)BFGS.cpp, revA"

#include <solution/analysis/algorithm/equiSolnAlgo/BFGS.h>
#include <solution/system_of_eqn/linearSOE/LinearSOE.h> 
#include <solution/analysis/convergenceTest/ConvergenceTest.h>
#include "solution/SolutionStrategy.h"

//! @brief Constructor
XC::BFGS::BFGS(SolutionStrategy *owr,int theTangentToUse, int n)
  :BFBRoydenBase(owr,EquiALGORITHM_TAGS_BFGS,theTangentToUse,n) {}

//! @brief Virtual constructor.
XC::SolutionAlgorithm *XC::BFGS::getCopy(void) const
  { return new BFGS(*this); }

//! @brief Constructor.
XC::BFGS::BFGS(SolutionStrategy *owr,ConvergenceTest &theT, int theTangentToUse, int n)
  :BFBRoydenBase(owr,EquiALGORITHM_TAGS_BFGS,theT,theTangentToUse,n) {}

//! @brief resuelve el paso actual.
int XC::BFGS::solveCurrentStep(void)
  {
    AnalysisModel *theModel = getAnalysisModelPtr(); 
    LinearSOE *theSysOfEqn = getLinearSOEPtr();
    IncrementalIntegrator *theIntegrator= getIncrementalIntegratorPtr();
    ConvergenceTest *theTest= getConvergenceTestPtr();
 
    if((theModel == 0) || (theIntegrator == 0) || (theSysOfEqn == 0) || (theTest == 0))
      {
	std::cerr << getClassName() << "::" << __FUNCTION__
	          << "; WARNING - setLinks() "
		  << " analysis model, integrator or sytem of equations "
	          << " not assigned yet." << std::endl;
	return -5;
      }	

    // set itself as the ConvergenceTest objects EquiSolnConvAlgo
    theTest->set_owner(getSolutionStrategy());
    if(theTest->start() < 0)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
		  << "; the ConvergenceTest object failed in start()\n";
        return -3;
      }

    localTest->set_owner(getSolutionStrategy());

    rdotz.resize(numberLoops+3);
    sdotr.resize(numberLoops+3);

    int result = -1;
    int count = 0;
    do
      {
        // std::cerr << getClassName() << "::" << __FUNCTION__
	//           << "      BFGS -- Forming New_ Tangent" << std::endl;
        //form the initial tangent
        if(theIntegrator->formTangent(tangent) < 0)
          {
            std::cerr << getClassName() << "::" << __FUNCTION__
		      << "; WARNING the Integrator failed in formTangent()\n";
            return -1; 
          }

        //form the initial residual 
        if(theIntegrator->formUnbalance() < 0)
          {
            std::cerr << getClassName() << "::" << __FUNCTION__
		      << "; WARNING the Integrator failed in formUnbalance()\n";	
          }	    

        //solve
        if(theSysOfEqn->solve() < 0)
          {
	    std::cerr << getClassName() << "::" << __FUNCTION__
		      << "; WARNING the LinearSysOfEqn failed in solve()\n";	
	    return -3;
	  }	    

        //update
        if(theIntegrator->update(theSysOfEqn->getX() ) < 0)
          {
	    std::cerr << getClassName() << "::" << __FUNCTION__
		      << "; WARNING the Integrator failed in update()\n";	
	    return -4;
          }	        


        //    int systemSize = ( theSysOfEqn->getB() ).Size();
        int systemSize = theSysOfEqn->getNumEqn( );

        //temporary vector
        if(temp.isEmpty())
	  temp= Vector(systemSize);

        //initial displacement increment
        s[1]= theSysOfEqn->getX( );


        residOld= theSysOfEqn->getB( ) ;
        residOld*= (-1.0 );

        //form the residual again
        if(theIntegrator->formUnbalance() < 0)
          {
            std::cerr << getClassName() << "::" << __FUNCTION__
		      << "; WARNING the integrator failed in formUnbalance()\n";	
          }	    

        if(residNew.isEmpty()) 
	  residNew= Vector(systemSize);
 
        if(du.isEmpty()) 
	  du= Vector(systemSize);

        if(b.isEmpty())
	  b= Vector(systemSize);

        localTest->start();

        int nBFGS = 1;
        do
          {

            //save residual
            residNew=  theSysOfEqn->getB( ); 
            residNew*= (-1.0 );

      
            //solve
            if(theSysOfEqn->solve() < 0)
              {
	        std::cerr << getClassName() << "::" << __FUNCTION__
			  << "; WARNING the LinearSysOfEqn failed in solve()\n";	
	        return -3;
              }	    

	    //save right hand side
            b= theSysOfEqn->getB( );

            //save displacement increment
            du= theSysOfEqn->getX( );

            //BFGS modifications to du
            BFGSUpdate(theIntegrator, theSysOfEqn, du, b, nBFGS );

            if(theIntegrator->update(du) < 0)
              {
	        std::cerr << getClassName() << "::" << __FUNCTION__
			  << "; WARNING the integrator failed in update()\n";	
	        return -4;
              }	        

	    /* std::cerr << getClassName() << "::" << __FUNCTION__ 
                         << "        BFGS Iteration " << nBFGS 
                << " Residual Norm = " 
                << sqrt( (*residNew) ^ (*residNew) ) << std::endl;
	    */
        
            //increment broyden counter
            nBFGS += 1;

            //save displacement increment
            s[nBFGS]= du;

            //swap residuals
	    residOld= residNew;

            //form the residual again
            if(theIntegrator->formUnbalance() < 0)
              {
                std::cerr << getClassName() << "::" << __FUNCTION__
			  << "; WARNING the integrator failed in formUnbalance()\n";	
              }	    
            result = localTest->test();
          
          }
        while(result == -1 && nBFGS <= numberLoops );

        result = theTest->test();
        this->record(count++); //Calls the record(...) method of all recorders.
      }
    while(result == -1);

    if(result == -2)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
		  << "; the ConvergenceTest object failed in test()\n"
                  << "Convergence test message: "
		  << theTest->getStatusMsg(1) << std::endl;	  
        return -3;
      }
    // note - if positive result we are returning what the convergence test returned
    // which should be the number of iterations
    return result;
  }

//! @brief Actualiza.
void  XC::BFGS::BFGSUpdate(IncrementalIntegrator *theIntegrator, LinearSOE *theSOE, Vector &du, Vector &b, int nBFGS)
  {
    static const double eps = 1.0e-16;


    //compute z
    //  theSOE->setB( (*r[nBFGS]) - (*r[nBFGS-1]) );
    //    theSOE->setB( (residNew) - (residOld) );
    temp= residNew;
    temp-= residOld;
    theSOE->setB(temp);


    if(theSOE->solve() < 0)
        std::cerr << getClassName() << "::" << __FUNCTION__
		  << "; WARNING the LinearSysOfEqn failed in solve()\n";	
  
    z[nBFGS]= theSOE->getX(); 
    //  z[nBFGS] *= (-1.0);

    int i;
    for(i=1; i<=(nBFGS-1); i++ )
      {

        if(sdotr[i] < eps ) 
          break; 

        double fact1 = 1.0 + ( rdotz[i] / sdotr[i] );
        fact1/= sdotr[i];
        const double pdotb= (s[i]) ^ ( theSOE->getB() );
        fact1*= pdotb;

        //    z[nBFGS] +=  fact1 * ( s[i] );
        temp= s[i];
        temp*= fact1;
        z[nBFGS]+= temp;


        const double bdotz= (z[i]) ^ ( theSOE->getB() );  

        //    z[nBFGS] -= (1.0/sdotr[i]) * 
        //             ( bdotz * (s[i])   +  pdotb * (z[i]) );   
        temp= s[i];
        temp*= bdotz;
        temp/= sdotr[i];
        z[nBFGS]-= temp;

        temp= z[i];
        temp*= pdotb;
        temp/= sdotr[i];
        z[nBFGS]-= temp;
      } //end for i


    //sdotr[nBFGS] = s[nBFGS] ^ ( residNew - residOld );

    //rdotz[nBFGS] = z[nBFGS] ^ ( residNew - residOld );   

      temp= residNew;
      temp-= residOld;

      sdotr[nBFGS]= s[nBFGS]^temp;
      rdotz[nBFGS]= z[nBFGS]^temp;


    //BFGS modifications to du
    for(i=1;i<=nBFGS;i++ )
      {
        if(sdotr[i] < eps )
          break;

        double fact1 = 1.0 + ( rdotz[i] / sdotr[i] );
        fact1/= sdotr[i];

        const double sdotb = (s[i]) ^ b;
        fact1*= sdotb;

        //du +=  fact1 * ( s[i] );
        temp= s[i];
        temp*= fact1;
        du+= temp;


        const double bdotz = (z[i]) ^ b;  

        //du -= (1.0/sdotr[i]) * 
        //             ( bdotz * (s[i])   +  sdotb * (z[i]) );   
        temp= s[i];
        temp*= bdotz;
        temp/= sdotr[i];
        du-= temp;

        temp= z[i];
        temp*= sdotb;
        temp/= sdotr[i];
        du-= temp;
      } //end for i
  }


int XC::BFGS::sendSelf(Communicator &comm)
  { return -1; }

int XC::BFGS::recvSelf(const Communicator &comm)
  { return -1; }


void XC::BFGS::Print(std::ostream &s, int flag) const
  {
    if(flag == 0)
      {
        s << "BFGS" << std::endl;
        s << "  Number of Iterations = " << numberLoops << std::endl;
      }
  }
