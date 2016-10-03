#include "stdafx.h"
#include "FilterSpeed.h"


FilterSpeed::FilterSpeed()
{
}


FilterSpeed::~FilterSpeed()
{
}

WaveFile * FilterSpeed::Transform(WaveFile * wave, double num) {
	return wave->multiply_freq(num, 0);
}
