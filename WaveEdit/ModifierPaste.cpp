#include "stdafx.h"
#include "ModifierPaste.h"
#include "WaveEditDoc.h"

ModifierPaste::ModifierPaste(CWaveEditDoc * doc, short* clip)
{
	this->doc = doc;
	this->clip = clip;
}


ModifierPaste::~ModifierPaste()
{
}

WaveFile * ModifierPaste::TransformSelect(WaveFile * wave, double sStart, double sEnd, double num) {
	wave = &doc->wave;
	return wave->append_fragment(clip, sStart, sEnd);
}
