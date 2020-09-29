/*----------------------------------------------------
**
** Date: 2019
** Author : Thendeo
** Project : Particules
** File : Shader implementation file
**
---------------------------------------------------- */

#include "Shader.h"
#include "ShaderLoader.h"
#include "AssertHdl.h"

Shader::Shader(const char* pVertexFilePath, const char* pFragmentFilePath)
	: m_ProgramID(0U)
{
	GLenum l_Err = glGetError();
	doAssert(l_Err == GL_NO_ERROR); // Precheck before linking program
	m_ProgramID = ShaderLoader::_loadShader(pVertexFilePath, pFragmentFilePath);

	doAssert(l_Err == GL_NO_ERROR); // After linking check
}

Shader::Shader(const char* pVertexFilePath, const char* pGeometryFilePath, const char* pFragmentFilePath)
	: m_ProgramID(0U)
{
	GLenum l_Err = glGetError();
	doAssert(l_Err == GL_NO_ERROR); // Precheck before linking program
	m_ProgramID = ShaderLoader::_loadShader(pVertexFilePath, pGeometryFilePath, pFragmentFilePath);
	doAssert(l_Err == GL_NO_ERROR); // After linking check
}

Shader::~Shader()
{
	// Need to dealocate shader inside GPU ?
}
