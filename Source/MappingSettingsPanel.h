/*
  ==============================================================================

    MappingSettingsPanel.h
    Created: 14 May 2024
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_MAPPING_SETTINGS_PANEL_H
#define LUMATONE_EDITOR_MAPPING_SETTINGS_PANEL_H

// #include "./lumatone_editor_library/listeners/editor_listener.h"
#include "LumatoneEditorState.h"

class MappingSettingsControls;
class CurvesArea;

class MappingSettingsPanel : public juce::Component
                               , public LumatoneEditorState
                            //    , public LumatoneEditor::EditorListener
{
public:
    MappingSettingsPanel(const LumatoneEditorState& stateIn);
    ~MappingSettingsPanel() override;


    void paint(juce::Graphics& g) override;
    void resized() override;


private:

    std::unique_ptr<MappingSettingsControls>    settingsControls;
    std::unique_ptr<CurvesArea>                 curvesArea;


    int contentMarginY;
    int contentHeight;

    const float contentMarginHeightRatio    = 0.07f;

    const float controlsWidthRatio          = 0.466f;
    const float curvesAreaWidthRatio        = 0.512f;

};

#endif //LUMATONE_EDITOR_MAPPING_SETTINGS_PANEL_H
