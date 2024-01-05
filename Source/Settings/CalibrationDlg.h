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

#include "../LumatoneEditorState.h"
#include "../lumatone_editor_library/lumatone_midi_driver/firmware_types.h"
#include "../lumatone_editor_library/listeners/firmware_listener.h"

class WheelsCalibrationComponent;

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class CalibrationDlg  : public juce::Component,
                        public LumatoneEditorState,
                        public ChangeListener,
                        public LumatoneEditor::FirmwareListener,
                        public juce::Button::Listener
{
public:
    //==============================================================================
    CalibrationDlg (const LumatoneEditorState& stateIn);
    ~CalibrationDlg() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

	// Implementation of ChangeListener
	void changeListenerCallback(ChangeBroadcaster *source) override;

    void setupWheelCalibrationLayout();
    void updateWheelCalibrationStatus();

    //==============================================================================
    // LumatoneEditor::FirmwareListener Implementation

    void calibratePitchModWheelAnswer(LumatoneFirmware::ReturnCode code) override;

    void wheelsCalibrationDataReceived(LumatoneFirmware::WheelsCalibrationData calibrationData) override;


    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;
    void lookAndFeelChanged() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	enum calibrationMode
	{
		calibrateKeys = 0,
		calibrateAftertouch = 1,
		calibrateModulationWheel
	};

	std::unique_ptr<juce::TabbedButtonBar> calibrationSelectorTab;
    std::unique_ptr<WheelsCalibrationComponent> wheelsCalibrationComponent;

    bool startCalibration = false;

	juce::String instructionText;

	// Style Helpers
	juce::Rectangle<int> instructionsBounds;
    juce::Font instructionsFont;
	const float fontHeightInBounds = 0.125f;
	const int generalRim = 12;

    const float wheelsGraphicWidthScalar = 0.15f;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TextButton> btnStart;
    std::unique_ptr<juce::TextButton> btnStop;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CalibrationDlg)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
