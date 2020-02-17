/*----------------------------------------------------
**
**		Date: 2019
**		Author: Alexis LAFONT
**		Project: Particules
**		File: Image_8 implementation file
**
----------------------------------------------------*/

#include "Image_8.h"

#include <stdio.h>
#include <stdlib.h>

#include "AssertHdl.h"

Image_8::Image_8()
	: m_Width(0)
	, m_Height(0)
	, m_ImageSize(0)
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

	if (0 == m_ImageSize)
	{
		m_ImageSize = m_Width * m_Height * 3; // RGB
	}
	doAssert(0 != m_ImageSize); // Empty image
	doAssert(0 != l_DataPos); // No data pos

	m_Data = (unsigned char*)malloc(m_ImageSize);
	l_Readed = fread(m_Data, 1, m_ImageSize, l_File);
	//doAssert(m_ImageSize == (l_Readed * 3));

	fclose(l_File);
}

int Image_8::getWidth()
{
	return m_Width;
}

int Image_8::getHeight()
{
	return m_Height;
}

size_t Image_8::getType()
{
	return 8U;
}

unsigned char* Image_8::getData()
{
	doAssert(NULL != m_Data);
	return m_Data;
}
