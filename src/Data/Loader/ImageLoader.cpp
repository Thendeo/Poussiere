/*----------------------------------------------------
**
**		Date: 2020
**		Author: Thendeo
**		Project: Particules
**		File: ImageLoader implementation file
**
----------------------------------------------------*/

#pragma once

#include "Image.h"
#include "ImageLoader.h"

#include <string>

void ImageLoader::load(I_Image* p_TargetImage, const char* p_ImagePath)
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
		Image<UByte>* l_Image = dynamic_cast<Image<UByte>*>(p_TargetImage);

		loadPNG();
		loadInformations(p_TargetImage);

		l_Image->allocate(p_TargetImage->getWidth() * p_TargetImage->getHeight() * p_TargetImage->getPixelSize());
		readData(l_Image->getData(), p_TargetImage);

		// Free data
		png_destroy_read_struct(&m_PngPtr, &m_InfoPtr, NULL);
		fclose(m_File);
	}
}

void ImageLoader::loadPNG()
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

void ImageLoader::loadInformations(I_Image* p_TargetImage)
{
	unsigned int l_Width = 0U;
	unsigned int l_Height = 0U;
	l_Width = png_get_image_width(m_PngPtr, m_InfoPtr);
	l_Height = png_get_image_height(m_PngPtr, m_InfoPtr);

	// Allowing only 8 or 16 png images, 16bit images will be stripped to 8bit
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

	ImageType l_ImageType = ImageType::ImageType_MAX;
	char l_PixelSize = 0;

	switch (l_ImgType)
	{
	case PNG_COLOR_TYPE_GRAY:
		l_ImageType = ImageType::ImageType_Gray;
		l_PixelSize = 1;
		break;

	case PNG_COLOR_TYPE_PALETTE:
		l_ImageType = ImageType::ImageType_Palette;
		doAssert(false); // Don't know yet to handle palette size
		break;

	case PNG_COLOR_TYPE_RGB:
		l_ImageType = ImageType::ImageType_RGB;
		l_PixelSize = 3;
		break;

	case PNG_COLOR_TYPE_RGBA:
		l_ImageType = ImageType::ImageType_RGBA;
		l_PixelSize = 4;
		break;

	case PNG_COLOR_TYPE_GA:
		l_ImageType = ImageType::ImageType_GrayA;
		l_PixelSize = 2;
		break;
	default:
		doAssert(false);
		break;
	}

	p_TargetImage->setHeight(l_Height);
	p_TargetImage->setWidth(l_Width);
	p_TargetImage->setImgType(l_ImageType);
	p_TargetImage->setPixelSize(l_PixelSize);

}

void ImageLoader::readData(UByte* p_DataPtr, I_Image* p_TargetImage)
{
	int l_NbPass = png_set_interlace_handling(m_PngPtr); // Number of interlacing
	png_read_update_info(m_PngPtr, m_InfoPtr); //  Update struct info for debug

	png_bytep* l_RowsPtr = NULL;
	l_RowsPtr = new png_bytep[sizeof(png_bytep) * p_TargetImage->getHeight()];
	doAssert(NULL != l_RowsPtr);

	// Mapping image buffer addresses with rows from png
	for (unsigned int l_IdRow = 0; l_IdRow < p_TargetImage->getHeight(); l_IdRow++)
	{
		l_RowsPtr[l_IdRow] = p_DataPtr + ((p_TargetImage->getHeight() - (l_IdRow + 1)) * p_TargetImage->getWidth() * 4U);
	}

	// Read image
	png_read_image(m_PngPtr, l_RowsPtr);
	delete(l_RowsPtr);
}
