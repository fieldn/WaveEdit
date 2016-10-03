#pragma once
#include "Filter.h"

class FilterEcho : public Filter
{
public:
	FilterEcho();
	~FilterEcho();
	WaveFile * Transform(WaveFile * wave, double num);
};

