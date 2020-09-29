/*----------------------------------------------------
**
**		Date: 2019
**		Author: Alexis LAFONT
**		Project: Particules
**		File: AdvanceShader header file
**
----------------------------------------------------*/

/*----------------------------------------------------
**
**		Date: 2020
**		Author: Thendeo
**		Project: Particules
**		File: AdvanceShader header file
**
----------------------------------------------------*/

#pragma once
#include "Shader.h"

#include <GL/glew.h>

//! @brief Shader to draw particules
//! Allows one to update position texture given a velocity map
//! Vertex shader input : 
//! * None
//! Geometry shader input :
//! * None
//! Fragment shader input :
//! * Position texture
//! * Velocity Map
class AdvanceShader : public Shader
{
public:

	AdvanceShader();
	~AdvanceShader();

	virtual void draw();

	void setFragmentPosition(GLuint p_TexturePosition);
	void setFragmentVelocity(GLuint p_TextureVelocity);

private:

	// Uniform location
	GLuint m_UniformTexturePosition;
	GLuint m_UniformTextureVelocity;
};