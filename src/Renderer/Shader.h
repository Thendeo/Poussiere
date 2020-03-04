/*----------------------------------------------------
**
**		Date: 2019
**		Author: Alexis LAFONT
**		Project: Particules
**		File: Shader header file
**
----------------------------------------------------*/

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Image.h"

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