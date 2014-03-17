
#include <string>
#include <vector>
#include "mozaicDefinitions.h"
#include "Configurator.h"

#include <iostream>

#define cimg_use_png
#include "CImg.h"

std::vector<std::wstring> FindAllFiles(std::wstring directory)
{
	std::vector<std::wstring> listOfFiles;
	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile( (directory + L"\\*") .c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return std::vector<std::wstring>();

	do {
		const std::wstring file_name = file_data.cFileName;
		if (file_name.find(L".bmp", file_name.size() - 4) == std::string::npos) continue;
		const std::wstring full_file_name = directory + L"\\" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (is_directory)
			continue;

		listOfFiles.push_back(full_file_name);
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);

	return listOfFiles;
}

// Ugly : convert a wide string to a single char string, for compatibility with CImg
std::string wstring2string(std::wstring wstr)
{
	return std::string(wstr.begin(), wstr.end());
}

ListOfImagesPtr loadAllImages(std::vector<std::wstring> listOfFiles)
{
	ListOfImagesPtr lIm = new ListOfImages();
	ImagePtr image;
	std::vector<std::wstring>::iterator fIt;
	for (fIt = listOfFiles.begin(); fIt != listOfFiles.end(); fIt++)
	{
		std::string fileName = wstring2string(*fIt);
		image = new Image(fileName.c_str());
		lIm->insert(std::make_pair(*fIt, image));
	}
	return lIm;
}

int main(int argc, WCHAR* argv[])
{
	Configurator config;

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

	std::cout << "Loading images" << std::endl;

	ListOfImagesPtr lIm = loadAllImages(FindAllFiles(config.getDirectoryInputImages() ));

	std::cout << lIm->size() << " images loaded" << std::endl;

	algo->Initialize(lIm);

	std::cout << "Initialization done" << std::endl;
	
	std::string refImName = wstring2string(config.getReferenceImage());
	Image ref(refImName.c_str());

	std::cout << "Reference image loading done" << std::endl;

	std::wstring bestMatch = algo->FindBestMatch(&ref);
	std::string bestMatchImName = wstring2string(bestMatch);

	std::cout << "Best match found" << std::endl;

	cimg_library::CImgDisplay refDisp(cimg_library::CImg<unsigned char>(refImName.c_str()), "Reference Image");
	cimg_library::CImgDisplay bestDisp(cimg_library::CImg<unsigned char>(bestMatchImName.c_str()), "Best Match Image");

	while (!refDisp.is_closed() && !bestDisp.is_closed()) {
		refDisp.wait();
	}
	return 0;
}