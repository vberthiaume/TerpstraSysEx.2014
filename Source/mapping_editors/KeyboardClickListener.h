/*
  ==============================================================================

    KeyboardClickListener.h
    Created: 15 June 2024
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_KEY_SELECTION_CONTROLLER_H
#define LUMATONE_KEY_SELECTION_CONTROLLER_H

#include "../LumatoneEditorState.h"
#include "../lumatone_editor_library/ui/keyboard_component.h"

// Maybe KeyboardComponentSelectionController
class KeyboardClickListener : public LumatoneEditorState
                             , private LumatoneEditorState::Controller
                             , public LumatoneKeyboardComponent::Listener
{
public:

    KeyboardClickListener(const LumatoneEditorState& stateIn, LumatoneKeyboardComponent* keyboardComponent);
    ~KeyboardClickListener() override;


    // LumatoneKeyboardComponent::Listener

    void handleKeyUp(int keyNum) override;
    void handleKeyDown(int keyNum) override;
    void handleKeyHold(int key, float xDistance, float yDistance) override;

private:

    void toggleKeySelection(int keyNum);

private:

    LumatoneKeyboardComponent* keyboardComponent;

    int lastKeyDown = -1;
};

#endif // LUMATONE_KEY_SELECTION_CONTROLLER_H
