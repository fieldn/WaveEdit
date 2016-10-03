#include "stdafx.h"
#include "FilterEcho.h"


FilterEcho::FilterEcho()
{
}


FilterEcho::~FilterEcho()
{
}

WaveFile * FilterEcho::Transform(WaveFile * wave, double num) {
	return (wave->echo(.2, .1));
}
