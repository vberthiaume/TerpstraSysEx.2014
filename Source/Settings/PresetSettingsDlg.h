/*
  ==============================================================================

    PresetSettingsDlg.h
    Created: 29 Jun 2021 9:10:00pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "../LumatoneEditorState.h"

#include "../lumatone_editor_library/listeners/firmware_listener.h"
#include "../lumatone_editor_library/lumatone_midi_driver/firmware_types.h"

class PresetSettingsDlg : public Component
                        , LumatoneEditorState
                        , protected Button::Listener

{
public:

    PresetSettingsDlg(const LumatoneEditorState& stateIn);
    ~PresetSettingsDlg();

    void paint(juce::Graphics& g) override;

    void resized() override;

    void buttonClicked(juce::Button* btn) override;

    void setSupportedControls(LumatoneFirmware::Version version);

	  //==============================================================================
	  // Implementation of LumatoneEditorState

	  void handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier& property) override;


private:

    std::unique_ptr<juce::TextButton> resetPresetsBtn;
    FlexBox flexBox;

    // Style helpers
    int margin = 12;
    int buttonHeight = 30;
};
