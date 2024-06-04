/*
  ==============================================================================

    MultiSelectControls.h
    Created: 3 June 2024
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_MULTI_SELECT_CONTROLS_H
#define LUMATONE_EDITOR_MULTI_SELECT_CONTROLS_H

#include "../LumatoneEditorState.h"

class MultiSelectControls : public LumatoneEditorState
                          , public juce::Component
{
public:
    MultiSelectControls(const LumatoneEditorState& stateIn);

    virtual ~MultiSelectControls() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    const float headerH         = 0.19f;
    int headerHeight;
    juce::Path headerPath;
    juce::Path controlPath;

};

#endif // LUMATONE_EDITOR_MULTI_SELECT_CONTROLS_H
