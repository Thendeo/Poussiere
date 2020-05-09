/*----------------------------------------------------
**
**		Date: 2019
**		Author: Thendeo
**		Project: Particules
**		File: Image_8 implementation file
**
----------------------------------------------------*/

#include "Image_8.h"

#include <stdio.h>
#include <stdlib.h>

#define PNG_DEBUG 3
#include "png.h"

#include "AssertHdl.h"

Image_8::Image_8()
	: Image(8)
	, m_Data(NULL)
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

	FILE* l_PngFile = NULL;
	l_PngFile = fopen(p_Path, "rb");
	doAssert(NULL != l_PngFile);

	// Check if PNG
	unsigned char l_Header[8];
	size_t l_Ret = fread(&l_Header, 1, 8, l_PngFile);
	doAssert(8 == l_Ret);

	l_Ret = png_sig_cmp(l_Header, 0, 8);

	// Read structure
	png_structp l_PngPtr = NULL;
	l_PngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	doAssert(NULL != l_PngPtr);

	png_infop l_InfoPtr = NULL;
	l_InfoPtr = png_create_info_struct(l_PngPtr);
	doAssert(NULL != l_PngPtr);

	// Error jump context
	l_Ret = setjmp(png_jmpbuf(l_PngPtr));
	doAssert(0 == l_Ret);

	png_init_io(l_PngPtr, l_PngFile);
	png_set_sig_bytes(l_PngPtr, 8);

	png_read_info(l_PngPtr, l_InfoPtr);

	m_Width = png_get_image_width(l_PngPtr, l_InfoPtr);
	m_Height = png_get_image_height(l_PngPtr, l_InfoPtr);

	unsigned char l_ByteDepth = png_get_bit_depth(l_PngPtr, l_InfoPtr);
	doAssert(8 == l_ByteDepth);

	unsigned char l_ImgType = png_get_color_type(l_PngPtr, l_InfoPtr);
	unsigned char l_ImgComposanteSize = 0;
	switch (l_ImgType)
	{
	case PNG_COLOR_TYPE_GRAY:
		m_ImageType = ImageType::ImageType_Gray;
		l_ImgComposanteSize = 1;
		break;

	case PNG_COLOR_TYPE_PALETTE :
		m_ImageType = ImageType::ImageType_Palette;
		doAssert(false); // Don't know yet to handle palette size
		break;

	case PNG_COLOR_TYPE_RGB :
		m_ImageType = ImageType::ImageType_RGB;
		l_ImgComposanteSize = 3;
		break;

	case PNG_COLOR_TYPE_RGBA :
		m_ImageType = ImageType::ImageType_RGBA;
		l_ImgComposanteSize = 4;
		break;

	case PNG_COLOR_TYPE_GA :
		m_ImageType = ImageType::ImageType_GrayA;
		l_ImgComposanteSize = 2;
		break;
	default:
		doAssert(false);
		break;
	}
	m_ImageSize = m_Width * m_Height * l_ImgComposanteSize;

	int l_NbPass = png_set_interlace_handling(l_PngPtr); // Number of interlacing
	png_read_update_info(l_PngPtr, l_InfoPtr); //  Update struct info for debug

	// Allocating image buffer + 2D buffer for png parsing
	m_Data = (unsigned char*)malloc(m_ImageSize);

	png_bytep* l_RowsPtr = NULL;
	l_RowsPtr = (png_bytep*)malloc(sizeof(png_bytep) * m_Height);
	doAssert(NULL != l_RowsPtr);

	// Mapping image buffer addresses with rows from png
	for (unsigned int l_IdRow = 0; l_IdRow < m_Height; l_IdRow++)
	{
		l_RowsPtr[l_IdRow] = m_Data + ((m_Height - (l_IdRow + 1)) * m_Width * 4U);
	}

	// Read image
	png_read_image(l_PngPtr, l_RowsPtr);
	
	// Free data
	free(l_RowsPtr);
	png_destroy_read_struct(&l_PngPtr, &l_InfoPtr, NULL);
	fclose(l_PngFile);
}

unsigned char* Image_8::getData()
{
	doAssert(NULL != m_Data);
	return m_Data;
}
