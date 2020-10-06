/*----------------------------------------------------
**
**		Date: 2019
**		Author: Thendeo
**		Project: Particules
**		File: Image header file
**
----------------------------------------------------*/
#pragma once

#include <stdio.h>
#include "AssertHdl.h"

#define PNG_DEBUG 3
#include "png.h"

enum class ImageType
{
	ImageType_Gray = 0,
	ImageType_GrayA,
	ImageType_RGB,
	ImageType_RGBA,
	ImageType_Palette,
	ImageType_MAX
};

template <typename PixelDepth>
class Image
{
public:

	Image();
	~Image();

	//! @brief Loads data from a BMP file
	//! @param p_Path Path to the file
	void loadFromBMP(const char* p_Path);

	//! @brief Loads data from a PNG file
	//! @param p_Path Path to the file
	void loadFromPNG(const char* p_Path);

	//! @brief Returns the width of the image
	int getWidth();

	//! @brief Returns the width of the image
	int getHeight();

	//! @brief Returns the pixel size of the image
	//! @return The size of one pixel given image type
	unsigned int getPixelSize();

	//! @brief Returns the image type
	//! @return The type of the image from ImageType enumeration
	ImageType getImgType();

	//! @brief Returns a reference to the buffer data
	//! @return Template defined ptr to the image buffer
	PixelDepth* getData();

protected:

	//! @brief Open a PNG file and verify header informations
	//! @param p_Path The path to the file
	void loadPNG(const char* p_Path);

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
};


template <typename PixelDepth>
Image<PixelDepth>::Image()
	: m_Width(0)
	, m_Height(0)
	, m_ImageSize(0)
	, m_PixelSize(0)
	, m_ImageType(ImageType::ImageType_MAX)
	, m_Data(NULL)
	, m_File(NULL)
	, m_PngPtr(NULL)
	, m_InfoPtr(NULL)
{
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
void Image<PixelDepth>::loadFromBMP(const char* p_Path)
{
	if (NULL != m_Data)
	{
		m_Width = 0;
		m_Height = 0;
		m_ImageSize = 0;
		m_Data = NULL;
	}
	FILE* l_File = fopen(p_Path, "rb");
	doAssert(NULL != l_File);

	unsigned char l_Header[54]; // 54o header
	size_t l_Readed = fread(l_Header, 1, 54, l_File);
	doAssert(54 == l_Readed); // File too small, or corrupted

	doAssert('B' == l_Header[0]);
	doAssert('M' == l_Header[1]);


	// Read size
	// Lit des entiers à partir du tableau d'octets
	unsigned int l_DataPos; // ptr to start of data
	l_DataPos = *(int*)&(l_Header[0x0A]);
	m_ImageSize = *(int*)&(l_Header[0x22]) * 3;
	m_Width = *(int*)&(l_Header[0x12]);
	m_Height = *(int*)&(l_Header[0x16]);
	m_ImageType = ImageType::ImageType_RGB;

	if (0 == m_ImageSize)
	{
		m_ImageSize = m_Width * m_Height * 3; // RGB
	}
	doAssert(0 != m_ImageSize); // Empty image
	doAssert(0 != l_DataPos); // No data pos

	//m_Data = (PixelDepth*)malloc(m_ImageSize);
	m_Data = new PixelDepth[m_ImageSize];
	doAssert(0 != m_Data);
	l_Readed = fread(m_Data, 1, m_ImageSize, l_File);

	fclose(l_File);
}

template <typename PixelDepth>
void Image<PixelDepth>::loadFromPNG(const char* p_Path)
{
	loadPNG(p_Path);

	unsigned int l_Width = m_Width;
	unsigned int l_Height = m_Height;
	char l_PixelSize = m_PixelSize;
	ImageType l_ImageType = m_ImageType;

	loadInformations();

	// Compute size and alloc memory
	m_ImageSize = m_Width * m_Height * m_PixelSize;
	m_Data = new PixelDepth[m_ImageSize];

	readData();

	// Free data
	png_destroy_read_struct(&m_PngPtr, &m_InfoPtr, NULL);
	fclose(m_File);
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
PixelDepth* Image<PixelDepth>::getData()
{
	doAssert(NULL != m_Data);
	return m_Data;
}

template <typename PixelDepth>
void Image<PixelDepth>::loadPNG(const char* p_Path)
{
	m_File = fopen(p_Path, "rb");
	doAssert(NULL != m_File);

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

	unsigned char l_ByteDepth = png_get_bit_depth(m_PngPtr, m_InfoPtr);
	doAssert(8 == l_ByteDepth);

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

template <typename PixelDepth>
void Image<PixelDepth>::readData()
{
	int l_NbPass = png_set_interlace_handling(m_PngPtr); // Number of interlacing
	png_read_update_info(m_PngPtr, m_InfoPtr); //  Update struct info for debug

	png_bytep* l_RowsPtr = NULL;
	l_RowsPtr = new png_bytep[sizeof(png_bytep) * m_Height];
	doAssert(NULL != l_RowsPtr);

	// Mapping image buffer addresses with rows from png
	for (unsigned int l_IdRow = 0; l_IdRow < m_Height; l_IdRow++)
	{
		l_RowsPtr[l_IdRow] = m_Data + ((m_Height - (l_IdRow + 1)) * m_Width * 4U);
	}

	// Read image
	png_read_image(m_PngPtr, l_RowsPtr);
	delete(l_RowsPtr);
}
