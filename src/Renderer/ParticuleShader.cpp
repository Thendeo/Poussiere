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



ParticuleShader::ParticuleShader(unsigned int pNumberOfParticule)
: Shader("DrawParticules.vertexshader", "DrawParticules.geometryshader", "DrawParticules.fragmentshader")
, m_UniformMVP()
, m_UniformTexturePosition(0U)
, m_UniformTextureRendering(0U)
, m_ParticuleNumber(pNumberOfParticule)
{
	glUseProgram(m_ProgramID);

	m_UniformMVP = glGetUniformLocation(m_ProgramID, "MVP");
	m_UniformTexturePosition = glGetUniformLocation(m_ProgramID, "particulePositionMatrix");
	m_UniformTextureRendering = glGetUniformLocation(m_ProgramID, "particuleSampler");


	GLenum l_Err = glGetError();
	doAssert(l_Err == GL_NO_ERROR);
}

ParticuleShader::~ParticuleShader()
{
}

void ParticuleShader::draw()
{
	// Use our shader
	GLenum l_Err = glGetError();

	glDrawArrays(GL_POINTS, 0, m_ParticuleNumber); // 12*3 indices starting at 0 -> 12 triangles

	doAssert(l_Err == GL_NO_ERROR);
}

void ParticuleShader::setVertexMVP(glm::mat4 pMVP)
{
	glUniformMatrix4fv(m_UniformMVP, 1, GL_FALSE, &pMVP[0][0]);
}
void ParticuleShader::setVertexPosTextureLoc(GLuint p_TexturePosition)
{
	glUniform1i(m_UniformTexturePosition, p_TexturePosition);
}
void ParticuleShader::setFragmentRenderTextureLoc(GLuint p_TextureRendering)
{
	glUniform1i(m_UniformTextureRendering, p_TextureRendering);
}
