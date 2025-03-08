#define XMD_H

#include "PS_ImageLib.h"

using namespace ImageLib;

Image::Image()
{
	mWidth = 0;
	mHeight = 0;
	mBits = NULL;
}

Image::~Image()
{
	delete mBits;
}

int	Image::GetWidth()
{
	return mWidth;
}

int	Image::GetHeight()
{
	return mHeight;
}

unsigned long* Image::GetBits()
{
	return mBits;
}


void ImageLib::InitJPEG2000() {
	ps::Image_Init();
}

void ImageLib::CloseJPEG2000() {
	ps::Image_Close();
}

Image* GetPNGImage(const std::string& theFileName)
{
	return ps::Image_GetPng(theFileName);
}

Image* GetTGAImage(const std::string& theFileName)
{
	return NULL;
}

Image* GetGIFImage(const std::string& theFileName)
{
	return ps::Image_GetGif(theFileName);
}

bool ImageLib::WriteJPEGImage(const std::string& theFileName, Image* theImage)
{
	return false;
}

bool ImageLib::WritePNGImage(const std::string& theFileName, Image* theImage)
{
	return false;
}

bool ImageLib::WriteTGAImage(const std::string& theFileName, Image* theImage)
{
	return false;
}

bool ImageLib::WriteBMPImage(const std::string& theFileName, Image* theImage)
{
	return false;
}

Image* GetJPEGImage(const std::string& theFileName)
{
	return ps::Image_GetJpeg(theFileName);
}


int ImageLib::gAlphaComposeColor = 0xFFFFFF;
bool ImageLib::gAutoLoadAlpha = true;
bool ImageLib::gIgnoreJPEG2000Alpha = true;

Image* ImageLib::GetImage(const std::string& theFilename, bool lookForAlphaImage)
{
	if (!gAutoLoadAlpha)
		lookForAlphaImage = false;

	int aLastDotPos = (int)theFilename.rfind('.');
	int aLastSlashPos = max((int)theFilename.rfind('\\'), (int)theFilename.rfind('/'));

	std::string anExt;
	std::string aFilename;

	if (aLastDotPos > aLastSlashPos)
	{
		anExt = theFilename.substr(aLastDotPos, theFilename.length() - aLastDotPos);
		aFilename = theFilename.substr(0, aLastDotPos);
	}
	else
		aFilename = theFilename;

	Image* anImage = NULL;

	// if ((anImage == NULL) && ((stricmp(anExt.c_str(), ".tga") == 0) || (anExt.length() == 0)))
	//	 anImage = GetTGAImage(aFilename + ".tga");

	if ((anImage == NULL) && ((stricmp(anExt.c_str(), ".jpg") == 0) || (anExt.length() == 0)))
		anImage = GetJPEGImage(aFilename + ".jpg");

	if ((anImage == NULL) && ((stricmp(anExt.c_str(), ".png") == 0) || (anExt.length() == 0)))
		anImage = GetPNGImage(aFilename + ".png");

	if ((anImage == NULL) && ((stricmp(anExt.c_str(), ".gif") == 0) || (anExt.length() == 0)))
		anImage = GetGIFImage(aFilename + ".gif");


	// Check for alpha images
	Image* anAlphaImage = NULL;
	if (lookForAlphaImage)
	{
		// Check _ImageName
		anAlphaImage = GetImage(theFilename.substr(0, aLastSlashPos + 1) + "_" +
			theFilename.substr(aLastSlashPos + 1, theFilename.length() - aLastSlashPos - 1), false);

		// Check ImageName_
		if (anAlphaImage == NULL)
			anAlphaImage = GetImage(theFilename + "_", false);
	}



	// Compose alpha channel with image
	if (anAlphaImage != NULL)
	{
		if (anImage != NULL)
		{
			if ((anImage->mWidth == anAlphaImage->mWidth) &&
				(anImage->mHeight == anAlphaImage->mHeight))
			{
				unsigned long* aBits1 = anImage->mBits;
				unsigned long* aBits2 = anAlphaImage->mBits;
				int aSize = anImage->mWidth * anImage->mHeight;

				for (int i = 0; i < aSize; i++)
				{
					*aBits1 = (*aBits1 & 0x00FFFFFF) | ((*aBits2 & 0xFF) << 24);
					++aBits1;
					++aBits2;
				}
			}

			delete anAlphaImage;
		}
		else if (gAlphaComposeColor == 0xFFFFFF)
		{
			anImage = anAlphaImage;

			unsigned long* aBits1 = anImage->mBits;

			int aSize = anImage->mWidth * anImage->mHeight;
			for (int i = 0; i < aSize; i++)
			{
				*aBits1 = (0x00FFFFFF) | ((*aBits1 & 0xFF) << 24);
				++aBits1;
			}
		}
		else
		{
			const int aColor = gAlphaComposeColor;
			anImage = anAlphaImage;

			unsigned long* aBits1 = anImage->mBits;

			int aSize = anImage->mWidth * anImage->mHeight;
			for (int i = 0; i < aSize; i++)
			{
				*aBits1 = aColor | ((*aBits1 & 0xFF) << 24);
				++aBits1;
			}
		}
	}

	return anImage;
}
