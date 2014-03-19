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


CharImageSPtr fillImage(ImageSPtr ref, ListOfImagesSPtr lIm, MatchingAlgorithmSPrt algo, Filesystem fs, int patchWidth, int patchHeight)
{
	CharImageSPtr result(new CharImage());
	int nbPatchWidth = ref->width() / patchWidth;
	int nbPatchHeight = ref->height() / patchHeight;

	ref->crop(0, 0, patchWidth * nbPatchWidth, patchHeight * nbPatchHeight); 
	
	result->assign(*ref);

	ImageSPtr extract(new Image());

	for (int i = 0; i < nbPatchWidth; i++)
	{
		for (int j = 0; j < nbPatchHeight; j++)
		{
			*extract = ref->get_crop(i * patchWidth, j*patchHeight, (i + 1) * patchWidth, (j + 1)*patchHeight);
			std::string bestMatchImName = algo->FindBestMatch(extract);
			result->draw_image(i * patchWidth, j * patchHeight, *fs.loadCharImage(bestMatchImName));
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

	std::cout << "Loading images ... "; std::cout.flush(); 
	bm.start();
	ListOfImagesSPtr lIm = fs.loadImageDirectory(config.getDirectoryInputImages());
	std::cout << lIm->size() << " done in " << bm.stopString() << std::endl;

	//crappy
	int patchWidth = lIm->cbegin()->second->width();
	int patchHeight = lIm->cbegin()->second->height();

	std::cout << "Initialization ... "; std::cout.flush();
	bm.start();
	algo->Initialize(lIm);
	std::cout << "done in " << bm.stopString() << std::endl;

	std::cout << "Reference image loading ... "; std::cout.flush();
	bm.start();
	ImageSPtr ref = fs.loadImage(config.getReferenceImage());

	std::cout << "done in " << bm.stopString() << std::endl;

	std::cout << "Filling reference image ... "; std::cout.flush();
	bm.start();
	CharImageSPtr result = fillImage(ref, lIm, algo, fs, patchWidth, patchHeight);
	std::cout << "done in " << bm.stopString() << std::endl;

	result->save("final.bmp");

	cimg_library::CImgDisplay refDisp( *fs.loadCharImage(config.getReferenceImage()) , "Original Image");
	cimg_library::CImgDisplay bestDisp(*result, "Composite image");

	while (!refDisp.is_closed() && !bestDisp.is_closed()) {
		refDisp.wait();
	}
	
	return 0;
}