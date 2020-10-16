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

#include "AssertHdl.h"

Texture2D::Texture2D(unsigned int p_Width, unsigned int p_Height, GLint p_Type, size_t p_SizeOfSample, eTextureUnitMap p_TUM)
: m_Width(p_Width)
, m_Height(p_Height)
, m_Size(p_Width*p_Height)
, m_CurrentTextureUnit(p_TUM)
, m_TextureName(0U)
, m_TextureType(p_Type)
{
	// Load velocity texture
	glGenTextures(1, &m_TextureName);
	glActiveTexture(m_CurrentTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_TextureName);

	GLenum l_TextureType = 0;
	switch (p_SizeOfSample)
	{
	case 1U:
		l_TextureType = GL_UNSIGNED_BYTE;
		break;
	case 2U:
		l_TextureType = GL_UNSIGNED_SHORT;
		break;
	default:
		doAssert(false);
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, m_TextureType, p_Width, p_Height,
		0, m_TextureType, l_TextureType, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}

Texture2D::Texture2D(Image<UByte>* p_Img, eTextureUnitMap p_TUM)
	: m_Width(0U)
	, m_Height(0U)
	, m_Size(0U)
	, m_CurrentTextureUnit(p_TUM)
	, m_TextureName(0U)
	, m_TextureType(0)
{
	// Load velocity texture
	glGenTextures(1, &m_TextureName);
	glActiveTexture(m_CurrentTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_TextureName);

	m_Width = p_Img->getWidth();
	m_Height = p_Img->getHeight();
	m_Size = m_Width * m_Height;

	switch (p_Img->getImgType())
	{
	case ImageType::ImageType_RGB:
		m_TextureType = GL_RGB;
		break;
	case ImageType::ImageType_RGBA:
		m_TextureType = GL_RGBA;
		break;
	default:
		doAssert(false);
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, m_TextureType, p_Img->getWidth(), p_Img->getHeight(),
		0, m_TextureType, GL_UNSIGNED_BYTE, p_Img->getData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}

Texture2D::Texture2D(Image<UWord>* p_Img, eTextureUnitMap p_TUM)
	: m_Width(0U)
	, m_Height(0U)
	, m_Size(0U)
	, m_CurrentTextureUnit(p_TUM)
	, m_TextureName(0U)
	, m_TextureType(0)
{
	// Load velocity texture
	glGenTextures(1, &m_TextureName);
	glActiveTexture(m_CurrentTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_TextureName);

	m_Width = p_Img->getWidth();
	m_Height = p_Img->getHeight();
	m_Size = m_Width * m_Height;

	switch (p_Img->getImgType())
	{
	case ImageType::ImageType_RGB:
		m_TextureType = GL_RGB;
		break;
	case ImageType::ImageType_RGBA:
		m_TextureType = GL_RGBA;
		break;
	default:
		doAssert(false);
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, m_TextureType, p_Img->getWidth(), p_Img->getHeight(),
		0, m_TextureType, GL_UNSIGNED_SHORT, p_Img->getData());

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

GLint Texture2D::getTextureType()
{
	return m_TextureType;
}
