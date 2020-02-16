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

#include <string>

//! @brief Shader to draw a particule
//! Basic class with vertex and pixel shader to draw a particule from a png.
//! Input for the vertex shader is MVP
//! Pixel shader render texture from UV map
class ParticuleShader
{
public:

	ParticuleShader();
	ParticuleShader(std::string pTexturePath);
	~ParticuleShader();

	GLuint bind(GLuint p_VertexBufferID);
	void draw();

	void setVertexParameters(glm::mat4 pMVP);
	void loadTexture(std::string pTexturePath);
	void setVertexBuffer(GLuint pVertexBufferID);
	void setUVBuffer(GLuint pUVBufferID);

private:
	
	GLuint m_VertexArrayID;
	GLuint m_vertexbuffer;
	GLuint m_uvBuffer;

	GLuint m_ProgramID;

	GLuint m_MatrixID;
	glm::mat4 m_MVP;

	GLuint m_TextureID;
	GLuint m_Texture;
	bool m_TextureLoaded;
};