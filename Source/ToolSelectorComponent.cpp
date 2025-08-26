#include "ToolSelectorComponent.h"
#include "CurvesArea.h"
#include "./backport/BatchToolsColourControls.h"

#include "LumatoneController.h"

#include "Main.h"

ToolSelectorComponent::ToolSelectorComponent()
    : juce::TabbedComponent(juce::TabbedButtonBar::Orientation::TabsAtTop)
{
    curvesArea = new CurvesArea();
    batchColourTools = new BatchToolsColourControls(TerpstraSysExApplication::getApp().getLumatoneController());

	LumatoneEditorLookAndFeel& lf = TerpstraSysExApplication::getApp().getLookAndFeel();
    juce::Colour tabColour = lf.findColour(LumatoneEditorColourIDs::LightBackground);
    addTab(juce::translate("Note Velocity"), tabColour, curvesArea, true);
    addTab(juce::translate("Global") + juce::translate(" Colour"), tabColour, batchColourTools, true);

    auto properties = TerpstraSysExApplication::getApp().getPropertiesFile();
    if (properties->getValue("ToolSelectorTab").contains(juce::String("Col")))
    {
        setCurrentTabIndex(1);
    }

    getTabbedButtonBar().addChangeListener(this);
}

ToolSelectorComponent::~ToolSelectorComponent()
{
    batchColourTools = nullptr;
    curvesArea = nullptr;

    getTabbedButtonBar().removeAllChangeListeners();
    selector = nullptr;
}

void ToolSelectorComponent::paint(juce::Graphics& g)
{
    g.setColour(findColour(LumatoneEditorColourIDs::ControlAreaBackground));
    g.fillRoundedRectangle(background, getParentHeight() * ROUNDEDCORNERTOAPPHEIGHT);
}

void ToolSelectorComponent::resized()
{
    juce::TabbedComponent::resized();
    
    setTabBarDepth(juce::roundToInt((float)getHeight() * 0.125f));
    background = getLocalBounds().withTrimmedTop(getTabBarDepth()).toFloat();
}

void ToolSelectorComponent::resetBatchColours()
{
    batchColourTools->updateFromState();
}

void ToolSelectorComponent::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    if (source == &getTabbedButtonBar())
    {
        TerpstraSysExApplication::getApp().getPropertiesFile()->setValue("ToolSelectorTab", getCurrentTabName());
    }
}
