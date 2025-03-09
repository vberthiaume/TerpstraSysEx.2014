#include "KeySelectionControlActions.h"

static juce::Colour getColourFromSelectedKeys(const juce::Array<MappedLumatoneKey>* selectedKeys)
{
    if (selectedKeys->size() > 0)
    {
        return selectedKeys->getUnchecked(0).getColour();
    }

    return juce::Colour();
}

AddOrRemoveKeySelectionAction::AddOrRemoveKeySelectionAction(const LumatoneEditorState &stateIn, int keyNumIn, bool removeIn)
	: LumatoneEditorState("AddOrRemoveKeySelectionAction", stateIn)
    , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
    , LumatoneAction(this, "AddOrRemoveKeySelectionAction")
    , keyNum(keyNumIn)
    , remove(removeIn)
{
}

AddOrRemoveKeySelectionAction::~AddOrRemoveKeySelectionAction()
{
}

bool AddOrRemoveKeySelectionAction::perform()
{
    if (remove)
    {
        removeKeyFromSelection(keyNum);
    }
    else
    {
        addKeyToSelection(keyNum);
    }

    return true;
}

bool AddOrRemoveKeySelectionAction::undo()
{
    if (remove)
    {
        addKeyToSelection(keyNum);
    }
    else
    {
        removeKeyFromSelection(keyNum);
    }

    return true;
}


// SetKeySelectionAction

SetKeySelectionAction::SetKeySelectionAction(const LumatoneEditorState &stateIn, juce::Array<MappedLumatoneKey> newSelectionIn)
	: LumatoneEditorState("SetKeySelectionAction", stateIn)
    , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
    , LumatoneAction(this, "SetKeySelectionAction")
{
    previousSelectedColour = juce::Colour();
    if (getSelectedKeys()->size() > 0)
    {
        previousSelection.addArray(*getSelectedKeys());
        previousSelectedColour = getSelectedKeys()->getUnchecked(0).getColour();
    }
    newSelection.addArray(newSelectionIn);
}

SetKeySelectionAction::~SetKeySelectionAction()
{
}

bool SetKeySelectionAction::perform()
{
    setSelectedKeys(newSelection);
    juce::Colour newColour = getColourFromSelectedKeys(getSelectedKeys());
    setAssignKeyColour(newColour.isOpaque(), newColour);
    return true;
}

bool SetKeySelectionAction::undo()
{
    setSelectedKeys(previousSelection);
    setAssignKeyColour(previousSelectedColour.isOpaque(), previousSelectedColour);
    return true;
}

SetKeySelectionAction *SetKeySelectionAction::NewSetKeySelectionActionByCoords(const LumatoneEditorState &stateIn, juce::Array<LumatoneKeyCoord> keyCoords)
{
    juce::Array<MappedLumatoneKey> keys;

    for (const LumatoneKeyCoord& coord : keyCoords)
    {
        auto key = stateIn.getMappingData()->getMappedKey(coord.boardIndex, coord.keyIndex);
        keys.add(key);
    }

    return new SetKeySelectionAction(stateIn, keys);
}
