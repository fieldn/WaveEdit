#pragma once
#include "Filter.h"

class FilterPlaySelect : public Filter
{

public:
	FilterPlaySelect();
	~FilterPlaySelect();
	WaveFile * Transform(WaveFile * wave, double num);
};
