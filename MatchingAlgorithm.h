#pragma once

#include "mozaicDefinitions.h"
#include "Configurator.h"

typedef std::map<CharImagePtr, ImagePtr> ListOfMatchingImages;
typedef ListOfMatchingImages* ListOfMatchingImagesPtr;


class MatchingAlgorithm
{
public:

	MatchingAlgorithm() : m_autoRemove(false) { }

	MatchingAlgorithm(bool autoremove): m_autoRemove(autoremove) { };

	// Warning : Will "optimize" every image in lim
	virtual void Initialize(ListOfImagesSPtr lim)
	{
		ListOfImages::iterator im;

		for (im = lim->begin(); im != lim->end(); im++)
		{
			preprocessOneImage(*im->second);
		}
		for (im = lim->begin(); im != lim->end(); im++)
		{
			NormalizeOneImage(*im->second);
		}

		m_listOfImages = lim;
	}

	virtual void preprocessOneImage(Image & im, bool isRef = false) = 0;
	virtual void NormalizeOneImage(Image & im, bool isRef = false) = 0;

	virtual double proximity(Image & a, Image & b) = 0;

	virtual CharImageSPtr FindBestMatch(Image &  imRef)
	{
		preprocessOneImage(imRef, true);
		NormalizeOneImage(imRef, true);

		ListOfImages::iterator im;
		ListOfImages::iterator bestPair;
		CharImageSPtr result;
		double grade, bestGrade = std::numeric_limits<double>::max();
		for (im = m_listOfImages->begin(); im != m_listOfImages->end(); im++)
		{
			grade = proximity(*im->second, imRef);
			if (grade < bestGrade)
			{
				bestPair = im;
				bestGrade = grade;
			}
		}

		result = bestPair->first;

		if (m_autoRemove)
			m_listOfImages->erase(bestPair);
			
		return result;
	}


	virtual ~MatchingAlgorithm() {	}

protected:
	ListOfImagesSPtr m_listOfImages;
	bool m_autoRemove;
};

