/*----------------------------------------------------
**
**		Date: 2019
**		Author: Alexis LAFONT
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

// TODO move this to shader parser (new class)
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


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

	m_ProgramID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

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
	m_ProgramID = LoadShaders("SimpleFragmentShader.vertexshader", "SimpleVertexShader.fragmentshader");

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

	glBindBuffer(GL_ARRAY_BUFFER, m_TextureID);
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	
	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // 12*3 indices starting at 0 -> 12 triangles
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void ParticuleShader::setVertexParameters(glm::mat4 pMVP)
{
	m_MVP = pMVP;
	glUniformMatrix4fv(m_MatrixID, 1, GL_FALSE, &m_MVP[0][0]);
}

void ParticuleShader::loadTexture(int pWidth, int pHeight, unsigned char* pData)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pWidth, pHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, pData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glActiveTexture(GL_TEXTURE0);
}