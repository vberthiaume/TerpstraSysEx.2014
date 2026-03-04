/*
  ==============================================================================

    BatchToolsColourControls.h
    Created: 16 July 2024
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_BATCH_TOOLS_COLOUR_CONTROLSH
#define LUMATONE_EDITOR_BATCH_TOOLS_COLOUR_CONTROLSH

// #include "./application_state.h"
// #include "./editor_listener.h"
// #include "./colour_selection_broadcaster.h"

#include <JuceHeader.h>
#include "./lumatone_layout.h"

class LumatoneController;

class BatchToolsColourControls : //public LumatoneApplicationState
                                // , protected LumatoneApplicationState::Controller
                                // , public LumatoneEditor::LayoutChangeListener
                                public juce::Component
{
public:
    BatchToolsColourControls(LumatoneController* stateIn);
    ~BatchToolsColourControls() override;

    void paint(juce::Graphics& g) override {}
    void resized() override;

    bool isSetToDefault() const;
    void setHasChanges(bool changes);


    void updateFromState();

private:
    void applyButtonCallback();
    void sendButtonCallback();
    void resetButtonCallback();

// private:
//     void handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier &property) override;

// private:
    // void layoutWasUpdated() override;

private:

    std::unique_ptr<juce::Slider>   brightness;
    std::unique_ptr<juce::Slider>   hue;
    std::unique_ptr<juce::Slider>   temperature;

    std::unique_ptr<juce::Label>   brightnessLabel;
    std::unique_ptr<juce::Label>   hueLabel;
    std::unique_ptr<juce::Label>   temperatureLabel;

    std::unique_ptr<juce::TextButton>   applyButton;
    std::unique_ptr<juce::TextButton>   sendButton;
    std::unique_ptr<juce::TextButton>   resetButton;

    bool hasChanges = false;
    bool isDragging = false;

private:

    LumatoneController*       state;

    const float marginParentX       = 0.02f;
    int contentMarginX;
    int contentMarginH;

    const float sliderH             = 0.4f;

    // int buttonHeight;
    const float buttonTextMarginH   = 0.2f;

};

#endif // LUMATONE_EDITOR_BATCH_TOOLS_COLOUR_CONTROLSH
