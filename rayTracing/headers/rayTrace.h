/**
 * @file rayTrace.h
 * @brief Exports function(s) for initializing triangles and Producing images using ray tracing.
 */

#pragma once

#include "mesh.h"
#include "camera.h"
#include "light.h"

#include <vector>

#include <iostream>

typedef unsigned char BYTE;


/**
* Generates a collTriangle array as per the given array of triangles.
* @param list The array of triangles.
* @param noOfTriangles The No of triangles in the array list.
* @return The array of generated collTriangles.
*/
collTriangle* initMesh(triangle* list, size_t noOfTrinagles);


/**
* Performs ray tracing.
* @param c Camera used.
* @param trs The array of triangles.
* @param collTrs The array of generated collTriangles.
* @param noTrs The no of triangles in the array.
* @param pLights A vector containing all the point lights in the scecne.
* @param dLights A vector containing all the directional lights in the scecne.
* @param iterations The depth to which the ray tracing tree is evaluated.
* @param dataOut An array in which the generated image is stored.
*/
void render(camera c, triangle* trs, collTriangle* collTrs, long long noTrs,const std::vector<pointLight> &pLights ,const std::vector<directionalLight> &dLights, unsigned char iterations , BYTE* dataOut);