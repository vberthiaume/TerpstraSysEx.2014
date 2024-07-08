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
    // , newEditData("SetKeySettingsActionData")
{
    previousData = stateIn.getEditSelectionData();

    newEditData.setColour = setKeyColourIn;
    newEditData.colour = colourIn;

    newEditData.setType = setKeyTypeIn;
    newEditData.type = typeIn;

    newEditData.setNote = setKeyNoteIn;
    newEditData.note = noteIn;

    newEditData.setChannel = setKeyChannelIn;
    newEditData.channel = channelIn;

    newEditData.setCCFaderDefault = setCCFaderDefaultIn;
    newEditData.ccFaderDefault = ccFaderDefaultIn;
}

bool SetKeySettingsAction::perform()
{
    addToEditAassignment();
    return true;
}

bool SetKeySettingsAction::undo()
{
    setAssignKeyColour(previousData.setColour, previousData.colour);
    setAssignKeyType(previousData.setType, previousData.type);
    setAssignKeyNote(previousData.setNote, previousData.note);
    setAssignKeyChannel(previousData.setChannel, previousData.channel);
    setAssignCCFader(previousData.setCCFaderDefault, previousData.ccFaderDefault);
    return true;
}

// void SetKeySettingsAction::assignToSelectedKeys()
// {
// }

void SetKeySettingsAction::addToEditAassignment()
{
    if (newEditData.setColour)
        setAssignKeyColour(newEditData.setColour, newEditData.colour);
    if (newEditData.setType)
        setAssignKeyType(newEditData.setType, newEditData.type);
    if (newEditData.setNote)
        setAssignKeyNote(newEditData.setNote, newEditData.note);
    if (newEditData.setChannel)
        setAssignKeyChannel(newEditData.setChannel, newEditData.channel);
    if (newEditData.setCCFaderDefault)
        setAssignCCFader(newEditData.setCCFaderDefault, newEditData.ccFaderDefault);
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

ApplyAssignmentsToSelectionAction::ApplyAssignmentsToSelectionAction(const LumatoneEditorState& stateIn
                                                                   , LumatoneEditSelectionState::Data assignData
                                                                   , const juce::Array<MappedLumatoneKey> &keySelectionIn)
	: LumatoneEditorState("ApplyAssignmentsToSelectionAction", stateIn)
    , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
    , LumatoneAction(this, "ApplyAssignmentsToSelectionAction")
{
    for (const MappedLumatoneKey& key : keySelectionIn)
    {
        keySelection.add(key);
        previousData.add(key);
    }

    newData = assignData;
}

bool ApplyAssignmentsToSelectionAction::perform()
{
    for (const MappedLumatoneKey key : previousData)
    {
        MappedLumatoneKey keyUpdate = key;
        if (newData.setColour)
            keyUpdate.setColour(newData.colour);
        if (newData.setType)
            keyUpdate.setKeyType(newData.type);
        if (newData.setNote)
            keyUpdate.setNoteOrCC(newData.note);
        if (newData.setChannel)
            keyUpdate.setChannelNumber(newData.channel);
        if (newData.ccFaderDefault)
            keyUpdate.setDefaultCCFader(newData.ccFaderDefault);

        setKey((const LumatoneKey&) keyUpdate, keyUpdate.boardIndex + 1, keyUpdate.keyIndex);
    }

    updatedSelectedKeys();

    return true;
}

bool ApplyAssignmentsToSelectionAction::undo()
{
    for (const MappedLumatoneKey key : previousData)
    {
        setKey((const LumatoneKey&) key, key.boardIndex + 1, key.keyIndex);
    }

    updatedSelectedKeys();

    return true;
}
