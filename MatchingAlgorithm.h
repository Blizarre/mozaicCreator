#pragma once

#include "mozaicDefinitions.h"
#include "Configurator.h"

class MatchingAlgorithm
{
public:

	MatchingAlgorithm() { };

	virtual void Initialize(ListOfImagesPtr lim) = 0;

	virtual double proximity(ImagePtr a, ImagePtr b) = 0;

	virtual ImagePtr FindBestMatch(ImagePtr imRef)
	{
		double currentBestScore = 0;
		ListOfImages::iterator im;
		ImagePtr bestImage;
		double grade, bestGrade = 0;
		for (im = m_listOfImages->begin(); im != m_listOfImages->end(); im++)
		{
			grade = proximity(*im, imRef);
			if (grade > bestGrade)
			{
				bestImage = *im;
				bestGrade = grade;
			}
		}
		return bestImage;
	}


	virtual ~MatchingAlgorithm() {}

protected:
	ListOfImagesPtr m_listOfImages;
};

