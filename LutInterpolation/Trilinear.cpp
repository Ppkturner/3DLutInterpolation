#include "Trilinear.h"

void TrilinearInterpolation(ColorTriplet<unsigned int> input, ColorTriplet<unsigned int>* output, unsigned int bitDepth, const Lut3d & lut)
{
	float R = static_cast<float>(input.R);
	float G = static_cast<float>(input.G);
	float B = static_cast<float>(input.B);

	float normFactor = static_cast<float>((1 << bitDepth) - 1);

	R /= normFactor;
	G /= normFactor;
	B /= normFactor;

	// convert from point to grid coordinates
	float x = R * (lut.size - 1);
	float y = G * (lut.size - 1);
	float z = B * (lut.size - 1);

	// round down for coordinates
	int intX = static_cast<int>(x);
	int intY = static_cast<int>(y);
	int intZ = static_cast<int>(z);

	// difference between x and intX (floor of x)
	float deltaX = x - intX;
	float deltaY = y - intY;
	float deltaZ = z - intZ;

	// calculate vertices in cube
	// cxyz is each coordinate, where xyz is a binary representation of a number
	// x changes fastest and represents the offset into a row, so add 1 each time it is set
	// y changes second fastest and represents a row in the cube. Add the lut size (the size of one dimension) when it is set
	// z changes the slowest and represents the plane in the cube. Add the square of the lut size when it is set

	int xOffset = 1;
	int yOffset = lut.size;
	int zOffset = lut.size * lut.size;

	// multiply by the number of components in an RGB triplet (3)
	// c000
	int index0 = (intZ * zOffset + intY * yOffset + intX * xOffset);

	// c001
	int index1 = (index0 + zOffset);

	// c010
	int index2 = (index0 + yOffset);

	// c011
	int index3 = (index2 + zOffset);

	// c100
	int index4 = (index0 + xOffset);

	// c101
	int index5 = (index4 + zOffset);

	// c110
	int index6 = (index4 + yOffset);

	// c111
	int index7 = (index6 + zOffset);

	index0 *= 3;

	index1 *= 3;
	
	index2 *= 3;
	
	index3 *= 3;
	
	index4 *= 3;
	
	index5 *= 3;
	
	index6 *= 3;
	
	index7 *= 3;

	ColorTriplet<float> c000 = { lut.data[index0], lut.data[index0 + 1], lut.data[index0 + 2] };
	ColorTriplet<float> c001 = { lut.data[index1], lut.data[index1 + 1], lut.data[index1 + 2] };
	ColorTriplet<float> c010 = { lut.data[index2], lut.data[index2 + 1], lut.data[index2 + 2] };
	ColorTriplet<float> c011 = { lut.data[index3], lut.data[index3 + 1], lut.data[index3 + 2] };
	ColorTriplet<float> c100 = { lut.data[index4], lut.data[index4 + 1], lut.data[index4 + 2] };
	ColorTriplet<float> c101 = { lut.data[index5], lut.data[index5 + 1], lut.data[index5 + 2] };
	ColorTriplet<float> c110 = { lut.data[index6], lut.data[index6 + 1], lut.data[index6 + 2] };
	ColorTriplet<float> c111 = { lut.data[index7], lut.data[index7 + 1], lut.data[index7 + 2] };

	// c00 -> interpolate c000 and c100
	ColorTriplet<float> c00 = Interpolate(c000, c100, deltaX);

	// c01 -> interpolate c001 and c101
	ColorTriplet<float> c01 = Interpolate(c001, c101, deltaX);

	// c10 -> interpolate c010 and c110
	ColorTriplet<float> c10 = Interpolate(c010, c110, deltaX);

	// c11 -> interpolate c011 and c111
	ColorTriplet<float> c11 = Interpolate(c011, c111, deltaX);

	// c0 -> interpolate c00 and c10
	ColorTriplet<float> c0 = Interpolate(c00, c10, deltaY);

	// c1 -> interpolate c01 and c11
	ColorTriplet<float> c1 = Interpolate(c01, c11, deltaY);

	// c -> interpolate c0 and c1
	ColorTriplet<float> c = Interpolate(c0, c1, deltaZ);

	output->R = static_cast<unsigned int>(c.R * normFactor);

	output->G = static_cast<unsigned int>(c.G * normFactor);

	output->B = static_cast<unsigned int>(c.B * normFactor);
}

ColorTriplet<float> Interpolate(ColorTriplet<float> &v0, ColorTriplet<float> &v1, float f)
{
	ColorTriplet<float> out = {
		InterpolateHelper(v0.R, v1.R, f),
		InterpolateHelper(v0.G, v1.G, f),
		InterpolateHelper(v0.B, v1.B, f)
	};

	return out;
}

// interpolation: v0 + (v1 - v0) * f, v0/v1 = RGB component, f = weight
float InterpolateHelper(float v0, float v1, float f)
{
	return v0 + (v1 - v0) * f;
}
