/*
  ==============================================================================

    MultiSelectControls.h
    Created: 3 June 2024
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_MULTI_SELECT_CONTROLS_H
#define LUMATONE_EDITOR_MULTI_SELECT_CONTROLS_H

#include "../../data/LumatoneEditorState.h"
#include "../../lumatone_editor_library/listeners/editor_listener.h"

// class ColourViewComponent;
class ColourTextEditor;
class LumatoneEditorControl;
class ColourDropdownSelector;

class MultiSelectControls : public LumatoneEditorState
                          , public LumatoneEditorState::Controller
                          , public juce::Component
                          , protected LumatoneEditor::EditorListener
{
    typedef enum {
      REPLACE = 0,
      MERGE = 1
    } UpdateType;

public:

    struct MultiSelection
    {
        juce::Array<juce::Colour> coloursSelected;
        juce::Array<LumatoneKeyType> typesSelected;
        juce::Array<int> notesSelected;
        juce::Array<int> channelsSelected;
    };

public:
    MultiSelectControls(const LumatoneEditorState& stateIn);

    virtual ~MultiSelectControls() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void setSelection(const MultiSelection& selectionIn, bool sendUpdate=true);
    void setSelection(juce::Colour colour, bool sendUpdate=true);
    void setSelection(LumatoneKeyType type, bool sendUpdate=true);
    void setSelection(int note=-1, int channel=-1, bool sendUpdate=true);

    void addToSelection(juce::Colour colour, bool sendUpdate=true);
    void addToSelection(LumatoneKeyType colour, bool sendUpdate=true);
    void addToSelection(int note=-1, int channel=-1, bool sendUpdate=true);

    void removeFromSelection(juce::Colour colour, bool sendUpdate=true);
    void removeFromSelection(LumatoneKeyType colour, bool sendUpdate=true);
    void removeFromSelection(int note=-1, int channel=-1, bool sendUpdate=true);

    void clearSelection(bool sendUpdate=true);

private:

    void colourInputCallback(juce::Colour colour, UpdateType update);
    void typeInputCallback(int type, UpdateType update);
    void noteInputCallback(int note, UpdateType update);
    void channelInputCallback(int channel, UpdateType update);


private:
    void layoutChanged(const LumatoneLayout& mappingData) override;
    void boardChanged(const LumatoneBoard& boardData) override;
    void keyChanged(int boardIndex, int keyIndex, const LumatoneKey& lumatoneKey) override;
    void keySetChanged(juce::Array<MappedLumatoneKey> selection) override;

    void selectionChanged() override;

private:
    void updateColoursOptions(const juce::Array<juce::Colour>& colours);
    void updateKeyTypesOptions(const juce::Array<LumatoneKeyType>& types);
    void updateKeyNotesOptions(const juce::Array<int>& notes);
    void updateKeyChannelsOptions(const juce::Array<int>& channels);

private:

    std::unique_ptr<juce::Label>    lblMultiSelect;

    std::unique_ptr<LumatoneEditorControl>  colourInput;
    std::unique_ptr<LumatoneEditorControl>  keyTypeCombo;
    std::unique_ptr<LumatoneEditorControl>  noteInput;
    std::unique_ptr<LumatoneEditorControl>  channelInput;

    ColourDropdownSelector*                 colourDropdown;

    juce::Array<juce::Colour> coloursSelected;
    juce::Array<LumatoneKeyType> typesSelected;
    juce::Array<int> notesSelected;
    juce::Array<int> channelsSelected;


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
    const float colourTypeColumnW       = 0.5f;

    int noteChannelColumnX;
    int noteChannelColumnWidth;
    const float noteChannelColumnW      = 0.27f;
};

#endif // LUMATONE_EDITOR_MULTI_SELECT_CONTROLS_H
