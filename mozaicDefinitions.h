#pragma once

#include <map>
#include <vector>
#include <string>
#include "CImg.h"


typedef cimg_library::CImg<double> Image;
typedef Image* ImagePtr;

typedef std::map<std::wstring, ImagePtr> ListOfImages;
typedef ListOfImages* ListOfImagesPtr;

