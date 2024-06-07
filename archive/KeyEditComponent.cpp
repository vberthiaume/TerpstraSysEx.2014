/*
  ==============================================================================

    key_edit_component.cpp
    Created: 16 Jun 2014 7:20:34pm
    Author:  hsstraub

  ==============================================================================
*/

#include "KeyEditComponent.h"
#include "LumatoneEditorState.h"

#include "LumatoneEditorLookAndFeel.h"

/*
==============================================================================
LumatoneKeyEdit class
==============================================================================
*/

LumatoneKeyEdit::LumatoneKeyEdit(const LumatoneEditorState& stateIn, int boardIndex, int keyIndex)
	: isSelected(false), state(stateIn), board_idx(boardIndex), key_idx(keyIndex), keyColour(juce::Colour()), keyType(LumatoneKeyType::noteOnNoteOff)
{
	midiNoteLabel = new juce::Label("midiNoteLabel", "0");
	addAndMakeVisible(midiNoteLabel);
	midiNoteLabel->setJustificationType(juce::Justification::centredRight);
	midiNoteLabel->setFont(state.getAppFonts().getFont(LumatoneEditorFont::GothamNarrowBold));


	midiChannelLabel = new juce::Label("midiChannelLabel", "0");
	addAndMakeVisible(midiChannelLabel);
	midiChannelLabel->setFont(state.getAppFonts().getFont(LumatoneEditorFont::GothamNarrowMedium));
	midiChannelLabel->setJustificationType(juce::Justification::centredRight);

	addMouseListener(this, true);
}

LumatoneKeyEdit::~LumatoneKeyEdit()
{
	listeners.clear();
	deleteAllChildren();
}

int LumatoneKeyEdit::getBoardIndex() const
{
	return board_idx;
}

int LumatoneKeyEdit::getKeyIndex() const
{
	return key_idx;
}


LumatoneKey LumatoneKeyEdit::getValue() const
{
	LumatoneKey newValue;
	newValue.setNoteOrCC(midiNoteLabel->getText().getIntValue());
	newValue.setChannelNumber(midiChannelLabel->getText().getIntValue());
	newValue.setColour(keyColour);
	newValue.setKeyType(keyType);
    newValue.setDefaultCCFader(ccFaderDefault);
	return newValue;
}

void LumatoneKeyEdit::setValue(const LumatoneKey& newValue)
{
	auto newKeyType = newValue.getType();
    if (newKeyType == LumatoneKeyType::disabled || newKeyType == LumatoneKeyType::disabledDefault)
    {
		// TODO move this behavior elsewhere
        //newValue.keyType = LumatoneKeyType::disabled;

		newKeyType = LumatoneKeyType::disabled;
        midiNoteLabel->setText("x", juce::NotificationType::sendNotification);
        midiChannelLabel->setText("x", juce::NotificationType::sendNotification);
    }
    else
    {
        midiNoteLabel->setText(juce::String(newValue.getMidiNumber()), juce::NotificationType::sendNotification);
        midiChannelLabel->setText(juce::String(newValue.getMidiChannel()), juce::NotificationType::sendNotification);
    }

	keyColour = newValue.getColour();
	keyType = newKeyType;
    ccFaderDefault = newValue.isCCFaderDefault();

	juce::String newTooltip = juce::translate("KeyType") + " ";
	switch (keyType)
	{
	case LumatoneKeyType::noteOnNoteOff:
		newTooltip += juce::translate("NoteOnOff");
		break;
	case LumatoneKeyType::continuousController:
		newTooltip += juce::translate("ContinuousController");
        newTooltip += juce::newLine;
        if (ccFaderDefault)
            newTooltip += "CC Default (0->127)";
        else
            newTooltip += "CC Inverted (127->0)";
		break;
	case LumatoneKeyType::lumaTouch:
		newTooltip += juce::translate("Lumatouch");
		break;
	case LumatoneKeyType::disabled:
		newTooltip += juce::translate("Disabled");
		break;
	default:
		//jassertfalse;
		newTooltip += juce::translate("Unknown");
		break;
	}

	newTooltip += juce::newLine;
	newTooltip += juce::translate("KeyColour") + " " + keyColour.toDisplayString(false);

	setTooltip(newTooltip);
	midiNoteLabel->setTooltip(newTooltip);
	midiChannelLabel->setTooltip(newTooltip);

	repaint();
}

void LumatoneKeyEdit::setIsSelected(bool newValue)
{
	if (this->isSelected != newValue)
	{
		this->isSelected = newValue;
		repaint();
	}
}

void LumatoneKeyEdit::paint(juce::Graphics& g)
{
	LumatoneKey currentValue = getValue();

	// Selected or not: color and thickness of the line
	float lineWidth = TERPSTRASINGLEKEYFLDLINEWIDTH;
	juce::Colour lineColour = findColour(selectedKeyOutlineId);
	
	// Color: empty or the parametrized color
	juce::Colour bgColour = currentValue.getColour().withAlpha(1.0f);
	juce::Colour textColour = bgColour.contrasting(0.7f);

	if (bgColour.getPerceivedBrightness() < 0.5)
	{
		textColour = textColour.brighter();
	}

    midiChannelLabel->setColour(juce::Label::textColourId, textColour);
    midiNoteLabel->setColour(juce::Label::textColourId, textColour);

    // Look depending on Key type
    if (currentValue.hasType(LumatoneKeyType::continuousController))
	{
		// Key type is continuous controller. Set colour gradient.
        float w = this->getWidth();
        float h = this->getHeight();
		juce::Colour inside, outside;
        if (currentValue.isCCFaderDefault())
        {
            outside = bgColour.darker();
            inside = bgColour.brighter();
        }
        else
        {
            outside = bgColour.brighter();
            inside = bgColour.darker();
        }
		g.setGradientFill(
			juce::ColourGradient(inside, w * 0.5f, h * 0.5f, outside, w * 0.5f, 0.0f, true));
	}
	else
	{
		// Key type is note on/note off. Just plain colour.
		g.setColour(bgColour);
	}

	g.fillPath(hexPath);

	if (isSelected)
	{
		// Shrink bounds slightly
		juce::Path hexOutline = juce::Path(hexPath);
		juce::Rectangle<int> hexBounds = getLocalBounds().reduced(1);
		hexOutline.scaleToFit(hexBounds.getX(), hexBounds.getY(), hexBounds.getWidth(), hexBounds.getHeight(), true);

		// Draw line
		g.setColour(lineColour);
		g.strokePath(hexOutline, juce::PathStrokeType(lineWidth));
	}

//	if (currentValue.keyType == LumatoneKeyType::disabled)
//	{
//		float w = this->getWidth();
//		float h = this->getHeight();
//		float xProportion = 0.25f;
//		// Draw X on key
//		g.setColour(bgColour.contrasting(0.5f));
//		g.drawLine(w * xProportion, h * xProportion, w * (1-xProportion), h * (1-xProportion), 2);
//		g.drawLine(w * (1 - xProportion), h * xProportion, w * xProportion, h * (1 - xProportion), 2);
//	}

//    if (currentValue.keyType == LumatoneKeyType::disabled)
//    {
//        TerpstraSysExApplication::getApp().getLookAndFeel().getLabelFont(*midiNoteLabel);
//        g.setColour(textColour);
//        g.setFont(midiChannelLabel->getFont());
//        g.drawText("x", midiChannelLabel->getBounds(), midiChannelLabel->getJustificationType());
//        g.drawText("x", midiNoteLabel->getBounds(), midiChannelLabel->getJustificationType());
//    }

	// Something parametrized or not?
	if (currentValue.isEmpty())
	{
		midiChannelLabel->setAlpha(0.3f);
		midiNoteLabel->setAlpha(0.3f);
	}
	else
	{
		midiChannelLabel->setAlpha(1.0f);
		midiNoteLabel->setAlpha(1.0f);
	}
}

void LumatoneKeyEdit::resized()
{
	juce::Point<int> centre = getLocalBounds().getCentre();
	float radius = keySize * 0.5f;

	// Draw hexagon
	hexPath.clear();
	hexPath.addPolygon(centre.toFloat(), 6, radius, TERPSTRASINGLEKEYROTATIONANGLE);
	//Rectangle<float> hexBounds = hexPath.getBounds().reduced(1, 1);

	float lblSize = radius * TERPSTRASINGLEKEYLABELSIZE;
	float lblOffset = radius * 0.375f;

	midiChannelLabel->setSize(radius, lblSize);
	midiChannelLabel->setCentrePosition(centre.translated(-lblOffset, -(radius * channelLabelRadiusScalar)));

	midiNoteLabel->setSize(radius, lblSize);
	midiNoteLabel->setCentrePosition(centre.translated(0, lblOffset));
}

void LumatoneKeyEdit::setKeySize(float keySizeIn)
{
	keySize = keySizeIn;
	setSize(keySize, keySize);
}

void LumatoneKeyEdit::mouseDown(const juce::MouseEvent& e)
{
	DBG("key clicked!");
	auto value = getValue();
	listeners.call(&LumatoneKeyEditListener::keyClickedCallback, e, board_idx, key_idx, value);
}

juce::String LumatoneKeyEdit::toString() const
{
	juce::StringArray tokens;
	tokens.add("Board " + juce::String(board_idx));
	tokens.add("Key " + juce::String(key_idx));
	tokens.add("Note " + midiNoteLabel->getText());
	tokens.add("Ch " + midiChannelLabel->getText());
	tokens.add("Type " + juce::String(keyType));
	tokens.add("Col " + keyColour.toString());
	tokens.add("Size " + juce::String(keySize));
	return tokens.joinIntoString(" | ");
}

void LumatoneKeyEdit::addKeyEditListener(LumatoneKeyEditListener* listenerIn)
{
	listeners.add(listenerIn);
}

void LumatoneKeyEdit::removeKeyEditListener(LumatoneKeyEditListener* listenerIn)
{
	listeners.remove(listenerIn);
}


/*
==============================================================================
LumatoneKeyPtr struct
==============================================================================
*/

LumatoneKeyPtr::LumatoneKeyPtr(int boardIndex, int keyIndex, LumatoneKey* keyPtr)
	: board_idx(boardIndex), key_idx(keyIndex), key(keyPtr) {}

bool LumatoneKeyPtr::isValid() const
{
	//if (key_idx >= 0 && key_idx < getOctaveBoardSize() && board_idx >= 0)
	//	return true;
	return false;
}

bool LumatoneKeyPtr::isNull() const
{
	return key == nullptr;
}


LumatoneBoardSet::LumatoneBoardSet(int boardIndex)
	: board_idx(boardIndex)
{
	//for (int i = 0; i < getOctaveBoardSize(); i++)
	//{
	//	keys[i].reset(new LumatoneKeyEdit(boardIndex, i));
	//}
}
