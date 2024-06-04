/*
  ==============================================================================

    MultiSelectControls.h
    Created: 3 June 2024
    Author:  Vincenzo

  ==============================================================================
*/

#include "MultiSelectControls.h"
#include "../LumatoneEditorLookAndFeel.h"

MultiSelectControls::MultiSelectControls(const LumatoneEditorState& stateIn)
        : LumatoneEditorState(stateIn)
        , juce::Component("MultiSelectControls")
{

}

MultiSelectControls::~MultiSelectControls()
{

}

void MultiSelectControls::paint(juce::Graphics& g)
{
    g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlAreaBackground));
    g.fillPath(controlPath);

    g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlAreaHeader));
    g.fillPath(headerPath);
}

void MultiSelectControls::resized()
{
    float h = (float)getHeight();

    headerHeight = roundToInt(h * headerH);
    headerPath = getConnectedRoundedRectPath(getLocalBounds().withBottom(headerHeight + 1).toFloat(), getRoundedRectCornerSize(), juce::Button::ConnectedEdgeFlags::ConnectedOnBottom);
    controlPath = getConnectedRoundedRectPath(getLocalBounds().withTop(headerHeight).toFloat(), getRoundedRectCornerSize(), juce::Button::ConnectedEdgeFlags::ConnectedOnTop);
}
