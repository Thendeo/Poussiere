/*----------------------------------------------------
**
**		Date: 2020
**		Author: Thendeo
**		Project: Particules
**		File: Shader header file
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

//! @brief Abstraction of a shader
//! Contains interface to render part of a pipeline
class Shader
{
public:

	//! @brief Constructor with vertex and fragment shader
	Shader(const char* pVertexFilePath, const char* pFragmentFilePath);

	//! @brief Constructor with vertex, geometry and fragment shader
	Shader(const char* pVertexFilePath, const char* pGeometryFilePath, const char* pFragmentFilePath);

	~Shader();

	virtual void draw() = 0;

protected:
	GLuint m_ProgramID;
};