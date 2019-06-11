#pragma once
#ifndef LOOKUPTABLE_H
#define LOOKUPTABLE_H

#include "CommonIncludes.h"

struct Lut3d
{
	float *data; // The actual LUT data

	unsigned int size; // size of one dimension of the 3D cube LUT
};

#endif