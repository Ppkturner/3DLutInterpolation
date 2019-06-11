#include "Tetrahedral.h"

void TetrahedralInterpolation(ColorTriplet<unsigned int> input, ColorTriplet<unsigned int>* output, unsigned int bitDepth, const Lut3d & lut)
{
	float R = static_cast<float>(input.R);
	float G = static_cast<float>(input.G);
	float B = static_cast<float>(input.B);

	float normFactor = static_cast<float>((1 << bitDepth) - 1);

	R /= normFactor;
	G /= normFactor;
	B /= normFactor;

	float x = R * (lut.size - 1);
	float y = G * (lut.size - 1);
	float z = B * (lut.size - 1);

	int intX = static_cast<int>(x);
	int intY = static_cast<int>(y);
	int intZ = static_cast<int>(z);

	float deltaX = x - intX;
	float deltaY = y - intY;
	float deltaZ = z - intZ;

	// int xOffset = 1; // X offset is always one as red always changes the fastest 
	int yOffset = lut.size;
	int zOffset = lut.size * lut.size;

	// calculate the points along the diagonal
	// c000
	int index0 = intZ * zOffset + intY * yOffset + intX;

	// c111
	int index7 = (index0 + zOffset + yOffset + 1) * 3;

	ColorTriplet<float> c000 = { lut.data[index0 * 3], lut.data[index0 * 3 + 1], lut.data[index0 * 3 + 2] };

	ColorTriplet<float> c111 = { lut.data[index7], lut.data[index7 + 1], lut.data[index7 + 2] };

	// Each interpolation has four weights, one for each vertex in the tetrahedron
	float factor1;
	float factor2;
	float factor3;
	float factor4;

	// There are 6 tetrahedron in total
	// The interpolated point in question will lie in one of the tetrahedra
	if (deltaX > deltaY)
	{
		if (deltaY > deltaZ)
		{
			// Tetrahedron 1: deltaX > deltaY > deltaZ

			// c100
			int index4 = index0 + 1;

			// c110
			int index6 = index4 + yOffset;

			index4 *= 3;

			index6 *= 3;

			ColorTriplet<float> c100 = { lut.data[index4], lut.data[index4 + 1], lut.data[index4 + 2] };

			ColorTriplet<float> c110 = { lut.data[index6], lut.data[index6 + 1], lut.data[index6 + 2] };

			factor1 = 1 - deltaX;
			factor2 = deltaX - deltaY;
			factor3 = deltaY - deltaZ;
			factor4 = deltaZ;

			output->R = static_cast<unsigned int>((factor1 * c000.R + factor2 * c100.R + factor3 * c110.R + factor4 * c111.R) * normFactor);
			output->G = static_cast<unsigned int>((factor1 * c000.G + factor2 * c100.G + factor3 * c110.G + factor4 * c111.G) * normFactor);
			output->B = static_cast<unsigned int>((factor1 * c000.B + factor2 * c100.B + factor3 * c110.B + factor4 * c111.B) * normFactor);
		}
		else if (deltaX > deltaZ)
		{
			// Tetrahedron 2: deltaX > deltaZ 
			// Either deltaY > deltaZ or deltaZ > deltaY

			// c100
			int index4 = index0 + 1;

			// c101
			int index5 = index4 + zOffset;

			index4 *= 3;

			index5 *= 3;

			ColorTriplet<float> c100 = { lut.data[index4], lut.data[index4 + 1], lut.data[index4 + 2] };

			ColorTriplet<float> c101 = { lut.data[index5], lut.data[index5 + 1], lut.data[index5 + 2] };

			factor1 = 1 - deltaX;
			factor2 = deltaX - deltaZ;
			factor3 = deltaZ - deltaY;
			factor4 = deltaY;

			output->R = static_cast<unsigned int>((factor1 * c000.R + factor2 * c100.R + factor3 * c101.R + factor4 * c111.R) * normFactor);
			output->G = static_cast<unsigned int>((factor1 * c000.G + factor2 * c100.G + factor3 * c101.G + factor4 * c111.G) * normFactor);
			output->B = static_cast<unsigned int>((factor1 * c000.B + factor2 * c100.B + factor3 * c101.B + factor4 * c111.B) * normFactor);
		}
		else
		{
			// Tetrahedron 3:
			// deltaZ >= deltaX

			// c001
			int index1 = index0 + zOffset;

			// c101
			int index5 = index1 + 1;

			index1 *= 3;

			index5 *= 3;

			ColorTriplet<float> c001 = { lut.data[index1], lut.data[index1 + 1], lut.data[index1 + 2] };

			ColorTriplet<float> c101 = { lut.data[index5], lut.data[index5 + 1], lut.data[index5 + 2] };

			factor1 = 1 - deltaZ;
			factor2 = deltaZ - deltaX;
			factor3 = deltaX - deltaY;
			factor4 = deltaY;

			output->R = static_cast<unsigned int>((factor1 * c000.R + factor2 * c001.R + factor3 * c101.R + factor4 * c111.R) * normFactor);
			output->G = static_cast<unsigned int>((factor1 * c000.G + factor2 * c001.G + factor3 * c101.G + factor4 * c111.G) * normFactor);
			output->B = static_cast<unsigned int>((factor1 * c000.B + factor2 * c001.B + factor3 * c101.B + factor4 * c111.B) * normFactor);
		}
	}
	else // deltaY >= deltaX
	{
		if (deltaZ > deltaY)
		{
			// Tetrahedron 4: 
			// deltaZ > deltaY

			//c001
			int index1 = index0 + zOffset;

			//c011
			int index3 = index1 + yOffset;

			index1 *= 3;

			index3 *= 3;

			ColorTriplet<float> c001 = { lut.data[index1], lut.data[index1 + 1], lut.data[index1 + 2] };

			ColorTriplet<float> c011 = { lut.data[index3], lut.data[index3 + 1], lut.data[index3 + 2] };

			factor1 = 1 - deltaZ;
			factor2 = deltaZ - deltaY;
			factor3 = deltaY - deltaX;
			factor4 = deltaX;

			output->R = static_cast<unsigned int>((factor1 * c000.R + factor2 * c001.R + factor3 * c011.R + factor4 * c111.R) * normFactor);
			output->G = static_cast<unsigned int>((factor1 * c000.G + factor2 * c001.G + factor3 * c011.G + factor4 * c111.G) * normFactor);
			output->B = static_cast<unsigned int>((factor1 * c000.B + factor2 * c001.B + factor3 * c011.B + factor4 * c111.B) * normFactor);
		}
		else if (deltaZ > deltaX)
		{
			// Tetrahedron 5:
			// deltaZ > deltaX

			// c010
			int index2 = index0 + yOffset;

			// c011
			int index3 = index2 + zOffset;

			index2 *= 3;

			index3 *= 3;

			ColorTriplet<float> c010 = { lut.data[index2], lut.data[index2 + 1], lut.data[index2 + 2] };

			ColorTriplet<float> c011 = { lut.data[index3], lut.data[index3 + 1], lut.data[index3 + 2] };

			factor1 = 1 - deltaY;
			factor2 = deltaY - deltaZ;
			factor3 = deltaZ - deltaX;
			factor4 = deltaX;

			output->R = static_cast<unsigned int>((factor1 * c000.R + factor2 * c010.R + factor3 * c011.R + factor4 * c111.R) * normFactor);
			output->G = static_cast<unsigned int>((factor1 * c000.G + factor2 * c010.G + factor3 * c011.G + factor4 * c111.G) * normFactor);
			output->B = static_cast<unsigned int>((factor1 * c000.B + factor2 * c010.B + factor3 * c011.B + factor4 * c111.B) * normFactor);
		}
		else
		{
			// Tetrahedron 6:

			// c010
			int index2 = index0 + yOffset;

			// c110
			int index6 = index2 + 1;

			index2 *= 3;

			index6 *= 3;

			ColorTriplet<float> c010 = { lut.data[index2], lut.data[index2 + 1], lut.data[index2 + 2] };

			ColorTriplet<float> c110 = { lut.data[index6], lut.data[index6 + 1], lut.data[index6 + 2] };

			factor1 = 1 - deltaY;
			factor2 = deltaY - deltaX;
			factor3 = deltaX - deltaZ;
			factor4 = deltaZ;

			output->R = static_cast<unsigned int>((factor1 * c000.R + factor2 * c010.R + factor3 * c110.R + factor4 * c111.R) * normFactor);
			output->G = static_cast<unsigned int>((factor1 * c000.G + factor2 * c010.G + factor3 * c110.G + factor4 * c111.G) * normFactor);
			output->B = static_cast<unsigned int>((factor1 * c000.B + factor2 * c010.B + factor3 * c110.B + factor4 * c111.B) * normFactor);
		}
	}
}
