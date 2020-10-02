/*----------------------------------------------------
**
**		Date: 2020
**		Author: Thendeo
**		Project: Particules
**		File: Textyre2D header file
**
----------------------------------------------------*/
#pragma once
#include "TextureDefinitions.h"

#include <list>
#include <string>

//! @brief Encapsulate OpenGL Client/Serveur usage and informations
class Texture2D
{
public:

	//! @brief Fills single level instance
	//! @param p_File Binary path
	Texture2D(const char* p_File, eTextureUnitMap p_TUM);

	//! @brief Fills instance with list of binary files
	//! Each level will have one binary texture data
	//! @param p_FileList List of the files path to parse
	Texture2D(std::list<std::string> p_FileList, eTextureUnitMap p_TUM);

	~Texture2D();

	unsigned int getWidth();
	unsigned int getHeight();
	unsigned int getSize();

	GLuint getTextureUnit();
	GLuint getTextureName();

private:
	unsigned int m_Width;
	unsigned int m_Height;
	unsigned int m_Size;
	
	GLuint m_CurrentTextureUnit;
	GLuint m_TextureName;
};