/**
 * @file light.h
 * @brief Defines different types of light classes.
 */

#pragma once
#include "vec3.h"


struct pointLight {
	vec3f color;
	vec3d pos;
};

struct directionalLight {
	vec3f color;
	vec3d dir;
};

