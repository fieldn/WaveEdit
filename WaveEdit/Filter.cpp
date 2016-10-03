#include "stdafx.h"
#include "Filter.h"


Filter::Filter()
{
}


Filter::~Filter()
{
}

WaveFile * Filter::TransformSelect(WaveFile * wave, double sStart, double sEnd, double num) {

	if (sStart != sEnd) {
		int sizeS = sEnd - sStart;
		int sizeE = wave->lastSample - sEnd;

		WaveFile * middle = new WaveFile(wave->numChannels, wave->sampleRate, wave->bitsPerSample);

		short* start = wave->get_fragment(0, sStart);
		short* mid = wave->get_fragment(sStart, sizeS);
		short* end = wave->get_fragment(sEnd, sizeE);

		middle = middle->append_fragment(mid, sizeS, 0);
		middle = Transform(middle, num);
		short* mm = middle->get_fragment(0, middle->lastSample);

		WaveFile * fin = new WaveFile(wave->numChannels, wave->sampleRate, wave->bitsPerSample);
		fin = fin->append_fragment(start, sStart, 0);
		fin = fin->append_fragment(mm, middle->lastSample, fin->lastSample);
		fin = fin->append_fragment(end, sizeE, fin->lastSample);

		return fin;
	}
	else {
		return Transform(wave, num);
	}
}