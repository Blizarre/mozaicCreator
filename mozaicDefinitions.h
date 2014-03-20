#pragma once

#include <memory>
#include <map>
#include <vector>
#include <string>
#include "CImg.h"

typedef cimg_library::CImg<unsigned char> CharImage;
typedef std::shared_ptr<CharImage> CharImageSPtr;
typedef CharImage* CharImagePtr;


typedef cimg_library::CImg<float> Image;
typedef std::shared_ptr<Image> ImageSPtr;
typedef Image* ImagePtr;


struct Thumbnail
{
	Thumbnail(CharImageSPtr _cim, ImageSPtr _fim) : fim(_fim), cim(_cim) { }
	ImageSPtr fim;
	CharImageSPtr cim;
	std::vector<std::pair<int, int>> pos;
};

typedef std::vector<Thumbnail> ListOfImages;
typedef std::shared_ptr<ListOfImages> ListOfImagesSPtr;
