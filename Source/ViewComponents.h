/*
  ==============================================================================

    ViewComponents.h
    Created: 16 Jun 2014 7:20:34pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "KeyboardDataStructure.h"
#include "ViewConstants.h"


class LumatoneKeyEditListener
{
public:
	virtual ~LumatoneKeyEditListener() {}

	virtual void keyClickedCallback(const juce::MouseEvent& e, int boardIndex, int keyIndex, TerpstraKey keyValue) = 0;
};

/*
==============================================================================
Display of the data of one key
==============================================================================
*/
class TerpstraKeyEdit : public Component, public SettableTooltipClient
{
public:
	TerpstraKeyEdit(int boardIndex, int keyIndex);
	~TerpstraKeyEdit();

	int getBoardIndex() const;
	int getKeyIndex() const;

	TerpstraKey getValue() const;
	void setValue(TerpstraKey newValue);

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

	void mouseDown(const juce::MouseEvent& e) override;

public:

	void addKeyEditListener(LumatoneKeyEditListener* listenerIn);
	void removeKeyEditListener(LumatoneKeyEditListener* listenerIn);

protected:

	ListenerList<LumatoneKeyEditListener> listeners;

private:
	bool isSelected;

	int board_idx = -1;
	int key_idx = -1;

    Path    hexPath;
	Label*	midiNoteLabel;
	Label*  midiChannelLabel;

	juce::Colour			keyColour;
	LumatoneKeyType			keyType;
    bool                    ccFaderDefault;
	
	float					keySize;

	const float				channelLabelRadiusScalar = 0.4347826f;
};


struct LumatoneKeyPtr
{
	int board_idx = -1;
	int key_idx = -1;
	TerpstraKey* key = nullptr;

	LumatoneKeyPtr() {}
	LumatoneKeyPtr(int boardIndex, int keyIndex, TerpstraKey* keyPtr = nullptr);

	bool isValid() const;

	bool isNull() const;
};

using LumatoneKeySelection = juce::Array<LumatoneKeyPtr>;

struct LumatoneBoardSet
{
	int board_idx = -1;
	std::unique_ptr<TerpstraKeyEdit> keys[56];

	LumatoneBoardSet(int boardIndex);
};

