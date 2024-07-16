#include "BatchTools.h"

#include "./BatchToolsColourControls.h"

#include "../LumatoneEditorLookAndFeel.h"


BatchTools::BatchTools(const LumatoneEditorState & stateIn)
    : LumatoneEditorState("BatchToolsControls", stateIn)
    , juce::Component("BatchToolsControls")
{
    colourControls = new BatchToolsColourControls(stateIn);

    tabs = std::make_unique<juce::TabbedComponent>(juce::TabbedButtonBar::Orientation::TabsAtTop);
    tabs->setName("BatchToolsTabs");
    tabs->setColour(juce::TabbedComponent::ColourIds::outlineColourId, juce::Colour());
    tabs->addTab("Colour", juce::Colour(), colourControls, true);
    tabs->getTabbedButtonBar().getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, 0.75f);
    addAndMakeVisible(tabs.get());

    batchToolsLabel = std::make_unique<juce::Label>("BatchToolsLabel", "Batch Tools");
    batchToolsLabel->setJustificationType(juce::Justification::centredLeft);
    batchToolsLabel->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LabelBlue));
    batchToolsLabel->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    addAndMakeVisible(batchToolsLabel.get());
}

BatchTools::~BatchTools()
{
    tabs = nullptr;
}

void BatchTools::paint(juce::Graphics &g)
{
    g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlAreaBackground));
    g.fillPath(controlPath);

    g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlAreaHeader));
    g.fillPath(headerPath);
}

void BatchTools::resized()
{
    tabs->setBounds(getLocalBounds());

    int windowH = getWindowBounds().getHeight();

    auto roundedCornerSize = getRoundedRectCornerSize();

    contentMarginWidth = juce::roundToInt(windowH * contentMarginWidthWindowH);

    headerHeight = roundToInt(windowH * contentLabelHeightWindowH);
    headerPath = getConnectedRoundedRectPath(getLocalBounds().withBottom(headerHeight + 1).toFloat(), roundedCornerSize, juce::Button::ConnectedEdgeFlags::ConnectedOnBottom);
    controlPath = getConnectedRoundedRectPath(getLocalBounds().withTop(headerHeight).toFloat(), roundedCornerSize, juce::Button::ConnectedEdgeFlags::ConnectedOnTop);

    tabs->setTabBarDepth(headerHeight);

    batchToolsLabel->setTopLeftPosition(contentMarginWidth, 0);
    resizeLabelWithHeight(batchToolsLabel.get(), headerHeight, contentLabelFontScalar);

    juce::TabbedButtonBar& tbb = tabs->getTabbedButtonBar();
    tbb.setBounds(tbb.getBounds().withTrimmedLeft(batchToolsLabel->getWidth() + contentMarginWidth));
}
