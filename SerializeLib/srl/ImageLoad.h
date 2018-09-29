#pragma once

#include <string>
#include <FreeImage.h>

namespace SRL {

	static BYTE* load_image(const char* filename, int* width, int* height)
	{
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP* dib = nullptr;
		fif = FreeImage_GetFileType(filename, 0);
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filename);
		if (fif == FIF_UNKNOWN)
			return nullptr;

		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename);
		if (!dib)
			return nullptr;

		BYTE* pixels = FreeImage_GetBits(dib);
		*width = FreeImage_GetWidth(dib);
		*height = FreeImage_GetHeight(dib);
		int bits = FreeImage_GetBPP(dib);

		int size = *width * *height * (bits / 8);
		BYTE* result = new BYTE[size];
		memcpy(result, pixels, size);
		FreeImage_Unload(dib);

		return result;
	}

	static void save_image(BYTE* pic, const char* filename, int width, int height)
	{
		//int pitch = ((((32 * width) + 31) / 32) * 4);
		int pitch = width * 4;
		FIBITMAP * dib = FreeImage_ConvertFromRawBitsEx(true, pic, FIT_BITMAP, width, height,pitch , 32, 0x00ff0000, 0x0000ff00, 0x000000FF);
		//FIBITMAP * dib = FreeImage_ConvertFromRawBitsEx(true, pic, FIT_BITMAP, width, height, pitch , 32, 0x000000ff, 0x0000ff00, 0x00ff0000);
		FreeImage_Save(FIF_PNG, dib , filename, PNG_Z_NO_COMPRESSION);
	}

}
