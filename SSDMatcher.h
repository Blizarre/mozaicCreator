#pragma once

#include "MatchingAlgorithm.h"

class SSDMatcher : public MatchingAlgorithm
{
public:

	SSDMatcher(int reductionFactor)
	{
		m_reductionFactor = reductionFactor;
	}

	// on fait somme de la vleur absolue des différences
	virtual double proximity(ImagePtr a, ImagePtr b)
	{
		Image im2 = (*a - *b);
		im2.abs();
		return im2.sum();
	}

	// Warning : Will "optimize" every image in lim
	virtual void Initialize(ListOfImagesPtr lim)
	{
		ListOfImages::iterator im;
		for (im = lim->begin(); im != lim->end(); im++)
		{
			(*im)->resize( (*im)->width() / m_reductionFactor, (*im)->height() / m_reductionFactor, 1, 1, 1);
		}
		m_listOfImages = lim;
	}

protected:
	int m_reductionFactor;
};

