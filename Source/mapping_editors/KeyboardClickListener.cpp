#include "KeyboardClickListener.h"

#include "../actions/KeySelectionControlActions.h"

KeyboardClickListener::KeyboardClickListener(const LumatoneEditorState &stateIn, LumatoneKeyboardComponent *keyboardComponentIn)
    : LumatoneEditorState("LumatoneKeyboardClickListener", stateIn)
    , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
    , keyboardComponent(keyboardComponentIn)
{
    keyboardComponent->addListener(this);
}

KeyboardClickListener::~KeyboardClickListener()
{
    keyboardComponent->removeListener(this);
}

void KeyboardClickListener::handleKeyUp(int keyNum)
{

}

void KeyboardClickListener::handleKeyDown(int keyNum)
{
    lastKeyDown = keyNum;
    // make quicker?
    toggleKeySelection(keyNum);
}

void KeyboardClickListener::handleKeyHold(int key, float xDistance, float yDistance)
{
    if (key != lastKeyDown)
    {
        lastKeyDown = key;
        toggleKeySelection(key);
    }
}

void KeyboardClickListener::toggleKeySelection(int keyNum)
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
