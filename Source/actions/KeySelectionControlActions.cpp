#include "KeySelectionControlActions.h"

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
        removeSelectedKey(keyNum);
    }
    else
    {
        addSelectedKey(keyNum);
    }

    return true;
}

bool AddOrRemoveKeySelectionAction::undo()
{
    if (remove)
    {
        addSelectedKey(keyNum);
    }
    else
    {
        removeSelectedKey(keyNum);
    }

    return true;
}


// SetKeySelectionAction

// SetKeySelectionAction::SetKeySelectionAction(const LumatoneEditorState &stateIn, juce::Array<MappedLumatoneKey> newSelectionIn)
// 	: LumatoneEditorState("SetKeySelectionAction", stateIn)
//     , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
//     , LumatoneAction(this, "SetKeySelectionAction")
// {
//     previousSelection.addArray(selectedKeys);
//     newSelection.addArray(newSelectionIn);
// }

// SetKeySelectionAction::~SetKeySelectionAction()
// {
// }

// bool SetKeySelectionAction::perform()
// {
//     // setSelectedKeys(newSelection);
//     return true;
// }

// bool SetKeySelectionAction::undo()
// {
//     // setSelectedKeys(previousSelection);
//     return true;
// }
