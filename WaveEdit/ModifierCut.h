#pragma once
#include "Modifier.h"
#include "WaveEditView.h"

class CWaveEditDoc;

class ModifierCut : public Modifier
{
public:
	CWaveEditDoc * doc;
	ModifierCut(CWaveEditDoc * doc);
	~ModifierCut();
	WaveFile * TransformSelect(WaveFile * wave, double sStart, double sEnd, double num = 0);
};
