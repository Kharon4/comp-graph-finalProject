#pragma once
#include "rayTrace.h"

vector<pointLight> pL;
vector<directionalLight> dL;
#define PI 3.14159

#define vertexNo(arcRes , noArcs) ((arcRes-2)*noArcs + 2)
#define triangleNo(arcRes , noArcs) ((arcRes-3)*noArcs*2 + 2*noArcs)


void generateSphere(vec3d center, double rad, unsigned char halfArcRes, unsigned char halfArcs, vec3d* OUTpts, triangle* OUTmesh,triangle shadingProperties) {
	//generate all arcs
	for (unsigned char arcNo = 0; arcNo < halfArcs; ++arcNo) {
		double azimuthal = arcNo * 2 * (PI/ halfArcs);
		double cosA = cos(azimuthal);
		double sinA = sin(azimuthal);
		for (unsigned char latitude = 1; latitude < halfArcRes - 1; ++latitude) {
			double pitch = ((((double)latitude) / (halfArcRes-1)) - 0.5) * PI;
			double cosP = cos(pitch);
			double sinP = sin(pitch);

			//set vertex
			unsigned int vIndex = (latitude - 1) + arcNo * (halfArcRes - 2);
			OUTpts[vIndex] = center + rad* vec3d(cosP * cosA , cosP*sinA , sinP);

		}
	}
	//set final two vertices
	int size = (halfArcRes - 2) * halfArcs;
	OUTpts[size] = center - rad * vec3d(0, 0, 1);
	OUTpts[size+1] = center + rad * vec3d(0, 0, 1);

	//connect inner triangles
	for (unsigned char arcNo = 0; arcNo < halfArcs; ++arcNo) {
		for (unsigned char latitude = 1; latitude < halfArcRes - 2; ++latitude) {
			//genrate two trs
			unsigned int vIndex = (latitude - 1) + arcNo * (halfArcRes - 2);
			unsigned int index = 2*((latitude - 1) + arcNo * (halfArcRes - 3));
			//tr1
			OUTmesh[index].pts[0] = OUTpts + vIndex;
			OUTmesh[index].pts[1] = OUTpts + (vIndex + (halfArcRes - 2)) % size;
			OUTmesh[index].pts[2] = OUTpts + vIndex + 1;
			
			OUTmesh[index].diffuseRefelctivity = shadingProperties.diffuseRefelctivity;
			OUTmesh[index].reflectivity = shadingProperties.reflectivity;
			OUTmesh[index].refractiveIndex = shadingProperties.refractiveIndex;
			OUTmesh[index].transmitivity = shadingProperties.transmitivity;


			//tr2
			OUTmesh[index + 1].pts[0] = OUTpts + (vIndex + (halfArcRes - 2)) % size;
			OUTmesh[index + 1].pts[1] = OUTpts + (vIndex + (halfArcRes - 2) + 1) % size;
			OUTmesh[index + 1].pts[2] = OUTpts + vIndex + 1;

			OUTmesh[index + 1].diffuseRefelctivity = shadingProperties.diffuseRefelctivity;
			OUTmesh[index + 1].reflectivity = shadingProperties.reflectivity;
			OUTmesh[index + 1].refractiveIndex = shadingProperties.refractiveIndex;
			OUTmesh[index + 1].transmitivity = shadingProperties.transmitivity;
		}
	}

	//connect outer triangles
	int trsSize = halfArcs * (halfArcRes - 3) * 2;
	for (unsigned char arcNO = 0; arcNO < halfArcs; ++arcNO) {
		//create trs
		OUTmesh[trsSize + arcNO].pts[0] = OUTpts + size;
		OUTmesh[trsSize + arcNO].pts[2] = OUTpts + arcNO * (halfArcRes - 2);
		OUTmesh[trsSize + arcNO].pts[1] = OUTpts + ((arcNO + 1) * (halfArcRes - 2))%size;
		
		OUTmesh[trsSize + arcNO].diffuseRefelctivity = shadingProperties.diffuseRefelctivity;
		OUTmesh[trsSize + arcNO].reflectivity = shadingProperties.reflectivity;
		OUTmesh[trsSize + arcNO].refractiveIndex = shadingProperties.refractiveIndex;
		OUTmesh[trsSize + arcNO].transmitivity = shadingProperties.transmitivity;
	}
	trsSize += halfArcs;
	//upper trs
	for (unsigned char arcNO = 0; arcNO < halfArcs; ++arcNO) {
		//create trs
		OUTmesh[trsSize + arcNO].pts[0] = OUTpts + size+1;
		OUTmesh[trsSize + arcNO].pts[1] = OUTpts + arcNO * (halfArcRes - 2) + halfArcRes-3;
		OUTmesh[trsSize + arcNO].pts[2] = OUTpts + ((arcNO + 1) * (halfArcRes - 2) + halfArcRes - 3) % size;
		OUTmesh[trsSize + arcNO].diffuseRefelctivity = vec3f(0, 255, 255);
		
		OUTmesh[trsSize + arcNO].diffuseRefelctivity = shadingProperties.diffuseRefelctivity;
		OUTmesh[trsSize + arcNO].reflectivity = shadingProperties.reflectivity;
		OUTmesh[trsSize + arcNO].refractiveIndex = shadingProperties.refractiveIndex;
		OUTmesh[trsSize + arcNO].transmitivity = shadingProperties.transmitivity;
	}

}


//scene specific

#define defaultSphereNoArcs 25
#define defaultSphereArcRez 25

void initLights() {
	//global dir light
	directionalLight dl;
	dl.color = vec3f(0.75, 0.75, 0.75);
	dl.dir = vec3d(0, 1, 0);
	//dL.push_back(dl);

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

