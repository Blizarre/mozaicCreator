mozaicCreator
=============

This is a small hack I made in a few afternoon, so don't expect too much on code quality :-/

However, it works very well ! Features :

- Can handle more than 5000 tiles.
- The tiles can be split into 5 smaller images to increase the sample number (split option), thus making more than 10 000 sample
- For now, the comparison function is a sum of square difference in the Lab space
- The Mindistance parameter prevent the frequent reuse of the same tile at the same place : you will limit the number of time a tile cans be used for a given radius
- The comparison is done using smaller tile to increase speed. This is the ReductionFactor, and has no impact on the final size of tiles in the picture
- Contrary to the reductionTile parameter, which will reduce the final size of the tiles.

The images from the gallery above were made in in less than 30 seconds !

Warning : should be run on x64 computer, it can fill up RAM very quickly, and support bmp images only for now.

Currently the source code is for visual studio 2013. Very easy to port on linux, the only windows-only parts are the filesystem module and the use of "#pragma once"

see: http://www.marache.net/mozaic-creator/
