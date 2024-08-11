#include "KeyboardClickListener.h"

#include "./KeySelectionControlActions.h"

KeyboardClickListener::KeyboardClickListener(const LumatoneEditorState &stateIn, LumatoneKeyboardComponent *keyboardComponentIn)
    : LumatoneEditorState("LumatoneKeyboardClickListener", stateIn)
    , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
    , keyboardComponent(keyboardComponentIn)
{
    keyboardComponent->addListener(this);
    addColourSelectionBroadcaster(this);
}

KeyboardClickListener::~KeyboardClickListener()
{
    removeColourSelectionBroadcaster(this);
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

    auto keyCoord = getMappingData()->keyNumToKeyCoord(keyNum);
    const LumatoneKey& key = getMappingData()->getKey(keyCoord.boardIndex, keyCoord.keyIndex);
    lastKeyColour = key.getColour();

    selectorListeners.call(&ColourSelectionListener::colourChangedCallback, this, lastKeyColour);
}

void KeyboardClickListener::handleKeyHold(int key, float xDistance, float yDistance)
{
    if (key != lastKeyDown)
    {
        lastKeyDown = key;
        toggleKeySelection(key);
    }
}

juce::Colour KeyboardClickListener::getSelectedColour()
{
    return lastKeyColour;
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
