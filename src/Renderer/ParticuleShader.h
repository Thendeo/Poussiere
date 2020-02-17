/*----------------------------------------------------
**
**		Date: 2019
**		Author: Alexis LAFONT
**		Project: Particules
**		File: Particule header file
**
----------------------------------------------------*/

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Image.h"

#include <string>

//! @brief Shader to draw a particule
//! Basic class with vertex and pixel shader to draw a particule from a png.
//! Input for the vertex shader is MVP
//! Pixel shader render texture from UV map
class ParticuleShader
{
public:

	ParticuleShader(GLfloat* pVertexData, int pVertexSize, GLfloat* pUVData, int pUVSize);
	ParticuleShader(std::string pTexturePath);
	~ParticuleShader();

	void draw();

	void setVertexParameters(glm::mat4 pMVP);
	void loadTexture(Image* pTexture);

private:
	
	// Arrays
	GLuint m_VertexArrayID;
	GLuint m_TextureID;

	// BufferData
	GLuint m_Vertexbuffer;
	GLuint m_Texture;
	GLuint m_uvBuffer;

	GLuint m_ProgramID;

	GLuint m_MatrixID;
	glm::mat4 m_MVP;

	bool m_TextureLoaded;
};