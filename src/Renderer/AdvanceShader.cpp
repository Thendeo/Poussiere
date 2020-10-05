/*----------------------------------------------------
**
**		Date: 2020
**		Author: Thendeo
**		Project: Particules
**		File: AdvanceShader implementation file
**
----------------------------------------------------*/

#include "AdvanceShader.h"
#include "TextureDefinitions.h"
#include "AssertHdl.h"

AdvanceShader::AdvanceShader(unsigned int pTextureSize)
: Shader("TexturedTriangle.vertexshader", "TexturedTriangle.geometryshader", "TextureAddition.fragmentshader")
, m_TextureSize(pTextureSize)
, m_UniformTexturePosition(0U)
, m_UniformTextureVelocity(0U)
, m_InputTexID(0U)
, m_OutputTexID(0U)
, m_InputTexLoc(0U)
, m_OutputTexLoc(0U)
, m_FrameBuffer(0U)
, m_OutputSwitch(true)
{
	glUseProgram(m_ProgramID);

	m_UniformTexturePosition = glGetUniformLocation(m_ProgramID, "textureA");
	glUniform1i(m_UniformTexturePosition, eUniformUnitMap::eUUM_Position);
	m_UniformTextureVelocity = glGetUniformLocation(m_ProgramID, "textureB");
	glUniform1i(m_UniformTextureVelocity, eUniformUnitMap::eUUM_Velocity);

	GLenum l_Err = glGetError();
	doAssert(l_Err == GL_NO_ERROR);

	// Create FrameBuffer
	glGenFramebuffers(1, &m_FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
	m_DrawTarget[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, m_DrawTarget);
}

AdvanceShader::~AdvanceShader()
{
}

void AdvanceShader::draw()
{
	swapOutput();

	// Get viewport information for backup and restoration of it
	GLint l_viewportInfo[4];
	glGetIntegerv(GL_VIEWPORT, l_viewportInfo);

	// Draw
	glViewport(0, 0, m_TextureSize, m_TextureSize);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_ProgramID);
	glDrawArrays(GL_POINTS, 0, 1);

	// Restore context to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, l_viewportInfo[2], l_viewportInfo[3]);

}

void AdvanceShader::setOutputLocation(GLuint p_InputTexID, GLuint p_OutputTexID)
{
	m_InputTexID = p_InputTexID;
	m_OutputTexID = p_OutputTexID;
}

void AdvanceShader::swapOutput()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
	if (true == m_OutputSwitch)
	{
		m_OutputSwitch = false;
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_OutputTexID, 0);
		glActiveTexture(eTextureUnitMap::eTUM_Position);
		glBindTexture(GL_TEXTURE_2D, m_InputTexID);
		glActiveTexture(eTextureUnitMap::eTUM_UpdatedPosition);
		glBindTexture(GL_TEXTURE_2D, m_OutputTexID);
	}
	else
	{
		m_OutputSwitch = true;
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_InputTexID, 0);
		glActiveTexture(eTextureUnitMap::eTUM_Position);
		glBindTexture(GL_TEXTURE_2D, m_OutputTexID);
		glActiveTexture(eTextureUnitMap::eTUM_UpdatedPosition);
		glBindTexture(GL_TEXTURE_2D, m_InputTexID);
	}
	GLenum l_Err = glGetError();
	doAssert(l_Err == GL_NO_ERROR);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
