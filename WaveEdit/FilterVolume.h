#pragma once
#include "Filter.h"

class FilterVolume : public Filter
{
public:
	bool inc;
	FilterVolume(bool inc);
	~FilterVolume();
	WaveFile * Transform(WaveFile * wave, double num);
};

