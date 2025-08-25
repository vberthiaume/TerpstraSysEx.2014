#include "ToolSelectorComponent.h"
#include "CurvesArea.h"
#include "./backport/BatchToolsColourControls.h"

#include "LumatoneController.h"

#include "Main.h"

ToolSelectorComponent::ToolSelectorComponent()
    : juce::Component("ToolSelectorComponent")
{

    curvesArea = new CurvesArea();
    batchColourTools = new BatchToolsColourControls(TerpstraSysExApplication::getApp().getLumatoneController());

    selector = std::make_unique<juce::TabbedComponent>(juce::TabbedButtonBar::Orientation::TabsAtTop);
    selector->addTab(juce::translate("Note Velocity"), juce::Colours::transparentBlack, curvesArea, true);
    selector->addTab(juce::translate("Colour") + juce::String(" Tools"), juce::Colours::transparentBlack, batchColourTools, true);
    addAndMakeVisible(selector.get());
}

ToolSelectorComponent::~ToolSelectorComponent()
{
    batchColourTools = nullptr;
    curvesArea = nullptr;
    selector = nullptr;
}

void ToolSelectorComponent::paint(juce::Graphics& g)
{
    g.setColour(findColour(LumatoneEditorColourIDs::ControlAreaBackground));
    g.fillRoundedRectangle(background, getParentHeight() * ROUNDEDCORNERTOAPPHEIGHT);
}

void ToolSelectorComponent::resized()
{
    selector->setBounds(getLocalBounds());
    selector->setTabBarDepth(juce::roundToInt((float)getHeight() * 0.125f));
    background = getLocalBounds().withTrimmedTop(selector->getTabBarDepth()).toFloat();
}
