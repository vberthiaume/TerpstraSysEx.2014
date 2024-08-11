/*
  ==============================================================================

    BatchToolsColourControls.h
    Created: 16 July 2024
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_BATCH_TOOLS_COLOUR_CONTROLSH
#define LUMATONE_EDITOR_BATCH_TOOLS_COLOUR_CONTROLSH

#include "../../data/LumatoneEditorState.h"
#include "../../lumatone_editor_library/listeners/editor_listener.h"

#include "../../lumatone_editor_library/palettes/colour_selection_broadcaster.h"


class BatchToolsColourControls : public LumatoneEditorState
                                , protected LumatoneEditorState::Controller
                                , public LumatoneEditor::LayoutChangeListener
                                , public juce::Component
{
public:
    BatchToolsColourControls(const LumatoneEditorState& stateIn);
    ~BatchToolsColourControls() override;

    void resized() override;

private:
    bool isSetToDefault() const;
    void setHasChanges(bool changes);

    void applyButtonCallback();
    void resetButtonCallback();

private:
    void handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier &property) override;

private:
    void layoutWasUpdated() override;

private:

    std::unique_ptr<juce::Slider>   brightness;
    std::unique_ptr<juce::Slider>   hue;
    std::unique_ptr<juce::Slider>   temperature;

    std::unique_ptr<juce::Label>   brightnessLabel;
    std::unique_ptr<juce::Label>   hueLabel;
    std::unique_ptr<juce::Label>   temperatureLabel;

    std::unique_ptr<juce::TextButton>   applyButton;
    std::unique_ptr<juce::TextButton>   resetButton;

    bool hasChanges = false;
    std::shared_ptr<LumatoneLayout>     lastSavedLayout;

private:

    const float marginParentX       = 0.02f;
    int contentMarginX;
    int contentMarginH;

    const float sliderH             = 0.5f;

    // int buttonHeight;
    const float buttonTextMarginH   = 0.2f;

};

#endif // LUMATONE_EDITOR_BATCH_TOOLS_COLOUR_CONTROLSH
