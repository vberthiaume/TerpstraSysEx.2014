/*
  ==============================================================================

    MappingSettingsControls.h
    Created: 15 Jan 2023
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_MAPPING_SETTINGS_COMPONENT_H
#define LUMATONE_EDITOR_MAPPING_SETTINGS_COMPONENT_H

#include "./lumatone_editor_library/listeners/editor_listener.h"
#include "LumatoneEditorState.h"

class MappingSettingsControls : public juce::Component
                               , public LumatoneEditorState
                               , public LumatoneEditor::EditorListener
{
public:
    MappingSettingsControls(const LumatoneEditorState& stateIn);
    ~MappingSettingsControls() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

	  void loadFromMapping();

    // LumatoneEditor::EditorListener Implementation
    void lightOnKeyStrokesChanged(bool lightOn) override;
    void aftertouchToggled(bool enabled) override;

    void expressionPedalSensitivityChanged(unsigned char value) override;
    void invertFootControllerChanged(bool inverted) override;

    void invertSustainToggled(bool inverted) override;

    void firmwareVersionChanged() override;

private:

    void exprSensitivitySliderCallback();

private:
    int roundedCornerSize = 0;
    juce::Rectangle<float> expressionBounds;
    juce::Rectangle<float> sustainBounds;
    const float sectionMarginWidth = 0.05f;

    //==============================================================================
    std::unique_ptr<juce::Label> labelGeneralSettingslTitle;
    std::unique_ptr<juce::ToggleButton> buttonAfterTouchActive;
    std::unique_ptr<juce::ToggleButton> buttonLightOnKeyStrokes;

    std::unique_ptr<juce::Label> labelExprContrSensitivity;
    std::unique_ptr<juce::ToggleButton> btnInvertExpression;
    std::unique_ptr<juce::Label> lblExpression;
    std::unique_ptr<juce::Slider> sldExprCtrlSensitivity;
    std::unique_ptr<juce::ToggleButton> btnInvertSustain;
    std::unique_ptr<juce::Label> lblSustain;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MappingSettingsControls)
};

#endif // LUMATONE_EDITOR_MAPPING_SETTINGS_COMPONENT_H
