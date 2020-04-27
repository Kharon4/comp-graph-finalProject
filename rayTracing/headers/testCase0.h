#pragma once

#include "rayTrace.h"
#include "primitives.h"

vector<pointLight> pL;
vector<directionalLight> dL;

//scene specific

#define lightBounces 3
#define defaultSphereNoArcs 50
#define defaultSphereArcRez 50

void initLights() {

	//point light
	pointLight pl;
	pl.color = vec3f(100, 0, 0);
	pl.pos = vec3d(0, 4, -2);
	pL.push_back(pl);
	pl.color = vec3f(0, 100, 0);
	pl.pos = vec3d(-3 * cos(PI / 6), 4, 3 * sin(PI / 6));
	pL.push_back(pl);
	pl.color = vec3f(0, 0, 100);
	pl.pos = vec3d(3 * cos(PI / 6), 4, 3 * sin(PI / 6));
	pL.push_back(pl);
}

unsigned int getNoVertices() {
	unsigned int rVal = 0;
	//base plane
	rVal += 4;

	//sphere
	rVal += 3*vertexNo(defaultSphereArcRez, defaultSphereNoArcs);
	return rVal;
}

unsigned int getNoFaces() {
	unsigned int rVal = 0;
	//base plane
	rVal += 2;

	//sphere
	rVal += 3 * triangleNo(defaultSphereArcRez, defaultSphereNoArcs);
	return rVal;
}

void generateScene(triangle* trs , vec3d * pts) {


	//generate basePlane
	{	
		double halfSize = 10;
		pts[0] = vec3d(-halfSize, 7, -halfSize);
		pts[1] = vec3d(halfSize, 7, -halfSize);
		pts[2] = vec3d(halfSize, 7 , halfSize);
		pts[3] = vec3d(-halfSize, 7 , halfSize);
	}
	{
		triangle basePlaneShade;
		basePlaneShade.diffuseRefelctivity = vec3f(70, 70, 70);
		trs[0] = basePlaneShade;
		trs[1] = basePlaneShade;
	}

	trs[0].pts[0] = pts;
	trs[0].pts[1] = pts + 1;
	trs[0].pts[2] = pts + 2;


	trs[1].pts[0] = pts;
	trs[1].pts[1] = pts + 2;
	trs[1].pts[2] = pts + 3;

	trs += 2;
	pts += 4;

	//generate sphere
	triangle sphereColorData;
	sphereColorData.diffuseRefelctivity = vec3f(100, 100, 100);
	//sphereColorData.reflectivity = vec3f(1, 1, 1);
	//sphereColorData.transmitivity = vec3f(1,1,1);
	//sphereColorData.refractiveIndex = 0.8;
	generateSphere(vec3d(0, 6, 3), 1, defaultSphereArcRez, defaultSphereNoArcs, pts, trs, sphereColorData);
	pts += vertexNo(defaultSphereArcRez, defaultSphereNoArcs);
	trs += triangleNo(defaultSphereArcRez, defaultSphereNoArcs);
	generateSphere(vec3d(3 * cos(PI / 6), 6, 3 * sin(-PI / 6)), 1, defaultSphereArcRez, defaultSphereNoArcs, pts, trs, sphereColorData);
	pts += vertexNo(defaultSphereArcRez, defaultSphereNoArcs);
	trs += triangleNo(defaultSphereArcRez, defaultSphereNoArcs);
	generateSphere(vec3d(-3 * cos(PI / 6), 6, 3 * sin(-PI / 6)), 1, defaultSphereArcRez, defaultSphereNoArcs, pts, trs, sphereColorData);

}

