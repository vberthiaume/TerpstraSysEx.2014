/*
  ==============================================================================

    KeyboardClickListener.h
    Created: 15 June 2024
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_KEY_SELECTION_CONTROLLER_H
#define LUMATONE_KEY_SELECTION_CONTROLLER_H

#include "../data/LumatoneEditorState.h"
#include "../lumatone_editor_library/ui/keyboard_component.h"
#include "../lumatone_editor_library/palettes/colour_selection_broadcaster.h"
// Maybe KeyboardComponentSelectionController
class KeyboardClickListener : public LumatoneEditorState
                             , private LumatoneEditorState::Controller
                             , public LumatoneKeyboardComponent::Listener
                             , public ColourSelectionBroadcaster
{
public:

    KeyboardClickListener(const LumatoneEditorState& stateIn, LumatoneKeyboardComponent* keyboardComponent);
    ~KeyboardClickListener() override;


    // LumatoneKeyboardComponent::Listener

    void handleKeyUp(int keyNum) override;
    void handleKeyDown(int keyNum) override;
    void handleKeyHold(int key, float xDistance, float yDistance) override;

private:

    juce::Colour getSelectedColour() override;
    void deselectColour() override {}

private:

    void toggleKeySelection(int keyNum);

private:

    LumatoneKeyboardComponent* keyboardComponent;

    int lastKeyDown = -1;

    juce::Colour lastKeyColour;

};

#endif // LUMATONE_KEY_SELECTION_CONTROLLER_H
