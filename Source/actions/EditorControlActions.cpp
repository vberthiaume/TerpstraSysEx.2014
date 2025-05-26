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

    newEditData.useColour = setKeyColourIn;
    newEditData.colour = colourIn;

    newEditData.useType = setKeyTypeIn;
    newEditData.type = typeIn;

    newEditData.useNote = setKeyNoteIn;
    newEditData.note = noteIn;

    newEditData.useChannel = setKeyChannelIn;
    newEditData.channel = channelIn;

    newEditData.useCCFaderDefault = setCCFaderDefaultIn;
    newEditData.ccFaderDefault = ccFaderDefaultIn;
}

bool SetKeySettingsAction::perform()
{
    addToEditAassignment();
    return true;
}

bool SetKeySettingsAction::undo()
{
    setAssignKeyColour(previousData.useColour, previousData.colour);
    setAssignKeyType(previousData.useType, previousData.type);
    setAssignKeyNote(previousData.useNote, previousData.note);
    setAssignKeyChannel(previousData.useChannel, previousData.channel);
    setAssignCCFader(previousData.useCCFaderDefault, previousData.ccFaderDefault);
    return true;
}

// void SetKeySettingsAction::assignToSelectedKeys()
// {
// }

void SetKeySettingsAction::addToEditAassignment()
{
    if (newEditData.useColour)
        setAssignKeyColour(newEditData.useColour, newEditData.colour);
    if (newEditData.useType)
        setAssignKeyType(newEditData.useType, newEditData.type);
    if (newEditData.useNote)
        setAssignKeyNote(newEditData.useNote, newEditData.note);
    if (newEditData.useChannel)
        setAssignKeyChannel(newEditData.useChannel, newEditData.channel);
    if (newEditData.useCCFaderDefault)
        setAssignCCFader(newEditData.useCCFaderDefault, newEditData.ccFaderDefault);
}

SetKeySettingsAction *SetKeySettingsAction::NewSetAssignColourAction(LumatoneEditorState &stateIn, juce::Colour colourIn)
{
    bool valid = colourIn.isOpaque();
    return new SetKeySettingsAction(stateIn, valid, false, false, false, false, colourIn);
}

SetKeySettingsAction *SetKeySettingsAction::NewSetAssignKeyTypeAction(LumatoneEditorState &stateIn, LumatoneKeyType typeIn)
{
    bool valid = typeIn > LumatoneKeyType::disabledDefault && typeIn <= LumatoneKeyType::disabled;
    return new SetKeySettingsAction(stateIn, false, valid, false, false, false, juce::Colour(), typeIn);
}

SetKeySettingsAction *SetKeySettingsAction::NewSetAssignKeyNoteAction(LumatoneEditorState &stateIn, int noteIn)
{
    bool valid = noteIn >= 0 && noteIn < 128;
    return new SetKeySettingsAction(stateIn, false, false, valid, false, false, juce::Colour(), LumatoneKeyType(), noteIn);
}

SetKeySettingsAction *SetKeySettingsAction::NewSetAssignKeyChannelAction(LumatoneEditorState &stateIn, int channelIn)
{
    bool valid = channelIn > 0 && channelIn <= 16;
    return new SetKeySettingsAction(stateIn, false, false, false, valid, false, juce::Colour(), LumatoneKeyType(), 0, channelIn);
}

SetKeySettingsAction *SetKeySettingsAction::NewSetAssignCCFaderAction(LumatoneEditorState &stateIn, bool faderDefaultIn)
{
    return new SetKeySettingsAction(stateIn, false, false, false, false, true, juce::Colour(), LumatoneKeyType(), 0, 0, faderDefaultIn);
}

ApplyAssignmentsToSelectionAction::ApplyAssignmentsToSelectionAction(const LumatoneEditorState& stateIn
                                                                   , LumatoneKeyPropertyData assignData
                                                                   , const juce::Array<MappedLumatoneKey> &keySelectionIn)
	: LumatoneEditorState("ApplyAssignmentsToSelectionAction", stateIn)
    , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
    , LumatoneAction(this, "ApplyAssignmentsToSelectionAction")
{

    newData = assignData;


    // should probably be added as an option
    // clean, only assign different values
    bool assignColour = false;
    bool assignType = false;
    bool assignNote = false;
    bool assignChannel = false;

    LumatoneKey sampleKey;
    if (keySelectionIn.size() > 0)
    {
        sampleKey = keySelectionIn[0];
    }
    if (newData.useColour)
        sampleKey.setColour(newData.colour);
    if (newData.useType)
        sampleKey.setKeyType(newData.type);
    if (newData.useNote)
        sampleKey.setNoteOrCC(newData.note);
    if (newData.useChannel)
        sampleKey.setChannelNumber(newData.channel);

    for (const MappedLumatoneKey& key : keySelectionIn)
    {
        keySelection.add(key);
        previousData.add(key);

        //
        if (newData.useColour && !assignColour && !key.colourIsEqual(sampleKey))
            assignColour = true;
        if (newData.useType && !assignType && key.getType() != sampleKey.getType())
            assignType = true;
        if (newData.useNote && !assignNote && key.getMidiNumber() != sampleKey.getMidiNumber())
            assignNote = true;
        if (newData.useChannel && !assignChannel && key.getMidiChannel() != sampleKey.getMidiChannel())
            assignChannel = true;
    }

    if (newData.useColour)
        newData.useColour = assignColour;
    if (newData.useType)
        newData.useType = assignType;
    if (newData.useNote)
        newData.useNote = assignNote;
    if (newData.useChannel)
        newData.useChannel = assignChannel;
}

bool ApplyAssignmentsToSelectionAction::perform()
{
    juce::Array<MappedLumatoneKey> updatedKeys;

    for (const MappedLumatoneKey key : keySelection)
    {
        MappedLumatoneKey keyUpdate = key;
        if (newData.useColour)
            keyUpdate.setColour(newData.colour);
        if (newData.useType)
            keyUpdate.setKeyType(newData.type);
        if (newData.useNote)
            keyUpdate.setNoteOrCC(newData.note);
        if (newData.useChannel)
            keyUpdate.setChannelNumber(newData.channel);
        if (newData.ccFaderDefault)
            keyUpdate.setDefaultCCFader(newData.ccFaderDefault);

        LumatoneState::setKey((const LumatoneKey&) keyUpdate, keyUpdate.boardIndex + 1, keyUpdate.keyIndex);
        updatedKeys.add(keyUpdate);
    }

    bool useBuffer = updatedKeys.size() > 48;

    if (!newData.useColour)
    {
        // todo param-only update (no colour)
        sendSelectionParam(updatedKeys, true, useBuffer);
    }
    else if (newData.useType || newData.useNote || newData.useChannel || newData.ccFaderDefault)
    {
        sendSelectionParam(updatedKeys, true, useBuffer);
    }
    else
    {
        sendSelectionColours(updatedKeys, true, useBuffer);
    }

    updatedSelectedKeys();

    return true;
}

bool ApplyAssignmentsToSelectionAction::undo()
{
    // for (const MappedLumatoneKey key : previousData)
    // {
    //     setKey((const LumatoneKey&) key, key.boardIndex + 1, key.keyIndex);
    // }

    sendSelectionParam(previousData);
    updatedSelectedKeys();

    return true;
}

SetCurrentFileAction::SetCurrentFileAction(const LumatoneEditorState& stateIn, juce::File file)
    : LumatoneEditorState("SetCurrentFileAction", stateIn)
    , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
    , LumatoneAction(this, "SetCurrentFileAction")
    , newFile(file)
{
    previousFile = getCurrentFile();
    previousMappingData = shareMappingData();
    previousKeySelection.addArray(*getSelectedKeys());
}

SetCurrentFileAction::~SetCurrentFileAction()
{
    previousMappingData = nullptr;
}

bool SetCurrentFileAction::perform()
{
    setCurrentFile(newFile);
    if (resetToCurrentFile())
    {
        // Clear undo history
        // undoManager.clearUndoHistory();

        clearSelectedKeys();
        return true;
    }

    return false;
}

bool SetCurrentFileAction::undo()
{
    if (previousFile.existsAsFile())
        setCurrentFile(previousFile);
    setCompleteConfig(*previousMappingData);
    setSelectedKeys(previousKeySelection);
    return true;
}
