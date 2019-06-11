#pragma once
#ifndef TETRAHEDRAL_H
#define TETRAHEDRAL_H

#include "CommonIncludes.h"
#include "ColorTriplet.h"
#include "LookupTable.h"

void TetrahedralInterpolation(ColorTriplet<unsigned int> input, ColorTriplet<unsigned int> *output, unsigned int bitDepth, const Lut3d &lut);


#endif