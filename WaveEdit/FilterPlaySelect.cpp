#include "stdafx.h"
#include "FilterPlaySelect.h"


FilterPlaySelect::FilterPlaySelect()
{
}


FilterPlaySelect::~FilterPlaySelect()
{
}

WaveFile * FilterPlaySelect::Transform(WaveFile * wave, double num) {
	wave->play();
	return wave;
}
