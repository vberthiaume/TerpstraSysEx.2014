#include "LumatoneEditorKeyboard.h"
// #include "../data/LumatoneE/ditorState.h"

#include "../actions/KeySelectionControlActions.h"


LumatoneEditorKeyboardComponent::LumatoneEditorKeyboardComponent(const LumatoneEditorState &stateIn)
    : LumatoneEditorState("LumatoneEditorKeyboardComponent", stateIn)
    , LumatoneKeyboardComponent((const LumatoneApplicationState&)(stateIn))
    , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
    // , editorState(stateIn)
{
    addColourSelectionBroadcaster(this);
}

LumatoneEditorKeyboardComponent::~LumatoneEditorKeyboardComponent()
{
    removeColourSelectionBroadcaster(this);
}

bool LumatoneEditorKeyboardComponent::isKeySelected(const LumatoneKeyCoord &keyCoord)
{
    if (!LumatoneEditorState::getMappingData()->isKeyCoordValid(keyCoord))
        return false;

    int keyNum = LumatoneEditorState::getMappingData()->keyCoordToKeyNum(keyCoord);
    for (const MappedLumatoneKey& key : *LumatoneEditorState::getSelectedKeys())
    {
        int selectedNum = LumatoneEditorState::getMappingData()->keyCoordToKeyNum(key.boardIndex, key.keyIndex);
        if (selectedNum == keyNum)
        {
            return true;
            break;
        }
    }

    return false;
}

void LumatoneEditorKeyboardComponent::mouseMove(const juce::MouseEvent &e)
{
    LumatoneKeyboardComponent::mouseMove(e);
}

void LumatoneEditorKeyboardComponent::mouseDown(const juce::MouseEvent &e)
{
    auto key = getKeyFromMouseEvent(e);
    if (key)
    {
        LumatoneKeyCoord keyCoord = key->getKeyCoord();
        bool keyIsSelected = isKeySelected(keyCoord);

        LumatoneKeyboardComponent::mouseDownInternal(e, key);

        if (e.mods.isCommandDown())
        {
            performAction(new AddOrRemoveKeySelectionAction(*this, LumatoneEditorState::getMappingData()->keyCoordToKeyNum(keyCoord), keyIsSelected));
        }
        else
        {
            juce::Array<MappedLumatoneKey> keySelection;
            if (LumatoneEditorState::getSelectedKeys()->size() > 1 || !keyIsSelected)
            {
                keySelection.add(MappedLumatoneKey(LumatoneEditorState::getKey(keyCoord), keyCoord));
            }

            if (performAction(new SetKeySelectionAction(*this, keySelection)))
            {
                selectedColour = getEditSelectionData().colour;
                selectorListeners.call(&ColourSelectionListener::colourChangedCallback, this, selectedColour);
            }
        }
    }
}

void LumatoneEditorKeyboardComponent::mouseUp(const juce::MouseEvent &e)
{
    LumatoneKeyboardComponent::mouseUp(e);
    mouseWasDragging = false;
}

void LumatoneEditorKeyboardComponent::mouseDrag(const juce::MouseEvent &e)
{
    LumatoneKeyboardComponent::mouseDrag(e);
    mouseWasDragging = true;
}

void LumatoneEditorKeyboardComponent::mouseDragInternalOnNewKey(const juce::MouseEvent &e, LumatoneKeyDisplay *key)
{
    LumatoneKeyCoord keyCoord = key->getKeyCoord();
    bool isSelected = isKeySelected(keyCoord);

    bool undoable = true;
    bool newTransaction = !mouseWasDragging;

    if (e.mods.isCommandDown())
    {
        performAction(new AddOrRemoveKeySelectionAction(*this, LumatoneEditorState::getMappingData()->keyCoordToKeyNum(keyCoord), isSelected), undoable, newTransaction);
    }
    else
    {
        juce::Array<MappedLumatoneKey> keySelection;
        if (!isSelected)
        {
            keySelection.add(MappedLumatoneKey(LumatoneEditorState::getKey(keyCoord), keyCoord));
        }
        if (performAction(new SetKeySelectionAction(*this, keySelection), undoable, newTransaction))
        {
            selectedColour = getEditSelectionData().colour;
            selectorListeners.call(&ColourSelectionListener::colourChangedCallback, this, selectedColour);
        }
    }
}

bool LumatoneEditorKeyboardComponent::keyStateChanged(bool isKeyDown)
{
    return LumatoneKeyboardComponent::keyStateChanged(isKeyDown);
}

bool LumatoneEditorKeyboardComponent::keyPressed(const juce::KeyPress &key, juce::Component *originatingComponent)
{
    return LumatoneKeyboardComponent::keyPressed(key, originatingComponent);
}

void LumatoneEditorKeyboardComponent::modifierKeysChanged(const juce::ModifierKeys &modifiers)
{
    if (!shiftHeld && modifiers.isShiftDown())
    {
        shiftHeld = true;
    }

    if (shiftHeld && !modifiers.isShiftDown())
    {
        shiftHeld = false;
    }

    if (!altHeld && modifiers.isAltDown())
    {
       altHeld = true;
    }

    else if (altHeld && !modifiers.isAltDown())
    {
       altHeld = false;
    }

    if (!ctrlHeld && modifiers.isCtrlDown())
    {
       ctrlHeld = true;
    }

    else if (ctrlHeld && !modifiers.isCtrlDown())
    {
       ctrlHeld = false;
    }

    LumatoneKeyboardComponent::modifierKeysChanged(modifiers);
}

void LumatoneEditorKeyboardComponent::deselectColour()
{

}

juce::Colour LumatoneEditorKeyboardComponent::getSelectedColour()
{
    return selectedColour;
}
