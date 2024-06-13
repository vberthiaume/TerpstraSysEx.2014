#include "KeyEditorPanel.h"
#include "../LumatoneEditorLookAndFeel.h"

#include "./KeyEditorControls.h"
#include "./MultiSelectControls.h"

KeyEditorPanel::KeyEditorPanel(const LumatoneEditorState &stateIn)
  : juce::Component("KeyEditorPanel")
  , LumatoneEditorState("KeyEditorPanel", stateIn)
{
    keyEditorControls = std::make_unique<KeyEditorControls>(stateIn);
    addAndMakeVisible(keyEditorControls.get());

    multiSelectControls = std::make_unique<MultiSelectControls>(stateIn);
    addAndMakeVisible(multiSelectControls.get());
}

KeyEditorPanel::~KeyEditorPanel()
{
    multiSelectControls = nullptr;
    keyEditorControls = nullptr;
}

void KeyEditorPanel::paint(juce::Graphics &g)
{
    g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::MediumBackground));
    g.fillRoundedRectangle(keySettingsAndMultiSelectArea, getRoundedRectCornerSize());
    g.fillRoundedRectangle(batchToolsArea, getRoundedRectCornerSize());
}

void KeyEditorPanel::resized()
{
    float w = (float)getWidth();
    float h = (float)getHeight();

    areaHeight = roundToInt(h * areaHeightRatio);
    areaMargin = roundToInt((h - areaHeight) * 0.5f);

    keySettingsAndMultiSelectArea = juce::Rectangle<float>(0, areaMargin, roundToInt(w * keySettingsAndSelectAreaW), areaHeight);

    int batchToolsAreaWidth = roundToInt(w * batchToolsAreaW);
    batchToolsArea = juce::Rectangle<float>(w - batchToolsAreaWidth, areaMargin, batchToolsAreaWidth, areaHeight);

    contentHeight = roundToInt(h * contentHeightRatio);
    contentMargin = roundToInt(h - (float)contentHeight) * 0.5;

    keyEditorControls->setBounds(keySettingsAndMultiSelectArea.reduced(contentMargin)
                                                              .withWidth(roundToInt(keySettingsAndMultiSelectArea.getWidth() * keySettingsComponentAreaW))
                                                              .toNearestInt());

    multiSelectWidth = roundToInt(keySettingsAndMultiSelectArea.getWidth() * multiSelectComponentAreaW);
    multiSelectControls->setBounds(keySettingsAndMultiSelectArea.reduced(contentMargin)
                                                                .withLeft(keySettingsAndMultiSelectArea.getRight() - multiSelectWidth - contentMargin)
                                                                .toNearestInt());
}
