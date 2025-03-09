/*
  ==============================================================================

    KeyEditorControls.h
    Created: 3 June 2024
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_KEY_EDITOR_CONTROLS_H
#define LUMATONE_EDITOR_KEY_EDITOR_CONTROLS_H

#include "../../data/LumatoneEditorState.h"
#include "../../lumatone_editor_library/listeners/editor_listener.h"
#include "../../lumatone_editor_library/palettes/colour_selection_broadcaster.h"

class ColourSelectionGroup;
class ColourTextEditor;
class ColourViewComponent;
class RangedControl;
class ColourSelectorPanel;
class ColourDropdownSelector;
// class SelectionTabBar;
// class SingleSelectControls;
// class MultiSelectControls;

// enum class SelectionTabs;

class KeyEditorControls : public juce::Component
                        , public LumatoneEditorState
                        , private LumatoneEditorState::Controller
                        , public LumatoneEditor::EditorListener
                        , public ColourSelectionListener
                        , public ColourSelectionBroadcaster
                        , public juce::ChangeListener
{
public:
    KeyEditorControls(const LumatoneEditorState& stateIn);

    virtual ~KeyEditorControls() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void selectionChanged() override;

    // void setSelectionTab(SelectionTabs tab);

private:

  void updateColourHistory(const juce::Colour& newColour);

private:

    void handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier& property) override;

    // Implementation of ColourSelectionListener
    void colourChangedCallback(ColourSelectionBroadcaster* source, juce::Colour newColour) override;

    // Implementation of ColourSelectionBroadcaster
    virtual juce::Colour getSelectedColour() override;
    virtual void deselectColour() override;

    // Implementation of ChangeListener
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;

private:
    std::unique_ptr<juce::Label>            lblKeySettings;

    std::unique_ptr<ColourTextEditor>       colourTextEditor;
    // std::unique_ptr<ColourViewComponent>    colourSubwindow;
    std::unique_ptr<ColourDropdownSelector> colourDropdown;
    juce::Array<Colour>                     colourHistory;
    juce::Colour                            lastSelectedColour;

    std::unique_ptr<juce::ComboBox>          keyTypeCombo;
    std::unique_ptr<RangedControl>           noteInput;
    std::unique_ptr<RangedControl>           channelInput;

    std::unique_ptr<juce::TextButton>       noteAutoIncButton;
    std::unique_ptr<juce::Slider>           noteAutoIncrInput;
    std::unique_ptr<juce::Slider>           channelAutoIncrNoteInput;

    std::unique_ptr<juce::Label>            lblColour;
    std::unique_ptr<juce::Label>            lblKeyType;
    std::unique_ptr<juce::Label>            lblNote;
    std::unique_ptr<juce::Label>            lblChannel;
    std::unique_ptr<juce::Label>            lblAutoIncNotes;
    std::unique_ptr<juce::Label>            lblAutoIncChannels;
    std::unique_ptr<juce::Label>            lblChannelAutoIncr;

    std::unique_ptr<ColourSelectorPanel>    colourPalettePanel;
    // std::unique_ptr<ColourSelectionGroup>   colourSelectionGroup;
    // ColourSelectionGroup*                   colourSelectionGroup;

    // std::unique_ptr<SelectionTabBar>        selectionTabBar;
    // std::unique_ptr<SingleSelectControls>   singleSelectControls;
    // std::unique_ptr<MultiSelectControls>    multiSelectControls;
    juce::Component* selectionControls = nullptr;

    const juce::StringRef noteInputWidthRef = "_1.6.0._+.+_";

    juce::Path headerPath;
    juce::Path controlPath;

    int contentMarginWidth;

    // int contentMarginWidth;
    // const float contentMarginParentW  = 0.02f;

    int contentMarginHeight;
    float controlMarginH        = 0.05f;

    int headerHeight;
    // const float headerH         = 0.19f;

    int labelHeight;
    // const float labelToHeaderH         = 0.5f;
    // const float controlLabelFontScalar = 0.6f;

    int keyControlColumnWidth;
    int keyControlColumnRight;
    float keyControlColumnW     = 0.313f;

    int controlRowHeight;
    int keyControlHeight;
    float keyControlH           = 0.128f;

    int keyControlMarginHeight;
    float keyControlMarginH     = 0.059f;

    int colourColumnX;
    int colourColumnWidth;
    float columnMarginW         = 0.0333f;

    int colourButtonMargin;
    int colourButtonWidth;
    float colourButtonParentW   = 0.031f;

    int colourColumnHeight;
    float colourColumnH         = 0.6f;

    int colourPanelFix = 0;
};

#endif // LUMATONE_EDITOR_KEY_EDITOR_CONTROLS_H
