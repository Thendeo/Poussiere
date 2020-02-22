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

	virtual unsigned char* getData();

private:
	unsigned char* m_Data;
};