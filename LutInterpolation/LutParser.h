#pragma once
#ifndef LUTPARSER_H
#define LUTPARSER_H

#include "CommonIncludes.h"

enum CUBE_LUT_TOKENS
{
	Title,
	Domain_Min,
	Domain_Max,
	Comment,
	Lut1dSize,
	Lut3dSize,
	Value,
	NoType
};

struct Token
{
	std::vector<std::string> data;

	CUBE_LUT_TOKENS type;
};

class CubeLutParser
{
public:
	// Tokens in cube LUT
	static constexpr const char *TITLE = "TITLE";


	static constexpr const char *DOMAIN_MIN = "DOMAIN_MIN";

	static constexpr const char *DOMAIN_MAX = "DOMAIN_MAX";

	static constexpr const char COMMENT = '#';

	// Specifically for 1D LUTs
	static constexpr const char *LUT_1D_SIZE = "LUT_1D_SIZE";

	// Specifically for 3D LUTs
	static constexpr const char *LUT_3D_SIZE = "LUT_3D_SIZE";

public:
	// Defined Constants for cube LUTs

	// Max and Min for size of 1D LUT
	static const unsigned int Min1dSize = 2;

	static const unsigned int Max1dSize = 65536;

	// Max and Min for size of 3D LUT
	static const unsigned int Min3dSize = 2;

	static const unsigned int Max3dSize = 256;

private:
	
	std::ifstream cubeFileStream;

	std::string cubeFilePath;

	std::string line; // one line of text in a cube file is no longer than 250 characters

	std::string title; // Title of LUT

	bool is1DLut; // True if .cube file has a 1D LUT, False if a 3D LUT.

	unsigned int lutSize; // Size of the LUT;

	float *data; // the actual data contained within the .cube file

	int dataIndex; // current index into the data array

	// Domain min and max for cube file
	// component ordering is {r, g, b}
	float minVal[3];

	float maxVal[3];

	// Key words must occur before table data
	// Each key word may occur at most one time
	bool foundTitle;

	bool foundDomainMin;

	bool foundDomainMax;

	bool foundLutSize;

	bool foundStartOfTable;

	// flag for an invalid cube LUT
	bool invalidCubeFile;

public:

	CubeLutParser();

	CubeLutParser(char *cubePath);

	~CubeLutParser()
	{
		if (cubeFileStream.is_open())
			cubeFileStream.close();
	}

	float *Parse(); // Parse the entire .cube file and return the LUT

	bool Is1DLut() { return this->is1DLut; }

	unsigned int GetLutSize() { return this->lutSize; }

private:

	void ParseLine(); // read and parse a full line from the cube file

	void ParseTitle(Token titleToken); // Parse the Title

	void ParseSize(Token sizeToken); // Parse size of LUT 

	void ParseDomain(Token domainToken); // Parse the Domain min and max

	void ParseData(Token dataToken); // parse the number data 

	void Initialize(); // Read the first few lines in the source 

	Token Tokenize();

	void Flush();
};

bool isFloat(std::string myString);
#endif