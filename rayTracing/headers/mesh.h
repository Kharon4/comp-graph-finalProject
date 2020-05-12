/**
 * @file mesh.h
 * @brief Defines a triangle class and a collTriangle class that can be calculated from an instance of triangle class to help calculate ray triangle intersection.
 */
#pragma once
#include "linearMath.h"

struct triangle
{
	/* Data Members */
	vec3d *pts[3];
	vec3f reflectivity;
	vec3f transmitivity;
	vec3f diffuseRefelctivity;
	float refractiveIndex;
};

//can be modified
class collTriangle
{
public:
	/* Data Members */

	/**
	* Default constructor.
	*/
	collTriangle() {}

	/**
	* Generates a collTriangle that represents the given triangle.
	* @param t The triangle that is used to generate the collTriangle.
	*/
	collTriangle(const triangle &t);

	/**
	* calculates and makes the collTriangle represent the given triangle.
	* @param t The triangle that is used to calculate the collTriangle.
	*/
	void calc(const triangle &t);
	linearMathD::plane collPlane;
	linearMathD::plane sidePlanes[3];
};
