/*
  ==============================================================================

    CommonFunctions.h
    Created: 17 Jun 2020 9:54:26pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_LIBRARY_DATA_H
#define LUMATONE_EDITOR_LIBRARY_DATA_H

#include <JuceHeader.h>

template <class T>
struct PointPair
{
	juce::Point<T> x, y;

	PointPair() {}
	PointPair(juce::Point<T> xIn, juce::Point<T> yIn)
	{
		x = xIn;
		y = yIn;
	};
	PointPair(T x1, T y1, T x2, T y2)
	{
		x = juce::Point<T>(x1, y1);
		y = juce::Point<T>(x2, y2);
	}
};

#endif // LUMATONE_EDITOR_LIBRARY_DATA_H
