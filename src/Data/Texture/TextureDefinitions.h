/*----------------------------------------------------
**
**		Date: 2020
**		Author: Thendeo
**		Project: Particules
**		File: TextureDefinitions symbolics
**
----------------------------------------------------*/
#pragma once

// Needed for textureUnit defs
#include <GL/glew.h>

enum eTextureUnitMap : int
{
	eTUM_Position = GL_TEXTURE0,
	eTUM_UpdatedPosition = GL_TEXTURE1,
	eTUM_Velocity = GL_TEXTURE2,
	eTUM_UpdatedVelocity = GL_TEXTURE3,
	eTUM_ParticuleTexture = GL_TEXTURE4
};

enum eUniformUnitMap : char
{
	eUUM_Position = 0,
	eUUM_UpdatedPosition = 1,
	eUUM_Velocity = 2,
	eUUM_UpdatedVelocity = 3,
	eUUM_ParticuleTexture = 4
};