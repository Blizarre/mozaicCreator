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
			preprocessOneImage(*im->fim);
		}
		for (im = lim->begin(); im != lim->end(); im++)
		{
			NormalizeOneImage(*im->fim);
		}

		m_listOfImages = lim;
	}

	virtual void preprocessOneImage(Image & im, bool isRef = false) = 0;
	virtual void NormalizeOneImage(Image & im, bool isRef = false) = 0;

	virtual double proximity(Image & a, Image & b) = 0;

	float distance(const std::pair<int, int> & a, const std::pair<int, int> & b)
	{
		return (a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second);
	}

	virtual CharImageSPtr FindBestMatch(Image &  imRef, int posX, int posY, float minDist)
	{
		preprocessOneImage(imRef, true);
		NormalizeOneImage(imRef, true);

		ListOfImages::iterator im;
		ListOfImages::iterator bestPair;
		CharImageSPtr result;
		double grade, bestGrade = std::numeric_limits<double>::max();
		for (im = m_listOfImages->begin(); im != m_listOfImages->end(); im++)
		{
			grade = proximity(*im->fim, imRef);
			if (grade < bestGrade)
			{
				bool ok = true;
				if (minDist > 0)
				{
					for (auto it = im->pos.cbegin(); it != im->pos.cend(); it++)
					{
						if (distance(*it, std::make_pair(posX, posY)) < minDist * minDist)
							ok = false;
					}
				}
				if (ok)
				{
					bestPair = im;
					bestGrade = grade;
				}
			}
		}

		bestPair->pos.push_back(std::make_pair(posX, posY));

		result = bestPair->cim;

		if (m_autoRemove)
			m_listOfImages->erase(bestPair);

		return result;
	}


	virtual ~MatchingAlgorithm() {	}

protected:
	ListOfImagesSPtr m_listOfImages;
	bool m_autoRemove;
};

