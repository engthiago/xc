# -*- coding: utf-8 -*-

from __future__ import division
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) Ana Ortega (AO_O)"
__copyright__= "Copyright 2018,  LCPT AO_O "
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com ana.ortega.ort@gmail.com"

import sys
import geom
import xc
from actions import loads
from misc_utils import log_messages as lmsg
from actions.railway_traffic import dynamic_factor_load as dfl
from actions.railway_traffic import track_axis as ta

from geotechnics import horizontal_surcharge as hs
from geotechnics import boussinesq

# Railway traffic loads.
#     ____
#     |DD|____T_
#     |_ |_____|<
#     @-@-@-oo\
#

class UniformRailLoad(dfl.DynamicFactorLoad):
    ''' Uniform load along a rail.

    :ivar railAxis: 3D polyline defining the axis of the rail.
    :ivar load: value of the uniform load.
    '''
    def __init__(self, railAxis, load, dynamicFactor= 1.0, classificationFactor= 1.21):
        ''' Constructor.

        :param railAxis: 3D polyline defining the axis of the rail.
        :param load: value of the uniform load.
        :param classificationFactor: classification factor (on lines carrying
                                     rail traffic which is heavier or lighter
                                     than normal rail traffic).
        '''
        super().__init__(dynamicFactor= dynamicFactor, classificationFactor= classificationFactor)
        self.railAxis= railAxis
        self.load= load

    def getMidpoint(self):
        ''' Return the midpoint of the rail axis.'''
        return self.railAxis.getPointAtLength(0.5*self.railAxis.getLength())

    def getClassifiedLoad(self):
        ''' Return the value of the load affected by the classification
            factor.'''
        return self.load*self.classificationFactor

    def getDynamicLoad(self):
        ''' Return the value of the load affected by the dynamic factor.'''
        return self.getClassifiedLoad()*self.dynamicFactor

    def clip(self, clippingPlane):
        ''' Clips the rail axis with the plane argument. It's used to remove
        the part of the rail axis that lies behind the plane (normally the
        plane will correspond to the midplane of a wall that will resist
        the pressures caused by this load. That way, if some of the rail axis
        lies behind the wall we'll remove the "negative" pressures caused
        by that chunk of the rail load.

        :param clippingPlane: plane to clip with.
        '''
        intersections= self.railAxis.getIntersection(clippingPlane)
        if(len(intersections)>0):
            if(len(intersections)>1):
                className= type(self).__name__
                methodName= sys._getframe(0).f_code.co_name
                lmsg.warning(className+'.'+methodName+'; not implemented for multiple intersections.')
            else:
                pInt= intersections[0]
                chunks= self.railAxis.split(pInt)
                l0= chunks[0].getLength()
                l1= chunks[1].getLength()
                if(l0>l1):
                    self.railAxis= chunks[0]
                else:
                    self.railAxis= chunks[1]

    def getRailAxisProjection(self, midplane):
        ''' Return the projection of the rail axis onto the plane argument.

        :param midplane: mid-plane of the loaded surface.
        '''
        posList= list()
        ref= midplane.getRef()
        for v in self.railAxis.getVertexList():
            # vertLine= geom.Line3d(v, geom.Vector3d(0,0,-100.0))
            projPos= midplane.getProjection(v)
            posList.append(ref.getLocalPosition(projPos))
        return geom.Polyline2d(posList)

    def getLoadedContour(self, midplane, spreadingLayers= None):
        ''' Return the loaded contour of the wheel taking into account
            the dispersal through the different pavement, earth and
            concrete layers between the wheel contact area and the
            middle surface represented by the plane argument.

        :param midplane: mid-plane of the loaded surface.
        :param spreadingLayers: list of tuples containing the depth
                                and the spread-to-depth ratio of
                                the layers between the wheel contact
                                area and the middle surface of the
                                bridge deck.
        '''
        # Compute spread.
        spread= 0.0
        if(spreadingLayers):
            for sl in spreadingLayers:
                layerDepth= sl[0]
                layerSpreadToDepthRatio= sl[1]
                spread+= layerDepth*layerSpreadToDepthRatio
        railProjection= self.getRailAxisProjection(midplane)
        rLength= railProjection.getLength()
        # Remove consecutive vertices that are too close
        railProjection.removeRepeatedVertexes(1e-3)
        # Remove backward segments.
        railProjection.removeBackwardSegments(-0.1)
        # Remove collinear contiguous segments from the polyline
        # otherwise there are vertexes that are not in a "corner"
        # so the buffer algorithm cannot found the interseccion
        # between the "offseted" lines.
        epsilon= railProjection.getLength()/1e3 # compute a reasonable epsilon.
        railProjection.simplify(epsilon) # simplify the projected axis.
        retval= railProjection.getBufferPolygon(spread, 8)
        return retval

    def getDeckLoadedContourThroughLayers(self, spreadingLayers, deckMidplane, deckThickness, deckSpreadingRatio= 1/1):
        ''' Return the loaded contour of the rail taking into account
            the dispersal through the different pavement, earth and
            concrete layers between the wheel contact area and the
            middle surface of the bridge deck.

        :param spreadingLayers: list of tuples containing the depth
                                and the spread-to-depth ratio of
                                the layers between the wheel contact
                                area and the middle surface of the
                                bridge deck.
        :param deckMidplane: mid-plane of the bridge deck.
        :param deckThickness: thickness of the bridge deck.
        :param deckSpreadingRatio: spreading ratio of the load between the deck
                                   surface and the deck mid-plane (see
                                   clause 4.3.6 on Eurocode 1-2:2003).
        '''
        ## Append the deck spreading to the list.
        spreadingLayers.append((deckThickness/2.0, deckSpreadingRatio))
        # Call the regular method.
        return self.getLoadedContour(midplane= deckMidplane, spreadingLayers= spreadingLayers)
    
    def getDeckLoadedContourThroughEmbankment(self, embankment, deckMidplane, deckThickness, deckSpreadingRatio= 1/1):
        ''' Return the loaded contour of the rail taking into account
            the dispersal through the different pavement, earth and
            concrete layers between the wheel contact area and the
            middle surface of the bridge deck.

        :param embankment: embankment object as defined in
                           earthworks.embankment.
        :param deckMidplane: mid-plane of the bridge deck.
        :param deckThickness: thickness of the bridge deck.
        :param deckSpreadingRatio: spreading ratio of the load between the deck
                                   surface and the deck mid-plane (see
                                   clause 4.3.6 on Eurocode 1-2:2003).
        '''
        # Compute spreading Layers.
        ## Compute the vertical proyection of the midpoint of the rail on
        ## the deck mid-plane.
        vertLine= geom.Line3d(self.getMidpoint(), geom.Vector3d(0,0,-100.0))
        projPos= deckMidplane.getIntersection(vertLine)

        ## Compute half deck thickness.
        halfDeckThickness= deckThickness/2.0
        ## Ask the embankment about the layer thicknesses in this position.
        layerThicknesses= embankment.getLayerThicknesses(point= projPos+geom.Vector3d(0,0,halfDeckThickness))
        ## Construct the spreading layers list.
        spreadingLayers= list()
        for thk, layer in zip(layerThicknesses, embankment.layers):
            spreadingLayers.append((thk, layer.loadSpreadingRatio))
        # Call the regular method.
        return self.getLoadedContourThroughLayers(spreadingLayers= spreadingLayers, deckMidplane= deckMidplane, deckThickness= deckThickness, deckSpreadingRatio= deckSpreadingRation)

    def getNodalLoadVector(self, numNodes, gravityDir= xc.Vector([0,0,-1])):
        ''' Return the load vector at the contact surface.

        :param numNodes: number of loaded nodes.
        :param gravityDir: direction of the gravity field (unit vector).
        '''
        # Compute load vector.
        totalLoad= self.getDynamicLoad()*self.railAxis.getLength()
        nodalLoad= totalLoad/numNodes
        retval= nodalLoad*gravityDir
        return retval

    def pickLoadedNodes(self, loadedContour, originSet, deckMidplane):
        ''' Define uniform loads on the tracks with the argument values:

        :param loadedContour: 2D polygon defining the loaded region.
        :param originSet: set to pick the loaded nodes from.
        :param deckMidplane: midplane of the loaded surface.
        '''
        # Pick loaded nodes.
        retval= list()
        tol= .01
        ref= deckMidplane.getRef()
        for n in originSet.nodes:
            posProj= deckMidplane.getProjection(n.getInitialPos3d)
            nodePos2d= ref.getLocalPosition(posProj)
            if(loadedContour.In(nodePos2d, tol)):  # node in loaded contour.
                retval.append(n)
        return retval

    def applyNodalLoads(self, loadedNodes, gravityDir):
        ''' Apply the load in the given node list.

        :param loadedNodes: nodes that will be loaded.
        :param gravityDir: direction of the gravity field (unit vector).
        '''
        # Compute load vector
        numLoadedNodes= len(loadedNodes)
        vLoad= self.getNodalLoadVector(numLoadedNodes, gravityDir= gravityDir)
        loadVector= xc.Vector([vLoad[0],vLoad[1],vLoad[2],0.0,0.0,0.0])
        # Apply nodal loads.
        retval= list()
        for n in loadedNodes:
            retval.append(n.newLoad(loadVector))
        return retval

    def computeNodalBrakingLoads(self, loadedNodes, brakingLoad):
        ''' Compute the load vector for each loaded node due to braking.

        :param loadedNodes: nodes that will be loaded.
        :param brakingLoad: total rail load due to braking.
        '''
        numLoadedNodes= len(loadedNodes)
        brakingLoadPerNode= brakingLoad/numLoadedNodes # load for each node.
        retval= list()
        for n in loadedNodes:
            pos= n.getInitialPos3d
            nearestSegment= self.railAxis.getNearestSegment(pos)
            brakingDir= nearestSegment.getIVector
            brakingLoad= brakingLoadPerNode*brakingDir
            loadVector= xc.Vector([brakingLoad.x, brakingLoad.y, brakingLoad.z, 0.0,0.0,0.0])
            retval.append(loadVector)
        return retval
        
    def defDeckRailUniformLoadsThroughLayers(self, spreadingLayers, originSet, deckMidplane, deckThickness, deckSpreadingRatio= 1/1, gravityDir= xc.Vector([0,0,-1])):
        ''' Define uniform loads on the tracks with the argument values:

        :param spreadingLayers: list of tuples containing the depth
                                and the spread-to-depth ratio of
                                the layers between the wheel contact
                                area and the middle surface of the
                                bridge deck.
        :param originSet: set to pick the loaded nodes from.
        :param deckMidplane: midplane of the loaded surface.
        :param deckThickness: thickness of the bridge deck.
        :param deckSpreadingRatio: spreading ratio of the load between the deck
                                   surface and the deck mid-plane (see
                                   clause 4.3.6 on Eurocode 1-2:2003).
        :param gravityDir: direction of the gravity field (unit vector).
        '''
        loadedContour= self.getDeckLoadedContourThroughLayers(spreadingLayers= spreadingLayers, deckMidplane= deckMidplane, deckThickness= deckThickness, deckSpreadingRatio= deckSpreadingRatio)
        # Pick loaded nodes.
        loadedNodes= self.pickLoadedNodes(loadedContour= loadedContour, originSet= originSet, deckMidplane= deckMidplane)
        # Apply nodal loads.
        retval= self.applyNodalLoads(loadedNodes= loadedNodes, gravityDir= gravityDir)
        return retval
    
    def defDeckRailUniformLoadsThroughEmbankment(self, embankment, originSet, deckMidplane, deckThickness, deckSpreadingRatio= 1/1, gravityDir= xc.Vector([0,0,-1])):
        ''' Define uniform loads on the tracks with the argument values:

        :param embankment: embankment object as defined in
                           earthworks.embankment.
        :param originSet: set to pick the loaded nodes from.
        :param deckMidplane: midplane of the loaded surface.
        :param deckThickness: thickness of the bridge deck.
        :param deckSpreadingRatio: spreading ratio of the load between the deck
                                   surface and the deck mid-plane (see
                                   clause 4.3.6 on Eurocode 1-2:2003).
        :param gravityDir: direction of the gravity field (unit vector).
        '''
        loadedContour= self.getDeckLoadedContourThroughEmbankment(embankment= embankment, deckMidplane= deckMidplane, deckThickness= deckThickness, deckSpreadingRatio= deckSpreadingRatio)
        # Pick loaded nodes.
        loadedNodes= self.pickLoadedNodes(loadedContour= loadedContour, originSet= originSet, deckMidplane= deckMidplane)
        # Apply nodal loads.
        retval= self.applyNodalLoads(loadedNodes= loadedNodes, gravityDir= gravityDir)
        return retval

    def defDeckRailBrakingLoadsThroughLayers(self, brakingLoad, spreadingLayers, originSet, deckMidplane, deckThickness, deckSpreadingRatio= 1/1):
        ''' Define uniform loads on the tracks with the argument values:

        :param brakingLoad: total rail load due to braking.
        :param spreadingLayers: list of tuples containing the depth
                                and the spread-to-depth ratio of
                                the layers between the wheel contact
                                area and the middle surface of the
                                bridge deck.
        :param originSet: set to pick the loaded nodes from.
        :param deckMidplane: midplane of the loaded surface.
        :param deckThickness: thickness of the bridge deck.
        :param deckSpreadingRatio: spreading ratio of the load between the deck
                                   surface and the deck mid-plane (see
                                   clause 4.3.6 on Eurocode 1-2:2003).
        '''
        loadedContour= self.getDeckLoadedContourThroughLayers(spreadingLayers= spreadingLayers, deckMidplane= deckMidplane, deckThickness= deckThickness, deckSpreadingRatio= deckSpreadingRatio)
        # Pick loaded nodes.
        loadedNodes= self.pickLoadedNodes(loadedContour= loadedContour, originSet= originSet, deckMidplane= deckMidplane)
        # Compute braking load for each node.
        brakingLoads= self.computeNodalBrakingLoads(loadedNodes= loadedNodes, brakingLoad= brakingLoad)
        # Apply nodal loads.
        retval= list()
        for n, brakingLoad in zip(loadedNodes, brakingLoads):
            retval.append(n.newLoad(brakingLoad))
        return retval
    
    def defBackfillUniformLoads(self, originSet, embankment, delta, eta= 1.0, gravityDir= xc.Vector([0,0,-1])):
        ''' Define backfill loads due the uniform loads on the rail.

        :param originSet: set containing the elements to pick from.
        :param embankment: embankment object as defined in
                           earthworks.embankment.
        :param delta: friction angle between the soil and the element material.
        :param eta: Poisson's ratio (ATTENTION: defaults to 1.0: see
                    implementation remarks in boussinesq module).
        :param gravityDir: direction of the gravity field (unit dir).
        '''
        dynamicLoad= self.getDynamicLoad()
        vertexList= self.railAxis.getVertexList()
        if((abs(dynamicLoad)>0) and (len(vertexList)>1)):
            v0= vertexList[0]
            # Compute mid-plane of the loaded wall.
            wallMidplane= originSet.nodes.getRegressionPlane(0.0)
            for v1 in vertexList[1:]:
                segment= geom.Segment3d(v0,v1)
                # Check if segment is too near to the wall.
                d0= wallMidplane.dist(segment.getFromPoint())
                d1= wallMidplane.dist(segment.getToPoint())
                sgLength= segment.getLength()
                tol= 0.5  # too near (arbitrary value -> try to enhance this)
                if((d0<tol) or (d1<tol)):  # Segment too near to the wall.
                    if(d0<tol):
                        newPoint= segment.getPoint(tol)
                        segment= geom.Segment3d(newPoint, v1)
                    else:  # (d1<tol)
                        L= segment.getLength()-tol
                        newPoint= segment.getPoint(L)
                        segment= geom.Segment3d(v0, newPoint)
                loadedLine= boussinesq.LinearLoad(segment= segment, loadValues=[-dynamicLoad, -dynamicLoad], eSize= 0.25)
                # Compute loads on elements.
                loadedLine.appendLoadToCurrentLoadPattern(elements= originSet.elements, eta= eta, delta= delta)
                v0= v1
    

class TrackAxes(object):
    ''' Track axis container.

    :ivar trackAxes: list of track axes.
    '''
    def __init__(self, trackAxesPolylines):
        ''' Constructor.

        :param railAxesPolylines: 3D polylines defining the axis of track.
        '''
        self.trackAxes= list()
        for trackAxisPline in trackAxesPolylines:
            self.trackAxes.append(ta.TrackAxis(trackAxisPline))

    def getLengths(self):
        ''' Return the lengths of the track axes.'''
        retval= list()
        for axis in self.trackAxes:
            retval.append(axis.getArea())
        return retval

    def getNumberTracks(self):
        ''' Return the number of tracks.'''
        return len(self.trackAxes)

    def getWheelLoads(self, trainModels, relativePositions, wallMidplane, gravityDir= xc.Vector([0,0,-1])):
        ''' Return a the wheel loads due to the locomotives of the trains
            in the argument placed at the positions argument.

        :param trainModels: trainModels on each track (train model 1 -> track 1,
                            train model 2 -> track 2 and so on).
        :param relativePositions: relative positions of the locomotive center in
                                  the track axis (0 -> beginning of
                                  the axis, 0.5-> middle of the axis, 1-> end
                                  of the axis).
        :param wallMidplane: mid-plane of the loaded wall (to select the wheels
                             at one side of the wall).
        '''
        retval= list()
        locomotives= list()
        for tm in trainModels:
            locomotives.append(tm.locomotive)
        # Compute load positions in each track.
        for ta,rpos,td in zip(self.trackAxes, relativePositions, locomotives):
            locomotiveLoads= list()
            if(td is not None):
                locomotiveLoads= ta.getWheelLoads(loadModel= td, relativePosition= rpos, wallMidplane= wallMidplane, gravityDir= gravityDir)
            retval.extend(locomotiveLoads)
        return retval

    def defDeckWheelLoadsThroughLayers(self, trainModels, relativePositions, spreadingLayers= None, originSet= None, deckThickness= 0.0, deckSpreadingRatio= 1/1, gravityDir= xc.Vector([0,0,-1])):
        ''' Define the wheel loads due to the locomotives argument placed at
            the positions argument.

        :param trainModels: trainModels on each track (train model 1 -> track 1,
                            train model 2 -> track 2 and so on).
        :param relativePositions: relative positions of the locomotive center in
                                  the track axis (0 -> beginning of
                                  the axis, 0.5-> middle of the axis, 1-> end
                                  of the axis).
        :param spreadingLayers: list of tuples containing the depth
                                and the spread-to-depth ratio of
                                the layers between the wheel contact
                                area and the middle surface of the
                                concrete slab.
        :param originSet: pick the loaded nodes for each wheel load.
        :param deckThickness: thickness of the bridge deck.
        :param deckSpreadingRatio: spreading ratio of the load between the deck
                                   surface and the deck mid-plane (see
                                   clause 4.3.6 on Eurocode 1-2:2003).
        :param gravityDir: direction of the gravity field (unit vector).
        '''
        wheelLoads= self.getWheelLoads(trainModels= trainModels, relativePositions= relativePositions, wallMidplane= None, gravityDir= gravityDir)
        retval= list()
        if(originSet):  # pick the loaded by each wheel
            for wheelLoad in wheelLoads:
                retval.extend(wheelLoad.defDeckConcentratedLoadsThroughLayers(spreadingLayers= spreadingLayers, originSet= originSet, deckThickness= deckThickness, deckSpreadingRatio= deckSpreadingRatio))
        return retval

    def getBackfillConcentratedLoads(self, trainModels, relativePositions, wallMidplane, gravityDir= xc.Vector([0,0,-1])):
        ''' Return the wheel loads on the backfill due to the train models
        argument placed at the positions argument.

        :param trainModels: trainModels on each track (train model 1 -> track 1,
                            train model 2 -> track 2 and so on).
        :param relativePositions: relative positions of the locomotive center in
                                  the track axis (0 -> beginning of
                                  the axis, 0.5-> middle of the axis, 1-> end
                                  of the axis).
        :param wallMidplane: mid-plane of the loaded wall (to select the wheels
                             at one side of the wall).
        :param gravityDir: direction of the gravity field (unit vector).
        '''
        wheelLoads= self.getWheelLoads(trainModels= trainModels, relativePositions= relativePositions, wallMidplane= wallMidplane, gravityDir= gravityDir)
        retval= list()
        for wheelLoad in wheelLoads:
            retval.append(wheelLoad.getBackfillConcentratedLoad())
        return retval

    def defDeckRailUniformLoadsThroughEmbankment(self, trainModels, relativePositions, embankment, originSet, deckThickness, deckSpreadingRatio= 1/1, gravityDir= xc.Vector([0,0,-1])):
        ''' Define uniform loads on the tracks with the argument values:

        :param trainModels: trainModels on each track (train model 1 -> track 1,
                            train model 2 -> track 2 and so on).
        :param relativePositions: relative positions of the locomotive center in
                                  the track axis (0 -> beginning of
                                  the axis, 0.5-> middle of the axis, 1-> end
                                  of the axis).
        :param embankment: embankment object as defined in
                           earthworks.embankment.
        :param originSet: set to pick the loaded nodes from.
        :param deckThickness: thickness of the bridge deck.
        :param deckSpreadingRatio: spreading ratio of the load between the deck
                                   surface and the deck mid-plane (see
                                   clause 4.3.6 on Eurocode 1-2:2003).
        :param gravityDir: direction of the gravity field (unit vector).
        '''
        for ta, tm, rp in zip(self.trackAxes, trainModels, relativePositions):
            q= tm.getDynamicUniformLoad()
            if(abs(q)>0.0):
                ta.defDeckRailUniformLoadsThroughEmbankment(trainModel= tm, relativePosition= rp, embankment= embankment, originSet= originSet, deckThickness= deckThickness, deckSpreadingRatio= deckSpreadingRatio, gravityDir= gravityDir)

    def defDeckConcentratedLoadsThroughLayers(self, trainModels, relativePositions, originSet= None, deckThickness= 0.0, deckSpreadingRation= 1/1, gravityDir= xc.Vector([0,0,-1]), spreadingLayers= None):
        ''' Define punctual loads under the wheels.

        :param trainModels: trainModels on each track (train model 1 -> track 1,
                            train model 2 -> track 2 and so on).
        :param relativePositions: relative positions of the locomotive center in
                                  the track axis (0 -> beginning of
                                  the axis, 0.5-> middle of the axis, 1-> end
                                  of the axis).
        :param originSet: set with the nodes to pick from.
        :param deckThickness: thickness of the bridge deck.
        :param gravityDir: direction of the gravity field (unit vector).
        :param spreadingLayers: list of tuples containing the depth
                                and the spread-to-depth ratio of
                                the layers between the wheel contact
                                area and the middle surface of the
                                bridge deck.
        '''
        return self.defDeckWheelLoadsThroughLayers(trainModels= trainModels, relativePositions= relativePositions, spreadingLayers= spreadingLayers, originSet= originSet, deckThickness= deckThickness, deckSpreadingRatio= 1/1, gravityDir= gravityDir)

    def defDeckLoadsThroughEmbankment(self, trainModels, relativePositions, trackUniformLoads, embankment, deckThickness, deckSpreadingRatio= 1/1, originSet= None, gravityDir= xc.Vector([0,0,-1]), spreadingLayers= None):
        ''' Define punctual and uniform loads.

        :param trainModels: trainModels on each track (train model 1 -> track 1,
                            train model 2 -> track 2 and so on).
        :param relativePositions: relative positions of each locomotive center in
                                  the track axis (0 -> beginning of
                                  the axis, 0.5-> middle of the axis, 1-> end
                                  of the axis).
        :param trackUniformLoads: load for each track [1st, 2nd, 3rd,...].
        :param embankment: embankment object as defined in
                           earthworks.embankment.
        :param deckThickness: thickness of the bridge deck.
        :param deckSpreadingRatio: spreading ratio of the load between the deck
                                   surface and the deck mid-plane (see
                                   clause 4.3.6 on Eurocode 1-2:2003).
        :param originSet: set containing the nodes to pick from.
        :param gravityDir: direction of the gravity field (unit vector).
        :param spreadingLayers: list of tuples containing the depth
                                and the spread-to-depth ratio of
                                the layers between the wheel contact
                                area and the middle surface of the
                                bridge deck.
        '''
        # concentrated loads.
        self.defDeckConcentratedLoadsThroughLayers(trainModels= trainModels, relativePositions= relativePositions, originSet= originSet, gravityDir= gravityDir, spreadingLayers= spreadingLayers)
        # uniform load.
        self.defDeckRailUniformLoadsThroughEmbankment(trainModels= trainModels, relativePositions= relativePositions, embankment= embankment, originSet= originSet, deckThickness= deckThickness, deckSpreadingRatio= deckSpreadingRatio, gravityDir= gravityDir)

    def getDeckWheelLoadsThroughEmbankment(self, trainModels, relativePositions, embankment, deckThickness, deckSpreadingRatio= 1/1, originSet= None, gravityDir= xc.Vector([0,0,-1])):
        ''' Return a dictionary containing the wheel loads due to the locomotives
            argument in the positions argument.

        :param trainModels: trainModels on each track (train model 1 -> track 1,
                            train model 2 -> track 2 and so on).
        :param relativePositions: relative positions of the locomotive center in
                                  the track axis (0 -> beginning of
                                  the axis, 0.5-> middle of the axis, 1-> end
                                  of the axis).
        :param embankment: embankment object as defined in
                           earthworks.embankment.
        :param deckThickness: thickness of the bridge deck.
        :param deckSpreadingRatio: spreading ratio of the load between the deck
                                   surface and the deck mid-plane (see
                                   clause 4.3.6 on Eurocode 1-2:2003).
        :param originSet: set containing the nodes to pick from.
        :param gravityDir: direction of the gravity field (unit vector).
        '''
        retval= self.getWheelLoads(trainModels= trainModels, relativePositions= relativePositions, wallMidplane= None, gravityDir= gravityDir)
        if(originSet):  # pick the loaded by each wheel
            for load in retval:
                load.pickDeckNodesThroughEmbankment(originSet= originSet, embankment= embankment, deckThickness= deckThickness, deckSpreadingRatio= deckSpreadingRatio)
        return retval

    def defDeckConcentratedLoadsThroughEmbankment(self, trainModels, relativePositions, embankment, deckThickness, deckSpreadingRatio= 1/1, originSet= None, gravityDir= xc.Vector([0,0,-1])):
        ''' Define punctual loads under the wheels.

        :param trainModels: train models on each track (train model 1-> track 1,
                            train model 2 -> track 2 and so on).
        :param relativePositions: relative positions of the locomotive center in
                                  the track axis (0 -> beginning of
                                  the axis, 0.5-> middle of the axis, 1-> end
                                  of the axis).
        :param embankment: embankment object as defined in
                           earthworks.embankment.
        :param deckThickness: thickness of the deck bridge.
        :param deckSpreadingRatio: spreading ratio of the load between the deck
                                   surface and the deck mid-plane (see
                                   clause 4.3.6 on Eurocode 1-2:2003).
        :param originSet: set containing the nodes to pick from.
        :param gravityDir: direction of the gravity field (unit vector).
        '''
        wheelLoads= self.getDeckWheelLoadsThroughEmbankment(trainModels= trainModels, relativePositions= relativePositions, embankment= embankment, deckThickness= deckThickness, deckSpreadingRatio= deckSpreadingRatio, originSet= originSet)
        retval= list()
        for wl in wheelLoads:
            retval.append(wl.defNodalLoads(gravityDir= gravityDir))
        return retval

    def defDeckLoadsThroughEmbankment(self, trainModels, relativePositions, embankment, originSet, deckThickness, deckSpreadingRatio= 1/1, gravityDir= xc.Vector([0, 0, -1])):
        ''' Define punctual and uniform loads.
        :param trainModels: trainModels on each track (trainModel1 -> track 1,
                            trainModel 2 -> track 2 and so on).
        :param relativePositions: relative positions of each locomotive center in
                                  the track axis (0 -> beginning of
                                  the axis, 0.5-> middle of the axis, 1-> end
                                  of the axis).
        :param trackUniformLoads: load for each track [1st, 2nd, 3rd,...].
        :param embankment: embankment object as defined in
                           earthworks.embankment.
        :param originSet: set containing the nodes to pick from.
        :param deckThickness: thickness of the deck bridge.
        :param deckSpreadingRatio: spreading ratio of the load between the deck
                                   surface and the deck mid-plane (see
                                   clause 4.3.6 on Eurocode 1-2:2003).
        :param gravityDir: direction of the gravity field (unit vector).
        '''
        # punctual loads.
        self.defDeckConcentratedLoadsThroughEmbankment(trainModels= trainModels, relativePositions= relativePositions, embankment= embankment, deckThickness= deckThickness, deckSpreadingRatio=deckSpreadingRatio, originSet= originSet, gravityDir= gravityDir)
        # uniform load.
        self.defDeckRailUniformLoadsThroughEmbankment(trainModels= trainModels, relativePositions= relativePositions, embankment= embankment, originSet= originSet, deckThickness= deckThickness, deckSpreadingRatio= deckSpreadingRatio, gravityDir= gravityDir)

    def defBackfillConcentratedLoads(self, trainModels, relativePositions, originSet, embankment, delta, eta= 1.0, gravityDir= xc.Vector([0, 0, -1])):
        ''' Define punctual loads under the wheels.
        :param trainModels: train models on each track (trainModel1 -> track 1,
                            trainModel2 -> track 2 and so on).
        :param relativePositions: relative positions of the locomotive center in
                                  the track axis (0 -> beginning of
                                  the axis, 0.5-> middle of the axis, 1-> end
                                  of the axis).
        :param originSet: set containing the elements to pick from.
        :param embankment: embankment object as defined in
                           earthworks.embankment.
        :param delta: friction angle between the soil and the element material.
        :param eta: Poisson's ratio (ATTENTION: defaults to 1.0: see
                    implementation remarks in boussinesq module).
        :param gravityDir: direction of the gravity field (unit vector).
        '''
        setMidplane= originSet.nodes.getRegressionPlane(0.0)
        backfillLoads= self.getBackfillConcentratedLoads(trainModels= trainModels, relativePositions= relativePositions, wallMidplane= setMidplane, gravityDir= gravityDir)
        phi= embankment.layers[0].soil.phi
        sz= len(backfillLoads)
        avgLoadedAreaRatio= 0.0
        for bfl in backfillLoads:
            horizontalLoad= bfl[0]
            avgLoadedAreaRatio+= horizontalLoad.appendLoadToCurrentLoadPattern(elements= originSet.elements, phi= phi, delta= delta)
            verticalLoad= bfl[1]
            verticalLoad.appendLoadToCurrentLoadPattern(elements= originSet.elements, eta= eta, delta= delta)
        if(sz):
            avgLoadedAreaRatio/= sz
        return avgLoadedAreaRatio

    def defBackfillUniformLoads(self, trainModels, relativePositions, originSet, embankment, delta, eta= 1.0, gravityDir= xc.Vector([0, 0, -1])):
        ''' Define backfill loads due the uniform loads on the tracks.

        :param trainModels: train models on each track (trainModel1 -> track 1,
                            trainModel2 -> track 2 and so on).
        :param relativePositions: relative positions of the locomotive center in
                                  the track axis (0 -> beginning of
                                  the axis, 0.5-> middle of the axis, 1-> end
                                  of the axis).
        :param originSet: set containing the elements to pick from.
        :param embankment: embankment object as defined in
                           earthworks.embankment.
        :param delta: friction angle between the soil and the element material.
        :param eta: Poisson's ratio (ATTENTION: defaults to 1.0: see
                    implementation remarks in boussinesq module).
        :param gravityDir: direction of the gravity field (unit vector).
        '''
        for trackAxis, tm, rp in zip(self.trackAxes, trainModels, relativePositions):
            trackAxis.defBackfillUniformLoads(trainModel= tm, relativePosition= rp, originSet= originSet, embankment= embankment, delta= delta, eta= eta, gravityDir= gravityDir)

    def defBackfillLoads(self, trainModels, relativePositions, originSet, embankment, delta, eta= 1.0, gravityDir= xc.Vector([0, 0, -1])):
        ''' Define punctual and uniform loads.

        :param trainModels: train model on each track (train model 1 -> track 1,
                            train model 2 -> track 2 and so on).
        :param relativePositions: relative positions of each locomotive center in
                                  the track axis (0 -> beginning of
                                  the axis, 0.5-> middle of the axis, 1-> end
                                  of the axis).
        :param originSet: set containing the elements to pick from.
        :param embankment: embankment object as defined in
                           earthworks.embankment.
        :param delta: friction angle between the soil and the element material.
        :param eta: Poisson's ratio (ATTENTION: defaults to 1.0: see
                    implementation remarks in boussinesq module).
        :param gravityDir: direction of the gravity field (unit vector).
        '''
        # punctual loads.
        self.defBackfillConcentratedLoads(trainModels= trainModels, relativePositions= relativePositions, originSet= originSet, embankment= embankment, delta= delta, eta= eta, gravityDir= gravityDir)
        # uniform load.
        self.defBackfillUniformLoads(trainModels= trainModels, relativePositions= relativePositions, originSet= originSet, embankment= embankment, delta= delta, eta= eta, gravityDir= gravityDir)
