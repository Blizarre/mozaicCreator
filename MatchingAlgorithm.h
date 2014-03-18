#pragma once

#include "mozaicDefinitions.h"
#include "Configurator.h"

class MatchingAlgorithm
{
public:

	MatchingAlgorithm() { };

	// Warning : Will "optimize" every image in lim
	virtual void Initialize(ListOfImagesSPtr lim)
	{
		ListOfImages::iterator im;
		for (im = lim->begin(); im != lim->end(); im++)
		{
			preprocessOneImage(im->second);
		}
		for (im = lim->begin(); im != lim->end(); im++)
		{
			NormalizeOneImage(im->second);
		}
		m_listOfImages = lim;
	}

	virtual void preprocessOneImage(ImageSPtr im, bool isRef = false) = 0;
	virtual void NormalizeOneImage(ImageSPtr im, bool isRef = false) = 0;

	virtual double proximity(ImageSPtr a, ImageSPtr b) = 0;

	virtual std::string FindBestMatch(ImageSPtr imRef)
	{
		preprocessOneImage(imRef, true);
		NormalizeOneImage(imRef, true);

		ListOfImages::iterator im;
		std::string bestImage;
		double grade, bestGrade = std::numeric_limits<double>::max();
		for (im = m_listOfImages->begin(); im != m_listOfImages->end(); im++)
		{
			grade = proximity(im->second, imRef);
			if (grade < bestGrade)
			{
				bestImage = im->first;
				bestGrade = grade;
			}
		}
		return bestImage;
	}


	virtual ~MatchingAlgorithm() {}

protected:
	ListOfImagesSPtr m_listOfImages;
};

