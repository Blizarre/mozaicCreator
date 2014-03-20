#pragma once

#include "Norm_LAB_SSDMatcher.h"
#include "MatchingAlgorithm.h"


typedef std::shared_ptr<MatchingAlgorithm> MatchingAlgorithmSPrt;


// create the getter as well as the getter/setter
#define PARAM(type, name) protected: type m_##name; void set##name(type t) { m_##name = t; }; public: type get##name() const { return m_##name; }


class Configurator
{
public:

	// Default constructor
	Configurator()
	{
		setReferenceImage("referenceFull.bmp");
		setOutputImage("output.bmp");
		setDirectoryInputImages(".\\images");
		setReductionFactor(5);
		setAutoRemove(false);
		setCrop(5);
		setFading(3);
		setSplit(4);
		setReductionThumbnail(2);
		setMindistance(4.0f);
	}
	
	// Command line constructor
	void fromCommandLine(int argc, char* argv[])
	{
		if(argc != 4)
			throw std::string("Error, usage is <refImage> <directory> <reductionCoefficient>");

		setReferenceImage(argv[1]);
		setDirectoryInputImages(argv[2]);
		
		setReductionFactor(atoi(argv[3]));
		
		if(getReductionFactor() == 0)
			throw std::string("Error : SSD reduction factor invalid, must be > 0");
	}
	std::auto_ptr<MatchingAlgorithm> getMatchingAlgorithm()
	{
		return std::auto_ptr<MatchingAlgorithm>(new SSDMatcher(getReductionFactor(), getAutoRemove()));
	}

protected:

	PARAM(std::string,	ReferenceImage);
	PARAM(std::string,	OutputImage);
	PARAM(std::string,	DirectoryInputImages);
	PARAM(int,			ReductionFactor); // Used during matching to speed up the computation
	PARAM(bool,			AutoRemove);
	PARAM(unsigned int, Crop);
	PARAM(unsigned int, Fading);
	PARAM(unsigned int, Split); // 0-> no split, 1-> split en 4, 2-> split en 4 + centre, 3 -> split en 4 + centre + entiere
	PARAM(unsigned int, ReductionThumbnail); // reduction coefficient of the size of the thumbnail
	PARAM(float, Mindistance); // min distance between two same images
};