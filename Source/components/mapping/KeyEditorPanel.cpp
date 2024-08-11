#include "KeyEditorPanel.h"
#include "../../style/LumatoneEditorLookAndFeel.h"

#include "./KeyEditorControls.h"
#include "./MultiSelectControls.h"
#include "./BatchTools.h"

KeyEditorPanel::KeyEditorPanel(const LumatoneEditorState &stateIn)
  : juce::Component("KeyEditorPanel")
  , LumatoneEditorState("KeyEditorPanel", stateIn)
{
    keyEditorControls = std::make_unique<KeyEditorControls>(stateIn);
    addAndMakeVisible(keyEditorControls.get());

    multiSelectControls = std::make_unique<MultiSelectControls>(stateIn);
    addAndMakeVisible(multiSelectControls.get());

    batchTools = std::make_unique<BatchTools>(stateIn);
    addAndMakeVisible(batchTools.get());
}

KeyEditorPanel::~KeyEditorPanel()
{
    batchTools = nullptr;
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

    float windowH = (float)getWindowBounds().getHeight();

    areaHeight = juce::roundToInt(h * areaHeightRatio);
    areaMargin = juce::roundToInt((h - areaHeight) * 0.5f);

    keySettingsAndMultiSelectArea = juce::Rectangle<float>(0, areaMargin, juce::roundToInt(w * keySettingsAndSelectAreaW), areaHeight);

    int batchToolsAreaWidth = juce::roundToInt(w * batchToolsAreaW);
    batchToolsArea = juce::Rectangle<float>(w - batchToolsAreaWidth, areaMargin, batchToolsAreaWidth, areaHeight);

    contentMargin = juce::roundToInt(windowH * contentMarginWidthWindowH);
    keyEditorControls->setBounds(keySettingsAndMultiSelectArea.reduced(contentMargin)
                                                              .withWidth(roundToInt(keySettingsAndMultiSelectArea.getWidth() * keySettingsComponentAreaW))
                                                              .toNearestInt());

    multiSelectWidth = roundToInt(keySettingsAndMultiSelectArea.getWidth() * multiSelectComponentAreaW);
    multiSelectControls->setBounds(keySettingsAndMultiSelectArea.reduced(contentMargin)
                                                                .withLeft(keySettingsAndMultiSelectArea.getRight() - multiSelectWidth - contentMargin)
                                                                .toNearestInt());

    batchTools->setBounds(batchToolsArea.reduced(contentMargin).toNearestInt());
}
