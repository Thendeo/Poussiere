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

#include "Image.h"

#include <list>
#include <string>

//! @brief Encapsulate OpenGL Client/Serveur usage and informations
class Texture2D
{
public:

	//! @brief Empty texture
	//! @param p_Width Width
	//! @param p_Height Height
	//! @param p_Type OpenGL pixel type
	//! @param p_SizeOfSample Size of a pixel component (8/16/32/64 bits)
	//! @param p_TUM Texture unit
	Texture2D(unsigned int p_Width, unsigned int p_Height, GLint p_Type, size_t p_SizeOfSample, eTextureUnitMap p_TUM);

	//! @brief Fills single level instance for 8bit images
	//! @param p_Img Image data ptr
	//! @param p_TUM Texture unit
	Texture2D(Image<UByte>* p_Img, eTextureUnitMap p_TUM);

	//! @brief Fills single level instance for 16 bit images
	//! @param p_Img Image data ptr
	//! @param p_TUM Texture unit
	Texture2D(Image<UWord>* p_Img, eTextureUnitMap p_TUM);

	//! @brief Fills instance with list of binary files
	//! Each level will have one binary texture data
	//! @param p_FileList List of the files path to parse
	//! @param p_TUM Texture unit
	Texture2D(std::list<std::string> p_FileList, eTextureUnitMap p_TUM);

	~Texture2D();

	unsigned int getWidth();
	unsigned int getHeight();
	unsigned int getSize();

	GLuint getTextureUnit();
	GLuint getTextureName();
	GLint getTextureType();

private:
	unsigned int m_Width;
	unsigned int m_Height;
	unsigned int m_Size;
	
	GLuint m_CurrentTextureUnit;
	GLuint m_TextureName;
	GLint m_TextureType;
};
