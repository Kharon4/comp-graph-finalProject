#include "rayTrace.h"

#define defaultColor vec3f(64, 64, 64)
#define clearanceFactor 0.001

collTriangle* initMesh(triangle* list, size_t noOfTrinagles) {
	collTriangle* rVal = nullptr;
	rVal = new collTriangle[noOfTrinagles];
	for (size_t i = 0; i < noOfTrinagles; ++i) {
		rVal[i].calc(list[i]);
	}
	return rVal;
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
		if (tempDist < smallestDist || smallestDist < 0) {
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
		double factor = vec3d::dot(lightRay.getDr(), collTrs[id].collPlane.getDr());
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
		if (vec3d::dot(collTrs[id].collPlane.getDr(), ray.getDr()) < 0) {
			//calculate initial sin(i)
			float si = vec3d::dot(collTrs[id].collPlane.getDr(), -ray.getDr()) / (ray.getDr().mag() * collTrs[id].collPlane.getDr().mag());
			si = sqrt(1 - si * si);

			//calculate sin(r) and cos(r)
			float sr = si / trs[id].refractiveIndex;
			if (sr > 1) {
				std::cout << "error0" << std::endl;
				system("pasue");
			}
			float cr = sqrt(1 - sr * sr);
			vec3d finalDr = vec3d(0, 0, 0);
			finalDr += vec3d::normalize(-collTrs[id].collPlane.getDr()) * cr;
			finalDr += (ray.getDr() - collTrs[id].collPlane.getDr() * vec3d::dot(collTrs[id].collPlane.getDr(), ray.getDr()) / collTrs[id].collPlane.getDr().mag2()) * sr;
			refractedRay.setDR(finalDr);
		}
		else {
			//calculate initial sin(i)
			float si = vec3d::dot(-collTrs[id].collPlane.getDr(), ray.getDr()) / (ray.getDr().mag() * collTrs[id].collPlane.getDr().mag());
			si = sqrt(1 - si * si);

			//calculate sin(r) and cos(r)
			float sr = si / trs[id].refractiveIndex;
			if (sr > 1) {
				std::cout << "error2" << std::endl;
				system("pasue");
			}
			float cr = sqrt(1 - sr * sr);
			vec3d finalDr = vec3d(0, 0, 0);
			finalDr += vec3d::normalize(collTrs[id].collPlane.getDr()) * cr;
			finalDr += (ray.getDr() - collTrs[id].collPlane.getDr() * vec3d::dot(collTrs[id].collPlane.getDr(), ray.getDr()) / collTrs[id].collPlane.getDr().mag2()) * sr;
			refractedRay.setDR(finalDr);
		}

		vec3f refractedColor = rayTrace(refractedRay, trs, collTrs, noTrs, pLights, dLights, iterations - 1);
		if (refractedColor.x<0 || refractedColor.y < 0|| refractedColor.z < 0){
			
			std::cout << refractedColor.x << ",  " << refractedColor.y << ",  " << refractedColor.z << ",  " << std::endl;
			system("pause");
		}
		finalColor.x += refractedColor.x * trs[id].transminivity.x;
		finalColor.y += refractedColor.y * trs[id].transminivity.y;
		finalColor.z += refractedColor.z * trs[id].transminivity.z;
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
		}
	}
	long int finalCount = c.xRes * c.yRes;
	for (long int pixelId = 0; pixelId < finalCount; pixelId ++) {
		convertToByteColor(0, max*0.9, colorData[pixelId], dataOut + pixelId*3);
	}
	delete[] colorData;
}