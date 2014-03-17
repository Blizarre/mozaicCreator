#pragma once

#include "vector"
#include "CImg.h"


typedef cimg_library::CImg<unsigned char> Image;
typedef Image* ImagePtr;

typedef std::vector<ImagePtr> ListOfImages;
typedef ListOfImages* ListOfImagesPtr;

