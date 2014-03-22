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
	ImageSPtr fim; // Lab float image, the image on wich the matching will be make
	CharImageSPtr cim; // char RGB image, the image wich will be copied to the result image
	std::vector<std::pair<int, int>> pos; // all the position in the final image where the thumbnail has been used
};

typedef std::vector<Thumbnail> ListOfImages;
typedef std::shared_ptr<ListOfImages> ListOfImagesSPtr;
