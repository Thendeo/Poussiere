/*----------------------------------------------------
**
**		Date: 2019
**		Author: Thendeo
**		Project: Particules
**		File: Image_8 implementation file
**
----------------------------------------------------*/

#include "Image_8.h"

#include <stdlib.h>


#include "AssertHdl.h"

Image_8::Image_8()
	: Image(8)
	, m_Data(NULL)
	, m_File(NULL)
	, m_PngPtr(NULL)
	, m_InfoPtr(NULL)
{
}
Image_8::~Image_8()
{
	free(m_Data);
}

void Image_8::loadFromBMP(const char* p_Path)
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
	l_DataPos = *(int*) & (l_Header[0x0A]);
	m_ImageSize = *(int*) & (l_Header[0x22]) * 3;
	m_Width = *(int*) & (l_Header[0x12]);
	m_Height = *(int*) & (l_Header[0x16]);
	m_ImageType = ImageType::ImageType_RGB;

	if (0 == m_ImageSize)
	{
		m_ImageSize = m_Width * m_Height * 3; // RGB
	}
	doAssert(0 != m_ImageSize); // Empty image
	doAssert(0 != l_DataPos); // No data pos

	m_Data = (unsigned char*)malloc(m_ImageSize);
	doAssert(0 != m_Data);
	l_Readed = fread(m_Data, 1, m_ImageSize, l_File);

	fclose(l_File);
}

void Image_8::loadFromPNG(const char* p_Path)
{
	loadPNG(p_Path);

	unsigned int l_Width = m_Width;
	unsigned int l_Height = m_Height;
	char l_PixelSize = m_PixelSize;
	ImageType l_ImageType = m_ImageType;

	loadInformations();

	// Compute size and alloc memory
	m_ImageSize = m_Width * m_Height * m_PixelSize;
	m_Data = (unsigned char*)malloc(m_ImageSize);

	readData();
	
	// Free data
	png_destroy_read_struct(&m_PngPtr, &m_InfoPtr, NULL);
	fclose(m_File);
}

unsigned char* Image_8::getData()
{
	doAssert(NULL != m_Data);
	return m_Data;
}

void Image_8::loadPNG(const char* p_Path)
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

void Image_8::loadInformations()
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

void Image_8::readData()
{
	int l_NbPass = png_set_interlace_handling(m_PngPtr); // Number of interlacing
	png_read_update_info(m_PngPtr, m_InfoPtr); //  Update struct info for debug

	png_bytep* l_RowsPtr = NULL;
	l_RowsPtr = (png_bytep*)malloc(sizeof(png_bytep) * m_Height);
	doAssert(NULL != l_RowsPtr);

	// Mapping image buffer addresses with rows from png
	for (unsigned int l_IdRow = 0; l_IdRow < m_Height; l_IdRow++)
	{
		l_RowsPtr[l_IdRow] = m_Data + ((m_Height - (l_IdRow + 1)) * m_Width * 4U);
	}

	// Read image
	png_read_image(m_PngPtr, l_RowsPtr);
	free(l_RowsPtr);
}
