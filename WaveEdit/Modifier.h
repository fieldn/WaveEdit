#pragma once
#include "WaveFile.h"


class Modifier
{
public:
	double start;
	double end;
	double pnum;
	Modifier();
	~Modifier();
	virtual WaveFile * TransformSelect(WaveFile * wave, double sStart, double sEnd, double num = 0) = 0;
};

