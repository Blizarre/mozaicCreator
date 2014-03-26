mozaicCreator
=============

This is a small hack I made in a few afternoon, so don’t expect too much on code quality :-/

However, it works very well ! Features :

- Can handle more than 2000 small images.
- Theses images can be split into 5 smaller images to increase the sample number (split option), thus making more than 10 000 thumbnails
- For now, the comparison function is a sum of square difference in the Lab space
- The Mindistance parameter prevent the frequent reuse of the same thumbnail at the same place : you will limit the number of time a thumbnail cans be used for a given radius (in thumbnail number)
- The comparison is done using smaller thumbnails to increase speed. This is the ReductionFactor, and has no impact on the final size of thumbnails
- Contrary to the reductionThumb parameter, which will reduce the final size of the thumbnails.

The images from the gallery above were made in in less than 30 seconds !

Warning : should be run on x64 computer, it can fill up RAM very quickly, and support bmp images only for now.

Currently the source code is for visual studio 2013. Very easy to port on linux, the only windows-only parts are the filesystem module and the use of “#pragma once”

Usage :  

mozaicCreator "refImage" "directory" "ReductionFactor" "Fading" "reductionThumb"
  - refImage : Reference Image (bmp)
  - directory : directory containing all the thumbnails images, all with the same size (bmp)
  - reductionFactor : used during matching to speed up the computation, have no impact on the final image thumbnail size
  - fading : smooth the interface between the thumbnails in the final image (overlap in pixels)
  - reductionThumb : reduction coefficient of the thumbnail, reduce the size of thumbnails on the final image

See Configurator.h for more informations

see: http://www.marache.net/mozaic-creator/
