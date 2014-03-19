#pragma once

#include <memory>
#include <map>
#include <vector>
#include <string>
#include "CImg.h"

typedef cimg_library::CImg<unsigned char> CharImage;
typedef std::shared_ptr<CharImage> CharImageSPtr;
typedef CharImage* CharImagePtr;


typedef cimg_library::CImg<double> Image;
typedef std::shared_ptr<Image> ImageSPtr;
typedef Image* ImagePtr;

typedef std::vector<std::pair<CharImageSPtr, ImageSPtr>> ListOfImages;
typedef std::shared_ptr<ListOfImages> ListOfImagesSPtr;

