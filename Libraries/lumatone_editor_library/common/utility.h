/*
  ==============================================================================

    CommonFunctions.h
    Created: 17 Jun 2020 9:54:26pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_LIBRARY_UTILITY_H
#define LUMATONE_EDITOR_LIBRARY_UTILITY_H

#include <JuceHeader.h>

template <class T>
static juce::String arrayToString(juce::Array<T>& arrayIn, juce::String name = "", char arrayStart=0, char arrayEnd=0)
{
	String strOut;

	if (name != "")
		strOut += name + ": ";
	if (arrayStart != 0)
		strOut += arrayStart;

	for (int i = 0; i < arrayIn.size(); i++)
	{
		strOut += juce::String(arrayIn[i]);
		if (i < arrayIn.size() - 1)
			strOut += ',';
	}

	if (arrayEnd != 0)
		strOut += arrayEnd;

	return strOut;
}

#endif // LUMATONE_EDITOR_LIBRARY_UTILITY_H
