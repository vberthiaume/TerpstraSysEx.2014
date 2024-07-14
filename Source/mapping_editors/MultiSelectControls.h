/*
  ==============================================================================

    MultiSelectControls.h
    Created: 3 June 2024
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_MULTI_SELECT_CONTROLS_H
#define LUMATONE_EDITOR_MULTI_SELECT_CONTROLS_H

#include "../LumatoneEditorState.h"
#include "../lumatone_editor_library/listeners/editor_listener.h"

class ColourViewComponent;
class ColourTextEditor;
class RangedControl;

class ColourDropdownSelector;

class MultiSelectControls : public LumatoneEditorState
                          , public LumatoneEditorState::Controller
                          , public juce::Component
                          , protected LumatoneEditor::EditorListener
{
public:
    MultiSelectControls(const LumatoneEditorState& stateIn);

    virtual ~MultiSelectControls() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void completeMappingLoaded(const LumatoneLayout& mappingData) override;
    void boardChanged(const LumatoneBoard& boardData) override;
    void keyChanged(int boardIndex, int keyIndex, const LumatoneKey& lumatoneKey) override;
    void keySetChanged(juce::Array<MappedLumatoneKey> selection) override;

private:

    std::unique_ptr<juce::Label>    lblMultiSelect;

    std::unique_ptr<ColourTextEditor>       colourTextEditor;
    std::unique_ptr<ColourViewComponent>    colourSubwindow;
    std::unique_ptr<ColourDropdownSelector> colourDropdown;

    std::unique_ptr<juce::ComboBox>   keyTypeCombo;
    std::unique_ptr<RangedControl>    noteInput;
    std::unique_ptr<RangedControl>    channelInput;

    std::unique_ptr<juce::Label>    lblColour;
    std::unique_ptr<juce::Label>    lblKeyType;
    std::unique_ptr<juce::Label>    lblNote;
    std::unique_ptr<juce::Label>    lblChannel;


    const float headerH         = 0.19f;
    int headerHeight;
    juce::Path headerPath;
    juce::Path controlPath;

    int contentMarginWidth;
    const float contentMarginParentW   = 0.02f;

    int contentMarginHeight;
    const float controlMarginH        = 0.08f;

    int labelHeight;
    int labelMarginWidth;
    const float labelToHeaderH        = 0.5f;

    int colourTextBoxWidth;
    int colourButtonMargin;
    int colourButtonWidth;
    const float colourButtonParentW   = 0.03f;

    int controlHeight;
    const float controlH                = 0.13f;

    int controlLabelHeight;
    const float controlLabelH           = 0.12f;
    const float controlLabelFontScalar  = 0.66f;

    int controlMarginHeight;
    // const float controlMarginH          = 0.08f;

    int colourTypeColumnWidth;
    const float colourTypeColumnW       = 0.43f;

    int noteChannelColumnX;
    int noteChannelColumnWidth;
    const float noteChannelColumnW      = 0.27f;
};

#endif // LUMATONE_EDITOR_MULTI_SELECT_CONTROLS_H
