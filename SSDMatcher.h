#pragma once

#include "MatchingAlgorithm.h"

class SSDMatcher : public MatchingAlgorithm
{
public:

	SSDMatcher(int reductionFactor)
	{
		m_reductionFactor = reductionFactor;
	}

	// on fait somme de la valeur absolue des différences
	virtual double proximity(ImageSPtr a, ImageSPtr b)
	{
		Image im2 = (*a - *b);
		im2.abs();
/*		cimg_library::CImgDisplay disp = cimg_library::CImgDisplay(im2, "Diff", 1);
		cimg_library::CImgDisplay dispa = cimg_library::CImgDisplay(*a, "Test A", 1);
		cimg_library::CImgDisplay dispb = cimg_library::CImgDisplay(*b, "Test B", 1);
		while (!disp.is_closed()) disp.wait();*/
		return im2.sum();
	}

	void InitializeOneImage(ImageSPtr im) {
		im->RGBtoLab();
		im->resize(im->width() / m_reductionFactor, im->height() / m_reductionFactor, 1, 3, 1);
	}

protected:
	int m_reductionFactor;
};

