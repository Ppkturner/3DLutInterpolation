#pragma once
#ifndef TRILINEAR_H
#define TRILINEAR_H

#include "CommonIncludes.h"
#include "ColorTriplet.h"
#include "LookupTable.h"

void TrilinearInterpolation(ColorTriplet<unsigned int> input, ColorTriplet<unsigned int> *output, unsigned int bitDepth, const Lut3d &lut);

ColorTriplet<float> Interpolate(ColorTriplet<float> &v0, ColorTriplet<float> &v1, float f);

float InterpolateHelper(float v0, float v1, float f);

#endif