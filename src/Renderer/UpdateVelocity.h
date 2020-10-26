/*----------------------------------------------------
**
**		Date: 2020
**		Author: Thendeo
**		Project: Particules
**		File: UpdateVelocity header file
**
----------------------------------------------------*/

#pragma once

#include "Shader.h"

#include "Image.h"
#include "TextureDefinitions.h"


//! @brief Shader to update velocity texture
//! Given position matrix, it computes gravitational reactions between each position
//! To update velocity vectors
//! Vertex shader input : 
//! * None
//! Geometry shader input :
//! * None
//! Fragment shader input :
//! * Position texture
//! * Velocity Map
//! * Texture size
//! * Gravitational constant
class UpdateVelocity : public Shader
{
public:

	UpdateVelocity(unsigned int pTextureSize);
	~UpdateVelocity();

	virtual void draw();

	void setOutputLocation(GLuint p_InputLocation, GLuint p_OutputLocation);

	void swapOutput();

private:

	// Assuming same X/Y size
	unsigned int m_TextureSize;

	// Uniform location
	GLuint m_UniformTexturePosition;
	GLuint m_UniformTextureVelocity;
	GLuint m_UniformTextureSize;
	GLuint m_UniformGravitationalConstant;

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