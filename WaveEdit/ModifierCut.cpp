#include "stdafx.h"
#include "ModifierCut.h"

ModifierCut::ModifierCut(CWaveEditDoc * doc)
{
	this->doc = doc;
}


ModifierCut::~ModifierCut()
{
}

WaveFile * ModifierCut::TransformSelect(WaveFile * wave, double sStart, double sEnd, double num) {
	start = sStart;
	end = sEnd;
	pnum = num;
	wave = &doc->wave;
	return wave->remove_fragment(sStart, sEnd);
}
