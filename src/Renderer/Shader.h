/*----------------------------------------------------
**
**		Date: 2019
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

	Shader();
	~Shader();

	virtual void draw() = 0;

private:
};