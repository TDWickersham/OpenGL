#pragma once
#include <string>
#include "renderr.h"

struct import
{
public:
	static std::string* readFile(const char* filePath);

	static shader createShaderFromFile(std::string fileName);
	static geometry createGeometryFromOBJ(const char* filePath, int shapeNum);
	static geometry loadGeometry(const char * filePath);
	static texture createTexture2D(const char* filePath);

};