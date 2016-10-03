#pragma once
#include "Modifier.h"

class CWaveEditDoc;

class ModifierPaste : public Modifier
{
public:
	CWaveEditDoc * doc;
	short* clip;
	int size;
	ModifierPaste(CWaveEditDoc * doc, short* clip, int size);
	~ModifierPaste();
	WaveFile * TransformSelect(WaveFile * wave, double sStart, double sEnd, double num = 0);
};

