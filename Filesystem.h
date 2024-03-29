#pragma once

#include <map>
#include <string>
#include "mozaicDefinitions.h"

/**
 * This class is a very quck hack to be able to read pictures on the disk
 * After re-reading it, I feel sick to have made something like this... The string / wstring conversion
 * is a nonsense. If one day I work on this project again, I'll have to redo it from scratch.'
 **/
class Filesystem
{
public:

	ImageSPtr loadImage(std::string strName)
	{
		return ImageSPtr(new Image(strName.c_str()));
	}

	CharImageSPtr loadCharImage(std::string strName)
	{
		return CharImageSPtr(new CharImage(strName.c_str()));
	}

	ListOfImagesSPtr loadImageDirectory(std::string strDir)
	{
		std::wstring wstrDir = string2wstring(strDir);
		return loadAllImages(FindAllFiles(wstrDir));
	}

protected:

	ListOfImagesSPtr loadAllImages(std::vector<std::wstring> listOfFiles)
	{
		ListOfImagesSPtr lIm(new ListOfImages());
		ImageSPtr image;
		CharImageSPtr charImage;
		std::vector<std::wstring>::iterator fIt;
		int sizeX = -1, sizeY = -1;

		for (fIt = listOfFiles.begin(); fIt != listOfFiles.end(); fIt++)
		{
			std::string fileName = wstring2string(*fIt);
			image = ImageSPtr( new Image(fileName.c_str()) );

			fileName = wstring2string(*fIt);
			charImage = CharImageSPtr( new CharImage(fileName.c_str()) );

			if (sizeX == -1) sizeX = charImage->width();
			if (sizeY == -1) sizeY = charImage->height();

			if (sizeX != charImage->width() || sizeY != charImage->height())
				throw std::string("Image " + fileName + " size is different from the others.");

			lIm->push_back(Thumbnail(charImage, image));
		}
		return lIm;
	}

	// Ugly : convert a wide string to a single char string, for compatibility with CImg
	std::string wstring2string(std::wstring wstr)
	{
		return std::string(wstr.begin(), wstr.end());
	}

	// Ugly : convert a single char string to a wide char string, for compatibility with CImg
	std::wstring string2wstring(std::string str)
	{
		return std::wstring(str.begin(), str.end());
	}


	std::vector<std::wstring> FindAllFiles(std::wstring directory)
	{
		std::vector<std::wstring> listOfFiles;
		HANDLE dir;
		WIN32_FIND_DATA file_data;

		if ((dir = FindFirstFile((directory + L"\\*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
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
};

