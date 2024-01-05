/*
  ==============================================================================

    MidiSettingsDlg.h
    Created: 29 Jun 2021 10:02:43pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../LumatoneEditorState.h"

#include "../lumatone_editor_library/listeners/firmware_listener.h"
#include "../lumatone_editor_library/lumatone_midi_driver/firmware_types.h"

class MidiSettingsDlg : public Component
                      , public LumatoneEditorState
                      , protected Slider::Listener
                      , protected LumatoneEditor::FirmwareListener
{
public:

    MidiSettingsDlg(const LumatoneEditorState& stateIn);
    ~MidiSettingsDlg();

    void paint(juce::Graphics & g) override;

    void resized() override;

    void sliderValueChanged(juce::Slider* sld) override;

    void setSupportedControls(LumatoneFirmware::Version version);

    void updateChannelSettings(LumatoneFirmware::PeripheralChannelSettings channelSettings);

    void sendChannelSettings();

    //=========================================================================
    // LumatoneEditor::FirmwareListener implementation

    void firmwareRevisionReceived(LumatoneFirmware::Version version) override;

    void peripheralMidiChannelsReceived(LumatoneFirmware::PeripheralChannelSettings channelSettings) override;



private:
    //=========================================================================

    const juce::StringArray ControlNames =
    {
        "Pitch Wheel",
        "Mod Wheel",
        "Expression Pedal",
        "Sustain Pedal"
    };

private:

    std::unique_ptr<juce::Label>    setMidiChannelHeader;

    juce::OwnedArray<juce::Slider>  setMidiChannelSliders;
    juce::OwnedArray<juce::Label>   setMidiChannelLabels;

    LumatoneFirmware:: PeripheralChannelSettings channelSettings;

    juce::FlexBox flexBox;
    juce::Array<juce::FlexBox> flexRows;

    // Style helpers
    const int margin = 12;
    const int buttonHeight = 30;
    const float fontHeightInBounds = 0.08f;

    juce::String longestControlName;
    juce::Font controlLabelFont;
};
