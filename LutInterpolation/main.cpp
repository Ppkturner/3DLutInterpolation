#include "LutParser.h"
#include "CommonIncludes.h"
#include "Trilinear.h"
#include "Tetrahedral.h"
#include "ColorTriplet.h"

int main()
{
	std::ostringstream output;

	char cubeIdentityFilePath[] = "D:/scottt/Jiras/TXMF-3900/LUT/IDENTITY.cube";

	char cubePqHlgFilePath[] = "D:/scottt/Jiras/TXMF-3909/LUT/1e_PQ1000_HLG_mode-full_in-nar_out-nar_nocomp-v1_2.cube";

	CubeLutParser parser(cubePqHlgFilePath);

	CubeLutParser identityParse(cubeIdentityFilePath);

	Lut3d lut;

	Lut3d identity;

	// int size = parser.GetLutSize() * parser.GetLutSize() * parser.GetLutSize();

	// PQ -> HLG LUT 
	lut.data = parser.Parse();

	lut.size = parser.GetLutSize();

	// Identity LUT 
	identity.data = identityParse.Parse();

	identity.size = identityParse.GetLutSize();

	std::vector<ColorTriplet<unsigned int>> inputs = 
	{
		{4, 4, 4},				// -7% Black
		{16, 16, 16},			// -2% Black
		{64, 64, 64},			// 0% Black
		{940, 940, 64},			// 100% Yellow
		{64, 940, 940},			// 100% Cyan
		{64, 940, 64},			// 100% Green
		{940, 64, 940},			// 100% Magenta
		{940, 64, 64},			// 100% Red
		{64, 64, 940},			// 100% Blue
		{721, 721, 64},			// 75% Yellow
		{64, 721, 721},			// 75% Cyan
		{64, 721, 64},			// 75% Green
		{721, 64, 721},			// 75% Magenta
		{721, 64, 64},			// 75% Red
		{64, 64, 721},			// 75% Blue
		{940, 940, 940},		// 100% White
		{1019, 1019, 1019},		// 109% White
		{414, 414, 414},		// 40% Grey
		{721, 721, 721}			// 75% White
	};

	std::vector<ColorTriplet<unsigned int>> outputs;

	std::vector<ColorTriplet<unsigned int>> tetraOutputs;

	outputs.reserve(inputs.size());

	tetraOutputs.reserve(inputs.size());

	ColorTriplet<unsigned int> outTriplet;

	ColorTriplet<unsigned int> outTripletTetra;

	for (auto itr : inputs)
	{
		TrilinearInterpolation(itr, &outTriplet, 10, lut);

		TetrahedralInterpolation(itr, &outTripletTetra, 10, lut);
		
		outputs.push_back(outTriplet);

		tetraOutputs.push_back(outTripletTetra);
	}

	output << "Trilinear Outputs: " << "\n";

	output << "[";

	for (auto itr:outputs)
	{
		output << "[" << itr.R << ", " << itr.G << ", " << itr.B << "], ";
	}

	output << "]\n";

	output << "Tetrahedral Outputs: " << "\n";

	output << "[";

	for (auto itr : tetraOutputs)
	{
		output << "[" << itr.R << ", " << itr.G << ", " << itr.B << "], ";
	}

	output << "]\n";

	OutputDebugString(output.str().c_str());

	return 0;
}