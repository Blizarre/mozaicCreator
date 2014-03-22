/*
// detect memory leak
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
// end
*/

#include <string>
#include <vector>
#include "mozaicDefinitions.h"
#include "Configurator.h"
#include "Filesystem.h"

#include <iostream>
#include <string>
#include "CImg.h"
#include "Benchmarking.h"

// Show an images. Useful for debugging
template<class T>
void debug_showImage(T im, bool normalization = false, const char* caption = "")
{
	int norm = 3;
	if (normalization)
		norm = 1;
	cimg_library::CImgDisplay disp(im, caption, normalization);

	while (!disp.is_closed()) {
		disp.wait();
	}
};


// Split images into smaller thumbnails, if needed. This is useful if you have a relatively small et of images
// This the Split parameter
ListOfImagesSPtr splitImages(ListOfImagesSPtr originales, unsigned int split)
{
	ListOfImages::iterator im;

	ImageSPtr tmp;
	CharImageSPtr cTmp;

	int patch_width =  originales->begin()->cim->width() / 2;
	int patch_height = originales->begin()->cim->height() / 2;

	ListOfImagesSPtr splitted(new ListOfImages());

	for (im = originales->begin(); im != originales->end(); im++)
	{
		if (split == 0)
		{
			splitted->push_back(*im);
		}

		if (split > 0)
		{

			// upper left
			tmp = ImageSPtr(new Image(im->fim->get_crop(0, 0, patch_width - 1, patch_height - 1)));
			cTmp = CharImageSPtr(new CharImage(im->cim->get_crop(0, 0, patch_width -1, patch_height -1)));
			splitted->push_back(Thumbnail(cTmp, tmp));

			// upper right
			tmp = ImageSPtr(new Image(im->fim->get_crop(patch_width, 0, 2 * patch_width - 1, patch_height - 1)));
			cTmp = CharImageSPtr(new CharImage(im->cim->get_crop(patch_width, 0, 2 * patch_width - 1, patch_height - 1)));
			splitted->push_back(Thumbnail(cTmp, tmp));

			// lower left
			tmp = ImageSPtr(new Image(im->fim->get_crop(0, patch_height, patch_width - 1, 2 * patch_height - 1)));
			cTmp = CharImageSPtr(new CharImage(im->cim->get_crop(0, patch_height, patch_width - 1, 2 * patch_height - 1)));
			splitted->push_back(Thumbnail(cTmp, tmp));

			// lower right
			tmp = ImageSPtr(new Image(im->fim->get_crop(patch_width, patch_height, 2 * patch_width - 1, 2 * patch_height - 1)));
			cTmp = CharImageSPtr(new CharImage(im->cim->get_crop(patch_width, patch_height, 2 * patch_width - 1, 2 * patch_height - 1)));
			splitted->push_back(Thumbnail(cTmp, tmp));

		}

		if (split > 1)
		{
			// center
			tmp = ImageSPtr(new Image(im->fim->get_crop(patch_width / 2, patch_height / 2, patch_width + patch_width / 2 -1, patch_height + patch_height / 2 -1)));
			cTmp = CharImageSPtr(new CharImage(im->cim->get_crop(patch_width / 2, patch_height / 2, patch_width + patch_width / 2 - 1, patch_height + patch_height / 2 - 1)));
			splitted->push_back(Thumbnail(cTmp, tmp));
		}

		if (split > 2)
		{
			// all
			tmp = ImageSPtr(new Image(im->fim->resize(patch_width, patch_height)));
			cTmp = CharImageSPtr(new CharImage(im->cim->resize(patch_width, patch_height)));
			splitted->push_back(Thumbnail(cTmp, tmp));
		}
	}

	return splitted;
}

// Create the mask used to make a smooth transition between images
Image createMask(int width, int height, int fading)
{
	CharImage mask(width, height, 1, 1, fading);

	char val = 0;
	for (int i = 0; i < fading; i++)
	{
		for (int j = val; j < height - val; j++)
			mask.atXY(i, j) = val;
		val++;
	}

	val = fading;
	for (int i = width - fading; i < width; i++)
	{
		for (int j = val; j < height - val; j++)
			mask.atXY(i, j) = val;
		val--;
	}

	val = 0;
	for (int j = 0; j < fading; j++)
	{
		for (int i = val; i < width - val; i++)
			mask.atXY(i, j) = val;
		val++;
	}

	val = fading;
	for (int j = height - fading; j < height; j++)
	{
		for (int i = val - 1; i < width - val; i++)
			mask.atXY(i, j) = val;
		val--;
	}

	return mask;
}



void cropAndResize(ListOfImagesSPtr lIm, unsigned int crop, unsigned int reduction)
{
	int newWidth = lIm->begin()->cim->width() / reduction;
	int newHeight = lIm->begin()->cim->height() / reduction;

	ListOfImages::iterator it;

	// We will make several halving, ans we need to make sure tha the size is a multiple of 4
	newWidth = 4 * ceil(newWidth / 4.0f);
	newHeight = 4 * ceil(newHeight / 4.0f);

	for (it = lIm->begin(); it != lIm->end(); it++)
	{

		if (crop != 0)
		{
			it->cim->crop(crop, crop, it->cim->width() - crop, it->cim->height() - crop);
			it->fim->crop(crop, crop, it->fim->width() - crop, it->fim->height() - crop);
		}

		it->cim->resize(newWidth, newHeight);
		it->fim->resize(newWidth, newHeight);
	}
}

/* This is the main function : It takes a reference image, fading is the transition distance between patches (or thumbnails), and minDistance is the minimal distance between two
 same thumbnails in the final image to avoid repetition.
 It will try to match any (patchWidth, patchHeight) part of the image with a thumbnail
*/ 
CharImageSPtr fillImage(ImageSPtr ref, MatchingAlgorithmSPrt algo, Filesystem fs, int patchWidth, int patchHeight, unsigned int fading, float minDistance)
{
	CharImageSPtr result(new CharImage());
	
	int thumbWidth = patchWidth - 2 * fading;
	int thumbHeight = patchHeight - 2 * fading;

	int nbThumbWidth = ref->width() / thumbWidth;
	int nbThumbHeight = ref->height() / thumbHeight;

	ref->crop(0, 0, thumbWidth * nbThumbWidth, thumbHeight * nbThumbHeight);

	CharImage mask;
	if (fading > 0)
		mask = createMask(patchWidth, patchHeight, fading);

	result->assign(*ref);

	CharImageSPtr bestMatchIm;

	std::cout << nbThumbWidth << " " << nbThumbHeight << std::endl;

	Image extract;

	for (int j = 0; j < nbThumbHeight; j++)
	{
		for (int i = 0; i < nbThumbWidth; i++)
		{
			// Show progress
			if ((nbThumbWidth * j + i) % 100 == 0) std::cout << i << "," << j << " : " << 100.0 * (nbThumbWidth * (float)j + (float)i) / ((float)nbThumbWidth * nbThumbHeight) << "%" << std::endl;

			extract = ref->get_crop(i * thumbWidth, j*thumbHeight, (i + 1) * thumbWidth + 2 * fading - 1, (j + 1)*thumbHeight + 2 * fading - 1);
			bestMatchIm = algo->FindBestMatch(extract, i, j, minDistance);
			
			if (fading != 0)
				result->draw_image(i * thumbWidth, j*thumbHeight, *bestMatchIm, mask, 1.0F, (float)fading);
			else
				result->draw_image(i * thumbWidth, j*thumbHeight, *bestMatchIm);
		}
	}
	
	return result;
}

int main(int argc, char* argv[])
{
	Configurator config;
	Filesystem fs;
	Benchmarking bm;

	std::cout << "Starting" << std::endl;

	try {
		if (argc != 1)
			config.fromCommandLine(argc, argv);
	}
	catch (std::string err)
	{
		std::cout << "Error during arguments parsing :" << err << std::endl;
		return -1;
	}

	MatchingAlgorithmSPrt algo = config.getMatchingAlgorithm();

	ListOfImagesSPtr lThumbnail;

	int patchWidth;
	int patchHeight;
	{
		// Deletion of lIm at the end of the block
		bm.start("Loading images              ");
		ListOfImagesSPtr lIm = fs.loadImageDirectory(config.getDirectoryInputImages());
		bm.stopString(lIm->size());

		bm.start("Cropping and resizing images");
		cropAndResize(lIm, config.getCrop(), config.getReductionThumbnail());
		bm.stopString();

		bm.start("Splitting images            ");
		ListOfImagesSPtr lThumbnail = splitImages(lIm, config.getSplit());
		bm.stopString(lThumbnail->size());

		//crappy
		patchWidth = lThumbnail->cbegin()->cim->width();
		patchHeight = lThumbnail->cbegin()->cim->height();

		bm.start("Initializating database     ");
		algo->Initialize(lThumbnail);
		bm.stopString();
	}
	         
	bm.start("Reference image loading     ");
	ImageSPtr ref = fs.loadImage(config.getReferenceImage());
	bm.stopString();

	         
	bm.start("Filling reference image     ");
	CharImageSPtr result = fillImage(ref, algo, fs, patchWidth, patchHeight, config.getFading(), config.getMindistance());
	bm.stopString();

	result->save("final.bmp");

	cimg_library::CImgDisplay refDisp(*fs.loadCharImage(config.getReferenceImage()), "Original Image");
	cimg_library::CImgDisplay bestDisp(*result, "Composite image");

	while (!refDisp.is_closed() && !bestDisp.is_closed()) {
		refDisp.wait();
	}

	return 0;
}