#pragma once
#include "Filter.h"

class FilterSpeed : public Filter
{

public:
	FilterSpeed();
	~FilterSpeed();
	WaveFile * Transform(WaveFile * wave, double num);

};
