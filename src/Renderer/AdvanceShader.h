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

	AdvanceShader(unsigned int pTextureSize);
	~AdvanceShader();

	virtual void draw();

	void setOutputLocation(GLuint p_InputLocation, GLuint p_OutputLocation);

	void swapOutput();

private:

	// Assuming same X/Y size
	unsigned int m_TextureSize;

	// Uniform location
	GLuint m_UniformTexturePosition;
	GLuint m_UniformTextureVelocity;

	// Rendered texture location
	GLuint m_InputTexID;
	GLuint m_OutputTexID;
	GLuint m_InputTexLoc;
	GLuint m_OutputTexLoc;

	// FrameBuffer
	GLuint m_FrameBuffer;
	GLenum m_DrawTarget[1];

	bool m_OutputSwitch;
};