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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>

#include "lumatone_render.h"
#include "LumatoneController.h"

enum class LumatoneComponentRenderMode
{
	NoDisplay = -1,

	Shape = 0x000010,
	ShapeInteractive = 0x000011,

	Graphic = 0x000020,
	GraphicInteractive = 0x000021,

	MaxRes = 0x000030,
};

// Representation of a key inside the overview
class KeyMiniDisplayInsideAllKeysOverview : public MappedLumatoneKey, public Component, public LumatoneEditor::MidiListener
{
public:
	KeyMiniDisplayInsideAllKeysOverview(int newBoardIndex, int newKeyIndex, const LumatoneKey& keyDataIn);
	~KeyMiniDisplayInsideAllKeysOverview();

	void paint(Graphics&) override;
	void resized() override;
	void mouseDown(const MouseEvent& e) override;
	void mouseUp(const juce::MouseEvent& e) override;

	LumatoneComponentRenderMode getRenderMode() const { return renderMode; }
	void setRenderMode(LumatoneComponentRenderMode uiModeIn);

	juce::Colour getKeyColour() const;
	void setKeyGraphics(Image colourGraphicIn, Image shadowGraphicIn);

	// Implementation of TerpstraNidiDriver::Listener
	//void midiMessageReceived(const MidiMessage& midiMessage) override;
	//void midiMessageSent(const MidiMessage& midiMessage) override {}
	//void midiSendQueueSize(int queueSize) override {}
	//void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) override {}
	void handleMidiMessage(const MidiMessage& msg) override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyMiniDisplayInsideAllKeysOverview)

	LumatoneComponentRenderMode renderMode;

	bool isHighlighted = false;

	juce::Image colourGraphic;
	juce::Image shadowGraphic;
};

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class AllKeysOverview  : public juce::Component,
                         public LumatoneEditor::StatusListener,
                         public LumatoneEditor::FirmwareListener,
                         public juce::Button::Listener
{
public:
    //==============================================================================
    AllKeysOverview ();
    ~AllKeysOverview() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

	int getCurrentSetSelection() const { return currentSetSelection ;}
	void setCurrentSetSelection(int newSetSelection) { currentSetSelection = newSetSelection; repaint(); }

    void showDeveloperMode(bool developerModeOn);

    void setFirmwareVersion(FirmwareVersion versionIn);

	void resetOctaveSize();

	void refreshMappingData(int boardIndex, int keyIndex, bool repaint=true);
	void refreshMappingData(int boardIndex, bool repaint = true);
	void refreshMappingData();

	LumatoneComponentRenderMode getRenderMode() const { return renderMode; }
	void setRenderMode(LumatoneComponentRenderMode modeIn);

	void rerender();

	// LumatoneEditor::StatusListener
	void connectionEstablished(int, int) override;
	void connectionLost() override;

	// LumatoneEditor::FirmwareListener implementation
	void firmwareRevisionReceived(FirmwareVersion version) override;

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.

	struct OctaveBoard
	{
		OwnedArray<KeyMiniDisplayInsideAllKeysOverview>	keyMiniDisplay;
		int leftPos;
		int rightPos;
	};

	OwnedArray<OctaveBoard> octaveBoards;

	int			currentOctaveSize = 0;
	int			currentSetSelection;

	LumatoneComponentRenderMode renderMode;
	LumatoneRender lumatoneRender;

    std::unique_ptr<Label> lblFirmwareVersion;

	//==============================================================================
	// Style helpers

    int currentWidth = 0;
    int currentHeight = 0;

	Rectangle<int> lumatoneBounds;
	int octaveLineY = 0;

 	int keyWidth = 0;
    int keyHeight = 0;

    juce::Array<juce::Point<float>> keyCentres;

	juce::Image lumatoneGraphic;
	juce::Image keyShapeGraphic;
	juce::Image keyShadowGraphic;
	
	juce::Image currentRender;

	//==============================================================================
	// Position and sizing constants in reference to parent bounds

	const float imageAspect = 2.498233f;
	const float imageY      = 1.0f / 7.0f;
	const float imageHeight = 5.0f / 7.0f;

	const float importYFromImageTop = 0.0752688f;
	const float importH             = 0.0526882f;
	const float importW             = 0.132f;

	const float btnYFromImageTop    = 0.04172043f;
	const float saveLoadW           = 0.07416f;
	const float saveLoadH           = 0.0537634f;
	const float saveloadMarginW     = 0.0034f;

	const float filenameBaselineY   = 0.1221505f;

	const float octaveLineYRatio    = 0.0236559f;

	// In reference to lumatoneBounds
	const float keybedX = 0.06908748f;

	const float keyW = 0.027352f;
	const float keyH = 0.07307f;


	//===============================================================================

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> btnLoadFile;
    std::unique_ptr<juce::TextButton> btnSaveFile;
    std::unique_ptr<juce::TextButton> buttonReceive;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AllKeysOverview)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
