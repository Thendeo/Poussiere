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

AdvanceShader::AdvanceShader(unsigned int pTextureSize)
: Shader("TexturedTriangle.vertexshader", "TexturedTriangle.geometryshader", "TextureAddition.fragmentshader")
, m_TextureSize(pTextureSize)
, m_UniformTexturePosition(0U)
, m_UniformTextureVelocity(0U)
, m_OutputTextureLocation(0U)
{
	glUseProgram(m_ProgramID);

	m_UniformTexturePosition = glGetUniformLocation(m_ProgramID, "textureA");
	m_UniformTextureVelocity = glGetUniformLocation(m_ProgramID, "textureB");


	GLenum l_Err = glGetError();
	doAssert(l_Err == GL_NO_ERROR);
}

AdvanceShader::~AdvanceShader()
{
}

void AdvanceShader::draw()
{
	glClearColor(0.8f, 0.1f, 0.1f, 0.0f); // Clear in red

	// Get viewport information for backup and restoration of it
	GLint l_viewportInfo[4];
	glGetIntegerv(GL_VIEWPORT, l_viewportInfo);

	// Draw
	glViewport(0, 0, m_TextureSize, m_TextureSize);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_ProgramID);
	glDrawArrays(GL_POINTS, 0, 1);

	// Restore context to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, l_viewportInfo[2], l_viewportInfo[3]);
}

void AdvanceShader::setFragmentPosition(GLuint p_TexturePosition)
{
	glUniform1i(m_UniformTexturePosition, p_TexturePosition);
}

void AdvanceShader::setFragmentVelocity(GLuint p_TextureVelocity)
{
	glUniform1i(m_UniformTextureVelocity, p_TextureVelocity);
}
