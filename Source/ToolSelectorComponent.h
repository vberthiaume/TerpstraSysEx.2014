#ifndef TOOLSELECTORCOMPONENT_H
#define TOOLSELECTORCOMPONENT_H

#include <JuceHeader.h>

class CurvesArea;
class BatchToolsColourControls;
class ColourReplacePanel;

class ToolSelectorComponent : public juce::TabbedComponent,
                               private juce::ChangeListener
{
public:

    ToolSelectorComponent();
    ~ToolSelectorComponent();

    void paint(juce::Graphics&) override;
    void resized() override;

    CurvesArea* getCurvesArea() { return curvesArea; }

    void resetBatchColours();
    void refreshColourReplace();

private:
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:

    std::unique_ptr<juce::TabbedComponent> selector;

    CurvesArea* curvesArea = nullptr;
    BatchToolsColourControls* batchColourTools = nullptr;
    ColourReplacePanel* colourReplacePanel = nullptr;

    juce::Rectangle<float> background;
};


#endif // TOOLSELECTORCOMPONENT