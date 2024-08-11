/*
  ==============================================================================

    EditorControlActions.h
    Created: 6 June 2024
    Author:  Vito

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_CONTROL_ACTIONS_H
#define LUMATONE_EDITOR_CONTROL_ACTIONS_H

#include "../data/LumatoneEditorState.h"
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

    LumatoneKeyPropertyData previousData;
    LumatoneKeyPropertyData newEditData;

};

class ApplyAssignmentsToSelectionAction : public LumatoneEditorState
                                        , private LumatoneEditorState::Controller
                                        , public LumatoneAction
{
public:

    ApplyAssignmentsToSelectionAction(const LumatoneEditorState& stateIn
                                    , LumatoneKeyPropertyData assignData
                                    , const juce::Array<MappedLumatoneKey>& keySelectionIn);

    bool perform() override;
    bool undo() override;

    int getSizeInUnits() override { return sizeof(ApplyAssignmentsToSelectionAction); }

private:

    void applyData(LumatoneKeyPropertyData& dataToApply);

private:
    juce::Array<MappedLumatoneKey>    previousData;
    LumatoneKeyPropertyData  newData;

    juce::Array<MappedLumatoneKey> keySelection;

};

#endif // LUMATONE_EDITOR_CONTROL_ACTIONS_H
