#pragma once
#include "rayTrace.h"
#define noTrs 4

vector<pointLight> pL;
vector<directionalLight> dL;

void initLights() {
	pointLight p;
	p.color = vec3f(0.2, 0.5, 0.2);
	p.pos = vec3d(0, 1, -0.5);
	pL.push_back(p);
}

triangle* generateTriangles() {
	triangle* rval = new triangle[noTrs];
	rval[0].diffuseRefelctivity = vec3f(200, 200, 200);
	rval[0].reflectivity = vec3f(0.1, 0.1, 0.1);
	rval[0].transminivity = vec3f(0, 0, 0);
	rval[0].refractiveIndex = 01.1;
	rval[0].pts[0] = new vec3d(-1, 1, -1);
	rval[0].pts[1] = new vec3d(1, 1, -1);
	rval[0].pts[2] = new vec3d(1, 5, -1);
	
	rval[1].diffuseRefelctivity = vec3f(0, 0, 0);
	rval[1].reflectivity = vec3f(0.5, 0.5, 0.5);
	rval[1].transminivity = vec3f(0, 0, 0);
	rval[1].refractiveIndex = 1;
	rval[1].pts[0] = new vec3d(-1, 1, -1);
	rval[1].pts[1] = new vec3d(-1, 5, -1);
	rval[1].pts[2] = new vec3d(1, 5, -1);
	
	rval[2].diffuseRefelctivity = vec3f(70, 0, 0);
	rval[2].reflectivity = vec3f(0.9, 0.9, 0.9);
	rval[2].transminivity = vec3f(0, 0, 0);
	rval[2].refractiveIndex = 1;
	rval[2].pts[0] = new vec3d(-1, 5, -1);
	rval[2].pts[1] = new vec3d(1, 5, -1);
	rval[2].pts[2] = new vec3d(1, 5, 5);
	
	rval[3].diffuseRefelctivity = vec3f(70, 250, 70);
	rval[3].reflectivity = vec3f(0.1, 0.1, 0.1);
	rval[3].transminivity = vec3f(0, 0, 0);
	rval[3].refractiveIndex = 1;
	rval[3].pts[0] = new vec3d(-1, 1, 1);
	rval[3].pts[1] = new vec3d(1, 1, 1);
	rval[3].pts[2] = new vec3d(1, 5, 1);

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