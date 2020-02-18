/*----------------------------------------------------
**
**		Date: 2019
**		Author: Alexis LAFONT
**		Project: Particules
**		File: Image_8 header file
**
----------------------------------------------------*/

#pragma once

#include "Image.h"

//! @brief Pixel buffer of size char, loads different extensions
class Image_8 : public Image
{
public:
	Image_8();
	~Image_8();

	void loadFromBMP(const char* p_Path);
	void loadFromPNG(const char* p_Path);

	virtual int getWidth();
	virtual int getHeight();
	virtual size_t getType();
	virtual unsigned char* getData();


private:
	unsigned int m_Width;
	unsigned int m_Height;

	unsigned int m_ImageSize;
	unsigned char* m_Data;
};