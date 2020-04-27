#pragma once

#include "rayTrace.h"
#include "primitives.h"

vector<pointLight> pL;
vector<directionalLight> dL;

//scene specific

#define lightBounces 5
#define defaultSphereNoArcs 50
#define defaultSphereArcRez 50

void initLights() {
	//global dir light
	directionalLight dl;
	dl.color = vec3f(0.75, 0.75, 0.75);
	dl.dir = vec3d(-1, 0.5, -1);
	dL.push_back(dl);

	//point light
	pointLight pl;
	pl.color = vec3f(1.5, 0, 2);
	pl.pos = vec3d(-1, 3, -1);
	pL.push_back(pl);
}

unsigned int getNoVertices() {
	unsigned int rVal = 0;
	//base plane
	rVal += 4;
	rVal *= 2;

	//box
	rVal += cuboidVertexNo;

	//sphere
	rVal += 1 * vertexNo(defaultSphereArcRez, defaultSphereNoArcs);
	return rVal;
}

unsigned int getNoFaces() {
	unsigned int rVal = 0;
	//base plane
	rVal += 2;
	rVal *= 2;

	//box;
	rVal += cuboidTriangleNo;

	//sphere
	rVal += 1 * triangleNo(defaultSphereArcRez, defaultSphereNoArcs);
	return rVal;
}

void generateScene(triangle* trs, vec3d* pts) {


	//generate basePlane
	{
		double halfSize = 100;
		pts[0] = vec3d(-halfSize, 1, -1.5);
		pts[1] = vec3d(halfSize, 1, -1.5);
		pts[2] = vec3d(halfSize, 1 + halfSize, -1.5);
		pts[3] = vec3d(-halfSize, 1 + halfSize, -1.5);
	}
	{
		triangle basePlaneShade;
		basePlaneShade.diffuseRefelctivity = vec3f(100,100, 100);
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

	//side plane
	{
		vec3d center(-2.5, 5, 0);
		double halfSize = 1;
		pts[0] = center + vec3d(-halfSize, -halfSize, -1.5);
		pts[1] = center + vec3d(halfSize, halfSize, -1.5);
		pts[2] = center + vec3d(-halfSize, -halfSize, 4);
		pts[3] = center + vec3d(halfSize, halfSize, 4);
	}
	{
		triangle basePlaneShade;
		basePlaneShade.diffuseRefelctivity = vec3f(100, 100, 100);
		basePlaneShade.reflectivity = vec3f(1, 1, 1);
		trs[0] = basePlaneShade;
		trs[1] = basePlaneShade;
	}

	trs[0].pts[0] = pts;
	trs[0].pts[1] = pts + 1;
	trs[0].pts[2] = pts + 2;


	trs[1].pts[0] = pts + 3;
	trs[1].pts[1] = pts + 2;
	trs[1].pts[2] = pts + 1;

	trs += 2;
	pts += 4;
	//box

	triangle boxColorData;
	boxColorData.diffuseRefelctivity = vec3f(200, 0, 0);
	boxColorData.reflectivity = vec3f(0.5, .5, .5);
	generateCuboid(vec3d(2, 3, -0.5), vec3d(1, 1, 1), pts, trs, boxColorData);

	pts += cuboidVertexNo;
	trs += cuboidTriangleNo;


	//generate sphere
	triangle sphereColorData;
	sphereColorData.diffuseRefelctivity = vec3f(0, 0, 200);
	sphereColorData.reflectivity = vec3f(1, 1, 1);
	generateSphere(vec3d(0, 4, -0.75), 0.75, defaultSphereArcRez, defaultSphereNoArcs, pts, trs, sphereColorData);
	
}

