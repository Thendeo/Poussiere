/*----------------------------------------------------
**
**		Date: 2020
**		Author: Alexis LAFONT
**		Project: Particules
**		File: Image header file
**
----------------------------------------------------*/

#include "Image.h"
#include "AssertHdl.h"

Image::Image()
	: m_Width(0)
	, m_Height(0)
	, m_ImageSize(0)
	, m_PixelType(0)
	, m_ImageType(ImageType::ImageType_MAX)
{
}

Image::Image(unsigned int pPixelType)
	: m_Width(0)
	, m_Height(0)
	, m_ImageSize(0)
	, m_PixelType(pPixelType)
	, m_ImageType(ImageType::ImageType_MAX)
{
}

Image::~Image()
{
}


int Image::getWidth()
{
	doAssert(0 != m_Width);
	return m_Width;
}

int Image::getHeight()
{
	doAssert(0 != m_Height);
	return m_Height;
}

unsigned int Image::getPixelType()
{
	doAssert(0 != m_PixelType);
	return m_PixelType;
}

ImageType Image::getImgType()
{
	doAssert(ImageType::ImageType_MAX != m_ImageType);
	return m_ImageType;
}
