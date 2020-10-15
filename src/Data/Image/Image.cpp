#include "Image.h"

template <>
void Image<UByte>::readData()
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

template <>
void Image<UWord>::readData()
{
	m_Data = new UWord[m_ImageSize];

	size_t l_ScanLineSize = TIFFScanlineSize(m_TiffPtr);
	tdata_t l_RowBuffer = _TIFFmalloc(l_ScanLineSize);
	UWord* l_RowBufferParser = (UWord*)l_RowBuffer;

	// TIFF loads raw bytes as char (8bits), since we use 16bits samples we prepare parsing to short (16bits)
	// This is done by dividing row size by 2 (16/8)
	l_ScanLineSize /= 2;
	for (unsigned int i_Row = 0; i_Row < m_Height; i_Row++)
	{
		TIFFReadScanline(m_TiffPtr, l_RowBuffer, i_Row);

		for (unsigned int l_Col = 0; l_Col < l_ScanLineSize; l_Col++)
		{
			//unsigned char* l_Ptr = (unsigned char*)&l_RowBuffer[l_Col * 2];
			// Accesing rowBuffer -> l_col * 2 because each sample is 16bits <-> 2Bytes
			m_Data[i_Row * l_ScanLineSize + l_Col] = l_RowBufferParser[l_Col];
		}
	}
}
