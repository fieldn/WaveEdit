#include "stdafx.h"
#include "ModifierPaste.h"
#include "WaveEditDoc.h"

ModifierPaste::ModifierPaste(CWaveEditDoc * doc, short* clip, int size)
{
	this->doc = doc;
	this->size = size;
	this->clip = (short *)malloc(sizeof(short) * size);
	memcpy(this->clip, clip, size);
}


ModifierPaste::~ModifierPaste()
{
	free(this->clip);
}

WaveFile * ModifierPaste::TransformSelect(WaveFile * wave, double sStart, double sEnd, double num) {
	wave = &doc->wave;
	return wave->append_fragment(clip, sStart, sEnd);
}
