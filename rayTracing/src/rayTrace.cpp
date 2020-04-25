#include "rayTrace.h"

#define defaultColor vec3f(64, 64, 64)
#define clearanceFactor 0.001

collTriangle* initMesh(triangle* list, size_t noOfTriangles) {
	collTriangle* rVal = nullptr;
	rVal = new collTriangle[noOfTriangles];
	for (size_t i = 0; i < noOfTriangles; ++i) {
		rVal[i].calc(list[i]);
	}
	return rVal;
}

void initMesh(collTriangle* colTrs, triangle* list, size_t noOfTriangles) {
	for (size_t i = 0; i < noOfTriangles; ++i) {
		colTrs[i].calc(list[i]);
	}
}

long long int getClosestIntersection(linearMathD::line ray, collTriangle* collTrs,long long noOfTrs,vec3d * minCollPt = nullptr, double * minDist = nullptr) {
	ray.setDR(vec3d::normalize(ray.getDr()));
	long long closestId = -1;
	double smallestDist = -1;
	vec3d collPt;
	double tempDist;
	for (long long i = 0; i < noOfTrs; ++i) {
		collPt = linearMathD::rayCast(ray, collTrs[i].collPlane);
		if (linearMathD::errCode)continue;
		//check if inside
		if (vec3d::dot(collTrs[i].sidePlanes[0].getDr(), collPt - collTrs[i].sidePlanes[0].getPt()) < 0)continue;
		if (vec3d::dot(collTrs[i].sidePlanes[1].getDr(), collPt - collTrs[i].sidePlanes[1].getPt()) < 0)continue;
		if (vec3d::dot(collTrs[i].sidePlanes[2].getDr(), collPt - collTrs[i].sidePlanes[2].getPt()) < 0)continue;

		tempDist = vec3d::dot(ray.getDr(), collPt - ray.getPt());
		if (tempDist > 0 && ( tempDist < smallestDist || smallestDist < 0)) {
			smallestDist = tempDist;
			closestId = i;
			if (minCollPt != nullptr)*minCollPt = collPt;
		}
	}
	if (minDist != nullptr)*minDist = smallestDist;
	return closestId;
}

vec3f rayTrace(linearMathD::line ray, triangle* trs, collTriangle* collTrs, long long noTrs,const vector<pointLight>& pLights, const vector<directionalLight>& dLights,unsigned char iterations = 3) {
	if (iterations == 0)return defaultColor;
	vec3d collPt;
	long long id = getClosestIntersection(ray, collTrs, noTrs,&collPt);
	if (id < 0)return defaultColor;
	vec3f finalColor = vec3f(0, 0, 0);

	//global illuminantion
	finalColor += trs[id].diffuseRefelctivity;
	
	//point light
	for (size_t i = 0; i < pLights.size(); ++i) {
		//calculate light distance
		double lDistance = (pLights[i].pos - collPt).mag2();
		if (lDistance == 0)continue;
		//send a ray
		linearMathD::line lightRay;
		lightRay.setPT(collPt - ray.getDr() * clearanceFactor);
		lightRay.setDR(pLights[i].pos - collPt);
		if (vec3d::dot(lightRay.getDr(), collTrs[id].collPlane.getDr()) <= 0)continue;
		
		double minDist;
		getClosestIntersection(lightRay, collTrs, noTrs, nullptr, &minDist);
		if (minDist * minDist < lDistance && minDist>=0)continue;
		finalColor.x += trs[id].diffuseRefelctivity.x * pLights[i].color.x / lDistance;
		finalColor.y += trs[id].diffuseRefelctivity.y * pLights[i].color.y / lDistance;
		finalColor.z += trs[id].diffuseRefelctivity.z * pLights[i].color.z / lDistance;
	}

	//dir light
	for (size_t i = 0; i < dLights.size(); ++i) {
		//send a ray
		linearMathD::line lightRay;
		lightRay.setPT(collPt - ray.getDr() * clearanceFactor);
		lightRay.setDR(-dLights[i].dir);
		double factor = vec3d::dot(-dLights[i].dir, collTrs[id].collPlane.getDr());
		if (factor<= 0)continue;
		factor /= lightRay.getDr().mag() * collTrs[id].collPlane.getDr().mag();
		if (getClosestIntersection(lightRay, collTrs, noTrs) >= 0)continue;
		finalColor.x += trs[id].diffuseRefelctivity.x * dLights[i].color.x * factor;
		finalColor.y += trs[id].diffuseRefelctivity.y * dLights[i].color.y * factor;
		finalColor.z += trs[id].diffuseRefelctivity.z * dLights[i].color.z * factor;
	}

	//reflectance
	{
		linearMathD::line reflectedRay;
		reflectedRay.setPT(collPt - ray.getDr()*clearanceFactor);
		reflectedRay.setDR(linearMathD::getMirrorImage(collPt + ray.getDr(), collTrs[id].collPlane) - collPt);
		vec3f reflectedColor = rayTrace(reflectedRay,trs,collTrs,noTrs,pLights,dLights,iterations-1);
		reflectedColor.x *= trs[id].reflectivity.x;
		reflectedColor.y *= trs[id].reflectivity.y;
		reflectedColor.z *= trs[id].reflectivity.z;
		finalColor += reflectedColor;
	}

	//transmitance
	{
		linearMathD::line refractedRay;
		refractedRay.setPT(collPt + ray.getDr() * clearanceFactor);
		vec3d normal = vec3d::normalize(collTrs[id].collPlane.getDr());
		//get dr.n
		double dot = vec3d::dot(ray.getDr(), normal);
		double sinr;
		if (dot >= 0) {
			sinr = trs[id].refractiveIndex * sqrt(1 - (dot * dot / ray.getDr().mag2()));
		}
		else {
			sinr = sqrt(1 - (dot * dot / ray.getDr().mag2())) / trs[id].refractiveIndex;
		}

		//check for TIR
		if (sinr > 1) {
			refractedRay.setPT(collPt - ray.getDr() * clearanceFactor);
			refractedRay.setDR(linearMathD::getMirrorImage(collPt + ray.getDr(), collTrs[id].collPlane) - collPt);
		}else {
			double cosr = sqrt(1 - sinr * sinr);
			vec3d DR = vec3d(0, 0, 0);
			if(dot>0)DR += normal * cosr;
			else DR -= normal * cosr;
			DR += vec3d::normalize(ray.getDr() - normal * dot) * sinr;
			refractedRay.setDR(DR);
		}

		vec3f refractedColor = rayTrace(refractedRay, trs, collTrs, noTrs, pLights, dLights, iterations - 1);
		
		finalColor.x += (refractedColor.x * trs[id].transmitivity.x);
		finalColor.y += (refractedColor.y * trs[id].transmitivity.y);
		finalColor.z += (refractedColor.z * trs[id].transmitivity.z);
	}

	return finalColor;
}

void convertToByteColor(float min, float max, vec3f color, BYTE* OUTColor) {
	color -= vec3f(min, min, min);
	color *= 256 / (max - min);
	if (color.x < 0)color.x = 0;
	if (color.y < 0)color.y = 0;
	if (color.z < 0)color.z = 0;
	if (color.x > 255)color.x = 255;
	if (color.y > 255)color.y = 255;
	if (color.z > 255)color.z = 255;
	OUTColor[2] = color.x;//r
	OUTColor[1] = color.y;//g
	OUTColor[0] = color.z;//b
}

void render(camera c, triangle* trs, collTriangle* collTrs, long long noTrs, const vector<pointLight>& pLights, const vector<directionalLight>& dLights, BYTE* dataOut){
	double max = 0;
	vec3f* colorData = new vec3f[c.xRes * c.yRes];
	for (short y = 0; y < c.yRes; ++y) {
		for (short x = 0; x < c.xRes; ++x) {
			vec3f col = rayTrace(getRay(c, x, y), trs, collTrs, noTrs,pLights,dLights);
			if (col.x > max)max = col.x;
			if (col.y > max)max = col.y;
			if (col.z > max)max = col.z;
			colorData[x + y * c.xRes] = col;
			convertToByteColor(0, 255, colorData[x+y*c.xRes], dataOut +(x + y * c.xRes) * 3);
		}
	}
	long int finalCount = c.xRes * c.yRes;
	for (long int pixelId = 0; pixelId < finalCount; pixelId ++) {
		convertToByteColor(0, max*0.9, colorData[pixelId], dataOut + pixelId*3);
	}
	delete[] colorData;
}