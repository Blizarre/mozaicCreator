#pragma once

#include "mozaicDefinitions.h"
#include "Configurator.h"

class MatchingAlgorithm
{
public:

	MatchingAlgorithm() { };

	// Warning : Will "optimize" every image in lim
	virtual void Initialize(ListOfImagesPtr lim)
	{
		ListOfImages::iterator im;
		for (im = lim->begin(); im != lim->end(); im++)
		{
			InitializeOneImage(im->second);
		}
		m_listOfImages = lim;
	}

	virtual void InitializeOneImage(ImagePtr lim) = 0;

	virtual double proximity(ImagePtr a, ImagePtr b) = 0;

	virtual std::wstring FindBestMatch(ImagePtr imRef)
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
	ListOfImagesPtr m_listOfImages;
};

