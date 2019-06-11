#include "LutParser.h"

CubeLutParser::CubeLutParser():
	cubeFilePath(""),
	is1DLut(false),
	lutSize(0),
	data(nullptr),
	foundTitle(false),
	foundDomainMin(false),
	foundDomainMax(false),
	foundLutSize(false),
	foundStartOfTable(false),
	invalidCubeFile(false)
{
}

CubeLutParser::CubeLutParser(char * cubePath): 
	cubeFilePath(cubePath), 
	is1DLut(false), 
	lutSize(0), 
	data(nullptr),
	foundTitle(false),
	foundDomainMin(false),
	foundDomainMax(false),
	foundLutSize(false),
	foundStartOfTable(false),
	invalidCubeFile(false)
{
	Initialize();
}

float * CubeLutParser::Parse()
{
	while (!cubeFileStream.eof())
	{
		std::getline(cubeFileStream, this->line);

		ParseLine();
	}

	return this->data;
}

void CubeLutParser::ParseLine()
{
	if ((this->line.length() > 0) && 
		(this->line.length() <= 250))
	{
		Token token;
		token = Tokenize();

		switch (token.type)
		{
			case Title:
				
				ParseTitle(token);

				break;

			case Lut1dSize:
			case Lut3dSize:

				ParseSize(token);

				break;

			case Comment:
				
				// Comments can be discarded
				
				break;

			case Domain_Min:
			case Domain_Max:

				ParseDomain(token);

				break;

			case Value:

				ParseData(token);

				break;

			default:

				// unhandled type

				break;
		}
	}
}

void CubeLutParser::ParseTitle(Token titleToken)
{
	if (this->invalidCubeFile)
		return;
	else if (this->foundTitle || this->foundStartOfTable)
	{
		// There can be no more than one Title in a cube LUT
		// and the Title must occur before the start of the table
		this->invalidCubeFile = true; // no more than one title in the cube LUT file

		return;
	}
	else
		this->foundStartOfTable = true;

	std::vector<std::string>::iterator itr = titleToken.data.begin() + 1; // skip the key word

	for (; itr != titleToken.data.end(); itr++) this->title += *itr;
}

void CubeLutParser::ParseSize(Token sizeToken)
{
	if (this->invalidCubeFile)
		return;
	else if (this->foundLutSize || this->foundStartOfTable)
	{
		this->invalidCubeFile = true;

		return;
	}
	else
		this->foundLutSize = true;

	std::vector<std::string>::iterator itr = sizeToken.data.begin() + 1;

	if (sizeToken.type == Lut1dSize)
		this->is1DLut = true;

	this->lutSize = stoi(*itr);
}

void CubeLutParser::ParseDomain(Token domainToken)
{
	bool isMin = domainToken.type == Domain_Min;

	if (this->invalidCubeFile)
		return;
	else if (
		(isMin && this->foundDomainMin) ||
		(!isMin && this->foundDomainMax) ||
		this->foundStartOfTable)
	{
		this->invalidCubeFile = true;

		return;
	}
	else
	{
		if (isMin)
			this->foundDomainMin = true;
		else
			this->foundDomainMax = true;
	}

	std::vector<std::string>::iterator itr = domainToken.data.begin() + 1; // skip keyword

	int index = 0;

	for (; itr != domainToken.data.end(); itr++)
	{
		if(isMin) 
			this->minVal[index++] = stof(*itr);
		else 
			this->maxVal[index++] = stof(*itr);
	}

}

void CubeLutParser::ParseData(Token dataToken)
{
	if (this->invalidCubeFile)
		return;

	if (this->data == nullptr)
	{
		this->data = this->is1DLut ? 
			new float[this->lutSize * 3] : 
			new float[this->lutSize * this->lutSize * this->lutSize * 3];

		this->foundStartOfTable = true;
	}

	std::vector<std::string>::iterator itr = dataToken.data.begin();

	for (; itr != dataToken.data.end(); itr++)
		data[dataIndex++] = stof(*itr);
}

void CubeLutParser::Initialize()
{
	// open file
	cubeFileStream.open(cubeFilePath, std::ifstream::in);

	// Table starts when we find the first numeric values in the file not associated with a Key Word
	while (!foundStartOfTable && this->cubeFileStream.good())
	{
		std::getline(this->cubeFileStream, this->line);

		ParseLine();
	}
}

//if (!this->line.empty())
//{
//	std::string first = "";
//
//	std::string::iterator strItr = this->line.begin();
//
//	while (*strItr != ' ') first += *(strItr++);
//
//	if (first == TITLE)
//	{
//		if (this->foundTitle || this->foundStartOfTable)
//		{
//			invalidCubeFile = true;
//
//			return;
//		}
//
//		this->title = std::string(strItr + 1, first.end() - 1); // grab every character from after the space
//
//		this->foundTitle = true;
//	}
//	else if (first == DOMAIN_MIN)
//	{
//		strItr++;
//
//		if (this->foundTitle || this->foundStartOfTable)
//		{
//			invalidCubeFile = true;
//
//			return;
//		}
//
//		for (int ii = 0; ii < 3; ii++)
//		{
//			std::string domainMin(strItr + 1, first.end() - 1);
//
//			std::string::size_type sz = 0;
//
//			int index = 0;
//
//			while (sz < domainMin.length && index < 3)
//				minVal[index++] = std::stof(domainMin.substr(sz), &sz);
//		}
//	}
//}

Token CubeLutParser::Tokenize()
{
	Token token;

	token.type = NoType;

	if (!this->line.empty())
	{
		std::string temp = this->line;

		std::string::iterator itr = this->line.begin();

		token.data.reserve(4); // key word + 3 consequent elements

		while (itr != this->line.end())
		{
			std::size_t offset = temp.find(' ');

			token.data.push_back(temp.substr(0, offset));

			if (offset == std::string::npos)
				itr = this->line.end();
			else
				itr += offset + 1; // move past space

			temp = std::string(itr, this->line.end());
		}
	}

	std::string type = token.data[0];

	char commentType = token.data[0][0];

	if (type == TITLE)
	{
		token.type = CUBE_LUT_TOKENS::Title;
	}
	else if (type == DOMAIN_MIN)
	{
		token.type = CUBE_LUT_TOKENS::Domain_Min;
	}
	else if (type == DOMAIN_MAX)
	{
		token.type = CUBE_LUT_TOKENS::Domain_Max;
	}
	else if (type == LUT_1D_SIZE)
	{
		token.type = CUBE_LUT_TOKENS::Lut1dSize;
	}
	else if (type == LUT_3D_SIZE)
	{
		token.type = CUBE_LUT_TOKENS::Lut3dSize;
	}
	else if (isFloat(type))
	{
		token.type = CUBE_LUT_TOKENS::Value;
	}
	
	if (commentType == COMMENT)
	{
		token.type = CUBE_LUT_TOKENS::Comment;
	}

	return token;
}

bool isFloat(std::string myString)
{
	std::istringstream iss(myString);

	float f;

	iss >> std::noskipws >> f;

	return iss.eof() && !iss.fail();
}
