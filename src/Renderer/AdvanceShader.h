/*----------------------------------------------------
**
**		Date: 2019
**		Author: Alexis LAFONT
**		Project: Particules
**		File: AdvanceShader header file
**
----------------------------------------------------*/

#pragma once

#include "Shader.h"

#include <GL/glew.h>

//! @brief Shader to dvance a particule
//! Enable one to combine two textures by adding them
//! Input for the vertex shader : point with GS or member triangle ?
//! Pixel shader render adds two texture to a render buffer
class AdvanceShader : public Shader
{
public:

	AdvanceShader();
	~AdvanceShader();

	virtual void draw();

	void setVertexParameters(/*Triangle or point ?*/); // useless ?
	void loadTexture(/*buffer1 + buffer2*/);

private:

	// Arrays
	GLuint m_VertexArrayID;
	GLuint m_TextureID;

	// BufferData
	GLuint m_Vertexbuffer;

	GLuint m_ProgramID;

	bool m_TextureLoaded;
};