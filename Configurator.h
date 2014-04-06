#pragma once

#include "Norm_LAB_SSDMatcher.h"
#include "MatchingAlgorithm.h"

#include <sstream>

typedef std::shared_ptr<MatchingAlgorithm> MatchingAlgorithmSPrt;


// create the getter as well as the getter/setter
#define PARAM(type, name) protected: type m_##name; void set##name(type t) { m_##name = t; }; public: type get##name() const { return m_##name; }


class Configurator
{
public:


	std::string getConfig()
	{
		std::stringstream ss;
		ss << "- reference  Image       : " << getReferenceImage() << std::endl;
		ss << "- output  Image          : " << getOutputImage() << std::endl;
		ss << "- tiles directory        : " << getDirectoryInputTiles() << std::endl;
		ss << "- reduction factor       : " << getReductionFactor() << std::endl;
		ss << "- auto remove            : " << getAutoRemove() << std::endl;
		ss << "- cropping of tiles      : " << getCrop() << std::endl;
		ss << "- splitting of tiles     : " << getSplit() << std::endl;
		ss << "- reduction of tiles     : " << getReductionTiles() << std::endl;
		ss << "- min distance between ti: " << getMindistance() << std::endl;

		return ss.str();
	}


	// Default constructor
	Configurator()
	{
		setReferenceImage("referenceFull.bmp");
		setOutputImage("output.bmp");
		setDirectoryInputTiles(".\\images");
		setReductionFactor(10);
		setAutoRemove(false);
		setCrop(5);
		setFading(3);
		setSplit(4);
		setReductionTiles(2);
		setMindistance(4.0f);
	}

	// Command line constructor
	void fromCommandLine(int argc, char* argv[])
	{
		if (argc != 0) // no arguments, use default values
		{
			if (argc != 7)
				throw std::string("Error, usage is <refImage> <tilesDirectory> <ReductionFactor> <Fading> <reductionTiles> <splittingTiles>");


			setReferenceImage(argv[1]);
			setDirectoryInputTiles(argv[2]);

			setReductionFactor(atoi(argv[3]));

			setFading(atoi(argv[4]));

			setReductionTiles(atoi(argv[5]));

			setSplit(atoi(argv[6]));

			if (getReductionFactor() == 0)
				throw std::string("Error : SSD reduction factor invalid, must be > 0");
		}
	}
	std::auto_ptr<MatchingAlgorithm> getMatchingAlgorithm()
	{
		return std::auto_ptr<MatchingAlgorithm>(new SSDMatcher(getReductionFactor(), getAutoRemove()));
	}

protected:

	PARAM(std::string,	ReferenceImage);
	PARAM(std::string,	OutputImage);
	PARAM(std::string,	DirectoryInputTiles); // Directory where all thumbnails are stored. THey must be all of the exact same size
	PARAM(int,			ReductionFactor); // Used during matching to speed up the computation, have no impact on the final image thumbnail size
	PARAM(bool,			AutoRemove); // automatically remove any thumbnail as soon as it has been used, to avoir repetition.
	PARAM(unsigned int, Crop); // Crop all small images of "Crop" pixels, in case some have borders
	PARAM(unsigned int, Fading);
	PARAM(unsigned int, Split); // 0-> no split, 1-> split en 4, 2-> split en 4 + centre, 3 -> split en 4 + centre + entiere
	PARAM(unsigned int, ReductionTiles); // reduction coefficient of the size of the tiles, reduce the size of tiles on the final image (origSize{X,Y}/ReductionTiles)
	PARAM(float, Mindistance); // min distance between two same images
};