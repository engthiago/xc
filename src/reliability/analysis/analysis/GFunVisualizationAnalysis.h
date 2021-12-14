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
// $Date: 2003/10/27 23:45:41 $
// $Source: /usr/local/cvs/OpenSees/SRC/reliability/analysis/analysis/GFunVisualizationAnalysis.h,v $


//
// Written by Terje Haukaas (haukaas@ce.berkeley.edu)
//

#ifndef GFunVisualizationAnalysis_h
#define GFunVisualizationAnalysis_h

#include <reliability/analysis/analysis/ReliabilityAnalysis.h>
#include <reliability/analysis/designPoint/FindDesignPointAlgorithm.h>
#include <reliability/analysis/transformation/ProbabilityTransformation.h>
#include <reliability/domain/components/ReliabilityDomain.h>
#include <reliability/analysis/meritFunction/MeritFunctionCheck.h>
#include <reliability/analysis/convergenceCheck/ReliabilityConvergenceCheck.h>
#include <reliability/analysis/rootFinding/RootFinding.h>


namespace XC {

class GFunEvaluator;
class GradGEvaluator;

//! @ingroup ReliabilityAnalysis
//!
//! @brief ??
class GFunVisualizationAnalysis: public ReliabilityAnalysis
{
private:

	Vector getCurrentAxes12Point(int i, int j);
	Vector getCurrentAxes3Point(int i, int j);
	double findGSurface(Vector thePoint);
	double evaluateGFunction(Vector thePoint, bool printDivision);





	ReliabilityDomain *theReliabilityDomain;
	GFunEvaluator *theGFunEvaluator;
	ProbabilityTransformation *theProbabilityTransformation;
	MeritFunctionCheck *theMeritFunctionCheck;
	GradGEvaluator *theGradGEvaluator;
	ReliabilityConvergenceCheck *theReliabilityConvergenceCheck;
	Vector *theStartPoint;
	RootFinding *theRootFindingAlgorithm;

	std::string outputFileName;
	std::string convFileName;
	int convResults;
	int space;
	int funSurf;
	int axes;
	int dir;

	int rvDir;
	Vector theDirectionVector;
	int rv1, rv2;
	double from1, interval1;
	double from2, interval2;
	int numPts1, numPts2;
	Matrix theMatrix;
	int numLinePts;

	int nrv;

	double scaleValue;
public:
	GFunVisualizationAnalysis(
					ReliabilityDomain *theReliabilityDomain,
					GFunEvaluator *theGFunEvaluator,
					ProbabilityTransformation *theProbabilityTransformation,
					const std::string &outputFileName,
					const std::string &convFileName,
					int convResults,
					int space,
					int funSurf,
					int axes,
					int dir);

	int analyze(void);

	int setDirection(int rvDir);
	int setDirection(Vector theDirectionVector);
	
	int setAxes(Vector axesVector);
	int setAxes(Matrix theMatrix);
	int setNumLinePts(int numLinePts);
	
	int setRootFindingAlgorithm(RootFinding *theRootFinder);
	int setStartPoint(Vector *theStartPoint);
	int setGradGEvaluator(GradGEvaluator *theGradGEvaluator);
	int setMeritFunctionCheck(MeritFunctionCheck *theMeritFunctionCheck);
	int setReliabilityConvergenceCheck(ReliabilityConvergenceCheck *theReliabilityConvergenceCheck);


};
} // end of XC namespace

#endif
