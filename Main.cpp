#include <string>
#include <vector>
#include "mozaicDefinitions.h"
#include "Configurator.h"
#include "CImg.h"


std::vector<std::wstring> FindAllFiles(std::wstring directory)
{
	std::vector<std::wstring> listOfFiles;
	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile( (directory + L"\\*") .c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return;

	do {
		const std::wstring file_name = file_data.cFileName;
		const std::wstring full_file_name = directory + L"\\" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (is_directory)
			continue;

		listOfFiles.push_back(full_file_name);
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);

	return listOfFiles;
}

ListOfImagesPtr loadAllImages(std::vector<std::wstring> listOfFiles)
{
	ListOfImagesPtr lIm = new ListOfImages();
	cimg_library::CImg<unsigned char> * image;
	std::vector<std::wstring>::iterator fIt;
	for (fIt = listOfFiles.begin(); fIt != listOfFiles.end(); fIt++)
	{
		image = new cimg_library::CImg<unsigned char>((*fIt).c_str());
		lIm->push_back(image);
	}
}

int main(int argc, char* argv[])
{
	Configurator config;
	MatchingAlgorithmSPrt algo = config.getMatchingAlgorithm();

	std::wstring sourceDir = config[ConfigKey::SOURCE_DIRECTORY];
	ListOfImagesPtr lIm = loadAllImages(FindAllFiles(sourceDir.c_str()));

	algo->Initialize();
	return 0;
}