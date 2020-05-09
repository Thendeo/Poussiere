/*----------------------------------------------------
**
**		Date: 2019
**		Author: Thendeo
**		Project: Particules
**		File: Image_8 header file
**
----------------------------------------------------*/
#pragma once

enum class ImageType
{
	ImageType_Gray = 0,
	ImageType_GrayA,
	ImageType_RGB,
	ImageType_RGBA,
	ImageType_Palette,
	ImageType_MAX
};

class Image
{
public:

	Image();
	~Image();

	virtual int getWidth();
	virtual int getHeight();
	virtual unsigned int getPixelType();
	virtual ImageType getImgType();
	virtual unsigned char* getData() = 0;

protected:
	Image(unsigned int pPixelType);

	unsigned int m_Width;
	unsigned int m_Height;

	unsigned int m_ImageSize;
	unsigned int m_PixelType;
	ImageType m_ImageType;
};

