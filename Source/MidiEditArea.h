/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>

#include "LumatoneEditorState.h"

#include "./lumatone_editor_library/listeners/status_listener.h"
#include "./lumatone_editor_library/listeners/editor_listener.h"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MidiEditArea  : public juce::Component
                    , public LumatoneEditorState
                    , private LumatoneEditorState::Controller
                    , private LumatoneApplicationState::DeviceController
                    , public LumatoneEditor::StatusListener
                    , public LumatoneEditor::EditorListener
                    , public juce::ComboBox::Listener
                    , public juce::Timer
{
public:
    //==============================================================================
    MidiEditArea (const LumatoneEditorState& stateIn);
    ~MidiEditArea() override;

    bool isConnected() const;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void lookAndFeelChanged() override;

	void onOpenConnectionToDevice(String dialogTitle = "");

    void refreshInputMenuAndSetSelected(int inputDeviceIndex, juce::NotificationType notificationType = NotificationType::sendNotification);
    void refreshOutputMenuAndSetSelected(int outputDeviceIndex, juce::NotificationType notificationType = NotificationType::sendNotification);

	// Implementation of LumatoneEditor::StatusListener
    void connectionFailed() override;
    void connectionStateChanged(ConnectionState state) override;

    // Implementation of LumatoneEditor::EditorListener
    void editorModeChanged(EditorMode editModeIn);

    void timerCallback() override;

private:
    void setConnectivity(bool isConnected, juce::String connectionStatus=String());

    void toggleAutoConnection();

    void editModeChangedCallback();

public:
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;

private:

    bool                        isWaitingForConnectionTest = false;
    bool                        isWaitingForUserChoice = false;

    std::unique_ptr<juce::Label>      lumatoneLabel;

    std::unique_ptr<juce::TextButton> liveEditorBtn;
    std::unique_ptr<juce::TextButton> offlineEditorBtn;

    std::unique_ptr<juce::Label>      pleaseConnectLabel;
    std::unique_ptr<juce::Label>      offlineMsgLabel;

    std::unique_ptr<juce::Component>  logomark;
    juce::Path                        logomarkPath;

    const int                   deviceRefreshTimeoutMs = 500;

    //==============================================================================
    // Helpers

    //std::unique_ptr<AlertWindow>     alert;

    juce::FlexBox          ioAreaFlexBox;

    juce::Rectangle<int>   lumatoneLabelBounds;
    juce::Rectangle<float> connectivityArea;
    juce::Rectangle<float> logomarkBounds;
    juce::Rectangle<float> ioBounds;

    juce::Array<juce::Colour> connectedColours = { juce::Colour(0xffd7002a), juce::Colour(0xff84aea3) };

    //==============================================================================
    // Position & Size constants
    const float lumatoneLabelAreaWidth          = 0.1579f;
    const float lumatoneLabelWidthInArea        = 0.6667f;

    const float pleaseConnectX                  = 0.17f;
    const float pleaseConnectY                  = 0.28f;

    const float pleaseConnectHeight
#if JUCE_MAC
        = 0.162f;
#else
        = 0.17f;
#endif

    const float connectionDirectionsX           = 0.17f;
    const float connectionDirectionsY           = 0.47f;
    const float connectionDirectionsHeight
#if JUCE_MAC
        = 0.19f;
#else
        = 0.18f;
#endif

    const float editModeX                       = 0.0267f;
    const float editModeHeight                  = 0.24f;

    const float editModeButtonX                 = 0.10606f;
    const float editModeButtonHeight            = 0.4194f;
    const float editModeFontScalar              = 0.42857f;

    const float liveEditButtonWidth             = 0.0804f;
    const float offlineEditButtonWidth          = 0.0862f;

    const float controlBoundsX                  = 0.56;
    const float controlBoundsY                  = 0.2857f;
    const float controlBoundsWidth              = 0.3684f;
    const float controlBoundsHeight             = 0.4375f;
    const float controlBoundsCornerRadius       = 0.1273f;

    const float midiDeviceControlBoundsWidth    = 0.2955f;
    const float midiDeviceControlBoundsHeight   = 0.66f;

    const float disconnectedAreaX               = 0.5f;
    const float connectedAreaX                  = 0.8387f;

    const float controlBoundsMarginScalar       = 0.0325f;
    const float connectivityHeight              = 0.1957f;

    const float logomarkHeight                  = 0.5f;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::ComboBox> cbMidiInput;
    std::unique_ptr<juce::ComboBox> cbMidiOutput;
    std::unique_ptr<juce::Label> lblConnectionState;
    std::unique_ptr<juce::Label> lblEditMode;
    std::unique_ptr<juce::TextButton> btnAutoConnect;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiEditArea)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
