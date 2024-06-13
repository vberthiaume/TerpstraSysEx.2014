/*
  ==============================================================================

    EditorControlActions.h
    Created: 6 June 2024
    Author:  Vito

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_CONTROL_ACTIONS_H
#define LUMATONE_EDITOR_CONTROL_ACTIONS_H

#include "../LumatoneEditorState.h"
#include "../lumatone_editor_library/actions/lumatone_action.h"
#include "../data/LumatoneEditSelectionState.h"

// Records action of setting control values in the editor (before assign action)
class SetKeySettingsAction : public LumatoneEditorState
                           , private LumatoneEditorState::Controller
                           , public LumatoneAction
{
public:

    SetKeySettingsAction(const LumatoneEditorState& stateIn
                       , bool setKeyColourIn
                       , bool setKeyTypeIn
                       , bool setKeyNoteIn
                       , bool setKeyChannelIn
                       , bool setCCFaderDefaultIn
                       , juce::Colour colourIn = juce::Colour()
                       , LumatoneKeyType typeIn = LumatoneKeyType::noteOnNoteOff
                       , int noteIn = 0
                       , int channelIn = 0
                       , bool ccFaderDefaultIn = false);

    bool perform() override;
    bool undo() override;

    int getSizeInUnits() override { return sizeof(SetKeySettingsAction); }

    // void assignToSelectedKeys();
    void addToEditAassignment();

public:

    static SetKeySettingsAction* NewSetAssignColourAction(LumatoneEditorState&  stateIn,    juce::Colour colourIn);
    static SetKeySettingsAction* NewSetAssignKeyTypeAction(LumatoneEditorState& stateIn,    LumatoneKeyType typeIn);
    static SetKeySettingsAction* NewSetAssignKeyNoteAction(LumatoneEditorState& stateIn,    int noteIn);
    static SetKeySettingsAction* NewSetAssignKeyChannelAction(LumatoneEditorState& stateIn, int channelIn);
    static SetKeySettingsAction* NewSetAssignCCFaderAction(LumatoneEditorState& stateIn,    bool faderDefaultIn);

private:

    LumatoneEditSelectionState::Data previousData;
    LumatoneEditSelectionState::Data newEditData;

};




#endif // LUMATONE_EDITOR_CONTROL_ACTIONS_H
