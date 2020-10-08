/*----------------------------------------------------
**
**		Date: 2020
**		Author: Thendeo
**		Project: Particules
**		File: ImageLoader header file
**
----------------------------------------------------*/
#pragma once

#include "Image.h"
#include "ImageType.h"

//! @brief Virtual interface for image loading
class ImageLoader
{
public:

	//! @brief Loads an image from file path
	virtual void load(I_Image* p_TargetImage, const char* p_ImagePath);

private:
	/* PNG Functions */

	//! @brief Open a PNG file and verify header informations
	//! @param p_Path The path to the file
	void loadPNG();

	//! @brief Recover informations from the file (width, height, etc.)
	void loadInformations(I_Image* p_TargetImage);

	//! @brief Loads internal file data inside buffer member
	void readData(UByte* p_DataPtr, I_Image* p_TargetImage);

	//! @brief PNG structural ptr from PNGLib
	png_structp m_PngPtr;

	//! @brief PNG information struct from PNGLib
	png_infop m_InfoPtr;

	//! @brief File parser ptr
	FILE* m_File;
};