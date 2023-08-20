/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "ViewComponents.h"
#include "MainComponent.h"
#include "Main.h"
#include "LumatoneMenu.h"
#include "lumatone_assets.h"
//[/Headers]

#include "AllKeysOverview.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...


//==============================================================================
KeyMiniDisplayInsideAllKeysOverview::KeyMiniDisplayInsideAllKeysOverview(int newBoardIndex, int newKeyIndex, const LumatoneKey& keyDataIn)
	: MappedLumatoneKey(keyDataIn, newBoardIndex, newKeyIndex)
	, Component("AllKeysOverview_Key" + String(newKeyIndex) + "," + String(newBoardIndex))
{
	renderMode = LumatoneComponentRenderMode::GraphicInteractive;
//	TerpstraSysExApplication::getApp().getLumatoneController()->addMidiListener(this);
}

KeyMiniDisplayInsideAllKeysOverview::~KeyMiniDisplayInsideAllKeysOverview()
{
//	TerpstraSysExApplication::getApp().getLumatoneController()->removeMidiListener(this);
}

void KeyMiniDisplayInsideAllKeysOverview::paint(Graphics& g)
{
	jassert(getParentComponent() != nullptr);
	bool boardIsSelected = boardIndex == dynamic_cast<AllKeysOverview*>(getParentComponent())->getCurrentSetSelection();

	juce::Colour hexagonColour = getKeyColour();
	// juce::Colour hexagonColour = findColour(LumatoneKeyEdit::backgroundColourId).overlaidWith(getKeyColour());
	// if (hexagonColour.getPerceivedBrightness() >= 0.6f)
	//    hexagonColour = hexagonColour.darker((1.0f - hexagonColour.getPerceivedBrightness()));

	// set color
	juce::Colour drawColour = hexagonColour;

	//switch (renderMode)
	//{
	//default:
	//	break;

	//case LumatoneComponentRenderMode::ShapeInteractive:
	//case LumatoneComponentRenderMode::GraphicInteractive:
	//case LumatoneComponentRenderMode::MaxRes:
	//	if (isSelected)
	//	{
	//		drawColour = drawColour.overlaidWith(juce::Colours::slateblue.withAlpha(0.2f));
	//	}
	//	if (isNoteOn || isClicked)
	//	{
	//		drawColour = hexagonColour.contrasting(0.5f);
	//	}
	//	else if (mouseIsOver)
	//	{
	//		drawColour = drawColour.contrasting(0.25f);
	//	}
	//	break;
	//}

	// draw
	switch (renderMode)
	{
	default:
		break;

	case LumatoneComponentRenderMode::Shape:
	case LumatoneComponentRenderMode::ShapeInteractive:
		break; // TODO

	case LumatoneComponentRenderMode::Graphic:
	case LumatoneComponentRenderMode::GraphicInteractive:
	{
		if (colourGraphic.isValid() && shadowGraphic.isValid())
		{
			int x = 0;
			int y = 0;

			g.setColour(drawColour);

			g.drawImageAt(colourGraphic, x, y, true);
			g.drawImageAt(shadowGraphic, x, y);
		}
		break;
	}

	case LumatoneComponentRenderMode::MaxRes:
	{
		if (drawColour != hexagonColour && shadowGraphic.isValid())
		{
			g.setColour(juce::Colours::black.withAlpha(0.2f).overlaidWith(drawColour));
			g.drawImageAt(shadowGraphic, 0, 0, true);
		}
		break;
	}
	}
}

void KeyMiniDisplayInsideAllKeysOverview::resized()
{

}

void KeyMiniDisplayInsideAllKeysOverview::mouseDown(const MouseEvent& e)
{
	Component::mouseDown(e);

	// Select this octave board
	jassert(getParentComponent() != nullptr);
	jassert(getParentComponent()->getParentComponent() != nullptr);
	dynamic_cast<MainContentComponent*>(getParentComponent()->getParentComponent())->
		getOctaveBoardSelectorTab()->setCurrentTabIndex(boardIndex);

	isHighlighted = true;
	repaint();

	if (e.mods.isRightButtonDown())
	{
		// Right mouse click: popup menu
		PopupMenu menu;
		TerpstraSysExApplication::getApp().getMainMenu()->createEditMenu(menu);
		menu.showMenuAsync(PopupMenu::Options());
	}

	// TODO integrate interaction through LumatoneController

	//else
	//{
	//	// NoteOn MIDI message
	//	auto keyData = getKeyData();
	//	if (keyData != nullptr && keyData->channelNumber > 0)
	//	{
	//		if (keyData->keyType == LumatoneKeyType::noteOnNoteOff)
	//		{
	//			// Send "note on" event
	//			//TerpstraSysExApplication::getApp().getMidiDriver().sendNoteOnMessage(keyData->noteNumber, keyData->channelNumber, 60);
	//		}
	//		// ToDo if keyType is "continuous controller": send controller event?
	//	}
	//}
}

void KeyMiniDisplayInsideAllKeysOverview::mouseUp(const MouseEvent& e)
{
	Component::mouseDown(e);

	isHighlighted = false;
	repaint();

	// TODO integrate interaction through LumatoneController

	//// NoteOff MIDI message
	//auto keyData = getKeyData();
	//if (keyData != nullptr && keyData->channelNumber > 0)
	//{
	//	if (keyData->keyType == TerpstraKey::noteOnNoteOff)
	//	{
	//		// Send "note off" event
	//		TerpstraSysExApplication::getApp().getMidiDriver().sendNoteOffMessage(keyData->noteNumber, keyData->channelNumber, 60);
	//	}
	//}
}

void KeyMiniDisplayInsideAllKeysOverview::handleMidiMessage(const MidiMessage& msg)
{
	// auto keyData = getKeyData();
	// if (keyData != nullptr && msg.getChannel() == keyData.channelNumber && msg.getNoteNumber() == keyData.noteNumber)
	// {
	// 	isHighlighted = msg.isNoteOn();
	// 	repaint();
	// }
}

//LumatoneKey KeyMiniDisplayInsideAllKeysOverview::getKeyData() const
//{
//	if (boardIndex >= 0 && boardIndex < MAXNUMBOARDS && keyIndex >= 0 && keyIndex < TerpstraSysExApplication::getApp().getOctaveBoardSize())
//	{
//		return TerpstraSysExApplication::getApp().getMappingData()->getBoard(boardIndex)->theKeys[keyIndex];
//	}
//
//	return LumatoneKey();
//}

Colour KeyMiniDisplayInsideAllKeysOverview::getKeyColour() const
{
	return TerpstraSysExApplication::getApp().getColourModel()->getModelColour(colour);
}

void KeyMiniDisplayInsideAllKeysOverview::setRenderMode(LumatoneComponentRenderMode modeIn)
{
	renderMode = modeIn;
}

void KeyMiniDisplayInsideAllKeysOverview::setKeyGraphics(Image colourGraphicIn, Image shadowGraphicIn)
{
	colourGraphic = colourGraphicIn;
	shadowGraphic = shadowGraphicIn;
}

//[/MiscUserDefs]

//==============================================================================
AllKeysOverview::AllKeysOverview ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("AllKeysOverview");
    btnLoadFile.reset (new juce::TextButton ("btnLoadFile"));
    addAndMakeVisible (btnLoadFile.get());
    btnLoadFile->setButtonText (TRANS("LoadFile"));
    btnLoadFile->addListener (this);

    btnLoadFile->setBounds (368, 8, 96, 24);

    btnSaveFile.reset (new juce::TextButton ("btnSaveFile"));
    addAndMakeVisible (btnSaveFile.get());
    btnSaveFile->setButtonText (TRANS("SaveFile"));
    btnSaveFile->addListener (this);

    btnSaveFile->setBounds (472, 8, 96, 24);

    buttonReceive.reset (new juce::TextButton ("buttonReceive"));
    addAndMakeVisible (buttonReceive.get());
    buttonReceive->setTooltip (TRANS("ImportTooltip"));
    buttonReceive->setButtonText (TRANS("Import from Lumatone"));
    buttonReceive->addListener (this);

    buttonReceive->setBounds (584, 8, 176, 24);


    //[UserPreSize]
	btnLoadFile->getProperties().set(LumatoneEditorStyleIDs::textButtonIconHashCode, LumatoneEditorIcon::LoadIcon);
	btnSaveFile->getProperties().set(LumatoneEditorStyleIDs::textButtonIconHashCode, LumatoneEditorIcon::SaveIcon);
	buttonReceive->getProperties().set(LumatoneEditorStyleIDs::textButtonIconHashCode, LumatoneEditorIcon::ArrowUp);
	buttonReceive->getProperties().set(LumatoneEditorStyleIDs::textButtonIconPlacement, LumatoneEditorStyleIDs::TextButtonIconPlacement::RightOfText);

	lblFirmwareVersion.reset(new Label("FirmwareVersionLabel"));
	addChildComponent(lblFirmwareVersion.get());

	TerpstraSysExApplication::getApp().getLumatoneController()->addStatusListener(this);
	TerpstraSysExApplication::getApp().getLumatoneController()->addFirmwareListener(this);

	renderMode = LumatoneComponentRenderMode::Graphic;
	resetOctaveSize();

    //[/UserPreSize]

    setSize (928, 214);


    //[Constructor] You can add your own custom stuff here..
	currentSetSelection = -1;
    buttonReceive->setVisible(false);
    showDeveloperMode(TerpstraSysExApplication::getApp().getPropertiesFile()->getBoolValue("DeveloperMode", false));
    //[/Constructor]
}

AllKeysOverview::~AllKeysOverview()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    btnLoadFile = nullptr;
    btnSaveFile = nullptr;
    buttonReceive = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	//imageProcessor = nullptr;
    //[/Destructor]
}

//==============================================================================
void AllKeysOverview::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..

	switch (renderMode)
	{
	case LumatoneComponentRenderMode::Graphic:
	case LumatoneComponentRenderMode::GraphicInteractive:
		g.drawImageAt(lumatoneGraphic, lumatoneBounds.getX(), lumatoneBounds.getY());
		break;

	case LumatoneComponentRenderMode::MaxRes:
		if (currentRender.isValid())
			g.drawImageAt(currentRender, lumatoneBounds.getX(), lumatoneBounds.getY());
		else if (lumatoneGraphic.isValid())
			g.drawImageAt(lumatoneGraphic, lumatoneBounds.getX(), lumatoneBounds.getY());
		break;
	}

	// Draw a line under the selected sub board
	if (currentSetSelection >= 0 && currentSetSelection < MAXNUMBOARDS)
	{
		Path selectionMarkPath;
		selectionMarkPath.startNewSubPath(octaveBoards[currentSetSelection]->leftPos, octaveLineY);
		selectionMarkPath.lineTo(octaveBoards[currentSetSelection]->rightPos, octaveLineY);

		Colour lineColour = findColour(TerpstraKeyEdit::outlineColourId);
		g.setColour(lineColour);
		g.strokePath(selectionMarkPath, PathStrokeType(1.0f));
	}

    //[/UserPaint]
}

void AllKeysOverview::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

	// Prepare position helpers for graphics
	currentHeight = roundToInt(getHeight() * imageHeight);
	currentWidth = roundToInt(imageAspect * currentHeight);

	lumatoneBounds.setBounds(
		roundToInt((getWidth() - currentWidth) * 0.5f), roundToInt(getHeight() * imageY),
		currentWidth, currentHeight
	);

	int btnHeight = roundToInt(getHeight() * saveLoadH);
	int btnMargin = roundToInt(getWidth() * saveloadMarginW);
	int saveLoadWidth = roundToInt(getWidth() * saveLoadW);
	int btnY = lumatoneBounds.getY() - roundToInt(getHeight() * btnYFromImageTop);

	int halfWidthX = roundToInt(getWidth() * 0.5f);

	btnLoadFile->setBounds(halfWidthX - btnMargin - saveLoadWidth, btnY, saveLoadWidth, btnHeight);
	btnSaveFile->setBounds(halfWidthX + btnMargin, btnY, saveLoadWidth, btnHeight);

	octaveLineY = lumatoneBounds.getBottom() + roundToInt(getHeight() * octaveLineYRatio);

	int importY = lumatoneBounds.getY() - roundToInt(getHeight() * importYFromImageTop);
	int importWidth = roundToInt(getWidth() * importW);
	buttonReceive->setBounds(lumatoneBounds.getRight() - importWidth, importY, importWidth, btnHeight);

	resizeLabelWithHeight(lblFirmwareVersion.get(), btnHeight * 0.6f);
	lblFirmwareVersion->setTopLeftPosition(lumatoneBounds.getX(), lumatoneBounds.getY() - btnHeight * 0.6f);

	keyWidth = roundToInt(lumatoneBounds.getWidth() * keyW);
	keyHeight = roundToInt(lumatoneBounds.getHeight() * keyH);

	switch (renderMode)
	{
	default:
	case LumatoneComponentRenderMode::Graphic:
	case LumatoneComponentRenderMode::GraphicInteractive:
		lumatoneGraphic = lumatoneRender.getResizedAsset(LumatoneAssets::ID::LumatoneGraphic, lumatoneBounds.getWidth(), lumatoneBounds.getHeight());
		keyShapeGraphic = lumatoneRender.getResizedAsset(LumatoneAssets::ID::KeyShape, keyWidth, keyHeight);
		keyShadowGraphic = lumatoneRender.getResizedAsset(LumatoneAssets::ID::KeyShadow, keyWidth, keyHeight);
		break;
	case LumatoneComponentRenderMode::MaxRes:
		currentRender = lumatoneRender.getResizedRender(lumatoneBounds.getWidth(), lumatoneBounds.getHeight());
		break;
	}

	int octaveIndex = 0;
	octaveBoards[octaveIndex]->leftPos = keyCentres[0].getX() * lumatoneBounds.getWidth() + lumatoneBounds.getX() - keyWidth * 0.5f;

	for (int keyIndex = 0; keyIndex < keyCentres.size(); keyIndex++)
	{
		int keyOctaveIndex = keyIndex % TerpstraSysExApplication::getApp().getOctaveBoardSize();

		juce::Point<int> keyPos = juce::Point<int>(
            (int)(keyCentres[keyIndex].x * lumatoneBounds.getWidth() + lumatoneBounds.getX() - keyWidth * 0.5f),
            (int)(keyCentres[keyIndex].y * lumatoneBounds.getHeight() + lumatoneBounds.getY() - keyHeight * 0.5f)
        );

        auto key = octaveBoards[octaveIndex]->keyMiniDisplay[keyOctaveIndex];
        key->setKeyGraphics(keyShapeGraphic, keyShadowGraphic);
        key->setBounds(keyPos.x, keyPos.y, keyWidth, keyHeight);

		if (keyOctaveIndex + 1 == TerpstraSysExApplication::getApp().getOctaveBoardSize())
		{
			octaveBoards[octaveIndex]->rightPos = key->getRight();
			octaveIndex++;

			if (octaveIndex < MAXNUMBOARDS)
				octaveBoards[octaveIndex]->leftPos = key->getX();
		}
	}

    //[/UserResized]
}

void AllKeysOverview::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnLoadFile.get())
    {
        //[UserButtonCode_btnLoadFile] -- add your button handler code here..
		TerpstraSysExApplication::getApp().openSysExMapping();
        //[/UserButtonCode_btnLoadFile]
    }
    else if (buttonThatWasClicked == btnSaveFile.get())
    {
        //[UserButtonCode_btnSaveFile] -- add your button handler code here..
		TerpstraSysExApplication::getApp().saveSysExMappingAs();
        //[/UserButtonCode_btnSaveFile]
    }
    else if (buttonThatWasClicked == buttonReceive.get())
    {
        //[UserButtonCode_buttonReceive] -- add your button handler code here..
		TerpstraSysExApplication::getApp().requestConfigurationFromDevice();
        //[/UserButtonCode_buttonReceive]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void AllKeysOverview::setFirmwareVersion(FirmwareVersion versionIn)
{
	if (versionIn.isValid())
	{
		if (versionIn.major == 0 && versionIn.minor == 0)
		{
			if (versionIn.revision == 55)
			{
				lblFirmwareVersion->setText("55-keys Prototype", NotificationType::dontSendNotification);
			}
		}
		else
		{
#if JUCE_DEBUG
			lblFirmwareVersion->setText("Firmware version: " + versionIn.toString(), NotificationType::dontSendNotification);
#else
			lblFirmwareVersion->setText("Firmware version: " + versionIn.toDisplayString(), NotificationType::dontSendNotification);
#endif
		}

		lblFirmwareVersion->setVisible(true);
		resized();
	}
    else
    {
        lblFirmwareVersion->setVisible(false);
    }

    repaint();
}

void AllKeysOverview::showDeveloperMode(bool developerModeOn)
{
	if (developerModeOn)
		buttonReceive->setVisible(true);

    repaint();
}

void AllKeysOverview::connectionEstablished(int, int)
{
	buttonReceive->setVisible(true);
}

void AllKeysOverview::connectionLost()
{
	buttonReceive->setVisible(false);
}

void AllKeysOverview::firmwareRevisionReceived(FirmwareVersion version)
{
	setFirmwareVersion(version);
}

void AllKeysOverview::resetOctaveSize()
{
	int octaveSize = TerpstraSysExApplication::getApp().getOctaveBoardSize();
	if (currentOctaveSize != octaveSize)
	{
		octaveBoards.clear();

		for (int subBoardIndex = 0; subBoardIndex < MAXNUMBOARDS; subBoardIndex++)
		{
			OctaveBoard* board = octaveBoards.add(new OctaveBoard());

			for (int keyIndex = 0; keyIndex < octaveSize; keyIndex++)
			{
				auto keyData = TerpstraSysExApplication::getApp().getMappingData()->readKey(subBoardIndex, keyIndex);
				auto keyDisplay= board->keyMiniDisplay.add(new KeyMiniDisplayInsideAllKeysOverview(subBoardIndex, keyIndex, *keyData));
				keyDisplay->setRenderMode(renderMode);
				addAndMakeVisible(keyDisplay);
			}

			jassert(board->keyMiniDisplay.size() == octaveSize);
		}

		currentOctaveSize = octaveSize;
	}

	jassert(octaveBoards.size() == MAXNUMBOARDS);

	lumatoneRender.resetOctaveSize();
	keyCentres = lumatoneRender.getKeyCentres();
	resized();
}

void AllKeysOverview::refreshMappingData(int boardIndex, int keyIndex, bool repaint)
{
	const LumatoneKey* key = TerpstraSysExApplication::getApp().getMappingData()->readKey(boardIndex, keyIndex);
	KeyMiniDisplayInsideAllKeysOverview* keyDisplay = octaveBoards[boardIndex]->keyMiniDisplay[keyIndex];
	*(LumatoneKey*)keyDisplay = *key;

	if (repaint)
	{
		if (renderMode == LumatoneComponentRenderMode::MaxRes)
		{
			rerender();
		}
		else
			keyDisplay->repaint();
	}
}

void AllKeysOverview::refreshMappingData(int boardIndex, bool repaint)
{
	for (int i = 0; i < currentOctaveSize; i++)
		refreshMappingData(boardIndex, i, repaint);
}

void AllKeysOverview::refreshMappingData()
{
	bool doRepaint = renderMode != LumatoneComponentRenderMode::MaxRes;

	for (int i = 0; i < TerpstraSysExApplication::getApp().getNumBoards(); i++)
		refreshMappingData(i, doRepaint);
	
	if (renderMode == LumatoneComponentRenderMode::MaxRes)
	{
		rerender();
	}
}

void AllKeysOverview::setRenderMode(LumatoneComponentRenderMode modeIn)
{
	renderMode = modeIn;
	for (int boardIndex = 0; boardIndex < octaveBoards.size(); boardIndex++)
	{
		auto board = octaveBoards[boardIndex];

		for (int keyIndex = 0; keyIndex < board->keyMiniDisplay.size(); keyIndex++)
		{
			auto key = board->keyMiniDisplay[keyIndex];
			key->setRenderMode(renderMode);
		}
	}

	if (renderMode == LumatoneComponentRenderMode::MaxRes)
		lumatoneRender.render();

	if (currentWidth == 0 || currentHeight == 0)
		return;

	resized();
}

void AllKeysOverview::rerender()
{
	lumatoneRender.render();
	currentRender = lumatoneRender.getResizedRender(lumatoneBounds.getWidth(), lumatoneBounds.getHeight());
	repaint(lumatoneBounds);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AllKeysOverview" componentName="AllKeysOverview"
                 parentClasses="public juce::Component, public LumatoneEditor::StatusListener, public LumatoneEditor::FirmwareListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="928"
                 initialHeight="214">
  <BACKGROUND backgroundColour="0"/>
  <TEXTBUTTON name="btnLoadFile" id="6c0c074c9f137f23" memberName="btnLoadFile"
              virtualName="" explicitFocusOrder="0" pos="368 8 96 24" buttonText="Load File"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnSaveFile" id="abbc33d699ba1e52" memberName="btnSaveFile"
              virtualName="" explicitFocusOrder="0" pos="472 8 96 24" buttonText="Save File"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonReceive" id="6a7ed19ee86a3b97" memberName="buttonReceive"
              virtualName="" explicitFocusOrder="0" pos="584 8 176 24" tooltip="Receive current layout from connected Lumatone"
              buttonText="Import from Lumatone" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...

//[/EndFile]