/*----------------------------------------------------
**
**		Date: 2020
**		Author: Thendeo
**		Project: Particules
**		File: AdvanceShader implementation file
**
----------------------------------------------------*/

#include "AdvanceShader.h"
#include "AssertHdl.h"

AdvanceShader::AdvanceShader()
: Shader("AdvanceParticules.vertexshader", "AdvanceParticules.geometryshader", "AdvanceParticules.fragmentshader")
, m_UniformTexturePosition(0U)
, m_UniformTextureVelocity(0U)
{
	glUseProgram(m_ProgramID);

	m_UniformTexturePosition = glGetUniformLocation(m_ProgramID, "particulePosition");
	m_UniformTextureVelocity = glGetUniformLocation(m_ProgramID, "particuleVelocity");


	GLenum l_Err = glGetError();
	doAssert(l_Err == GL_NO_ERROR);
}

AdvanceShader::~AdvanceShader()
{
}

void AdvanceShader::draw()
{
	doAssert(false);
}

void AdvanceShader::setFragmentPosition(GLuint p_TexturePosition)
{
	doAssert(false);
}

void AdvanceShader::setFragmentVelocity(GLuint p_TextureVelocity)
{
	doAssert(false);
}
