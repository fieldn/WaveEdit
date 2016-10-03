#include "stdafx.h"
#include "FilterFade.h"


FilterFade::FilterFade(bool in)
{
	this->in = in;
}


FilterFade::~FilterFade()
{
}

WaveFile * FilterFade::Transform(WaveFile * wave, double num) {
	return wave->fade(in);
}