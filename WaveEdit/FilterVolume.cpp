#include "stdafx.h"
#include "FilterVolume.h"


FilterVolume::FilterVolume(bool inc)
{
	this->inc = inc;
}


FilterVolume::~FilterVolume()
{
}

WaveFile * FilterVolume::Transform(WaveFile * wave, double num) {
	return wave->volume(inc);
}