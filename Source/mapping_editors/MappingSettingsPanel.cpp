/*
  ==============================================================================

    MappingSettingsPanel.h
    Created: 14 May 2024
    Author:  Vincenzo

  ==============================================================================
*/

#include "./MappingSettingsPanel.h"
#include "./MappingSettingsControls.h"
#include "./CurvesArea.h"

MappingSettingsPanel::MappingSettingsPanel(const LumatoneEditorState& stateIn)
  : juce::Component("MappingSettingsPanel")
  , LumatoneEditorState(stateIn)
{
    settingsControls = std::make_unique<MappingSettingsControls>(stateIn);
    addAndMakeVisible(settingsControls.get());

    curvesArea = std::make_unique<CurvesArea>(stateIn);
    addAndMakeVisible(curvesArea.get());
}

MappingSettingsPanel::~MappingSettingsPanel()
{
    curvesArea = nullptr;
    settingsControls = nullptr;
}

void MappingSettingsPanel::paint(juce::Graphics& g)
{

}

void MappingSettingsPanel::resized()
{
    contentMarginY = proportionOfHeight(contentMarginHeightRatio);
    contentHeight = juce::roundToInt(getHeight() - contentMarginY * 2);

    settingsControls->setBounds(0, contentMarginY, proportionOfWidth(controlsWidthRatio), contentHeight);

    int curvesAreaW = proportionOfWidth(curvesAreaWidthRatio);
    int curvesAreaX = getWidth() - curvesAreaW;
    curvesArea->setBounds(curvesAreaX, contentMarginY, curvesAreaW, contentHeight);
}
