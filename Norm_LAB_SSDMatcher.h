#pragma once

#include "MatchingAlgorithm.h"

class SSDMatcher : public MatchingAlgorithm
{
public:

	SSDMatcher(int reductionFactor, bool autoremove) : MatchingAlgorithm(autoremove)
	{
		m_reductionFactor = reductionFactor;

		m_preProcess_L_min = m_preProcess_a_min = m_preProcess_b_min = std::numeric_limits<double>::max();
		m_preProcess_L_max = m_preProcess_a_max = m_preProcess_b_max = std::numeric_limits<double>::min();
	}

	// Sum of square difference
	virtual double proximity(Image & a, Image & b)
	{
		Image tmp = a - b;
		tmp.sqr();
		return tmp.sum();
	}

	inline void updateMin(double & var, const double candidate) { if (candidate < var) var = candidate; }
	inline void updateMax(double & var, const double candidate) { if (candidate > var) var = candidate; }

	void updateMinMax(Image & im)
	{
		Image L = im.get_channel(0);
		Image a = im.get_channel(1);
		Image b = im.get_channel(2);

		updateMin(m_preProcess_L_min, L.min());
		updateMax(m_preProcess_L_max, L.max());

		updateMin(m_preProcess_a_min, a.min());
		updateMax(m_preProcess_a_max, a.max());

		updateMin(m_preProcess_b_min, b.min());
		updateMax(m_preProcess_b_max, b.max());
	}

	void convertImage(Image & im)
	{
		im.RGBtoLab();
		im.resize(im.width() / m_reductionFactor, im.height() / m_reductionFactor, 1, 3, 1);
	}

	void preprocessOneImage(Image & im, bool isRef)
	{
		convertImage(im);

		if(! isRef)
			updateMinMax(im);
	}

	void NormalizeOneImage(Image & im, bool isRef) {
		double L_span = m_preProcess_L_max + m_preProcess_L_min;
		double a_span = m_preProcess_a_max + m_preProcess_a_min;
		double b_span = m_preProcess_b_max + m_preProcess_b_min;

		double L_div = m_preProcess_L_max - m_preProcess_L_min;
		double a_div = m_preProcess_a_max - m_preProcess_a_min;
		double b_div = m_preProcess_b_max - m_preProcess_b_min;

		Image L = im.get_shared_channel(0);
		Image a = im.get_shared_channel(1);
		Image b = im.get_shared_channel(2);

		L = (L - L_span / 2) / L_div;
		a = (a - a_span / 2) / a_div;
		b = (b - b_span / 2) / b_div;
	}

protected:
	int m_reductionFactor;

	double m_preProcess_L_min;
	double m_preProcess_L_max;
	double m_preProcess_a_min;
	double m_preProcess_a_max;
	double m_preProcess_b_min;
	double m_preProcess_b_max;
};

