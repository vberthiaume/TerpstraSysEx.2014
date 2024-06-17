#include "KeySelectionController.h"

#include "../actions/KeySelectionControlActions.h"

KeySelectionController::KeySelectionController(const LumatoneEditorState &stateIn, LumatoneKeyboardComponent *keyboardComponentIn)
    : LumatoneEditorState(stateIn)
    , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
    , keyboardComponent(keyboardComponentIn)
{
    keyboardComponent->addListener(this);
}

KeySelectionController::~KeySelectionController()
{
    keyboardComponent->removeListener(this);
}

void KeySelectionController::handleKeyUp(int keyNum)
{

}

void KeySelectionController::handleKeyDown(int keyNum)
{
    lastKeyDown = keyNum;
    // make quicker?
    toggleKeySelection(keyNum);
}

void KeySelectionController::handleKeyHold(int key, float xDistance, float yDistance)
{
    if (key != lastKeyDown)
    {
        lastKeyDown = key;
        toggleKeySelection(key);
    }
}

void KeySelectionController::toggleKeySelection(int keyNum)
{
    bool isSelected = false;
    for (const MappedLumatoneKey& key : *getSelectedKeys())
    {
        int selectedNum = mappingData->keyCoordToKeyNum(key.boardIndex, key.keyIndex);
        if (selectedNum == keyNum)
        {
            isSelected = true;
            break;
        }
    }

    // Toggle selection
    performAction(new AddOrRemoveKeySelectionAction(*this, keyNum, isSelected));
}
