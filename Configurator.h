#pragma once

#include <map>
#include <memory>
#include "SSDMatcher.h"
#include "MatchingAlgorithm.h"

enum ConfigKey
{
	INPUT_IMAGE,
	OUTPUT_IMAGE,
	SOURCE_DIRECTORY,
	SSD_REDUCTION_FACTOR
};

typedef  std::pair<ConfigKey, std::string> KeyValue;

typedef std::auto_ptr<MatchingAlgorithm> MatchingAlgorithmSPrt;

class Configurator : public std::map<ConfigKey, std::string>
{
public:
	Configurator()
	{
		this->insert(std::make_pair(ConfigKey::INPUT_IMAGE, std::string("reference.jpg")));

		this->insert(std::make_pair(ConfigKey::OUTPUT_IMAGE, std::string("output.jpg")));
		this->insert(std::make_pair(ConfigKey::SOURCE_DIRECTORY, std::string(".\\images")));
		this->insert(std::make_pair(ConfigKey::SSD_REDUCTION_FACTOR, std::string("3")));
	}
	
	std::auto_ptr<MatchingAlgorithm> getMatchingAlgorithm()
	{
		int reductionFactor = atoi(this->at(ConfigKey::SSD_REDUCTION_FACTOR).c_str());
		
		if (reductionFactor <= 0)
			reductionFactor = 1;

		return std::auto_ptr<MatchingAlgorithm>(new SSDMatcher(reductionFactor));
	}
};