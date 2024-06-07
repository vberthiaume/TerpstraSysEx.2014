#include "EditorControlActions.h"


SetKeySettingsAction::SetKeySettingsAction(const LumatoneEditorState & stateIn,
                                           bool setKeyColourIn,
                                           bool setKeyTypeIn,
                                           bool setKeyNoteIn,
                                           bool setKeyChannelIn,
                                           bool setCCFaderDefaultIn,
                                           juce::Colour colourIn,
                                           LumatoneKeyType typeIn,
                                           int noteIn,
                                           int channelIn,
                                           bool ccFaderDefaultIn)
	: LumatoneEditorState("SetKeySettingsAction", stateIn)
    , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
    , LumatoneAction(this, "SetKeySettingsAction")
    , newEditData("SetKeySettingsActionData")
{
    previousData = stateIn.getEditSelectionData();

    newEditData.setKeyColour(setKeyColourIn, colourIn);
    newEditData.setKeyType(setKeyTypeIn, typeIn);
    newEditData.setKeyNote(setKeyNoteIn, noteIn);
    newEditData.setKeyChannel(setKeyChannelIn, channelIn);
    newEditData.setCCFader(setCCFaderDefaultIn, ccFaderDefaultIn);
}

bool SetKeySettingsAction::perform()
{
    LumatoneEditSelectionState::Data data = newEditData.getData();
    setAssignKeyColour(data.setColour, data.colour);
    setAssignKeyType(data.setType, data.type);
    setAssignKeyNote(data.setNote, data.note);
    setAssignKeyChannel(data.setChannel, data.channel);
    setAssignCCFader(data.setCCFaderDefault, data.ccFaderDefault);
    return false;
}

bool SetKeySettingsAction::undo()
{
    setAssignKeyColour(previousData.setColour, previousData.colour);
    setAssignKeyType(previousData.setType, previousData.type);
    setAssignKeyNote(previousData.setNote, previousData.note);
    setAssignKeyChannel(previousData.setChannel, previousData.channel);
    setAssignCCFader(previousData.setCCFaderDefault, previousData.ccFaderDefault);
    return false;
}

SetKeySettingsAction *SetKeySettingsAction::NewSetAssignColourAction(LumatoneEditorState &stateIn, juce::Colour colourIn)
{
    return new SetKeySettingsAction(stateIn, true, false, false, false, false, colourIn);
}

SetKeySettingsAction *SetKeySettingsAction::NewSetAssignKeyTypeAction(LumatoneEditorState &stateIn, LumatoneKeyType typeIn)
{
    return new SetKeySettingsAction(stateIn, false, true, false, false, false, juce::Colour(), typeIn);
}

SetKeySettingsAction *SetKeySettingsAction::NewSetAssignKeyNoteAction(LumatoneEditorState &stateIn, int noteIn)
{
    return new SetKeySettingsAction(stateIn, false, false, true, false, false, juce::Colour(), LumatoneKeyType(), noteIn);
}

SetKeySettingsAction *SetKeySettingsAction::NewSetAssignKeyChannelAction(LumatoneEditorState &stateIn, int channelIn)
{
    return new SetKeySettingsAction(stateIn, false, false, false, true, false, juce::Colour(), LumatoneKeyType(), 0, channelIn);
}

SetKeySettingsAction *SetKeySettingsAction::NewSetAssignCCFaderAction(LumatoneEditorState &stateIn, bool faderDefaultIn)
{
    return new SetKeySettingsAction(stateIn, false, false, false, false, true, juce::Colour(), LumatoneKeyType(), 0, 0, faderDefaultIn);
}
