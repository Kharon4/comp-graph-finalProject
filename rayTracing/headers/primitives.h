#pragma once
#include <vec3.h>
#include <math.h>

#define PI 3.14159

#define vertexNo(arcRes , noArcs) ((arcRes-2)*noArcs + 2)
#define triangleNo(arcRes , noArcs) ((arcRes-3)*noArcs*2 + 2*noArcs)


void generateSphere(vec3d center, double rad, unsigned char halfArcRes, unsigned char halfArcs, vec3d* OUTpts, triangle* OUTmesh, triangle shadingProperties) {
	//generate all arcs
	for (unsigned char arcNo = 0; arcNo < halfArcs; ++arcNo) {
		double azimuthal = arcNo * 2 * (PI / halfArcs);
		double cosA = cos(azimuthal);
		double sinA = sin(azimuthal);
		for (unsigned char latitude = 1; latitude < halfArcRes - 1; ++latitude) {
			double pitch = ((((double)latitude) / (halfArcRes - 1)) - 0.5) * PI;
			double cosP = cos(pitch);
			double sinP = sin(pitch);

			//set vertex
			unsigned int vIndex = (latitude - 1) + arcNo * (halfArcRes - 2);
			OUTpts[vIndex] = center + rad * vec3d(cosP * cosA, cosP * sinA, sinP);

		}
	}
	//set final two vertices
	int size = (halfArcRes - 2) * halfArcs;
	OUTpts[size] = center - rad * vec3d(0, 0, 1);
	OUTpts[size + 1] = center + rad * vec3d(0, 0, 1);

	//connect inner triangles
	for (unsigned char arcNo = 0; arcNo < halfArcs; ++arcNo) {
		for (unsigned char latitude = 1; latitude < halfArcRes - 2; ++latitude) {
			//genrate two trs
			unsigned int vIndex = (latitude - 1) + arcNo * (halfArcRes - 2);
			unsigned int index = 2 * ((latitude - 1) + arcNo * (halfArcRes - 3));
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
		OUTmesh[trsSize + arcNO].pts[1] = OUTpts + ((arcNO + 1) * (halfArcRes - 2)) % size;

		OUTmesh[trsSize + arcNO].diffuseRefelctivity = shadingProperties.diffuseRefelctivity;
		OUTmesh[trsSize + arcNO].reflectivity = shadingProperties.reflectivity;
		OUTmesh[trsSize + arcNO].refractiveIndex = shadingProperties.refractiveIndex;
		OUTmesh[trsSize + arcNO].transmitivity = shadingProperties.transmitivity;
	}
	trsSize += halfArcs;
	//upper trs
	for (unsigned char arcNO = 0; arcNO < halfArcs; ++arcNO) {
		//create trs
		OUTmesh[trsSize + arcNO].pts[0] = OUTpts + size + 1;
		OUTmesh[trsSize + arcNO].pts[1] = OUTpts + arcNO * (halfArcRes - 2) + halfArcRes - 3;
		OUTmesh[trsSize + arcNO].pts[2] = OUTpts + ((arcNO + 1) * (halfArcRes - 2) + halfArcRes - 3) % size;
		OUTmesh[trsSize + arcNO].diffuseRefelctivity = vec3f(0, 255, 255);

		OUTmesh[trsSize + arcNO].diffuseRefelctivity = shadingProperties.diffuseRefelctivity;
		OUTmesh[trsSize + arcNO].reflectivity = shadingProperties.reflectivity;
		OUTmesh[trsSize + arcNO].refractiveIndex = shadingProperties.refractiveIndex;
		OUTmesh[trsSize + arcNO].transmitivity = shadingProperties.transmitivity;
	}

}

#define cuboidVertexNo 8
#define cuboidTriangleNo 12

void generateCuboid(vec3d center, vec3d halfDiagonal, vec3d* OUTpts, triangle* Outmesh, triangle shadingProperties) {
	//create pts
	OUTpts[0] = center - halfDiagonal;//lower left
	for (unsigned char i = 1; i < 8; ++i)OUTpts[i] = OUTpts[0];
	halfDiagonal *= 2;
	OUTpts[1].x += halfDiagonal.x;
	OUTpts[2].y += halfDiagonal.y;
	OUTpts[3].x += halfDiagonal.x;
	OUTpts[3].y += halfDiagonal.y;

	for (unsigned char i = 4; i < 8; ++i)OUTpts[i].z += halfDiagonal.z;
	OUTpts[5].x += halfDiagonal.x;
	OUTpts[6].y += halfDiagonal.y;
	OUTpts[7].x += halfDiagonal.x;
	OUTpts[7].y += halfDiagonal.y;

	for (unsigned char i = 0; i < 12; ++i)Outmesh[i] = shadingProperties;

	//XY lower
	Outmesh[0].pts[0] = OUTpts + 0;
	Outmesh[0].pts[1] = OUTpts + 2;
	Outmesh[0].pts[2] = OUTpts + 1;

	Outmesh[1].pts[0] = OUTpts + 3;
	Outmesh[1].pts[1] = OUTpts + 1;
	Outmesh[1].pts[2] = OUTpts + 2;

	//XY upper
	Outmesh[2].pts[0] = OUTpts + 4;
	Outmesh[2].pts[1] = OUTpts + 5;
	Outmesh[2].pts[2] = OUTpts + 6;

	Outmesh[3].pts[0] = OUTpts + 7;
	Outmesh[3].pts[1] = OUTpts + 6;
	Outmesh[3].pts[2] = OUTpts + 5;

	//XZ front
	Outmesh[4].pts[0] = OUTpts + 0;
	Outmesh[4].pts[1] = OUTpts + 1;
	Outmesh[4].pts[2] = OUTpts + 4;

	Outmesh[5].pts[0] = OUTpts + 5;
	Outmesh[5].pts[1] = OUTpts + 4;
	Outmesh[5].pts[2] = OUTpts + 1;

	//XZ back
	Outmesh[6].pts[0] = OUTpts + 2;
	Outmesh[6].pts[1] = OUTpts + 6;
	Outmesh[6].pts[2] = OUTpts + 3;

	Outmesh[7].pts[0] = OUTpts + 7;
	Outmesh[7].pts[1] = OUTpts + 4;
	Outmesh[7].pts[2] = OUTpts + 6;

	//YZ left
	Outmesh[8].pts[0] = OUTpts + 0;
	Outmesh[8].pts[1] = OUTpts + 4;
	Outmesh[8].pts[2] = OUTpts + 2;

	Outmesh[9].pts[0] = OUTpts + 6;
	Outmesh[9].pts[1] = OUTpts + 4;
	Outmesh[9].pts[2] = OUTpts + 2;

	//YZ right
	Outmesh[10].pts[0] = OUTpts + 1;
	Outmesh[10].pts[1] = OUTpts + 3;
	Outmesh[10].pts[2] = OUTpts + 5;

	Outmesh[11].pts[0] = OUTpts + 7;
	Outmesh[11].pts[1] = OUTpts + 5;
	Outmesh[11].pts[2] = OUTpts + 3;

}