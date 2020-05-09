/*----------------------------------------------------
**
**		Date: 2019
**		Author: Thendeo
**		Project: Particules
**		File: ShaderLoader header file
**
----------------------------------------------------*/
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Image.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

#include <string>

class ShaderLoader
{
public:
	static GLuint _loadShader(const char* pVertexFilePath, const char* pFragmentFilePath);
	static GLuint _loadShader(const char* pVertexFilePath,
		const char* pGeometryFilePath, const char* pFragmentFilePath);

private:
	static void loadVertexShader(const char* pVertexFilePath, GLuint& pVertexShaderID);
	static void loadGeometryShader(const char* pGeometryFilePath, GLuint& pGeometryShaderID);
	static void loadFragmentShader(const char* pFragmentFilePath, GLuint& pFragmentShaderID);
};