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

	std::cout << "Initialization ... "; std::cout.flush();
	bm.start();
	algo->Initialize(lIm);
	std::cout << "done in " << bm.stopString() << std::endl;

	std::cout << "Reference image loading ... "; std::cout.flush();
	bm.start();
	ImageSPtr ref = fs.loadImage(config.getReferenceImage());
	std::cout << "done in " << bm.stopString() << std::endl;

	std::cout << "Finding best match ... "; std::cout.flush();
	bm.start();
	std::string bestMatchImName = algo->FindBestMatch(ref);
	std::cout << "done in " << bm.stopString() << std::endl;

	cimg_library::CImgDisplay refDisp(cimg_library::CImg<unsigned char>(config.getReferenceImage().c_str()), "Reference Image");
	cimg_library::CImgDisplay bestDisp(cimg_library::CImg<unsigned char>(bestMatchImName.c_str()), "Best Match Image");

	while (!refDisp.is_closed() && !bestDisp.is_closed()) {
		refDisp.wait();
	}
	
	return 0;
}