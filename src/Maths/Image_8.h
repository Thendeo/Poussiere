/*----------------------------------------------------
**
**		Date: 2019
**		Author: Thendeo
**		Project: Particules
**		File: Image_8 header file
**
----------------------------------------------------*/

#pragma once

#include "Image.h"

#include <stdio.h>

#define PNG_DEBUG 3
#include "png.h"

//! @brief Pixel buffer of size char, loads different extensions
class Image_8 : public Image
{
public:
	Image_8();
	~Image_8();

	void loadFromBMP(const char* p_Path);
	void loadFromPNG(const char* p_Path);

	virtual unsigned char* getData();

private:

	void loadPNG(const char* p_Path);
	void loadInformations();
	void readData();

	unsigned char* m_Data;

	FILE* m_File;

	png_structp m_PngPtr;
	png_infop m_InfoPtr;
};