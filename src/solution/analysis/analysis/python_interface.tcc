//----------------------------------------------------------------------------
//  XC program; finite element analysis code
//  for structural analysis and design.
//
//  Copyright (C)  Luis C. Pérez Tato
//
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
//python_interface.tcc

XC::Domain *(XC::Analysis::*getAnalysisDomain)(void)= &XC::Analysis::getDomainPtr;
class_<XC::Analysis, XC::Analysis *, bases<CommandEntity>, boost::noncopyable >("Analysis", no_init)
  .add_property("getAnalysisResult", &XC::Analysis::getAnalysisResult)
  .add_property("analysisAggregation",make_function(&XC::Analysis::getSolutionStrategyPtr, return_internal_reference<>() ),"return a reference to the analysis aggregation.")
  .add_property("getDomain", make_function( getAnalysisDomain, return_internal_reference<>() ),"return a reference to the domain.")
  .add_property("linearSOE", make_function(&XC::Analysis::getLinearSOEPtr, return_internal_reference<>() ),"return a reference to the system of equations.")
  .add_property("eigenSOE", make_function(&XC::Analysis::getEigenSOEPtr, return_internal_reference<>() ),"return a reference to the eigen system of equations.")
  ;

class_<XC::StaticAnalysis, bases<XC::Analysis>, boost::noncopyable >("StaticAnalysis", no_init)
  .def("analyze", &XC::StaticAnalysis::analyze,"Performs the analysis. A number of steps greater than 1 is useless if the loads are constant.")
  .def("initialize", &XC::StaticAnalysis::initialize,"Initialize analysis.")
    ;

class_<XC::EigenAnalysis , bases<XC::Analysis>, boost::noncopyable >("EigenAnalysis", no_init)
  //Eigenvectors.
  .def("getEigenvector", make_function(&XC::EigenAnalysis::getEigenvector, return_internal_reference<>()) )
  .def("getEigenvectors", &XC::EigenAnalysis::getEigenvectors)
  .def("getNormalizedEigenvector", &XC::EigenAnalysis::getNormalizedEigenvector)
  .def("getNormalizedEigenvectorComponents", &XC::EigenAnalysis::getNormalizedEigenvectorPy, "Return the components of the eigenvector in a Python list.")
  .def("getNormalizedEigenvectors",&XC::EigenAnalysis::getNormalizedEigenvectors)
  .def("getNormalizedEigenvectorsList",&XC::EigenAnalysis::getNormalizedEigenvectorsPy, "Return a Python list with the computed eigenvectors for each mode.")
  //Eigenvalues.
  .def("getEigenvalue", make_function(&XC::EigenAnalysis::getEigenvalue, return_value_policy<copy_const_reference>()) )
  .def("getPulsatance",&XC::EigenAnalysis::getAngularFrequency)
  .def("getAngularFrequency",&XC::EigenAnalysis::getAngularFrequency)
  .def("getPeriod",&XC::EigenAnalysis::getPeriod)
  .def("getFrequency",&XC::EigenAnalysis::getFrequency)
  .def("getEigenvalues",&XC::EigenAnalysis::getEigenvalues,"Return the computed eigenvalues.")
  .def("getEigenvaluesList",&XC::EigenAnalysis::getEigenvaluesPy,"Return a Python list with the computed eigenvalues.")
  .def("getPulsatances",&XC::EigenAnalysis::getAngularFrequencies)
  .def("getAngularFrequencies",&XC::EigenAnalysis::getAngularFrequencies)
  .def("getPeriods",&XC::EigenAnalysis::getPeriods)
  .def("getFrequencies",&XC::EigenAnalysis::getFrequencies)
  .def("getNumModes",&XC::EigenAnalysis::getNumModes)
// Domain stamp.
  .add_property("domainStamp",&XC::EigenAnalysis::getDomainStamp,&XC::EigenAnalysis::setDomainStamp, "Get/set the value of the domain stamp.")
  .def("getDomainStamp",&XC::EigenAnalysis::getDomainStamp, "Get the value of the domain stamp.")
  .def("setDomainStamp",&XC::EigenAnalysis::setDomainStamp, "Set the value of the domain stamp.")
  //Modal participation factors.
  .def("getModalParticipationFactor",&XC::EigenAnalysis::getModalParticipationFactor)
  .def("getModalParticipationFactors",&XC::EigenAnalysis::getModalParticipationFactors)
  //Distribution factors.
  .def("getDistributionFactor",&XC::EigenAnalysis::getDistributionFactor)
  .def("getDistributionFactors",&XC::EigenAnalysis::getDistributionFactors)
  //Effective modal masses.
  .def("getEffectiveModalMass",&XC::EigenAnalysis::getEffectiveModalMass)
  .def("getEffectiveModalMasses",&XC::EigenAnalysis::getEffectiveModalMasses)
  .def("getTotalMass",&XC::EigenAnalysis::getTotalMass)
  //Static equivalent load.
  .def("getEquivalentStaticLoad",&XC::EigenAnalysis::getEquivalentStaticLoad)

  .def("analyze", &XC::EigenAnalysis::analyze,"Performs the analysis, the argument is the number of modes to compute.")
  ;

class_<XC::LinearBucklingEigenAnalysis, bases<XC::EigenAnalysis>, boost::noncopyable >("LinearBucklingEigenAnalysis", no_init)
  .def("getEigenvalue", make_function(&XC::LinearBucklingEigenAnalysis::getEigenvalue, return_value_policy<copy_const_reference>()) )
  ;

XC::LinearBucklingEigenAnalysis &(XC::LinearBucklingAnalysis::*getLinearBucklingEigenAnalysis)(void)= &XC::LinearBucklingAnalysis::getEigenAnalysis;
class_<XC::LinearBucklingAnalysis, bases<XC::StaticAnalysis>, boost::noncopyable >("LinearBucklingAnalysis", no_init)
  .add_property("numModes",&XC::LinearBucklingAnalysis::getNumModes,&XC::LinearBucklingAnalysis::setNumModes, "Get/set the number of modes to compute.")
  .def("getLinearBucklingAnalysisStep", &XC::LinearBucklingAnalysis::getLinearBucklingAnalysisStep, "Return the step number for linear buckling analysis.")
  .def("setLinearBucklingAnalysisStep", &XC::LinearBucklingAnalysis::setLinearBucklingAnalysisStep, "Set the step number for linear buckling analysis.")
  .add_property("linearBucklingAnalysisStep",&XC::LinearBucklingAnalysis::getLinearBucklingAnalysisStep,&XC::LinearBucklingAnalysis::setLinearBucklingAnalysisStep, "Get/set the step number for linear buckling analysis.")
  .def("getEigenvalue", make_function(&XC::LinearBucklingAnalysis::getEigenvalue, return_value_policy<copy_const_reference>()) )
  .def("getEigenvaluesList",&XC::LinearBucklingAnalysis::getEigenvaluesPy,"Return a Python list with the computed eigenvalues.")
  .def("getNormalizedEigenvectorComponents", &XC::LinearBucklingAnalysis::getNormalizedEigenvectorPy, "Return the components of the eigenvector in a Python list.")
  .def("getNormalizedEigenvectorsList",&XC::LinearBucklingAnalysis::getNormalizedEigenvectorsPy, "Return a Python list with the computed eigenvectors for each mode.")
  .add_property("getEigenAnalysis", make_function( getLinearBucklingEigenAnalysis, return_internal_reference<>() ),"TO DEPRECATE. Return a reference to the internal eigenanalysis object.")
  .add_property("eigenAnalysis", make_function( getLinearBucklingEigenAnalysis, return_internal_reference<>() ),"return a reference to the internal eigenanalysis object.")
  ;

class_<XC::IllConditioningAnalysis, bases<XC::EigenAnalysis>, boost::noncopyable >("IllConditioningAnalysis", no_init)
  .def("getEigenvalue", make_function(&XC::IllConditioningAnalysis::getEigenvalue, return_value_policy<copy_const_reference>()) )
  ;

class_<XC::ModalAnalysis , bases<XC::EigenAnalysis>, boost::noncopyable >("ModalAnalysis", no_init)
  .add_property("spectrum", make_function(&XC::ModalAnalysis::getSpectrum,return_internal_reference<>()),&XC::ModalAnalysis::setSpectrum,"Response spectrum,") 
  .def("getCQCModalCrossCorrelationCoefficients",&XC::ModalAnalysis::getCQCModalCrossCorrelationCoefficients,"Returns CQC correlation coefficients.")
  ;


//class_<XC::SubdomainAnalysis, bases<XC::Analysis, XC::MovableObject>, boost::noncopyable >("SubdomainAnalysis", no_init);

class_<XC::TransientAnalysis, bases<XC::Analysis>, boost::noncopyable >("TransientAnalysis", no_init)
  .def("analyze", &XC::TransientAnalysis::analyze,"analyze(nSteps,dT) performs the analysis.")
  ;

class_<XC::DirectIntegrationAnalysis, bases<XC::TransientAnalysis>, boost::noncopyable >("DirectIntegrationAnalysis", no_init);

class_<XC::VariableTimeStepDirectIntegrationAnalysis, bases<XC::DirectIntegrationAnalysis>, boost::noncopyable >("VariableTimeStepDirectIntegrationAnalysis", no_init);

#ifdef _PARALLEL_PROCESSING
class_<XC::DomainDecompositionAnalysis, bases<XC::Analysis, XC::MovableObject>, boost::noncopyable >("DomainDecompositionAnalysis", no_init);

class_<XC::StaticDomainDecompositionAnalysis, bases<XC::DomainDecompositionAnalysis>, boost::noncopyable >("StaticDomainDecompositionAnalysis", no_init);

class_<XC::TransientDomainDecompositionAnalysis, bases<XC::DomainDecompositionAnalysis>, boost::noncopyable >("TransientDomainDecompositionAnalysis", no_init);

//class_<XC::SubstructuringAnalysis, bases<XC::DomainDecompositionAnalysis>, boost::noncopyable >("SubstructuringAnalysis", no_init);

#endif
