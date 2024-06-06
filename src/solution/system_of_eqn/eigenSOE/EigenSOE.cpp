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
// File: ~/system_of_eqn/eigenSOE/EigenSOE.C
//
// Written: Jun Peng
// Created: Sat Feb. 6, 1999
// Revision: A
//
// Description: This file contains the class definition of XC::EigenSOE.
// EigenSOE is a subclass of XC::SystemOfEqn.
// It has pure virtual functions which must be implemented in it's derived
// subclasses. To solve the genreal eigen value equations means that
// by the given K and M, find the corresponding eigen value and eigen
// vectors.
//
// This class is inherited from the base class of XC::SystemOfEqn
// which was created by fmk (Frank).


#include <solution/system_of_eqn/eigenSOE/EigenSOE.h>
#include <solution/system_of_eqn/eigenSOE/EigenSolver.h>
#include <solution/analysis/model/AnalysisModel.h>

#include <solution/system_of_eqn/eigenSOE/BandArpackSolver.h>
#include <solution/system_of_eqn/eigenSOE/BandArpackppSolver.h>
#include <solution/system_of_eqn/eigenSOE/SymArpackSolver.h>
#include <solution/system_of_eqn/eigenSOE/SymBandEigenSolver.h>
#include <solution/system_of_eqn/eigenSOE/FullGenEigenSolver.h>
#ifdef USE_SPECTRA
#include <solution/system_of_eqn/eigenSOE/SpectraSolver.h>
#endif



#include "boost/numeric/ublas/io.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "utility/matrix/Matrix.h"

//! @brief Constructor.
XC::EigenSOE::EigenSOE(SolutionStrategy *owr,int classTag)
  :SystemOfEqn(owr,classTag), size(0), factored(false), theSolver(nullptr) {}

void XC::EigenSOE::free_mem(void)
  {
    if(theSolver)
      {
        delete theSolver;
        theSolver= nullptr;
      }
  }

void XC::EigenSOE::copy(const EigenSolver *newSolver)
  {
    if(newSolver)
      {
        free_mem();
        EigenSolver *tmp= newSolver->getCopy();
        if(tmp)
          setSolver(tmp);
        else
	  std::cerr << getClassName() << "::" << __FUNCTION__
		    << "; can't allocate the solver."
                    << std::endl;
      }
  }

//! @brief Set the solver.
bool XC::EigenSOE::setSolver(EigenSolver *newSolver)
  {
    bool retval= false;
    if(newSolver)
      {
        free_mem();
        theSolver= newSolver;
        theSolver->setEigenSOE(this);
        const int solverOK= theSolver->setSize();
        if(solverOK < 0)
          {
            std::cerr << getClassName() << "::" << __FUNCTION__
		      << "; WARNING: solver failed setSize() in constructor\n";
          }
        retval= true;
      }
    else
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; pointer to solver is null." << std::endl;
    return retval;
  }

//! @brief Returns the number of equations.
int XC::EigenSOE::getNumEqn(void) const
  { return size; }

//! @brief Set the solver to use
//! @param type: solver type to use.
XC::EigenSolver &XC::EigenSOE::newSolver(const std::string &type)
  {
    if((type=="band_arpack_solver") || (type=="band_arpack_eigen_solver"))
      setSolver(new BandArpackSolver());
    else if((type=="band_arpackpp_solver") || (type=="band_arpackpp_eigen_solver"))
      setSolver(new BandArpackppSolver());
#ifdef USE_SPECTRA
    else if((type=="spectra_solver") || (type=="spectra_eigen_solver"))
      setSolver(new SpectraSolver());
#endif
    else if((type=="sym_band_eigen_solver") || (type=="sym_band_lapack_solver"))
      setSolver(new SymBandEigenSolver());
    else if(type=="full_gen_eigen_solver")
      setSolver(new FullGenEigenSolver());
    else if((type=="sym_arpack_solver") || (type=="band_arpack_eigen_solver"))
      setSolver(new SymArpackSolver());
    else
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; Solver of type: '"
                << type << "' unknown." << std::endl;
    assert(theSolver);
    return *theSolver;
  }

//! @brief Destructor.
XC::EigenSOE::~EigenSOE(void)
  { free_mem(); }

void XC::EigenSOE::resize_mass_matrix_if_needed(const size_t &sz)
  {
    if((massMatrix.size1() != sz) || (massMatrix.size2() != sz))
      massMatrix= sparse_matrix(sz,sz,0.0);
  }

//! @brief Solve the eigenproblem con the number of modos passed as parameter.
int XC::EigenSOE::solve(int numModes)
  { return (theSolver->solve(numModes)); }

//! @brief Do nothing.
int XC::EigenSOE::solve(void)
  {
    std::cerr << getClassName() << "::" << __FUNCTION__
              << "; ERROR - need to specify numModes\n";
    return -1;
  }

//! @brief Returns the determinant of the system matrix.
double XC::EigenSOE::getDeterminant(void)
  { return theSolver->getDeterminant(); }

//! @brief Returns the reciprocal of the condition number.
//!
//! Computes the reciprocal of the condition number using
//! the norm specified by the char argument:
//! if norm = '1' or 'O', then the routine estimates the condition number
//! of matrix in 1-norm, if norm = 'I', then the routine estimates the
//! condition number of matrix in infinity-norm.
double XC::EigenSOE::getRCond(const char &norm)
  { return theSolver->getRCond(norm); }


//! @brief Return a pointer to the solver used to solve the eigenproblem.
XC::EigenSolver *XC::EigenSOE::getSolver(void)
  { return theSolver; }


//! @brief Anula la matrix M.
void XC::EigenSOE::zeroM(void)
  {
    massMatrix.clear();
  }

//! @brief Return the rows of the mass matrix in a Python list.
boost::python::list XC::EigenSOE::getMPy(void) const
  {
    boost::python::list retval;
    const size_t nRows= massMatrix.size1();
    const size_t nCols= massMatrix.size2();
    for(size_t i=0; i < nRows; ++i )
      {
	boost::python::list row= boost::python::list();
        for(size_t j=0; j<nCols; ++j )
	  {
            const double *tmp= massMatrix.find_element( i, j );
            if(tmp)
	      row.append(*tmp);
            else
	      row.append(0.0);
	  }
	retval.append(row);
      }
    return retval;
  }

//! @brief Makes M the identity matrix (to find stiffness matrix eigenvalues).
void XC::EigenSOE::identityM(void)
  {
    const size_t sz1= massMatrix.size1();
    const size_t sz2= massMatrix.size2();
    if(sz1!=sz2)
      std::cerr << getClassName() << "::" << __FUNCTION__
                << "mass matrix is not square " << sz1
                << "x" << sz2 << ".\n";
    massMatrix= boost::numeric::ublas::identity_matrix<double>(std::min(sz1,sz2));
  }

//! @brief Return the autovector that correspond to the mode
//! being passed as parameter.
const XC::Vector &XC::EigenSOE::getEigenvector(int mode) const
  { return theSolver->getEigenvector(mode); }

//! @brief Returns the normalized autovector that correspond to the
//! mode being passed as parameter. The vector is normalized to make
//! the maximum of the absolute values of its components to be
//! 1 (infinity_norm(v)= 1).
XC::Vector XC::EigenSOE::getNormalizedEigenvector(int mode) const
  { return theSolver->getNormalizedEigenvector(mode); }

//! @brief Returns a matrix with the computed eigenvectors disposed
//! by columns.
XC::Matrix XC::EigenSOE::getEigenvectors(void) const
  { return theSolver->getEigenvectors(); }

//! @brief Returns a matrix whit the normalized eigenvectors disposed
//! by columns (infinity norm).
XC::Matrix XC::EigenSOE::getNormalizedEigenvectors(void) const
  { return theSolver->getNormalizedEigenvectors(); }

//! @brief Returns the eigenvalue of the mode passed as parameter.
const double &XC::EigenSOE::getEigenvalue(int mode) const
  { return theSolver->getEigenvalue(mode); }

//! @brief Returns the angular frequency of the i-th mode.
double XC::EigenSOE::getAngularFrequency(int i) const
  { return sqrt(getEigenvalue(i)); }

//! @brief Returns the period of the i-th mode.
double XC::EigenSOE::getPeriod(int i) const
  { return 2.0*M_PI/getAngularFrequency(i); }

//! @brief Return the frequency of the i-th mode.
double XC::EigenSOE::getFrequency(int i) const
  { return 1./getPeriod(i); }

//! @brief Returns a vector with computed eigenvalues for each mode.
XC::Vector XC::EigenSOE::getEigenvalues(const int &numModes) const
  {
    Vector retval(numModes);
    for(int i= 1;i<=numModes;i++)
      retval[i-1]= getEigenvalue(i);
    return retval;    
  }

//! @brief Returns a vector with computed eigenvalues for each mode.
XC::Vector XC::EigenSOE::getEigenvalues(void) const
  { return theSolver->getEigenvalues(); }

//! @brief Returns a vector with the computed angular frequencies
//! for each mode.
XC::Vector XC::EigenSOE::getAngularFrequencies(void) const
  { return theSolver->getAngularFrequencies(); }


//! @brief Returns a vector with the computed periods
//! for each mode.
XC::Vector XC::EigenSOE::getPeriods(void) const
  { return theSolver->getPeriods(); }

//! @brief Returns a vector with the computed frequencies
//! for each mode.
XC::Vector XC::EigenSOE::getFrequencies(void) const
  { return theSolver->getFrequencies(); }

//! @brief Returns the number of computed eigenvalues.
const int &XC::EigenSOE::getNumModes(void) const
  { return theSolver->getNumModes(); }

bool XC::EigenSOE::standardProblem(void) const
  {
    bool retval= false;
    if((massMatrix.size1()==0) || (massMatrix.size2()==0))
      retval= true;
    return retval;
  }


//! @brief Returns the modal participation factor for the mode.
double XC::EigenSOE::getModalParticipationFactor(int mode) const
  {
    const Vector ev= getEigenvector(mode);
    const size_t sz= ev.Size();
    if((massMatrix.size1()!=sz) || (massMatrix.size2()!=sz))
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; ERROR the eigenvector has dimension " << sz
                << " and the mass matrix " << massMatrix.size1()
                << "x" << massMatrix.size2() << ".\n";
    boost::numeric::ublas::vector<double> fi_mode(sz);
    for(size_t i= 0;i<sz;i++)
      fi_mode(i)= ev(i);
    const boost::numeric::ublas::vector<double> J(sz,1.0);
    const double num= boost::numeric::ublas::inner_prod(fi_mode,prod(massMatrix,J));
    const boost::numeric::ublas::vector<double> tmp= prod(massMatrix,fi_mode);
    const double denom= boost::numeric::ublas::inner_prod(fi_mode,tmp);
    return num/denom;
  }

//! @brief Returns the modal participation factors.
XC::Vector XC::EigenSOE::getModalParticipationFactors(const int &numModes) const
  {
    Vector retval(numModes);
    if(standardProblem())
      retval.Zero();
    else
      for(int i= 1;i<=numModes;i++)
	retval[i-1]= getModalParticipationFactor(i);
    return retval;
  }

//! @brief Returns the modal participation factors.
XC::Vector XC::EigenSOE::getModalParticipationFactors(void) const
  { 
    const int nm= getNumModes();
    return getModalParticipationFactors(nm);
  }

//! @brief Returns the distribution factors for the i-th mode.
XC::Vector XC::EigenSOE::getDistributionFactor(int i) const
  { return getModalParticipationFactor(i)*getEigenvector(i); }

//! @brief Returns a matrix with the computed distribution factors 
//! placed by columns.
XC::Matrix XC::EigenSOE::getDistributionFactors(void) const
  {
    Matrix retval;
    const int nm= getNumModes();
    if(nm>0)
      {
        Vector distribFactor= getDistributionFactor(1);
        const int n_rows= distribFactor.Size();
        retval= Matrix(n_rows,nm);
        for(int i= 0;i<n_rows;i++)
          retval(i,0)= distribFactor(i);
        for(int j= 2;j<=nm;j++)
          {
            distribFactor= getDistributionFactor(j);
            for(int i= 0;i<n_rows;i++)
              retval(i,j-1)= distribFactor(i);
          }
      }
    return retval;
  }

//! @brief Return the effective modal mass for the i-th mode.
double XC::EigenSOE::getEffectiveModalMass(int i) const
  {
    const double tau= getModalParticipationFactor(i);
    const Vector ev= getEigenvector(i);
    const int sz= ev.Size();
    boost::numeric::ublas::vector<double> fi_mode(sz);
    for(int i= 0;i<sz;i++)
      fi_mode(i)= ev(i);
    boost::numeric::ublas::vector<double> J(sz,1.0);
    const double p= boost::numeric::ublas::inner_prod(fi_mode,prod(massMatrix,J));
    return tau*p;
  }

//! @brief Returns the effective modal masses for each mode.
XC::Vector XC::EigenSOE::getEffectiveModalMasses(void) const
  {
    const int nm= getNumModes();
    Vector retval(nm);
    for(int i= 1;i<=nm;i++)
      retval[i-1]= getEffectiveModalMass(i);
    return retval;
  }

//! @brief Return the model total mass.
double XC::EigenSOE::getTotalMass(void) const
  {
    const size_t sz= massMatrix.size1();
    const boost::numeric::ublas::vector<double> J(sz,1.0);
    const double retval= boost::numeric::ublas::inner_prod(J,prod(massMatrix,J));
    return retval;
  }

//! @brief Return the equivalent static force for the mode
//! passed as parameter.
XC::Vector XC::EigenSOE::getEquivalentStaticLoad(int mode,const double &accel_mode) const
  {
    Vector tmp= getDistributionFactor(mode);
    const int sz= tmp.Size();
    boost::numeric::ublas::vector<double> df(sz);
    for(int i= 0;i<sz;i++)
      df(i)= tmp(i);
    df= prod(massMatrix,df);
    for(int i= 0;i<sz;i++)
      tmp(i)= df(i)*accel_mode;
    return tmp;
  }

