/*----------------------------------------------------
**
**		Date: 2020
**		Author: Thendeo
**		Project: Particules
**		File: Textyre2D implementation file
**
----------------------------------------------------*/
#pragma once

#include "Texture2D.h"

#include "Image_8.h"

Texture2D::Texture2D(const char* p_File, eTextureUnitMap p_TUM)
: m_Width(0U)
, m_Height(0U)
, m_Size(0U)
, m_CurrentTextureUnit(p_TUM)
, m_TextureName(0U)
{
	// Load velocity texture
	glGenTextures(1, &m_TextureName);
	glActiveTexture(m_CurrentTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_TextureName);

	Image_8 l_TextureData;
	l_TextureData.loadFromPNG(p_File);

	GLint l_Type = 0;
	switch (l_TextureData.getImgType())
	{
	case ImageType::ImageType_RGB:
		l_Type = GL_RGB;
		break;
	case ImageType::ImageType_RGBA:
		l_Type = GL_RGBA;
		break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, l_Type, l_TextureData.getWidth(), l_TextureData.getHeight(),
		0, l_Type, GL_UNSIGNED_BYTE, l_TextureData.getData()); // TODO change Image_8 to generic. Here we assume texture is 8bit, but no

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}

Texture2D::Texture2D(std::list<std::string> p_FileList, eTextureUnitMap p_TUM)
	: m_Width(0U)
	, m_Height(0U)
	, m_Size(0U)
	, m_CurrentTextureUnit(p_TUM)
	, m_TextureName(0U)
{
	/*
	// Parse the list to get raw data
	for (std::list<std::string>::iterator iFilesIterator = p_FileList.begin();
		iFilesIterator != p_FileList.end(); iFilesIterator++)
	{
		l_TextureData.loadFromPNG(iFilesIterator->c_str);
	}*/
}

Texture2D::~Texture2D()
{
}

unsigned int Texture2D::getWidth()
{
	return m_Width;
}

unsigned int Texture2D::getHeight()
{
	return m_Height;
}

unsigned int Texture2D::getSize()
{
	return m_Size;
}

GLuint Texture2D::getTextureUnit()
{
	return m_CurrentTextureUnit;
}

GLuint Texture2D::getTextureName()
{
	return m_TextureName;
}
