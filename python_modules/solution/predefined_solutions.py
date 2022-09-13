# -*- coding: utf-8 -*-
from __future__ import print_function
from __future__ import division


__author__= "Luis C. Pérez Tato (LCPT) Ana Ortega (AO_O)"
__copyright__= "Copyright 2016, LCPT, AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com, ana.ortega.ort@gmail.com"

import sys
import xc
from misc_utils import log_messages as lmsg

class SolutionProcedure(object):
    '''
    :ivar solutionStrategy: object that aggregates all the component objects
          that define the type of analysis (constraint handler, DOF_Numberer, 
          integrator, solution algorithm, system of equations, convergence test)
    :ivar cHandler:  constraint handler. Determines how the constraint equations 
          are enforced in the analysis, how it handles the boundary
          conditions/imposed displacements 
    :ivar numberer:  DOF numberer. Determines the mapping between equation 
          numbers and degrees of freedom (how DOF are numbered)
    :ivar integ: integrator, used to:

                   - determine the predictive step for time t+dt
                   - specify the tangent matrix and residual vector at any
                     iteration
                   - determine the corrective step based on the displacement 
                     increment
    :ivar solAlgo: solution algorithm, which determines the sequence of steps
          taken to solve the non-linear equation:

                   - linear
                   - Newton Raphson (the tangent is updated at each iteration)
                   - Newton with Line Search
                   - Modified Newton Raphson (the tangent is not updated at 
                   each iteration)
                   - Kyrlov-Newton
                   - BFGS, for symetric systems
                   - Broyden
    :ivar soe:       sparse system of equations: band general, band SPD, 
                     profile SPD, sparse general, umfPack, sparse SPD
    :ivar solver: solver for the system of equations.
    :ivar analysis:  determines what type of analysis is to be performed
    :ivar convergenceTestTol: convergence tolerance (defaults to 1e-9)
    :ivar printFlag: if not zero print convergence results on each step.
    :ivar numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
    :ivar numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
    :ivar convTestType: convergence test type for non linear analysis (norm unbalance,...).
    :ivar soeType: type of the system of equations object.
    :ivar solverType: type of the solver.
    :ivar maxNumIter: maximum number of iterations (defauts to 10)
    :ivar numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
    :ivar solu:
    :ivar solCtrl:
    :ivar modelWrapper: model representation for the analysis.
    :ivar shift: shift-and-invert mode (used with ARPACK).
    '''
    _counter = 0 # Counts the objects of this type.
    
    def __init__(self, name= None, constraintHandlerType= 'plain', maxNumIter= 10, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= None, soeType:str= None, solverType:str = None, shift:float= None):
        ''' Constructor.

        :param name: identifier for the solution procedure.
        :param constraintHandlerType: type of the constraint handler (plain, penalty, transformation or langrange).
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test type for non linear analysis (norm unbalance,...).
        :param soeType: type of the system of equations object.
        :param solverType: type of the solver.
        :param shift: shift-and-invert mode (used with ARPACK).
        '''
        SolutionProcedure._counter += 1
        self.id = SolutionProcedure._counter
        if(name):
            self.name= name
        else:
            self.name= str(self.id)
        self.cHandlerType= constraintHandlerType
        self.maxNumIter= maxNumIter
        self.convergenceTestTol= convergenceTestTol
        self.printFlag= printFlag
        self.numSteps= numSteps
        self.numberingMethod= numberingMethod
        self.convTestType= convTestType
        self.soeType= soeType
        self.solverType= solverType
        self.shift= shift
        self.solu= None
        self.solCtrl= None
        self.modelWrapper= None
        self.numberer= None
        self.cHandler= None
        self.solutionStrategy= None
        self.solAlgo= None
        self.integrator= None
        self.soe= None
        self.solver= None
        self.analysis= None
        
    def clear(self):
        ''' Wipe out the solution procedure.'''
        if(self.solu):
            self.solu.clear()
        self.solu= None
        self.solCtrl= None
        self.modelWrapper= None
        self.numberer= None
        self.cHandler= None
        self.solutionStrategy= None
        self.solAlgo= None
        self.integrator= None
        self.soe= None
        self.solver= None
        self.analysis= None

    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        self.clear()
        unusedModelWrapperName= self.modelWrapperSetup()
        self.constraintHandlerSetup()

    def getModelWrapperName(self):
        ''' Return the name for the model wrapper.'''
        retval= 'sm_'+self.name
        if(self.modelWrapper):
            mw= self.modelWrapper
            tmp= mw.name
            if(tmp!='nil'):
                retval= tmp
        return retval

    def soluControlSetup(self):
        ''' Defines the solution control object.'''
        self.solu= self.feProblem.getSoluProc
        self.solCtrl= self.solu.getSoluControl        

    def modelWrapperSetup(self):
        ''' Defines the model wrapper. '''
        self.soluControlSetup()
        modelWrapperName= self.getModelWrapperName()
        if(not self.modelWrapper):
            solModels= self.solCtrl.getModelWrapperContainer
            self.modelWrapper= solModels.newModelWrapper(modelWrapperName)
            self.numberer= self.modelWrapper.newNumberer("default_numberer")
            self.numberer.useAlgorithm(self.numberingMethod)
        else:
            className= type(self).__name__
            methodName= sys._getframe(0).f_code.co_name
            lmsg.warning(className+'.'+methodName+'; model wrapper already set. Command ignored.')
        return modelWrapperName

    def integratorSetup(self, integratorType= 'load_control_integrator'):
        ''' Define the type of integrator to use in the analysis.

        :param integratorType: type of integrator to use.
        '''
        if(integratorType in ['load_control_integrator','eigen_integrator','standard_eigen_integrator', 'ill-conditioning_integrator', 'linear_buckling_integrator']):
            self.integrator= self.solutionStrategy.newIntegrator(integratorType,xc.Vector([]))
        elif(integratorType=='newmark_integrator'):
            self.integrator= self.solutionStrategy.newIntegrator(integratorType,xc.Vector([0.5,0.25]))
        elif(integratorType=='displacement_control_integrator'):
            self.integrator= self.solutionStrategy.newIntegrator(integratorType,xc.Vector([]))
            self.integrator.nodeTag= self.dispControlNode.tag
            self.integrator.dof= self.dispControlDof
            self.integrator.increment= self.dispControlIncrement
            
        else:
            className= type(self).__name__
            methodName= sys._getframe(0).f_code.co_name
            lmsg.warning(className+'.'+methodName+'; integrator type: '+str(integratorType)+' unknown.')

    def getSolutionStrategyName(self):
        ''' Return the name for the model wrapper.'''
        if(self.solutionStrategy):
            return self.solutionStrategy.name
        else:
            return 'se_'+self.name
    
    def solutionAlgorithmSetup(self, solAlgType= 'linear_soln_algo', integratorType= 'load_control_integrator'):
        ''' Define the solution strategy.
        
        :param solAlgType: type of the solution algorithm (linear, Newton, modified Newton, ...)
        :param integratorType: type of integrator to use.
        '''
        solutionStrategies= self.solCtrl.getSolutionStrategyContainer
        modelWrapperName= self.getModelWrapperName()
        solutionStrategyName= self.getSolutionStrategyName()
        self.solutionStrategy= solutionStrategies.newSolutionStrategy(solutionStrategyName, modelWrapperName)
        self.solAlgo= self.solutionStrategy.newSolutionAlgorithm(solAlgType)
        self.integratorSetup(integratorType)
        if(self.convTestType):
            self.ctest= self.solutionStrategy.newConvergenceTest(self.convTestType)
            if(self.ctest):
                self.ctest.tol= self.convergenceTestTol
                self.ctest.maxNumIter= self.maxNumIter
                self.ctest.printFlag= self.printFlag
            else:
                className= type(self).__name__
                methodName= sys._getframe(0).f_code.co_name
                lmsg.error(className+'.'+methodName+'; something went wrong: '+str(self.convTestType) + ' seems not to be valid convergence test type.')
                exit(1)
                

    def getConvergenceTest(self):
        ''' Return the convergence test.'''
        
        return self.solutionStrategy.getConvergenceTest

    def sysOfEqnSetup(self):
        ''' Defines the solver to use for the resulting system of
            equations.
        '''
        self.soe= self.solutionStrategy.newSystemOfEqn(self.soeType)
        if(self.shift!=None):
            self.soe.shift= self.shift
        self.solver= self.soe.newSolver(self.solverType)

    def setPenaltyFactors(self, alphaSP= 1e15, alphaMP= 1e15):
        ''' Define the penalty factors to use with the penalty constraint handler.
        
        :param alphaSP: penalty factor on single points constraints (defaults to 1e15).
        :param alphaMP: penalty factor on multi-poing constraints (defaults to 1e15).
        '''
        self.alphaSP= alphaSP
        self.alphaMP= alphaMP
        
    def constraintHandlerSetup(self):
        ''' Define the constraint handler and return a reference to it.'''
        if(self.cHandlerType=='penalty'):
            self.cHandler= self.modelWrapper.newConstraintHandler("penalty_constraint_handler")
            self.cHandler.alphaSP= self.alphaSP
            self.cHandler.alphaMP= self.alphaMP
        elif(self.cHandlerType=='transformation'):
            self.cHandler= self.modelWrapper.newConstraintHandler("transformation_constraint_handler")
        elif(self.cHandlerType=='lagrange'):
            self.cHandler= self.modelWrapper.newConstraintHandler("lagrange_constraint_handler")
        elif(self.cHandlerType=='plain'):
            self.cHandler= self.modelWrapper.newConstraintHandler("plain_handler")
        else:
            className= type(self).__name__
            methodName= sys._getframe(0).f_code.co_name
            lmsg.error(className+'.'+methodName+'; unknown constraint handler type: '+self.cHandlerType)

    def analysisSetup(self, analysisType= 'static_analysis'):
        ''' Define the analysis object.

        :param analysisType: type of the analysis to perform.
        '''
        self.analysis= self.solu.newAnalysis(analysisType, self.getSolutionStrategyName(),"")

    def solve(self, calculateNodalReactions= False, includeInertia= False, reactionCheckTolerance= 1e-12):
        ''' Compute the solution (run the analysis).

        :param calculateNodalReactions: if true calculate reactions at
                                        nodes.
        :param includeInertia: if true calculate reactions including inertia
                               effects.
        :param reactionCheckTolerance: tolerance when checking reaction values.
        '''
        if(not self.analysis):
            self.setup()
        result= self.analysis.analyze(self.numSteps)
        if(result!=0):
            className= type(self).__name__
            methodName= sys._getframe(0).f_code.co_name
            lmsg.error(className+'.'+methodName+'; can\'t solve for current load case.')
            exit(-1)
        else:
            if(calculateNodalReactions):
                nodeHandler= self.feProblem.getPreprocessor.getNodeHandler
                result= nodeHandler.calculateNodalReactions(includeInertia, reactionCheckTolerance)
        return result

    def resetLoadCase(self):
        ''' Remove previous load from the domain.'''
        preprocessor= self.feProblem.getPreprocessor
        preprocessor.resetLoadCase() # Remove previous loads.
        preprocessor.getDomain.revertToStart() # Revert to initial state.
        
    def solveComb(self,combName, calculateNodalReactions= False, includeInertia= False, reactionCheckTolerance= 1e-12):
        ''' Obtains the solution for the combination argument.

        :param combName: name of the combination to obtain the response for.
        :param calculateNodalReactions: if true calculate reactions at
                                        nodes.
        :param includeInertia: if true calculate reactions including inertia
                               effects.
        :param reactionCheckTolerance: tolerance when checking reaction values.
        '''
        self.resetLoadCase() # Remove previous loads.
        preprocessor= self.feProblem.getPreprocessor
        preprocessor.getLoadHandler.addToDomain(combName) # Add comb. loads.
        analOk= self.solve(calculateNodalReactions, includeInertia, reactionCheckTolerance)
        preprocessor.getLoadHandler.removeFromDomain(combName) # Remove comb.
        # lmsg.info("Combination: ",combName," solved.\n")
        return analOk

#Typical solution procedures.

## Linear static analysis.
class PenaltyStaticLinearBase(SolutionProcedure):
    ''' Base class for linear static solution algorithm
        with a penalty constraint handler.
    '''
    def __init__(self, prb, name= None, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', soeType= 'band_spd_lin_soe', solverType= 'band_spd_lin_lapack_solver'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param soeType: type of the system of equations object.
        :param solverType: type of the solver.
        '''
        super(PenaltyStaticLinearBase,self).__init__(name, constraintHandlerType='penalty', maxNumIter= 1, convergenceTestTol= 1e-9, printFlag= printFlag, numSteps= numSteps, numberingMethod= numberingMethod, soeType= soeType, solverType= solverType)
        self.feProblem= prb
        self.setPenaltyFactors()

    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PenaltyStaticLinearBase,self).setup()
        self.solutionAlgorithmSetup(solAlgType= 'linear_soln_algo', integratorType= 'load_control_integrator')
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')
        
class SimpleStaticLinear(PenaltyStaticLinearBase):
    ''' Return a linear static solution algorithm
        with a penalty constraint handler.
    '''
    def __init__(self, prb, name= None, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', soeType= 'band_spd_lin_soe', solverType= 'band_spd_lin_lapack_solver'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        '''
        super(SimpleStaticLinear,self).__init__(name, printFlag= printFlag, numSteps= numSteps, numberingMethod= numberingMethod, soeType= soeType, solverType= solverType)
        self.feProblem= prb
        self.setPenaltyFactors()
        
### Convenience function.
def simple_static_linear(prb):
    ''' Return a simple static linear solution procedure.'''
    solProc= SimpleStaticLinear(prb)
    solProc.setup()
    return solProc.analysis
        
class SimpleStaticLinearUMF(PenaltyStaticLinearBase):
    ''' Return a linear static solution algorithm
        with a penalty constraint handler.
    '''
    def __init__(self, prb, name= None, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', soeType= 'umfpack_gen_lin_soe', solverType= 'umfpack_gen_lin_solver'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        '''
        super(SimpleStaticLinearUMF,self).__init__(name, printFlag= printFlag, numSteps= numSteps, numberingMethod= numberingMethod, soeType= soeType, solverType= solverType)
        self.feProblem= prb
        self.setPenaltyFactors()
        
### Convenience function.
def simple_static_linear_umf(prb):
    ''' Return a simple static linear solution procedure.'''
    solProc= SimpleStaticLinearUMF(prb)
    solProc.setup()
    return solProc.analysis
        
class SimpleStaticLinearMUMPS(PenaltyStaticLinearBase):
    ''' Return a linear static solution algorithm
        with a penalty constraint handler.
    '''
    def __init__(self, prb, name= None, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', soeType= 'mumps_soe', solverType= 'mumps_solver'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        '''
        super(SimpleStaticLinearMUMPS,self).__init__(name, printFlag= printFlag, numSteps= numSteps, numberingMethod= numberingMethod, soeType= soeType, solverType= solverType)
        self.feProblem= prb
        self.setPenaltyFactors()
        
### Convenience function.
def simple_static_linear_mumps(prb):
    ''' Return a simple static linear solution procedure.'''
    solProc= SimpleStaticLinearMUMPS(prb)
    solProc.setup()
    return solProc.analysis

class SimpleLagrangeStaticLinear(SolutionProcedure):
    ''' Linear static solution algorithm
        with a Lagrange constraint handler.
    '''
    def __init__(self, prb, name= None, maxNumIter= 10, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        '''
        super(SimpleLagrangeStaticLinear,self).__init__(name, 'lagrange', maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, soeType= 'sparse_gen_col_lin_soe', solverType= 'super_lu_solver')
        self.feProblem= prb

    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(SimpleLagrangeStaticLinear,self).setup()
        self.solutionAlgorithmSetup(solAlgType= 'linear_soln_algo', integratorType= 'load_control_integrator')
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')

class SimpleTransformationStaticLinear(SolutionProcedure):
    ''' Linear static solution algorithm with a 
        transformation constraint handler.
    '''
    def __init__(self, prb, name= None, maxNumIter= 10, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        '''
        super(SimpleTransformationStaticLinear,self).__init__(name, 'transformation', maxNumIter, convergenceTestTol, printFlag, numSteps, soeType= 'sparse_gen_col_lin_soe', solverType= 'super_lu_solver')
        self.feProblem= prb
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(SimpleTransformationStaticLinear,self).setup()
        self.solutionAlgorithmSetup(solAlgType= 'linear_soln_algo', integratorType= 'load_control_integrator')
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')

## Non-linear static analysis.
class PlainNewtonRaphson(SolutionProcedure):
    ''' Newton-Raphson solution algorithm with a 
        plain constraint handler.
    '''
    def __init__(self, prb, name= None, maxNumIter= 10, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'norm_unbalance_conv_test'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        '''
        super(PlainNewtonRaphson,self).__init__(name, 'plain', maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, soeType= 'sparse_gen_col_lin_soe', solverType= 'super_lu_solver')
        self.feProblem= prb
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PlainNewtonRaphson,self).setup()
        self.solutionAlgorithmSetup(solAlgType= 'newton_raphson_soln_algo', integratorType= 'load_control_integrator')
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')

### Convenience function
def plain_newton_raphson(prb, maxNumIter= 10, convergenceTestTol= 1e-9):
    solProc= PlainNewtonRaphson(prb, maxNumIter= maxNumIter, convergenceTestTol= convergenceTestTol)
    solProc.setup()
    return solProc.analysis

class PlainNewtonRaphsonBandGen(SolutionProcedure):
    ''' Newton-Raphson solution algorithm with a
        plain constraint handler and a band general
        SOE solver.
    '''
    def __init__(self, prb, name= None, maxNumIter= 10, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'norm_unbalance_conv_test'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        '''
        super(PlainNewtonRaphsonBandGen,self).__init__(name, 'plain', maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, soeType= 'band_gen_lin_soe', solverType= 'band_gen_lin_lapack_solver')
        self.feProblem= prb
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PlainNewtonRaphsonBandGen,self).setup()
        self.solutionAlgorithmSetup(solAlgType= 'newton_raphson_soln_algo', integratorType= 'load_control_integrator')
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')

### Convenience function
def plain_newton_raphson_band_gen(prb, maxNumIter= 10):
    solProc= PlainNewtonRaphsonBandGen(prb, maxNumIter= maxNumIter)
    solProc.setup()
    return solProc.analysis

class PenaltyNewtonRaphsonBase(SolutionProcedure):
    ''' Base class for penalty modified Newton solution aggregation.'''
    def __init__(self, prb, name, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, soeType, solverType):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        :param soeType: type of the system of equations object.
        :param solverType: type of the solver.
        '''
        super(PenaltyNewtonRaphsonBase,self).__init__(name, constraintHandlerType='penalty', maxNumIter= maxNumIter, convergenceTestTol= convergenceTestTol, printFlag= printFlag, numSteps= numSteps, numberingMethod= numberingMethod, convTestType= convTestType, soeType= soeType, solverType= solverType)
        self.feProblem= prb
        self.setPenaltyFactors()

    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PenaltyNewtonRaphsonBase,self).setup()
        self.solutionAlgorithmSetup(solAlgType= 'newton_raphson_soln_algo', integratorType= 'load_control_integrator')

class PenaltyNewtonRaphson(PenaltyNewtonRaphsonBase):
    ''' Return a static solution procedure with a Newton Raphson algorithm
        and a penalty constraint handler.'''
    def __init__(self, prb, name= None, maxNumIter= 10, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'norm_unbalance_conv_test'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        '''
        super(PenaltyNewtonRaphson,self).__init__(prb, name, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, soeType= 'band_gen_lin_soe', solverType= 'band_gen_lin_lapack_solver')

    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PenaltyNewtonRaphson,self).setup()
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')
        

### Convenience function
def penalty_newton_raphson(prb, maxNumIter= 10, convergenceTestTol= 1e-4, printFlag= 0):
    ''' Return a penalty Newton-Raphson solution procedure.

    :param maxNumIter: maximum number of iterations (defauts to 10)
    :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
    :param printFlag: print message on each iteration
    '''
    solProc= PenaltyNewtonRaphson(prb, maxNumIter= maxNumIter, convergenceTestTol= convergenceTestTol, printFlag= printFlag)
    solProc.setup()
    return solProc.analysis

class PenaltyNewtonRaphsonUMF(PenaltyNewtonRaphsonBase):
    ''' Static solution procedure with a Newton algorithm,
        a penalty constraint handler and a UMF
        (Unsimmetric multi-frontal method) solver.'''
    def __init__(self, prb, name= None, maxNumIter= 150, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'norm_unbalance_conv_test'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        '''
        super(PenaltyNewtonRaphsonUMF,self).__init__(prb, name, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, soeType= 'umfpack_gen_lin_soe', solverType= 'umfpack_gen_lin_solver')

    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PenaltyNewtonRaphsonUMF,self).setup()
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')
        
class PenaltyNewtonRaphsonMUMPS(PenaltyNewtonRaphsonBase):
    ''' Static solution procedure with a Newton algorithm,
        a penalty constraint handler and a MUMPS
        (parallel sparse direct solver) solver.'''
    def __init__(self, prb, name= None, maxNumIter= 150, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'norm_unbalance_conv_test'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        '''
        super(PenaltyNewtonRaphsonMUMPS,self).__init__(prb, name, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, soeType= 'mumps_soe', solverType= 'mumps_solver')

    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PenaltyNewtonRaphsonMUMPS,self).setup()
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')

class PlainStaticModifiedNewton(SolutionProcedure):
    ''' Static solution procedure with a modified Newton
        solution algorithm with a plain constraint handler.
    '''
    def __init__(self, prb, name= None, maxNumIter= 10, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'relative_total_norm_disp_incr_conv_test'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        '''
        super(PlainStaticModifiedNewton,self).__init__(name, 'plain', maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, soeType= 'sparse_gen_col_lin_soe', solverType= 'super_lu_solver')
        self.feProblem= prb
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PlainStaticModifiedNewton,self).setup()
        self.maxNumIter= 150 #Make this configurable
        self.solutionAlgorithmSetup(solAlgType= 'modified_newton_soln_algo', integratorType= 'load_control_integrator')
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')
    
### Convenience function
def plain_static_modified_newton(prb, maxNumIter= 10, convergenceTestTol= .01, printFlag= 0):
    ''' Return a simple static modified Newton solution procedure.

    :param maxNumIter: maximum number of iterations (defauts to 10)
    :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
    :param printFlag: print message on each iteration
    '''
    solProc= PlainStaticModifiedNewton(prb, maxNumIter= maxNumIter, convergenceTestTol= convergenceTestTol, printFlag= printFlag)
    solProc.setup()
    return solProc.analysis

class PenaltyModifiedNewtonBase(SolutionProcedure):
    ''' Base class for penalty modified Newton solution aggregation.'''
    def __init__(self, prb, name, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType= 'relative_total_norm_disp_incr_conv_test', soeType= 'sparse_gen_col_lin_soe', solverType= 'super_lu_solver'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        :param soeType: type of the system of equations object.
        :param solverType: type of the solver.
        '''
        super(PenaltyModifiedNewtonBase,self).__init__(name, constraintHandlerType='penalty', maxNumIter= maxNumIter, convergenceTestTol=convergenceTestTol, printFlag=printFlag, numSteps=numSteps, numberingMethod=numberingMethod, convTestType=convTestType, soeType= soeType, solverType= solverType)
        self.feProblem= prb
        self.setPenaltyFactors()

    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PenaltyModifiedNewtonBase,self).setup()
        self.solutionAlgorithmSetup(solAlgType= 'modified_newton_soln_algo', integratorType= 'load_control_integrator')

class PenaltyModifiedNewton(PenaltyModifiedNewtonBase):
    ''' Static solution procedure with a modified Newton algorithm
        and a penalty constraint handler.'''
    def __init__(self, prb, name= None, maxNumIter= 150, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'relative_total_norm_disp_incr_conv_test'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        '''
        super(PenaltyModifiedNewton,self).__init__(prb= prb, name= name, maxNumIter= maxNumIter, convergenceTestTol= convergenceTestTol, printFlag= printFlag, numSteps= numSteps, numberingMethod= numberingMethod, convTestType= convTestType, soeType= 'sparse_gen_col_lin_soe', solverType= 'super_lu_solver')

    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PenaltyModifiedNewton,self).setup()
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')        

### Convenience function
def penalty_modified_newton(prb, maxNumIter= 10, convergenceTestTol= 1e-4, printFlag= 0, convTestType= 'relative_total_norm_disp_incr_conv_test'):
    ''' Return a simple static modified Newton solution procedure.

    :param maxNumIter: maximum number of iterations (defauts to 10)
    :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
    :param printFlag: print message on each iteration
    '''
    solProc= PenaltyModifiedNewton(prb,maxNumIter= maxNumIter, convergenceTestTol= convergenceTestTol, printFlag= printFlag, convTestType= convTestType)
    solProc.setup()
    return solProc.analysis
    
class PenaltyModifiedNewtonUMF(PenaltyModifiedNewtonBase):
    ''' Static solution procedure with a modified Newton algorithm,
        a penalty constraint handler and a UMF
        (Unsimmetric multi-frontal method) solver.'''
    def __init__(self, prb, name= None, maxNumIter= 150, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'relative_total_norm_disp_incr_conv_test'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        '''
        super(PenaltyModifiedNewtonUMF,self).__init__(prb, name, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, soeType= 'umfpack_gen_lin_soe', solverType= 'umfpack_gen_lin_solver')
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PenaltyModifiedNewtonUMF,self).setup()
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')
        
class PenaltyModifiedNewtonMUMPS(PenaltyModifiedNewtonBase):
    ''' Static solution procedure with a modified Newton algorithm,
        a penalty constraint handler and a MUMPS
        (parallel sparse direct solver) solver.'''
    def __init__(self, prb, name= None, maxNumIter= 150, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'relative_total_norm_disp_incr_conv_test'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        '''
        super(PenaltyModifiedNewtonMUMPS,self).__init__(prb, name, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, soeType= 'mumps_soe', solverType= 'mumps_solver')
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PenaltyModifiedNewtonMUMPS,self).setup()
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')

class LineSearchBase(SolutionProcedure):
    ''' Base class for line search solution aggregations.'''
    def __init__(self, prb, name, constraintHandlerType, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, lineSearchMethod, soeType, solverType):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        :param lineSearchMethod: line search method to use (bisection_line_search, initial_interpolated_line_search, regula_falsi_line_search, secant_line_search).
        :param soeType: type of the system of equations object.
        :param solverType: type of the solver.
        '''
        super(LineSearchBase,self).__init__(name, constraintHandlerType, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, soeType= soeType, solverType= solverType)
        self.feProblem= prb
        self.lineSearchMethod= lineSearchMethod

class PenaltyNewtonLineSearchBase(LineSearchBase):
    ''' Base class for penalty Newton line search solution aggregation.'''
    def __init__(self, prb, name, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, lineSearchMethod, soeType, solverType):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        :param lineSearchMethod: line search method to use (bisection_line_search, initial_interpolated_line_search, regula_falsi_line_search, secant_line_search).
        :param soeType: type of the system of equations object.
        :param solverType: type of the solver.
        '''
        super(PenaltyNewtonLineSearchBase,self).__init__(prb, name, constraintHandlerType='penalty', maxNumIter=maxNumIter, convergenceTestTol=convergenceTestTol, printFlag=printFlag, numSteps=numSteps, numberingMethod=numberingMethod, convTestType=convTestType, lineSearchMethod=lineSearchMethod, soeType=soeType, solverType= solverType)
        self.setPenaltyFactors()
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PenaltyNewtonLineSearchBase,self).setup()
        self.solutionAlgorithmSetup(solAlgType= 'newton_line_search_soln_algo', integratorType= 'load_control_integrator')
        self.solAlgo.setLineSearchMethod(self.lineSearchMethod)

class PenaltyNewtonLineSearch(PenaltyNewtonLineSearchBase):
    ''' Static solution procedure with a Newton line search algorithm
        and a penalty constraint handler.'''
    def __init__(self, prb, name= None, maxNumIter= 150, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'relative_total_norm_disp_incr_conv_test', lineSearchMethod= 'regula_falsi_line_search'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        :param lineSearchMethod: line search method to use (bisection_line_search, initial_interpolated_line_search, regula_falsi_line_search, secant_line_search).
        '''
        super(PenaltyNewtonLineSearch,self).__init__(prb, name, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, lineSearchMethod, soeType= 'sparse_gen_col_lin_soe', solverType= 'super_lu_solver')
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PenaltyNewtonLineSearch,self).setup()
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')

class PenaltyNewtonLineSearchUMF(PenaltyNewtonLineSearchBase):
    ''' Static solution procedure with a Newton line search algorithm,
        a penalty constraint handler and a UMF
        (Unsimmetric multi-frontal method) solver.'''
    def __init__(self, prb, name= None, maxNumIter= 150, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'relative_total_norm_disp_incr_conv_test', lineSearchMethod= 'regula_falsi_line_search'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        :param lineSearchMethod: line search method to use (bisection_line_search, initial_interpolated_line_search, regula_falsi_line_search, secant_line_search).
        '''
        super(PenaltyNewtonLineSearchUMF,self).__init__(prb, name, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, lineSearchMethod, soeType= 'umfpack_gen_lin_soe', solverType= 'umfpack_gen_lin_solver')
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PenaltyNewtonLineSearchUMF,self).setup()
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')
        
class PenaltyNewtonLineSearchMUMPS(PenaltyNewtonLineSearchBase):
    ''' Static solution procedure with a Newton line search algorithm,
        a penalty constraint handler and a MUMPS
        (parallel sparse direct solver) solver.'''
    def __init__(self, prb, name= None, maxNumIter= 150, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'relative_total_norm_disp_incr_conv_test', lineSearchMethod= 'regula_falsi_line_search'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        :param lineSearchMethod: line search method to use (bisection_line_search, initial_interpolated_line_search, regula_falsi_line_search, secant_line_search).
        '''
        super(PenaltyNewtonLineSearchMUMPS,self).__init__(prb, name, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, lineSearchMethod, soeType= 'mumps_soe', solverType= 'mumps_solver')
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PenaltyNewtonLineSearchMUMPS,self).setup()
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')
        
### Convenience function
def penalty_newton_line_search_mumps(prb, maxNumIter= 150, convergenceTestTol= 1e-9, printFlag= 0, convTestType= 'relative_total_norm_disp_incr_conv_test', lineSearchMethod= 'regula_falsi_line_search'):
    ''' Return a simple static modified Newton solution procedure.

    :param maxNumIter: maximum number of iterations (defauts to 10)
    :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
    :param printFlag: print message on each iteration
    '''
    solProc= PenaltyNewtonLineSearchMUMPS(prb,maxNumIter= maxNumIter, convergenceTestTol= convergenceTestTol, printFlag= printFlag, convTestType= convTestType, lineSearchMethod= lineSearchMethod)
    solProc.setup()
    return solProc.analysis
 
class TransformationNewtonLineSearchBase(LineSearchBase):
    ''' Base class for transformation Newton line search solution aggregation.'''
    def __init__(self, prb, name, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, lineSearchMethod, soeType, solverType):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        :param lineSearchMethod: line search method to use (bisection_line_search, initial_interpolated_line_search, regula_falsi_line_search, secant_line_search).
        :param soeType: type of the system of equations object.
        :param solverType: type of the solver.
        '''
        super(TransformationNewtonLineSearchBase,self).__init__(prb, name, 'transformation', maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, lineSearchMethod, soeType, solverType)
        #self.setTransformationFactors()
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(TransformationNewtonLineSearchBase,self).setup()
        self.solutionAlgorithmSetup(solAlgType= 'newton_line_search_soln_algo', integratorType= 'load_control_integrator')
        self.solAlgo.setLineSearchMethod(self.lineSearchMethod)

class TransformationNewtonLineSearch(TransformationNewtonLineSearchBase):
    ''' Static solution procedure with a Newton line search algorithm
        and a transformation constraint handler.'''
    def __init__(self, prb, name= None, maxNumIter= 150, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'relative_total_norm_disp_incr_conv_test', lineSearchMethod= 'regula_falsi_line_search'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        :param lineSearchMethod: line search method to use (bisection_line_search, initial_interpolated_line_search, regula_falsi_line_search, secant_line_search).
        '''
        super(TransformationNewtonLineSearch,self).__init__(prb, name, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, lineSearchMethod, soeType= 'sparse_gen_col_lin_soe', solverType= 'super_lu_solver')
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(TransformationNewtonLineSearch,self).setup()
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')

class TransformationNewtonLineSearchUMF(TransformationNewtonLineSearchBase):
    ''' Static solution procedure with a Newton line search algorithm,
        a transformation constraint handler and a UMF
        (Unsimmetric multi-frontal method) solver.'''
    def __init__(self, prb, name= None, maxNumIter= 150, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'relative_total_norm_disp_incr_conv_test', lineSearchMethod= 'regula_falsi_line_search'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        :param lineSearchMethod: line search method to use (bisection_line_search, initial_interpolated_line_search, regula_falsi_line_search, secant_line_search).
        '''
        super(TransformationNewtonLineSearchUMF,self).__init__(prb, name, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, lineSearchMethod, soeType= 'umfpack_gen_lin_soe', solverType= 'umfpack_gen_lin_solver')
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(TransformationNewtonLineSearchUMF,self).setup()
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')
        
class TransformationNewtonLineSearchMUMPS(TransformationNewtonLineSearchBase):
    ''' Static solution procedure with a Newton line search algorithm,
        a transformation constraint handler and a MUMPS
        (parallel sparse direct solver) solver.'''
    def __init__(self, prb, name= None, maxNumIter= 150, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'relative_total_norm_disp_incr_conv_test', lineSearchMethod= 'regula_falsi_line_search'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        :param lineSearchMethod: line search method to use (bisection_line_search, initial_interpolated_line_search, regula_falsi_line_search, secant_line_search).
        '''
        super(TransformationNewtonLineSearchMUMPS,self).__init__(prb, name, maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, lineSearchMethod, soeType= 'mumps_soe', solverType= 'mumps_solver')
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(TransformationNewtonLineSearchMUMPS,self).setup()
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')
        
### Convenience function
def transformation_newton_line_search_mumps(prb, maxNumIter= 150, convergenceTestTol= 1e-9, printFlag= 0, convTestType= 'relative_total_norm_disp_incr_conv_test'):
    ''' Return a simple static modified Newton solution procedure.

    :param maxNumIter: maximum number of iterations (defauts to 10)
    :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
    :param printFlag: print message on each iteration
    '''
    solProc= TransformationNewtonLineSearchMUMPS(prb,maxNumIter= maxNumIter, convergenceTestTol= convergenceTestTol, printFlag= printFlag, convTestType= convTestType)
    solProc.setup()
    return solProc.analysis

class PlainKrylovNewton(SolutionProcedure):
    ''' KrylovNewton algorithm object which uses a Krylov subspace 
        accelerator to accelerate the convergence of the modified 
        newton method.

    See appendix C. Analysis model script of the document
    "Finite Element Modeling of Gusset Plate Failure Using Opensees"
    Andrew J. Walker. Oregon State University
    '''
    def __init__(self, prb, name= None, maxNumIter= 150, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'simple', convTestType= 'energy_incr_conv_test', maxDim= 6):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        :param maxDim: max number of iterations until the tangent is reformed and the acceleration restarts (default = 6).
        '''
        super(PlainKrylovNewton,self).__init__(name, 'plain', maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, convTestType, soeType= 'umfpack_gen_lin_soe', solverType= 'umfpack_gen_lin_solver')
        self.feProblem= prb
        self.maxDim= maxDim
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PlainKrylovNewton,self).setup()
        self.solutionAlgorithmSetup(solAlgType= 'krylov_newton_soln_algo', integratorType= 'load_control_integrator')
        self.solAlgo.maxDimension= self.maxDim
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')

### Convenience function
def plain_krylov_newton(prb, maxNumIter= 300, convergenceTestTol= 1e-9, printFlag= 0, maxDim= 6):
    ''' Return a plain Krylov Newton solution procedure.

    :param maxNumIter: maximum number of iterations (defauts to 300)
    :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
    :param printFlag: print message on each iteration
    :param maxDim: max number of iterations until the tangent is reformed and the acceleration restarts (default = 6).
    '''
    solProc= PlainKrylovNewton(prb, maxNumIter= maxNumIter, convergenceTestTol= convergenceTestTol, printFlag= printFlag, maxDim= 6)
    solProc.setup()
    return solProc.analysis

class PenaltyKrylovNewton(SolutionProcedure):
    ''' KrylovNewton algorithm object which uses a Krylov subspace 
        accelerator to accelerate the convergence of the modified 
        newton method.

    See appendix C. Analysis model script of the document
    "Finite Element Modeling of Gusset Plate Failure Using Opensees"
    Andrew J. Walker. Oregon State University
    '''
    def __init__(self, prb, name= None, maxNumIter= 150, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'norm_disp_incr_conv_test', maxDim= 6):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        :param maxDim: max number of iterations until the tangent is reformed and the acceleration restarts (default = 6).
        '''
        super(PenaltyKrylovNewton,self).__init__(name, constraintHandlerType='penalty', maxNumIter= maxNumIter, convergenceTestTol= convergenceTestTol, printFlag= printFlag, numSteps= numSteps, numberingMethod= numberingMethod, convTestType= convTestType, soeType= 'mumps_soe', solverType= 'mumps_solver')
        self.feProblem= prb
        self.maxDim= maxDim
        self.setPenaltyFactors()
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PenaltyKrylovNewton,self).setup()
        self.solutionAlgorithmSetup(solAlgType= 'krylov_newton_soln_algo', integratorType= 'load_control_integrator')
        self.integrator.dLambda1= 1.0/self.numSteps
        self.solAlgo.maxDimension= self.maxDim
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')


## Dynamic analysis        
class PlainLinearNewmark(SolutionProcedure):
    ''' Return a linear Newmark solution algorithm
        with a plain constraint handler.
    '''
    def __init__(self, prb, timeStep, name= None, maxNumIter= 10, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'simple'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param timeStep: time step.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        '''
        super(PlainLinearNewmark,self).__init__(name, 'plain', maxNumIter, convergenceTestTol, printFlag, numSteps, numberingMethod, soeType= 'band_gen_lin_soe', solverType= 'band_gen_lin_lapack_solver')
        self.feProblem= prb
        self.timeStep= timeStep
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PlainLinearNewmark,self).setup()
        self.solutionAlgorithmSetup(solAlgType= 'linear_soln_algo', integratorType= 'newmark_integrator')
        self.sysOfEqnSetup()
        self.analysisSetup('direct_integration_analysis')
        
    def solve(self, calculateNodalReactions= False, includeInertia= False, reactionCheckTolerance= 1e-12):
        ''' Compute the solution (run the analysis).

        :param calculateNodalReactions: if true calculate reactions at
                                        nodes.
        :param includeInertia: if true calculate reactions including inertia
                               effects.
        :param reactionCheckTolerance: tolerance when checking reaction values.
        '''
        if(not self.analysis):
            self.setup()
        result= self.analysis.analyze(self.numSteps, self.timeStep)
        if(calculateNodalReactions and (result==0)):
            nodeHandler= self.feProblem.getPreprocessor.getNodeHandler
            result= nodeHandler.calculateNodalReactions(includeInertia, reactionCheckTolerance)
        return result

class PenaltyNewmarkNewtonRapshon(SolutionProcedure):
    ''' Newmark solution procedure with a Newton Raphson algorithm
        and a penalty constraint handler.'''
    def __init__(self, prb, name= None, maxNumIter= 10, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= 'norm_disp_incr_conv_test'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alternative minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        '''
        super(PenaltyNewmarkNewtonRapshon,self).__init__(name, constraintHandlerType='penalty', maxNumIter=maxNumIter, convergenceTestTol=convergenceTestTol, printFlag=printFlag, numSteps=numSteps, numberingMethod=numberingMethod, convTestType=convTestType, soeType= 'profile_spd_lin_soe', solverType= 'profile_spd_lin_direct_solver')
        self.feProblem= prb
        self.setPenaltyFactors(alphaSP= 1.0e18, alphaMP= 1.0e18)
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(PenaltyNewmarkNewtonRapshon,self).setup()
        self.solutionAlgorithmSetup(solAlgType= 'newton_raphson_soln_algo', integratorType= 'newmark_integrator')
        self.sysOfEqnSetup()
        self.analysisSetup('direct_integration_analysis')


## Eigen analysis
class OrdinaryEigenvalues(SolutionProcedure):
    ''' Procedure to obtain the ordinary eigenvalues of the stiffness matrix
        of the finite element model.'''
    
    def __init__(self, prb, name= None, printFlag= 0, systemPrefix= 'sym_band_lapack', numberingMethod= 'rcm', shift:float= None):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param printFlag: if not zero print convergence results on each step.
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alternative minimum degree).
        '''        
        super(OrdinaryEigenvalues, self).__init__(name= name, constraintHandlerType= 'penalty', printFlag= printFlag, numberingMethod= numberingMethod, soeType= systemPrefix+"_soe", solverType= systemPrefix+"_solver", shift= shift)
        self.feProblem= prb
        self.setPenaltyFactors()

    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(OrdinaryEigenvalues, self).setup()
        self.solutionAlgorithmSetup(solAlgType= 'standard_eigen_soln_algo', integratorType= 'standard_eigen_integrator')
        self.sysOfEqnSetup()
        self.analysisSetup('ill-conditioning_analysis')
        
### Convenience function
def ordinary_eigenvalues(prb):
    ''' Return a solution procedure that computes the ordinary eigenvalues
        of the model stiffness matrix.'''
    solProc= OrdinaryEigenvalues(prb)
    solProc.setup()
    return solProc.analysis


class FrequencyAnalysis(SolutionProcedure):
    ''' Return a natural frequency computation procedure.'''

    def __init__(self, prb, name= None, printFlag= 0, systemPrefix= 'sym_band', numberingMethod= 'rcm', shift:float= None):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param printFlag: if not zero print convergence results on each step.
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        '''        
        self.systemPrefix= systemPrefix
        soe_string= self.systemPrefix+'_eigen_soe'
        solver_string= self.systemPrefix+'_eigen_solver'
        super(FrequencyAnalysis,self).__init__(name, 'transformation', printFlag, numberingMethod= numberingMethod, soeType= soe_string, solverType= solver_string, shift= shift)
        self.feProblem= prb
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(FrequencyAnalysis,self).setup()
        self.solutionAlgorithmSetup(solAlgType= 'frequency_soln_algo', integratorType= 'eigen_integrator')
        self.sysOfEqnSetup()
        self.analysisSetup('modal_analysis')
        
### Convenience function
def frequency_analysis(prb, systemPrefix= 'sym_band', shift:float= None):
    ''' Return a solution procedure that computes the natural
        frequencies of the model.'''
    solProc= FrequencyAnalysis(prb, systemPrefix= systemPrefix, shift= shift)
    solProc.setup()
    return solProc.analysis

class IllConditioningAnalysisBase(SolutionProcedure):
    ''' Base class for ill-conditioning
        solution procedures.
    '''
    def __init__(self, prb, name= None, printFlag= 0, systemPrefix= 'sym_band_eigen', shift:float= None, numberingMethod= 'rcm'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param printFlag: if not zero print convergence results on each step.
        :param systemPrefix: string that identifies the SOE and Solver types.
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        '''        
        super(IllConditioningAnalysisBase,self).__init__(name, constraintHandlerType='penalty', printFlag= printFlag, numberingMethod= numberingMethod, soeType= systemPrefix+"_soe", solverType= systemPrefix+"_solver", shift= shift)
        self.feProblem= prb
        self.setPenaltyFactors()
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(IllConditioningAnalysisBase,self).setup()
        self.solutionAlgorithmSetup(solAlgType= 'ill-conditioning_soln_algo', integratorType= 'ill-conditioning_integrator')
        self.sysOfEqnSetup()
        self.analysisSetup('ill-conditioning_analysis')
    
class ZeroEnergyModes(IllConditioningAnalysisBase):
    ''' Procedure to obtain the zero energy modes
        of the finite element model.'''
    def __init__(self, prb, name= None, printFlag= 0, systemPrefix= 'sym_band_eigen', shift:float= None):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param printFlag: if not zero print convergence results on each step.
        '''        
        # Very slow solver but apparently Arpack cannot
        # compute zero eigenvalues. XXX
        super(ZeroEnergyModes,self).__init__(prb, name, printFlag= printFlag, systemPrefix= systemPrefix, shift= shift)
        
### Convenience function
def zero_energy_modes(prb):
    ''' Return a solution procedure that computes the zero
        energy modes of the model.'''
    solProc= ZeroEnergyModes(prb)
    solProc.setup()
    return solProc.analysis        

class IllConditioningAnalysis(IllConditioningAnalysisBase):
    ''' Procedure to obtain the modes
        associated with very small eigenvalues of the
        stiffness matrix.'''
    def __init__(self, prb, name= None, printFlag= 0, systemPrefix= 'band_arpack', shift:float= 0.0):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param printFlag: if not zero print convergence results on each step.
        '''        
        # Very slow solver but apparently Arpack cannot
        # compute zero eigenvalues. XXX
        super(IllConditioningAnalysis,self).__init__(prb, name, printFlag= printFlag, systemPrefix= systemPrefix, shift= shift)
    
### Convenience function
def ill_conditioning_analysis(prb):
    ''' Return a solution procedure that computes the modes
        that correspond to ill-conditioned degrees of freedom.'''
    solProc= IllConditioningAnalysis(prb)
    solProc.setup()
    return solProc.analysis

## Utility functions

def solveStaticLinearComb(combName,solutionProcedure):
    methodName= sys._getframe(0).f_code.co_name
    lmsg.warning(methodName+'; DEPRECATED; use solveComb.')
    solutionProcedure.solveComb(combName)

def solveCombEstat2ndOrderLin(combName,solutionProcedure):
    solutionProcedure.resetLoadCase()
    preprocessor= solutionProcedure.feProblem.getPreprocessor
    preprocessor.getLoadHandler.addToDomain(combName)
    analOk= solutionProcedure.solve() # 1st order.
    if(analOk!=0):
        analOk= solutionProcedure.solve() # 2nd order.
        if(analOk!=0):
            methodName= sys._getframe(0).f_code.co_name
            lmsg.error(methodName+'; can\'t solve.')
    preprocessor.getLoadHandler.removeFromDomain(combName)
    # lmsg.info("Resuelta combinación: ",combName,"\n")

def solveStaticNoLinCase(combName):
    methodName= sys._getframe(0).f_code.co_name
    lmsg.warning(methodName+'; DEPRECATED; use solveComb.')
    solveComb(preprocessor,combName,analysis,numSteps)

class BucklingAnalysisEigenPart(SolutionProcedure):
    ''' Eigenvalue part of a linear buckling analysis.'''

    def __init__(self, prb, staticAnalysisPart, name= None, printFlag= 0, soeType:str= None, solverType:str= None, shift:float= 0.0):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param staticAnalysisPart: static analysis part of the linear buckling analysis. 
        :param name: identifier for the solution procedure.
        :param printFlag: if not zero print convergence results on each step.
        :param soeType: type of the system of equations object.
        :param solverType: type of the solver.
        :param shift: shift-and-invert mode (used with ARPACK).
        '''        
        super(BucklingAnalysisEigenPart,self).__init__(name, constraintHandlerType= None, printFlag= printFlag, numberingMethod= None, soeType= soeType, solverType= solverType, shift= shift)
        modelWrapper= staticAnalysisPart.getModelWrapper
        if(modelWrapper is not None):
            self.modelWrapper= modelWrapper
        else:
            className= type(self).__name__
            methodName= sys._getframe(0).f_code.co_name
            lmsg.error(className+'.'+methodName+'; model wrapper not set.')
        self.feProblem= prb
                
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        self.soluControlSetup()
        self.solutionAlgorithmSetup(solAlgType= 'linear_buckling_soln_algo', integratorType= 'linear_buckling_integrator')
        self.sysOfEqnSetup()

class BucklingAnalysisStaticPart(SolutionProcedure):
    ''' Static analysis part of a linear buckling analysis.'''

    def __init__(self, prb, name= None, constraintHandlerType= 'transformation', maxNumIter= 100, convergenceTestTol= 1e-9, printFlag= 0, numSteps= 1, numberingMethod= 'rcm', convTestType= None, soeType:str= None, solverType:str= None, solnAlgorithmType= 'krylov_newton_soln_algo'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param name: identifier for the solution procedure.
        :param printFlag: if not zero print convergence results on each step.
        :param soeType: type of the system of equations object.
        :param solverType: type of the solver.
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        '''        
        super(BucklingAnalysisStaticPart,self).__init__(name, constraintHandlerType= constraintHandlerType, maxNumIter= maxNumIter, convergenceTestTol= convergenceTestTol, printFlag= printFlag, numSteps= numSteps, numberingMethod= numberingMethod, convTestType= convTestType, soeType= soeType, solverType= solverType)
        self.feProblem= prb
        self.solnAlgorithmType= solnAlgorithmType
                
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super(BucklingAnalysisStaticPart,self).setup()
        self.solutionAlgorithmSetup(solAlgType= self.solnAlgorithmType, integratorType= 'load_control_integrator')
        # self.solAlgo.maxDimension= self.maxDim
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')


class LinearBucklingAnalysis(object):
    ''' Linear buckling analysis.

    :ivar staticPart: solution for the static problem.
    :ivar eigenPart: solution for the eigen problem.
    :ivar numModes: number of buckling modes to compute.
    '''
    def __init__(self, prb, numModes, constraintHandlerType= 'transformation', numberingMethod= 'rcm', convTestType= 'norm_disp_incr_conv_test', convergenceTestTol= 1e-8, maxNumIter= 1000, soeType= 'band_gen_lin_soe', solverType= 'band_gen_lin_lapack_solver', solnAlgorithmType= 'krylov_newton_soln_algo', eigenSOEType= 'band_arpackpp_soe', eigenSolverType= 'band_arpackpp_solver'):
        ''' Constructor.

        '''
        self.feProblem= prb
        self.numModes= numModes
        self.constraintHandlerType= constraintHandlerType 
        self.numberingMethod= numberingMethod 
        self.convTestType= convTestType 
        self.convergenceTestTol= convergenceTestTol 
        self.maxNumIter= maxNumIter 
        self.soeType= soeType 
        self.solverType= solverType 
        self.solnAlgorithmType= solnAlgorithmType 
        self.eigenSOEType= eigenSOEType 
        self.eigenSolverType= eigenSolverType 
                
    def soluControlSetup(self):
        ''' Defines the solution control object.'''
        self.solu= self.feProblem.getSoluProc
        self.solCtrl= self.solu.getSoluControl
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        self.staticPart= BucklingAnalysisStaticPart(prb= self.feProblem, constraintHandlerType= self.constraintHandlerType , numberingMethod= self.numberingMethod , convTestType= self.convTestType , convergenceTestTol= self.convergenceTestTol, maxNumIter= self.maxNumIter, soeType= self.soeType , solverType= self.solverType , solnAlgorithmType= self.solnAlgorithmType )
        self.staticPart.setup()

        self.eigenPart= BucklingAnalysisEigenPart(prb= self.feProblem, staticAnalysisPart= self.staticPart.solutionStrategy, soeType= self.eigenSOEType, solverType= self.eigenSolverType)
        self.eigenPart.setup()
        self.soluControlSetup()
        self.analysisSetup()
        self.analysis.numModes= self.numModes

    def analysisSetup(self):
        ''' Define the analysis object.

        :param analysisType: type of the analysis to perform.
        '''
        self.analysis= self.solu.newAnalysis('linear_buckling_analysis',self.staticPart.getSolutionStrategyName(),self.eigenPart.getSolutionStrategyName())
        
    def solve(self):
        ''' Compute the linear buckling modes.'''
        return self.analysis.analyze(self.numModes)

    
# Displacement control analysis.
class DisplacementControlBase(SolutionProcedure):
    ''' Base class for displacement control analysis.

    :ivar dispControlNode: node whose response controls solution.
    :ivar dispControlDof: degree of freedom that controls solution.
    :ivar dispControlIncrement: first displacement increment.
    '''
    def __init__(self, prb, node, dof, increment, numSteps, name, constraintHandlerType, maxNumIter, convergenceTestTol, printFlag, numberingMethod, convTestType, soeType, solverType):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param node: node whose response controls solution.
        :param dof: degree of freedom that controls solution.
        :param increment: first displacement increment.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        '''
        super().__init__(name, constraintHandlerType= constraintHandlerType, maxNumIter= maxNumIter, convergenceTestTol= convergenceTestTol, printFlag= printFlag, numSteps= numSteps, numberingMethod= numberingMethod, convTestType= convTestType, soeType= soeType, solverType= solverType)
        self.feProblem= prb
        self.dispControlNode= node
        self.dispControlDof= dof
        self.dispControlIncrement= increment
        
    def setup(self, solAlgType):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super().setup()
        self.solutionAlgorithmSetup(solAlgType= solAlgType, integratorType= 'displacement_control_integrator')
        self.sysOfEqnSetup()
        self.analysisSetup('static_analysis')
        
class SimpleNewtonRaphsonDisplacementControl(DisplacementControlBase):
    ''' Newton-Raphson solution algorithm with a
        plain constraint handler and a band general
        SOE solver.
    '''
    def __init__(self, prb, node, dof, increment, numSteps, name= None, maxNumIter= 10, convergenceTestTol= 1e-12, printFlag= 0, numberingMethod= 'rcm', convTestType= 'norm_disp_incr_conv_test'):
        ''' Constructor.

        :param prb: XC finite element problem.
        :param node: node whose response controls solution.
        :param dof: degree of freedom that controls solution.
        :param increment: first displacement increment.
        :param numSteps: number of steps to use in the analysis (useful only when loads are variable in time).
        :param name: identifier for the solution procedure.
        :param maxNumIter: maximum number of iterations (defauts to 10)
        :param convergenceTestTol: convergence tolerance (defaults to 1e-9)
        :param printFlag: if not zero print convergence results on each step.
        :param numberingMethod: numbering method (plain or reverse Cuthill-McKee or alterntive minimum degree).
        :param convTestType: convergence test for non linear analysis (norm unbalance,...).
        '''
        super().__init__(prb= prb, node= node, dof= dof, increment= increment, name= name, constraintHandlerType= 'transformation', maxNumIter= maxNumIter, convergenceTestTol= convergenceTestTol, printFlag= printFlag, numSteps= numSteps, numberingMethod= numberingMethod, convTestType= convTestType, soeType= 'band_gen_lin_soe', solverType= 'band_gen_lin_lapack_solver')
        self.feProblem= prb
        self.dispControlNode= node
        self.dispControlDof= dof
        self.dispControlIncrement= increment
        
    def setup(self):
        ''' Defines the solution procedure in the finite element 
            problem object.
        '''
        super().setup(solAlgType= 'newton_raphson_soln_algo')
