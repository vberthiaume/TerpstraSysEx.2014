/*
  ==============================================================================

    KeyEditorControls.h
    Created: 3 June 2024
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_KEY_EDITOR_CONTROLS_H
#define LUMATONE_EDITOR_KEY_EDITOR_CONTROLS_H

#include "../LumatoneEditorState.h"
#include "../lumatone_editor_library/listeners/editor_listener.h"

#include "../lumatone_editor_library/palettes/colour_selection_broadcaster.h"

class ColourSelectionGroup;
class ColourTextEditor;
class ColourViewComponent;
class RangedControl;
class ColourPaletteWindow;

class KeyEditorControls : public juce::Component
                        , public LumatoneEditorState
                        , private LumatoneEditorState::Controller
                        , public LumatoneEditor::EditorListener
                        , public ColourSelectionListener
{
public:
    KeyEditorControls(const LumatoneEditorState& stateIn);

    virtual ~KeyEditorControls() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void selectionChanged() override;

private:

    void handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier& property) override;

    void colourChangedCallback(ColourSelectionBroadcaster* source, juce::Colour newColour) override;

private:
    std::unique_ptr<juce::Label>            lblKeySettings;

    std::unique_ptr<ColourTextEditor>       colourTextEditor;
    std::unique_ptr<ColourViewComponent>    colourSubwindow;

    std::unique_ptr<juce::ComboBox>         keyTypeCombo;
    std::unique_ptr<RangedControl>           noteInput;
    std::unique_ptr<RangedControl>           channelInput;

    std::unique_ptr<juce::Label>            lblColour;
    std::unique_ptr<juce::Label>            lblKeyType;
    std::unique_ptr<juce::Label>            lblNote;
    std::unique_ptr<juce::Label>            lblChannel;

    std::unique_ptr<ColourPaletteWindow>    colourPalettePanel;
    // std::unique_ptr<ColourSelectionGroup>   colourSelectionGroup;
    ColourSelectionGroup*                   colourSelectionGroup;

    juce::Path headerPath;
    juce::Path controlPath;

    int contentMarginWidth;

    // int contentMarginWidth;
    // const float contentMarginParentW  = 0.02f;

    int contentMarginHeight;
    const float controlMarginH        = 0.08f;

    int headerHeight;
    // const float headerH         = 0.19f;

    int labelHeight;
    // const float labelToHeaderH         = 0.5f;
    // const float controlLabelFontScalar = 0.6f;

    int keyControlColumnWidth;
    int keyControlColumnRight;
    const float keyControlColumnW     = 0.3f;

    int keyControlHeight;
    const float keyControlH           = 0.128f;

    int keyControlMarginHeight;
    const float keyControlMarginH     = 0.059f;

    int colourColumnX;
    int colourColumnWidth;
    const float columnMarginW         = 0.055f;

    int colourButtonMargin;
    int colourButtonWidth;
    const float colourButtonParentW   = 0.031f;

    int colourColumnHeight;
    const float colourColumnH         = 0.6f;

};

#endif // LUMATONE_EDITOR_KEY_EDITOR_CONTROLS_H
