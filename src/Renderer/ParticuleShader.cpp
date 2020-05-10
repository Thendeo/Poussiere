/*----------------------------------------------------
**
**		Date: 2019
**		Author: Thendeo
**		Project: Particules
**		File: Particule implementation file
**
----------------------------------------------------*/

#include "ParticuleShader.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include "AssertHdl.h"
#include "ShaderLoader.h"



ParticuleShader::ParticuleShader(GLfloat* pVertexData, int pVertexSize, GLfloat* pUVData, int pUVSize)
	: m_VertexArrayID(0)
	, m_TextureID(0)
	, m_Vertexbuffer(0)
	, m_Texture(0)
	, m_uvBuffer(0)
	, m_ProgramID(0)
	, m_MatrixID(0)
	, m_MVP()
	, m_TextureLoaded(false)
{
	// Create Arrays
	glGenVertexArrays(1, &m_VertexArrayID);
	glBindVertexArray(m_VertexArrayID);

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	m_ProgramID = ShaderLoader::_loadShader("SimpleVertexShader.vertexshader", "SimpleGeometryShader.geometryshader", "SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" and Texture uniforms
	m_MatrixID = glGetUniformLocation(m_ProgramID, "MVP");
	m_TextureID = glGetUniformLocation(m_ProgramID, "myTextureSampler");

	// Set texture to buffer ID 0
	glUniform1i(m_TextureID, 0);

	glGenBuffers(1, &m_Vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, pVertexSize, pVertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &m_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, pUVSize, pUVData, GL_STATIC_DRAW);


}

ParticuleShader::ParticuleShader(std::string pTexturePath)
	: m_VertexArrayID(0)
	, m_TextureID(0)
	, m_Vertexbuffer(0)
	, m_Texture(0)
	, m_uvBuffer(0)
	, m_ProgramID(0)
	, m_MatrixID(0)
	, m_MVP()
	, m_TextureLoaded(false)
{
	m_ProgramID = ShaderLoader::_loadShader("SimpleVertexShader.vertexshader", "SimpleGeometryShader.geometryshader", "SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" and Texture uniforms
	m_MatrixID = glGetUniformLocation(m_ProgramID, "MVP");
	m_TextureID = glGetUniformLocation(m_ProgramID, "myTextureSampler");

	// Set texture to buffer ID 0
	glUniform1i(m_TextureID, 0);
}

ParticuleShader::~ParticuleShader()
{
}

void ParticuleShader::draw()
{
	// Use our shader
	glUseProgram(m_ProgramID);

	glActiveTexture(GL_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, m_TextureID);
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	
	// 2nd attribute buffer : UVs
/*	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	*/
	// Draw the triangle !
	glDrawArrays(GL_POINTS, 0, 3); // 12*3 indices starting at 0 -> 12 triangles
	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
}

void ParticuleShader::setVertexParameters(glm::mat4 pMVP)
{
	m_MVP = pMVP;
	glUniformMatrix4fv(m_MatrixID, 1, GL_FALSE, &m_MVP[0][0]);
}

void ParticuleShader::loadTexture(Image* pTexture)
{
	GLint l_Type = 0;
	switch (pTexture->getImgType())
	{
	case ImageType::ImageType_Gray:
		doAssert(false);
		break;
	case ImageType::ImageType_GrayA:
		doAssert(false);
		break;
	case ImageType::ImageType_Palette:
		doAssert(false);
		break;
	case ImageType::ImageType_RGB:
		l_Type = GL_RGB;
		break;
	case ImageType::ImageType_RGBA:
		l_Type = GL_RGBA;
		break;
	default:
		doAssert(false);
		break;

	}
	glTexImage2D(GL_TEXTURE_2D, 0, l_Type, pTexture->getWidth(), pTexture->getHeight(), 0, l_Type, GL_UNSIGNED_BYTE, pTexture->getData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//glActiveTexture(GL_TEXTURE0);
}

void ParticuleShader::setTexture(GLuint pTexture)
{
	glUniform1i(m_TextureID, pTexture);
}