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


ListOfImagesSPtr splitImages(ListOfImagesSPtr originales)
{
	ListOfImages::iterator im;

	ImageSPtr tmp;
	CharImageSPtr cTmp;

	int patch_width =  originales->begin()->first->width() / 2;
	int patch_height = originales->begin()->first->height() / 2;

	ListOfImagesSPtr splitted(new ListOfImages());

	for (im = originales->begin(); im != originales->end(); im++)
	{
		// upper left
		tmp = ImageSPtr(		new Image(    im->second->get_crop(0, 0, patch_width, patch_height)));
		cTmp = CharImageSPtr(	new CharImage(im->first->get_crop( 0, 0, patch_width, patch_height)));
		splitted->push_back(std::make_pair(cTmp, tmp));

		// upper right
		tmp = ImageSPtr(		new Image(    im->second->get_crop(patch_width, 0, 2 * patch_width, patch_height)));
		cTmp = CharImageSPtr(	new CharImage(im->first->get_crop( patch_width, 0, 2 * patch_width, patch_height)));
		splitted->push_back(std::make_pair(cTmp, tmp));

		// lower left
		tmp = ImageSPtr(		new Image(    im->second->get_crop(0, patch_height, patch_width, 2 * patch_height)));
		cTmp = CharImageSPtr(	new CharImage(im->first->get_crop( 0, patch_height, patch_width, 2 * patch_height)));
		splitted->push_back(std::make_pair(cTmp, tmp));

		// lower right
		tmp = ImageSPtr(		new Image(    im->second->get_crop(patch_width, patch_height, 2 * patch_width, 2 * patch_height)));
		cTmp = CharImageSPtr(	new CharImage(im->first->get_crop( patch_width, patch_height, 2 * patch_width, 2 * patch_height)));
		splitted->push_back(std::make_pair(cTmp, tmp));
	}

	return splitted;
}

CharImageSPtr fillImage(ImageSPtr ref, MatchingAlgorithmSPrt algo, Filesystem fs, int patchWidth, int patchHeight)
{
	CharImageSPtr result(new CharImage());

	int nbPatchWidth = ref->width() / patchWidth;
	int nbPatchHeight = ref->height() / patchHeight;

	ref->crop(0, 0, patchWidth * nbPatchWidth, patchHeight * nbPatchHeight);

	result->assign(*ref);

	ImageSPtr extract(new Image());

	CharImageSPtr bestMatchIm;

	std::cout << nbPatchWidth << " " << nbPatchHeight << std::endl;
	for (int j = 0; j < nbPatchHeight; j++)
	{
		for (int i = 0; i < nbPatchWidth; i++)
		{
			if ((nbPatchWidth * j + i) % 100 == 0) std::cout << i << "," << j << " : " << 100.0 * (nbPatchWidth * (float)j + (float)i) / ((float)nbPatchWidth * nbPatchHeight) << "%" << std::endl;
			*extract = ref->get_crop(i * patchWidth, j*patchHeight, (i + 1) * patchWidth, (j + 1)*patchHeight);
			bestMatchIm = algo->FindBestMatch(extract);
			result->draw_image(i * patchWidth, j * patchHeight, *bestMatchIm);
		}
	}

	return result;
}

int main(int argc, char* argv[])
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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
		std::cout << "Loading images ... "; std::cout.flush();
		bm.start();
		ListOfImagesSPtr lIm = fs.loadImageDirectory(config.getDirectoryInputImages());
		std::cout << lIm->size() << " done in " << bm.stopString() << std::endl;

		ListOfImages::iterator it;
		for (it = lIm->begin(); it != lIm->end(); it++)
		{

			if (config.getCrop() != 0)
			{
				int c = config.getCrop();
				it->first->crop( c, c, it->first->width() - c, it->first->height() - c);
				it->second->crop(c, c, it->second->width() - c, it->second->height() - c);
			}
			
			/*it->first->resize_halfXY();
			it->second->resize_halfXY();*/

			it->first->resize_halfXY();
			it->second->resize_halfXY();
		}

		std::cout << "Splitting images ... "; std::cout.flush();
		bm.start();
		ListOfImagesSPtr lThumbnail = splitImages(lIm);
		std::cout << lThumbnail->size() << " done in " << bm.stopString() << std::endl;

		//crappy
		patchWidth = lThumbnail->cbegin()->first->width();
		patchHeight = lThumbnail->cbegin()->first->height();

		std::cout << "Initialization ... "; std::cout.flush();
		bm.start();
		algo->Initialize(lThumbnail);
		std::cout << " done in " << bm.stopString() << std::endl;

	}

	std::cout << "Reference image loading ... "; std::cout.flush();
	bm.start();
	ImageSPtr ref = fs.loadImage(config.getReferenceImage());

	std::cout << "done in " << bm.stopString() << std::endl;

	std::cout << "Filling reference image ... "; std::cout.flush();
	bm.start();

	// Add threading (Process.h) on fillImage
	CharImageSPtr result = fillImage(ref, algo, fs, patchWidth, patchHeight);
	std::cout << "done in " << bm.stopString() << std::endl;

	result->save("final.bmp");

	cimg_library::CImgDisplay refDisp(*fs.loadCharImage(config.getReferenceImage()), "Original Image");
	cimg_library::CImgDisplay bestDisp(*result, "Composite image");

	while (!refDisp.is_closed() && !bestDisp.is_closed()) {
		refDisp.wait();
	}

	return 0;
}