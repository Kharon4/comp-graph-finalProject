#pragma once
#pragma once

#include "rayTrace.h"
#include "primitives.h"

vector<pointLight> pL;
vector<directionalLight> dL;

//scene specific

#define lightBounces 5
#define defaultSphereNoArcs 25
#define defaultSphereArcRez 25

void initLights() {
	//global dir light
	directionalLight dl;
	dl.color = vec3f(1, 1, 1);
	dl.dir = vec3d(1, 0.5, -1);
	dL.push_back(dl);

	dl.color = vec3f(-0.75, -0.75, -0.75);
	dl.dir = vec3d(1, -0.5, 1);
	//dL.push_back(dl);
	//point light
	pointLight pl;
	pl.color = vec3f(1.5, 0, 2);
	pl.pos = vec3d(-1, 3, -1);
	//pL.push_back(pl);
}

unsigned int getNoVertices() {
	unsigned int rVal = 0;
	//base plane
	rVal += 4;
	rVal *= 0;

	//box
	rVal += 2*cuboidVertexNo;

	//sphere
	rVal += 1 * vertexNo(defaultSphereArcRez, defaultSphereNoArcs);
	return rVal;
}

unsigned int getNoFaces() {
	unsigned int rVal = 0;
	//base plane
	rVal += 2;
	rVal *= 0;

	//box;
	rVal += 2*cuboidTriangleNo;

	//sphere
	rVal += 1 * triangleNo(defaultSphereArcRez, defaultSphereNoArcs);
	return rVal;
}

void generateScene(triangle* trs, vec3d* pts) {


	
	//box

	triangle boxColorData;
	boxColorData.diffuseRefelctivity = vec3f(0, 20, 0);
	//boxColorData.reflectivity = vec3f(0.1, .1, .1);
	boxColorData.transmitivity = vec3f(1, 1.3, 1.3);
	boxColorData.refractiveIndex = 1.6;
	generateCuboid(vec3d(0, 4, -1.5), vec3d(2, 2, 1), pts, trs, boxColorData);

	pts += cuboidVertexNo;
	trs += cuboidTriangleNo; 
	boxColorData.diffuseRefelctivity = vec3f(200, 0, 0);
	boxColorData.reflectivity = vec3f(0, 0,0);
	boxColorData.transmitivity = vec3f(0, 0, 0);
	boxColorData.refractiveIndex = 1;
	generateCuboid(vec3d(2, 2.5, -0.5), vec3d(1, 1, 1), pts, trs, boxColorData);

	pts += cuboidVertexNo;
	trs += cuboidTriangleNo;


	//generate sphere
	triangle sphereColorData;
	sphereColorData.diffuseRefelctivity = vec3f(0, 0, 200);
	//sphereColorData.reflectivity = vec3f(1, 1, 1);
	generateSphere(vec3d(-1.2, 4, -0.5), 0.75, defaultSphereArcRez, defaultSphereNoArcs, pts, trs, sphereColorData);

}

