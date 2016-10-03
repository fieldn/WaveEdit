#pragma once
#include "Filter.h"

class FilterFade : public Filter
{
public:
	bool in;
	FilterFade(bool in);
	~FilterFade();
	WaveFile * Transform(WaveFile * wave, double num);
};

