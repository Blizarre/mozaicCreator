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
			InitializeOneImage(im->second);
		}
		m_listOfImages = lim;
	}

	virtual void InitializeOneImage(ImageSPtr lim) = 0;

	virtual double proximity(ImageSPtr a, ImageSPtr b) = 0;

	virtual std::wstring FindBestMatch(ImageSPtr imRef)
	{
		
		InitializeOneImage(imRef);

		double currentBestScore = 0;
		ListOfImages::iterator im;
		std::wstring bestImage;
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

