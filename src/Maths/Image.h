/*----------------------------------------------------
**
**		Date: 2019
**		Author: Alexis LAFONT
**		Project: Particules
**		File: Image_8 header file
**
----------------------------------------------------*/

#pragma once

class Image
{
public:
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
	virtual size_t getType() = 0;
	virtual unsigned char* getData() = 0;
};