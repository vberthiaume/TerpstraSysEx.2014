/*
  ==============================================================================

    BatchTools.h
    Created: 16 July 2024
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_BATCH_TOOLS_H
#define LUMATONE_EDITOR_BATCH_TOOLS_h

#include "../LumatoneEditorState.h"


class BatchToolsColourControls;


class BatchTools : public LumatoneEditorState
                 , public juce::Component
{
public:
    BatchTools(const LumatoneEditorState& stateIn);
    ~BatchTools() override;

    void paint(juce::Graphics& g) override;
    void resized() override;


private:

    BatchToolsColourControls* colourControls;

    std::unique_ptr<juce::TabbedComponent> tabs;

    std::unique_ptr<juce::Label> batchToolsLabel;


private:

    int contentMarginWidth;

    int headerHeight;

    juce::Path headerPath;
    juce::Path controlPath;

};

#endif // LUMATONE_EDITOR_BATCH_TOOLS_H
