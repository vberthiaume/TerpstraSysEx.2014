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

#include "./LumatoneEditorState.h"

#include "./lumatone_editor_library/listeners/status_listener.h"
#include "./colour_view_component.h"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class GlobalSettingsArea  : public juce::Component,
                            public LumatoneEditorState,
                            public ChangeListener,
                            public LumatoneEditor::StatusListener,
                            public juce::Button::Listener
{
public:
    //==============================================================================
    GlobalSettingsArea (const LumatoneEditorState& stateIn);
    ~GlobalSettingsArea() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void changeListenerCallback(ChangeBroadcaster *source) override;

	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);

    void listenToColourEditButtons(Button::Listener* listenerIn);

    void lookAndFeelChanged() override;

    void setDeveloperMode(bool devModeOn);

    // LumatoneEditor::StatusListener implementation
    // void connectionEstablished(int inputDevice, int outputDevice) override;
    void connectionStateChanged(ConnectionState newState) override;
    // void connectionFailed() override;

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;

private:
    void restoreStateFromPropertiesFile();

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	std::unique_ptr<ColourViewComponent> inactiveMacroButtonColourEdit;
	std::unique_ptr<ColourViewComponent> activeMacroButtonColourEdit;

    std::unique_ptr<Label> lblDeveloperMode;

    bool showDeveloperMode = false;

    //==============================================================================
    // Style helpers
    FlexBox flexBox;

    //==============================================================================
    // Size and position constants

    const float colourButtonAspect          = 1.235294f;
    const float inactiveButtonAspect        = 4.695652f;

    const float controlsHeight              = 0.333333f;

    const float calibrateX                  = 0.25f;
    const float calibrateHeight             = 0.55f;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Label> lblPresetButtonColours;
    std::unique_ptr<juce::Label> lblColourInactiveMacroButton;
    std::unique_ptr<juce::Label> lblColourActiveMacroButton;
    std::unique_ptr<juce::TextButton> settingsButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlobalSettingsArea)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
