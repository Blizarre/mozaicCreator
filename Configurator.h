#pragma once

#include "SSDMatcher.h"
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
		setReferenceImage(L"reference.bmp");
		setOutputImage(L"output.bmp");
		setDirectoryInputImages(L".\\images");
		setReductionFactor(1);
	}
	
	// Command line constructor
	void fromCommandLine(int argc, WCHAR* argv[])
	{
		if(argc != 4)
			throw std::string("Error, usage is <refImage> <directory> <reductionCoefficient>");

		setReferenceImage(argv[1]);
		setDirectoryInputImages(argv[2]);
		
		setReductionFactor(_wtoi(argv[3]));
		
		if(getReductionFactor() == 0)
			throw std::string("Error : SSD reduction factor invalid, must be > 0");
	}
	std::auto_ptr<MatchingAlgorithm> getMatchingAlgorithm()
	{
		return std::auto_ptr<MatchingAlgorithm>(new SSDMatcher(getReductionFactor()));
	}

protected:

	PARAM(std::wstring, ReferenceImage);
	PARAM(std::wstring, OutputImage);
	PARAM(std::wstring, DirectoryInputImages);
	PARAM(int,			ReductionFactor);
};