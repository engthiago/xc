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
// File: ~/system_of_eqn/eigenSOE/BandArpackSOE.C
//
// Written: Jun Peng
// Created: February 1999
// Revision: A
//
// Description: This file contains the class definition for XC::BandArpackSOE
// BandArpackSOE is a subclass of XC::ArpackSOE. It uses the LAPACK storage
// scheme to store the components of the K, M matrix, which is a full matrix.
// It uses the ARPACK to do eigenvalue analysis.

#include <solution/system_of_eqn/eigenSOE/BandArpackSOE.h>
#include <solution/system_of_eqn/eigenSOE/BandArpackSolver.h>
#include <utility/matrix/Matrix.h>
#include <solution/graph/graph/Graph.h>
#include <solution/graph/graph/Vertex.h>
#include <solution/graph/graph/VertexIter.h>

//! @brief Constructor.
XC::BandArpackSOE::BandArpackSOE(SolutionStrategy *owr, double theShift)
  :ArpackSOEBase(owr,EigenSOE_TAGS_BandArpackSOE,theShift),
   numSuperD(0), numSubD(0) {}

//! @brief Sets the solver that will be used to compute the solution.
bool XC::BandArpackSOE::setSolver(EigenSolver *newSolver)
  {
    bool retval= false;
    BandArpackSolver *tmp= dynamic_cast<BandArpackSolver *>(newSolver);
    if(tmp)
      {
        tmp->setEigenSOE(*this);
        retval= ArpackSOEBase::setSolver(tmp);
      }
    else
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; incompatible solver." << std::endl;
    return retval;
  }

//! @brief Sets the size of the system from the number of vertices in the graph.
int XC::BandArpackSOE::setSize(Graph &theGraph)
  {
    int result = 0;
    size= checkSize(theGraph);

    // determine the number of superdiagonals and subdiagonals
    theGraph.getBand(numSubD,numSuperD);

    const int newSize = size * (2*numSubD + numSuperD +1);
    if(newSize > A.Size())
      A.resize(newSize);
    A.Zero();
    factored = false;

    // invoke setSize() on the solver
    EigenSolver *theSolvr = this->getSolver();
    int solverOK = theSolvr->setSize();
    if(solverOK < 0)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
		  << "; WARNING: solver failed setSize()\n";
        return solverOK;
      }
    return result;
  }

//! @brief Assemblies in A the matrix being passed as parameter
//! multiplied by the fact parameter.
int XC::BandArpackSOE::addA(const Matrix &m, const ID &id, double fact)
  {
    // check for a quick return
    if(fact == 0.0)  return 0;

    // check that m and id are of same size
    const int idSize = id.Size();
    if(idSize != m.noRows() && idSize != m.noCols())
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
		  << "; Matrix and ID not of similar sizes"
	          << std::endl;
        return -1;
      }

    const int ldA = 2*numSubD + numSuperD + 1;

    if(fact == 1.0)
      { // do not need to multiply
        for(int i=0; i<idSize; i++)
          {
            const int col= id(i);
            if(col < size && col >= 0)
              {
		const int offset=  col*ldA + numSubD + numSuperD;
		if(offset<0)
		  {
		    std::cerr << getClassName() << "::" << __FUNCTION__
			      << "; negative offset due to integer overflow."
			      << std::endl;
		    return -1;
		  }
                double *coliiPtr= A.getDataPtr() + offset;
                for(int j=0; j<idSize; j++)
                  {
                    const int row = id(j);
                    if(row <size && row >= 0)
                      {
                        int diff= col - row;
                        if(diff > 0)
                          {
                            if(diff <= numSuperD)
                              {
                                double *APtr= coliiPtr - diff;
                                *APtr += m(j,i);
                              }
                          }
                        else
                          {
                            diff*= -1;
                            if(diff <= numSubD)
                              {
                                double *APtr= coliiPtr + diff;
                                *APtr += m(j,i);
                              }
                          }
                      }
                  }  // for j
              }
          }  // for i
      }
    else
      {
        for(int i=0;i<idSize;i++)
          {
            const int col = id(i);
            if(col < size && col >= 0)
              {
		const int offset=  col*ldA + numSubD + numSuperD;
		if(offset<0)
		  {
		    std::cerr << getClassName() << "::" << __FUNCTION__
			      << "; negative offset due to integer overflow."
			      << std::endl;
		    return -1;
		  }
                double *coliiPtr= A.getDataPtr() + offset;
                for(int j=0; j<idSize; j++)
                  {
                    const int row = id(j);
                    if(row <size && row >= 0)
                      {
                        int diff = col - row;
                        if(diff > 0)
                          {
                            if(diff <= numSuperD)
                              {
                                double *APtr = coliiPtr - diff;
                                *APtr+= m(j,i)*fact;
                              }
                          }
                        else
                          {
                            diff*= -1;
                            if(diff <= numSubD)
                              {
                                double *APtr = coliiPtr + diff;
                                *APtr+= m(j,i)*fact;
                              }
                          }
                      }
                  }  // for j
              }
          }  // for i
      }
    return 0;
  }

//! @brief Zeroes the matrix M.
void XC::BandArpackSOE::zeroA(void)
  {
    A.Zero();
    factored = false;
  }

//! @brief Assemblies in M the matrix being passed as parameter
//! multiplied by the fact parameter.
int XC::BandArpackSOE::addM(const Matrix &m, const ID &id, double fact)
  {
    bool retval= 0;
    //Added by LCPT.
    if(fact!=0.0)
      {
        const int idSize = id.Size();
        // check that m and id are of same size
        if(idSize != m.noRows() && idSize != m.noCols())
          {
            std::cerr << getClassName() << "::" << __FUNCTION__
		      << "; matrix and ID not of similar sizes\n";
            retval= -1;
          }
        else
          {
            resize_mass_matrix_if_needed(size);      
            int col= 0, row= 0;
            if(fact==1.0)
              {
                for(int i=0; i<idSize; i++)
                  for(int j=0; j<idSize; j++)
                    {
                      col= id(i);
                      row = id(j);
                      massMatrix(row,col)+= m(i,j);
                    }
              }
            else
              {
                for(int i=0; i<idSize; i++)
                  for(int j=0; j<idSize; j++)
                    {
                      col= id(i);
                      row = id(j);
                      massMatrix(row,col)+= m(i,j)*fact;
                    }
              }
          }
      }
    //Added by LCPT ends.
    retval= this->addA(m, id, -shift);
    return retval;
  }

int XC::BandArpackSOE::sendSelf(Communicator &comm)
  { return 0; }

int XC::BandArpackSOE::recvSelf(const Communicator &comm)
  { return 0; }

//! @brief Save the SOE matrices and vectors to file.
//!
//! Normally it's used to store temporarily those
//! objects on disk while executing getRCond to avoid
//! interferences with solve (different types of
//! factorization...).
void XC::BandArpackSOE::save(void) const
  {
    tmpFileName= std::tmpnam(nullptr);
    std::ofstream out(tmpFileName,std::ios::out|std::ios::binary);
    if(!out)
      std::cerr << getClassName() << "::" << __FUNCTION__
	        << "; cannot open file: "
		<< tmpFileName << std::endl;
    const size_t sz= A.Size();
    out.write((char *)(&sz), sizeof(sz));
    out.write((char *)(A.getDataPtr()),size);
    out.close();
  }

//! @brief Restore the SOE matrices and vectors from file.
//!
//! Normally it's used to restore those
//! objects from disk after executing getRCond to avoid
//! interferences with solve (different types of
//! factorization...).
void XC::BandArpackSOE::restore(void)
  {
    std::ifstream in(tmpFileName,std::ios::in|std::ios::binary);
    if(!in)
      std::cerr << getClassName() << "::" << __FUNCTION__
	        << "; cannot open file: " << tmpFileName
		<< std::endl;
    const size_t sz= A.Size();
    in.read((char *)(&sz), sizeof(sz));
    in.read((char *)(A.getDataPtr()),size);
    in.close();
    remove(tmpFileName.c_str()); //Not needed anymore;
    tmpFileName= "";
  }
