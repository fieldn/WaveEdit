#pragma once
#include "WaveFile.h"
#include "Modifier.h"

class Filter : public Modifier
{
public:
	Filter();
	~Filter();
	WaveFile * TransformSelect(WaveFile * wave, double sStart, double sEnd, double num = 0);
	virtual WaveFile * Transform(WaveFile * wave, double num) = 0;
};

