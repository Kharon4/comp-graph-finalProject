#pragma once
#include "rayTrace.h"
#define noTrs 4

vector<pointLight> pL;
vector<directionalLight> dL;
#define PI 3.14159

#define vertexNo(arcRes , noArcs) (arcRes-2)*noArcs + 2
#define triangleNo(arcRes , noArcs) (arcRes-3)*noArcs*2 + 2*noArcs


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

void initLights() {
	pointLight p;
	p.color = vec3f(0.25, 0.25, 0.25);
	p.pos = vec3d(0, 1.25, -0.5);
	pL.push_back(p);
}

triangle* generateTriangles() {
	triangle* rval = new triangle[noTrs];
	rval[0].diffuseRefelctivity = vec3f(200, 200, 200);
	//rval[0].reflectivity = vec3f(0.1, 0.1, 0.1);
	rval[0].transmitivity = vec3f(0.1, 0.1, 0.1);
	rval[0].refractiveIndex = 1.5;
	rval[0].pts[0] = new vec3d(-1, 1, -1);
	rval[0].pts[1] = new vec3d(1, 1, -1);
	rval[0].pts[2] = new vec3d(1, 5, -1);
	
	rval[1].diffuseRefelctivity = vec3f(0, 0, 0);
	rval[1].reflectivity = vec3f(0.5, 0.5, 0.5);
	rval[1].transmitivity = vec3f(0, 0, 0);
	rval[1].refractiveIndex = 1.2;
	rval[1].pts[0] = new vec3d(-1, 1, -1);
	rval[1].pts[2] = new vec3d(-1, 5, -1);
	rval[1].pts[1] = new vec3d(1, 5, -1);
	
	rval[2].diffuseRefelctivity = vec3f(100, 0, 0);
	rval[2].reflectivity = vec3f(0.5, 0.5, 0.5);
	rval[2].transmitivity = vec3f(0, 0, 0);
	rval[2].refractiveIndex = 1;
	rval[2].pts[0] = new vec3d(-1, 5, -1);
	rval[2].pts[1] = new vec3d(1, 5, -1);
	rval[2].pts[2] = new vec3d(1, 5, 5);
	
	rval[3].diffuseRefelctivity = vec3f(0, 50, 0);
	rval[3].reflectivity = vec3f(0, 0, 0);
	rval[3].transmitivity = vec3f(0.75, 1, 0.75);
	rval[3].refractiveIndex = 01.5;
	rval[3].pts[0] = new vec3d(0, 1, -1);
	rval[3].pts[1] = new vec3d(1, 1, -1);
	rval[3].pts[2] = new vec3d(0, 1, 1);

	return rval;
}


void deleteTrs(triangle* trs, collTriangle* cTrs, long long no = noTrs) {
	delete[] cTrs;
	for (long long i = 0; i < no; ++i) {
		delete trs[i].pts[0];
		delete trs[i].pts[1];
		delete trs[i].pts[2];
	}
	delete[] trs;
}