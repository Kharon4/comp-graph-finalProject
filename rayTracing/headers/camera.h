/**
 * @file camera.h
 * @brief Defines a camera class and exports associated functions.
 */


#pragma once

#include"linearMath.h"


struct camera {

	/* Data Members */
	short xRes, yRes;
	vec3d vertex;
	vec3d topLeftCorner;
	vec3d right;
	vec3d down;
};

/**
* Returns the projector ray for a given camera and pixel.
* @return projector ray
*/
linearMathD::line getRay(camera c, short x, short y);