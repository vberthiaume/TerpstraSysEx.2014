/*
  ==============================================================================

    KeyEditorPanel.h
    Created: 3 June 2024
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_KEY_EDITOR_PANEL_H
#define LUMATONE_EDITOR_KEY_EDITOR_PANEL_H

// #include "./lumatone_editor_library/listeners/editor_listener.h"
#include "../../data/LumatoneEditorState.h"

class KeyEditorControls;
class MultiSelectControls;
class BatchToolsPanel;

class KeyEditorPanel : public juce::Component
                     , public LumatoneEditorState
{
public:
    KeyEditorPanel(const LumatoneEditorState& stateIn);
    ~KeyEditorPanel() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:

    std::unique_ptr<KeyEditorControls>    keyEditorControls;
    std::unique_ptr<MultiSelectControls>  multiSelectControls;
    std::unique_ptr<BatchToolsPanel>      batchTools;

    juce::Rectangle<float>  keySettingsAndMultiSelectArea;
    juce::Rectangle<float>  keySettingsArea;
    juce::Rectangle<float>  multiSelectArea;
    juce::Rectangle<float>  batchToolsArea;

    const float areaHeightRatio = 0.865f;
    const float keySettingsAndSelectAreaW     = 0.71f;
    const float batchToolsAreaW               = 0.27f;

    const float keySettingsComponentW         = 0.471f;
    const float batchToolsComponentW          = 0.232f;

    const float keySettingsComponentAreaW     = 0.66f;
    const float multiSelectComponentAreaW     = 0.253f;

    int areaHeight;
    int areaMargin;
    int contentMargin;
    int contentHeight;
    int multiSelectWidth;

    // const float contentHeightRatio    = 0.81f;

    const float controlsWidthRatio          = 0.466f;
    const float curvesAreaWidthRatio        = 0.512f;

};

#endif //LUMATONE_EDITOR_KEY_EDITOR_PANEL_H
