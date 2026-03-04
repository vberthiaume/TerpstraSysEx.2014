/*
  ==============================================================================

    ColourReplacePanel.cpp
    Created: 2024
    Author:  Vincenzo

  ==============================================================================
*/

#include "ColourReplacePanel.h"
#include "Main.h"
#include "LumatoneController.h"
#include "ColourPaletteWindow.h"

//==============================================================================

void ColourSwatchButton::showPicker()
{
    TerpstraSysExApplication::getApp().reloadColourPalettes();

    auto* paletteWindow = new ColourPaletteWindow(TerpstraSysExApplication::getApp().getColourPalettes());
    paletteWindow->setSize(360, 300);
    paletteWindow->listenToColourSelection(this);
    paletteWindow->addComponentListener(this);
    activeWindow = paletteWindow;

    auto* topLevel = getTopLevelComponent();
    auto area = getScreenBounds().translated(-topLevel->getScreenX(), -topLevel->getScreenY());

    juce::CallOutBox::launchAsynchronously(
        std::unique_ptr<juce::Component>(paletteWindow),
        area,
        topLevel
    );
}

//==============================================================================

ColourReplacePanel::ColourReplacePanel()
    : juce::Component("ColourReplacePanel")
{
    swatchContainer = std::make_unique<juce::Component>("SwatchContainer");

    viewport = std::make_unique<juce::Viewport>();
    viewport->setViewedComponent(swatchContainer.get(), false);
    viewport->setScrollBarsShown(true, false);
    addAndMakeVisible(*viewport);

    emptyLabel = std::make_unique<juce::Label>("EmptyLabel", "No colours in mapping");
    emptyLabel->setJustificationType(juce::Justification::centred);
    emptyLabel->setVisible(false);
    addAndMakeVisible(*emptyLabel);
}

ColourReplacePanel::~ColourReplacePanel()
{
    swatches.clear();
    viewport  = nullptr;
    emptyLabel = nullptr;
}

//==============================================================================

void ColourReplacePanel::paint(juce::Graphics& /*g*/)
{
    // Background drawn by parent ToolSelectorComponent.
}

void ColourReplacePanel::resized()
{
    const auto bounds = getLocalBounds();
    emptyLabel->setBounds(bounds);
    viewport->setBounds(bounds);
    layoutSwatches();
}

//==============================================================================

void ColourReplacePanel::refreshMatchCount()
{
    auto* app = dynamic_cast<TerpstraSysExApplication*>(juce::JUCEApplication::getInstance());
    if (app == nullptr || app->getMainContentComponent() == nullptr)
    {
        rebuildSwatches({});
        return;
    }

    const TerpstraKeyMapping& mapping = app->getMainContentComponent()->getMappingInEdit();
    const int boardSize = app->getOctaveBoardSize();

    juce::Array<juce::Colour> uniqueColours;
    for (int board = 0; board < NUMBEROFBOARDS; board++)
        for (int key = 0; key < boardSize; key++)
        {
            const juce::Colour c = mapping.sets[board].theKeys[key].colour;
            if (c.getAlpha() > 0)
                uniqueColours.addIfNotAlreadyThere(c);
        }

    rebuildSwatches(uniqueColours);
}

//==============================================================================

void ColourReplacePanel::rebuildSwatches(const juce::Array<juce::Colour>& uniqueColours)
{
    swatches.clear(true);
    swatchContainer->removeAllChildren();

    const bool hasColours = !uniqueColours.isEmpty();
    emptyLabel->setVisible(!hasColours);
    viewport->setVisible(hasColours);

    if (!hasColours)
        return;

    for (const auto& colour : uniqueColours)
    {
        auto* btn = swatches.add(new ColourSwatchButton(colour));
        btn->onColourChanged = [this](juce::Colour orig, juce::Colour picked)
        {
            onSwatchColourChanged(orig, picked);
        };
        swatchContainer->addAndMakeVisible(btn);
    }

    layoutSwatches();
}

void ColourReplacePanel::layoutSwatches()
{
    if (swatches.isEmpty())
        return;

    const int panelW  = viewport->getWidth();
    const int panelH  = getHeight();
    const int margin  = juce::roundToInt(panelH * 0.04f);
    const int swatchSz = juce::roundToInt(panelH * 0.28f);

    const int usableW      = panelW - margin * 2;
    const int perRow       = juce::jmax(1, usableW / (swatchSz + margin));
    const int numRows      = juce::roundToInt(std::ceil(swatches.size() / (float) perRow));
    const int containerH   = margin + numRows * (swatchSz + margin);

    swatchContainer->setSize(panelW, juce::jmax(containerH, panelH));

    juce::FlexBox fb;
    fb.flexWrap        = juce::FlexBox::Wrap::wrap;
    fb.flexDirection   = juce::FlexBox::Direction::row;
    fb.alignContent    = juce::FlexBox::AlignContent::flexStart;
    fb.justifyContent  = juce::FlexBox::JustifyContent::flexStart;

    for (auto* btn : swatches)
    {
        juce::FlexItem item((float) swatchSz, (float) swatchSz, *btn);
        item.margin = juce::FlexItem::Margin((float) margin, 0.0f, 0.0f, (float) margin);
        fb.items.add(item);
    }

    fb.performLayout(swatchContainer->getLocalBounds().withTrimmedLeft(0).toFloat());
}

void ColourReplacePanel::onSwatchColourChanged(juce::Colour original, juce::Colour newColour)
{
    // performUndoableAction will call refreshAllFields → refreshColourReplace → refreshMatchCount,
    // which rebuilds and replaces the swatch list. This is safe because we are called from
    // callAsync (i.e. the old swatch's destructor has already completed).
    TerpstraSysExApplication::getApp().performUndoableAction(
        new Lumatone::ColourReplaceAction(original, newColour)
    );
}
