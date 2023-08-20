/*
  ==============================================================================

    ViewComponents.h
    Created: 16 Jun 2014 7:20:34pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "./data/lumatone_key.h"
#include "ViewConstants.h"


/*
==============================================================================
Display of the data of one key
==============================================================================
*/
class TerpstraKeyEdit : public MappedLumatoneKey, public Component, public SettableTooltipClient
{
public:
	TerpstraKeyEdit(int boardIndex, int keyIndex, LumatoneKey keyData=LumatoneKey());
	~TerpstraKeyEdit();

	void setValue(const LumatoneKey& newValue);

	void setIsSelected(bool newValue);

	void paint(Graphics& g);
	void resized();

	void setKeySize(float keySizeIn);

	enum ColourIds
	{
		backgroundColourId = 0x2000100,
		outlineColourId = 0x2000101,
		selectedKeyOutlineId = 0x2000102
	};

private:
	bool isSelected;

    Path    hexPath;
	Label*	midiNoteLabel;
	Label*  midiChannelLabel;

	float					keySize;

	const float				channelLabelRadiusScalar = 0.4347826f;
};
