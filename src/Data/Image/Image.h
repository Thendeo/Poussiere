/*----------------------------------------------------
**
**		Date: 2019
**		Author: Thendeo
**		Project: Particules
**		File: Image header file
**
----------------------------------------------------*/
#pragma once

#include "AssertHdl.h"
#include "ImageType.h"

#define PNG_DEBUG 3
#include "png.h"

#include "tiffio.h"

#include <string>

enum class ImageType
{
	ImageType_Gray = 0,
	ImageType_GrayA,
	ImageType_RGB,
	ImageType_RGBA,
	ImageType_Palette,
	ImageType_MAX
};

class I_Image
{
public:

	//! @brief Returns the width of the image
	virtual int getWidth() = 0;

	//! @brief Returns the width of the image
	virtual int getHeight() = 0;

	//! @brief Returns the pixel size of the image
	//! @return The size of one pixel given image type
	virtual unsigned int getPixelSize() = 0;

	//! @brief Returns the image type
	//! @return The type of the image from ImageType enumeration
	virtual ImageType getImgType() = 0;

	virtual void setWidth(int p_Width) = 0;
	virtual void setHeight(int p_Height) = 0;
	virtual void setPixelSize(unsigned int p_PixelSize) = 0;
	virtual void setImgType(ImageType p_ImgType) = 0;
};

template <typename PixelDepth>
class Image : public I_Image
{
public:

	Image(const char* p_ImagePath);
	~Image();

	virtual int getWidth();
	virtual int getHeight();
	virtual unsigned int getPixelSize();
	virtual ImageType getImgType();

	virtual void setWidth(int p_Width);
	virtual void setHeight(int p_Height);
	virtual void setPixelSize(unsigned int p_PixelSize);
	virtual void setImgType(ImageType p_ImgType);

	//! @brief Returns a reference to the buffer data
	//! @return Template defined ptr to the image buffer
	PixelDepth* getData();

private:

	//! @brief Open a PNG file and verify header informations
	//! @param p_Path The path to the file
	void loadPNG();

	//! @brief Recover informations from the file (width, height, etc.)
	void loadInformations();

	//! @brief Loads internal file data inside buffer member
	void readData();

	//! @brief Number of horizontal pixel
	unsigned int m_Width;

	//! @brief Number of vertical pixel
	unsigned int m_Height;

	//! @brief Image size as number of byte used to store data
	//! This size take account of the width, the height and the pixel size
	unsigned int m_ImageSize;

	//! @brief Number of component inside one pixel
	//! The pixel size is defined by the pixel type, a gray image will have
	//! a pixel size of 1, RGBA images will have a piwel size of 4
	char m_PixelSize;

	//! @brief Image type, or color model, stored as an enumeration
	ImageType m_ImageType;

	//! Templatized buffer data
	PixelDepth* m_Data;

	//! @brief File ptr
	FILE* m_File;

	//! @brief PNG structural ptr from PNGLib
	png_structp m_PngPtr;

	//! @brief PNG information struct from PNGLib
	png_infop m_InfoPtr;

	//! @brief TIFF file ptr
	TIFF* m_TiffPtr;
};


template <typename PixelDepth>
Image<PixelDepth>::Image(const char* p_ImagePath)
	: m_Width(0)
	, m_Height(0)
	, m_ImageSize(0)
	, m_PixelSize(0)
	, m_ImageType(ImageType::ImageType_MAX)
	, m_Data(NULL)
	, m_File(NULL)
	, m_PngPtr(NULL)
	, m_InfoPtr(NULL)
	, m_TiffPtr(NULL)
{
	std::string l_ImagePath(p_ImagePath);

	// Get extension position inside string
	size_t l_ExtensionPos = l_ImagePath.rfind('.', l_ImagePath.length());
	doAssert(std::string::npos != l_ExtensionPos);

	// Get extension string and compare
	std::string l_Extension = l_ImagePath.substr(l_ExtensionPos + 1, l_ImagePath.length() - l_ExtensionPos);

	m_File = fopen(p_ImagePath, "rb");
	if (0 == strcmp("png", l_Extension.c_str()))
	{
		loadPNG();
		loadInformations();

		m_ImageSize = m_Width * m_Height * m_PixelSize;
		m_Data = new PixelDepth[m_ImageSize];

		readData();

		// Free data
		png_destroy_read_struct(&m_PngPtr, &m_InfoPtr, NULL);
		fclose(m_File);
	}
	else if (0 == strcmp("tif", l_Extension.c_str()))
	{

		// Test to load TIFF image
		m_TiffPtr = TIFFOpen(p_ImagePath, "r");
		doAssert(NULL != m_TiffPtr);

		TIFFGetField(m_TiffPtr, TIFFTAG_IMAGEWIDTH, &m_Width);
		TIFFGetField(m_TiffPtr, TIFFTAG_IMAGELENGTH, &m_Height);
		m_ImageSize = m_Width * m_Height;
		m_ImageType = ImageType::ImageType_RGBA;
		m_PixelSize = 4;
		m_ImageSize *= m_PixelSize;

		readData();

		TIFFClose(m_TiffPtr);
	}
	else
	{
		doAssert(false);
	}
}

template <typename PixelDepth>
Image<PixelDepth>::~Image()
{
	if (NULL != m_Data)
	{
		free(m_Data);
	}
}

template <typename PixelDepth>
int Image<PixelDepth>::getWidth()
{
	doAssert(0 != m_Width);
	return m_Width;
}

template <typename PixelDepth>
int Image<PixelDepth>::getHeight()
{
	doAssert(0 != m_Height);
	return m_Height;
}

template <typename PixelDepth>
unsigned int Image<PixelDepth>::getPixelSize()
{
	doAssert(0 != m_PixelSize);
	return m_PixelSize;
}

template <typename PixelDepth>
ImageType Image<PixelDepth>::getImgType()
{
	doAssert(ImageType::ImageType_MAX != m_ImageType);
	return m_ImageType;
}

template <typename PixelDepth>
void Image<PixelDepth>::setWidth(int p_Width)
{
	m_Width = p_Width;
}

template <typename PixelDepth>
void Image<PixelDepth>::setHeight(int p_Height)
{
	m_Height = p_Height;
}

template <typename PixelDepth>
void Image<PixelDepth>::setPixelSize(unsigned int p_PixelSize)
{
	m_PixelSize = p_PixelSize;
}

template <typename PixelDepth>
void Image<PixelDepth>::setImgType(ImageType p_ImgType)
{
	m_ImageType = p_ImgType;
}

template <typename PixelDepth>
PixelDepth* Image<PixelDepth>::getData()
{
	doAssert(NULL != m_Data);
	return m_Data;
}

template <typename PixelDepth>
void Image<PixelDepth>::loadPNG()
{
	// Check if PNG
	unsigned char l_Header[8];
	size_t l_Ret = fread(&l_Header, 1, 8, m_File);
	doAssert(8 == l_Ret);

	l_Ret = png_sig_cmp(l_Header, 0, 8);

	// Read structure
	m_PngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	doAssert(NULL != m_PngPtr);

	m_InfoPtr = png_create_info_struct(m_PngPtr);
	doAssert(NULL != m_PngPtr);

	// Error jump context
	l_Ret = setjmp(png_jmpbuf(m_PngPtr));
	doAssert(0 == l_Ret);

	png_init_io(m_PngPtr, m_File);
	png_set_sig_bytes(m_PngPtr, 8);

	png_read_info(m_PngPtr, m_InfoPtr);
}

template <typename PixelDepth>
void Image<PixelDepth>::loadInformations()
{
	m_Width = png_get_image_width(m_PngPtr, m_InfoPtr);
	m_Height = png_get_image_height(m_PngPtr, m_InfoPtr);

	// Allowing only 8 or 16 png images
	unsigned char l_ByteDepth = png_get_bit_depth(m_PngPtr, m_InfoPtr);
	if (16 == l_ByteDepth)
	{
		png_set_strip_16(m_PngPtr);
	}
	else
	{
		doAssert(8 == l_ByteDepth);
	}

	unsigned char l_ImgType = png_get_color_type(m_PngPtr, m_InfoPtr);
	switch (l_ImgType)
	{
	case PNG_COLOR_TYPE_GRAY:
		m_ImageType = ImageType::ImageType_Gray;
		m_PixelSize = 1;
		break;

	case PNG_COLOR_TYPE_PALETTE:
		m_ImageType = ImageType::ImageType_Palette;
		doAssert(false); // Don't know yet to handle palette size
		break;

	case PNG_COLOR_TYPE_RGB:
		m_ImageType = ImageType::ImageType_RGB;
		m_PixelSize = 3;
		break;

	case PNG_COLOR_TYPE_RGBA:
		m_ImageType = ImageType::ImageType_RGBA;
		m_PixelSize = 4;
		break;

	case PNG_COLOR_TYPE_GA:
		m_ImageType = ImageType::ImageType_GrayA;
		m_PixelSize = 2;
		break;
	default:
		doAssert(false);
		break;
	}
}