/*
  ==============================================================================

    ColourSelectionPanels.h
    Created: 21 Dec 2020 9:48:08pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "./ColorPalettesControls.h"

#include "../../controls/colour_palette_component.h"
#include "ColorPalettesControls.h"

ColorPalettesControls::ColorPalettesControls(const LumatoneEditorState& stateIn)
    : LumatoneEditorState("ColourSelectorPanel", stateIn)
{
    addMouseListener(this, true);
    setRepaintsOnMouseActivity(true);

    newPalette.reset(new ColourPaletteComponent("EmptyPalette"));
    // newPaletteBtn.reset(new juce::TextButton("NewButton", juce::translate("NewPaletteTip")));
    // newPaletteBtn->setButtonText("+");
    // // newPaletteBtn->getProperties().set(LumatoneEditorStyleIDs::textButtonHyperlinkFlag, 1);
    // newPaletteBtn->onClick = [&] { listeners.call(&ColorPalettesControls::Listener::newPaletteRequested); };

    // addColourSelectionListener(this);
    updateGrid();
};

ColorPalettesControls::~ColorPalettesControls()
{
    for (auto palette : allPalettes)
        removeColourSelectionBroadcaster(palette);
        // selectionGroup.removeSelector(palette);

    paletteLabels.clear();
    controlGroups.clear();
    newPaletteBtn = nullptr;
    newPalette = nullptr;

    removeColourSelectionListener(this);
}

// int ColorPalettesControls::getHeightFromNumRows(int widthIn, int numRowsIn)
// {
//     float rowHeight = widthIn * (itemHeightScalar + topMarginScalar + bottomMarginScalar);
//     auto height = juce::roundToInt(numRowsIn * rowHeight);
//     return height;
// }

void ColorPalettesControls::setParams(int itemHeightIn, bool doAutoWidth, int maxColumns)
{
    if (itemHeightIn > 0)
        itemHeight = itemHeightIn;
    if (maxColumns >= 0)
        maxNumColumns = maxColumns;

    autoSize = doAutoWidth;

    updateGrid(false);
}

void ColorPalettesControls::paint(juce::Graphics& g)
{
    // g.setColour(juce::Colours::red);
    // g.drawRect(getLocalBounds());

    // g.setColour(juce::Colours::pink);
    // g.drawRect(newPalette->getBounds());

    //Draws rectangles around items and margins
    // for (int i = 0; i < allPalettes.size(); i++)
    // {
    //     g.setColour(juce::Colours::green);
    //     g.drawRect(allPalettes.getUnchecked(i)->getBounds());

    //     g.setColour(juce::Colours::blue);
    //     g.drawRect(controlGroupHitBoxes[i].reduced(1));
    // }

    // for (auto item : dbgItems)
    // {
    //    g.setColour(Colours::violet);
    //    g.drawRect(item.currentBounds);

    //    g.setColour(Colours::green);
    //    g.drawRect(item.currentBounds.getX(), item.currentBounds.getBottom(), item.width, item.margin.bottom, 1.0f);

    //    g.setColour(Colours::yellow);
    //    g.drawRect(item.currentBounds.getX() - item.margin.left - 1, item.currentBounds.getY(), item.margin.left - 1, item.currentBounds.getHeight(), 1.0f);
    //    g.drawRect(item.currentBounds.getRight() + 1, item.currentBounds.getY(), item.margin.right - 1, item.currentBounds.getHeight(), 1.0f);

    //    g.setColour(Colours::violet);
    //    g.drawRect(item.currentBounds.getX(), item.currentBounds.getY() - item.margin.top, item.width, item.margin.top);
    // }

    // g.setColour(juce::Colours::pink);
    // g.drawRect(0, 0, itemWidth, itemHeight);
    // g.drawRect(itemWidth, 0, itemWidth, itemHeight);
    // g.drawRect(itemWidth*2, 0, itemWidth, itemHeight);
};

void ColorPalettesControls::resized()
{
    // juce::Rectangle<int> viewportBounds(getWidth(), getHeight());
    juce::FlexBox flexBox(juce::FlexBox::Direction::row,
        juce::FlexBox::Wrap::wrap,
        juce::FlexBox::AlignContent::flexStart,
        juce::FlexBox::AlignItems::center,
        juce::FlexBox::JustifyContent::flexStart);

    // float itemWidth         = (float)viewportBounds.proportionOfWidth(itemWidthScalar);
    // float itemHeight        = (float)viewportBounds.proportionOfWidth(itemHeightScalar);

    // float topMargin         = (float)viewportBounds.proportionOfWidth(topMarginScalar);
    // float bottomMargin      = (float)viewportBounds.proportionOfWidth(bottomMarginScalar);
    // float horizontalMargin  = (float)(viewportBounds.getWidth() - (3 * itemWidth)) * 0.143f;


    // newBtnWidth = (float)itemWidth * itemToButtonWidth;
    int btnHeight = getHeight() / 3;
    // float buttonMargin = horizontalMargin + (itemWidth - newBtnWidth) * 0.25f;
    // newBtnItem.margin = juce::FlexItem::Margin(topMargin, buttonMargin, bottomMargin, buttonMargin);
    // juce::FlexItem newBtnItem(newBtnWidth, controlHeight, *newPaletteBtn);
    // newBtnItem.margin = paletteItemMargins;
    // newBtnItem.maxWidth = newBtnWidth;
    // flexBox.items.add(newBtnItem.withFlex(1.0f));

    const float newPaletteMarginH = 0.6f;
    newPaletteMarginWidth = juce::roundToInt(newBtnWidth * newPaletteMarginW);
    // int newPaletteMarginHeight = juce::roundToInt(newButtonArea.getHeight() * newPaletteMarginH * 0.5f);
    // juce::Rectangle<int> newButtonArea = juce::Rectangle<int>(newPalette->getX(), newPalette->getY(), itemWidth, itemHeight - controlHeight);
    // newPaletteBtn->setBounds(newButtonArea.reduced(newPaletteMarginWidth, newPaletteMarginHeight).toNearestInt());
    // newPaletteBtn->toFront(false);
    // newPaletteBtn->setBounds(0, 0, newBtnWidth, btnHeight);

    for (int i = 0; i < allPalettes.size(); i++)
    {
        // PaletteControlGroup* group = controlGroups.getUnchecked(i);
        // ColourPaletteComponent* palette = group->getPaletteComponent();
        ColourPaletteComponent* palette = allPalettes.getUnchecked(i);
        juce::FlexItem item(itemWidth, itemHeight, *palette);
        item.margin = paletteItemMargins;
        flexBox.items.add(item);
    }

    flexBox.performLayout(getBounds().withTrimmedLeft(newPaletteMarginWidth));

    // float bottomMarginControlHeight = juce::roundToInt(viewportBounds.proportionOfWidth(btmMarginCtrlScalar));
    // float bottomMarginControlSpace  = (bottomMargin - bottomMarginControlHeight) * 0.5f;


    float labelYItemOffset = itemHeight * 0.8f;
    for (int i = 0; i < controlGroups.size(); i++)
    {
        juce::FlexItem& item = flexBox.items.getReference(i);
        juce::Rectangle<float> bottomMarginBounds(item.currentBounds.getX(), item.currentBounds.getBottom()-controlHeight, itemWidth, paletteItemMargins.bottom);

        int fourthWidthItem = bottomMarginBounds.proportionOfWidth(0.25f);

        juce::Rectangle<float> labelBounds = item.currentBounds.withTrimmedTop(labelYItemOffset);
        juce::Point<float> controlsPosition;
        if (paletteLabels[i]->getText().isNotEmpty())
        {
            auto label = paletteLabels[i];
            label->setBounds(labelBounds.toNearestInt());
            controlsPosition = bottomMarginBounds.getPosition().translated(0, controlHeight);
        }
        else
        {
            controlsPosition = labelBounds.getPosition();
        }

        auto group = controlGroups.getUnchecked(i);
        group->getEditButton()->setSize(fourthWidthItem, controlHeight - paletteItemMargins.top);
        group->getEditButton()->setTopLeftPosition(controlsPosition.roundToInt().translated(bottomMarginBounds.proportionOfWidth(0.125f), paletteItemMargins.top));
        group->getCloneButton()->setBounds(group->getEditButton()->getBounds().translated(fourthWidthItem, 0));
        group->getTrashButton()->setBounds(group->getEditButton()->getBounds().translated(fourthWidthItem * 2.0f, 0));

        // auto hitBox = juce::Rectangle<float>(item.currentBounds.getTopLeft().translated(-horizontalMargin, -topMargin), bottomMarginBounds.getBottomRight()).toNearestInt();
        auto hitBox = juce::Rectangle<int>(item.currentBounds.toNearestInt());
        controlGroupHitBoxes.set(i, hitBox);
    }

    dbgItems = flexBox.items;

    // juce::Rectangle<int> newButtonArea = juce::Rectangle<int>(newPalette->getX(), newPalette->getY(), itemWidth, itemHeight - controlHeight);
    // const float newPaletteMarginW = 0.3f;
    // int newPaletteMarginWidth = juce::roundToInt(newButtonArea.getWidth() * newPaletteMarginW * 0.5f);
    // const float newPaletteMarginH = 0.6f;
    // int newPaletteMarginHeight = juce::roundToInt(newButtonArea.getHeight() * newPaletteMarginH * 0.5f);
    // newPaletteBtn->setBounds(newButtonArea.reduced(newPaletteMarginWidth, newPaletteMarginHeight).toNearestInt());
    // newPaletteBtn->toFront(false);

    needsResize = false;
}

void ColorPalettesControls::mouseMove(const juce::MouseEvent& mouse)
{
    int newIndex = -1;
    if (mouse.eventComponent->getProperties().contains("index"))
    {
        newIndex = mouse.eventComponent->getProperties()["index"];
    }
    else
    {
        for (int i = 0; i < controlGroups.size(); i++)
        {
            if (controlGroupHitBoxes[i].contains(mouse.getEventRelativeTo(this).position.roundToInt()))
            {
                newIndex = i;
                break;
            }
        }
    }

    if (newIndex != lastPaletteMouseOver && lastPaletteMouseOver >= 0 && lastPaletteMouseOver < controlGroups.size())
        setControlsVisibleForPalette(lastPaletteMouseOver, false);

    if (newIndex >= 0 && newIndex < controlGroups.size())
    {
        setControlsVisibleForPalette(newIndex, true);
        lastPaletteMouseOver = newIndex;
    }
}

// Setup panels from scratch
void ColorPalettesControls::updateGrid(bool resize)
{
    for (auto group : controlGroups)
    {
        // selectionGroup.removeSelector(group->getPaletteComponent());
        removeColourSelectionBroadcaster(group->getPaletteComponent());
        removeColourSelectionListener(group->getPaletteComponent());
    }

    removeAllChildren();
    // addAndMakeVisible(newPaletteBtn.get());
    addAndMakeVisible(newPalette.get());

    controlGroups.clear();
    paletteLabels.clear();

    allPalettes = juce::Array<ColourPaletteComponent*>();
    // allPalettes = juce::Array<ColourPaletteComponent*>();

    const LumatoneEditorColourPalettes& palettes = getColourPalettes();

    // Palettes with colour
    for (int i = 0; i < palettes.size(); i++)
    {
        auto group = controlGroups.add(new PaletteControlGroup(palettes.getReference(i)));

        auto paletteComponent = group->getPaletteComponent();
        paletteComponent->getProperties().set("index", i);
        addAndMakeVisible(paletteComponent);

        allPalettes.add(paletteComponent);

        // if (selectionGroup)
        // selectionGroup.addSelector(paletteComponent);
        addColourSelectionBroadcaster(group->getPaletteComponent());
        addColourSelectionListener(group->getPaletteComponent());

        group->getEditButton()->getProperties().set("index", i);
        group->getEditButton()->onClick = [&, i, paletteComponent] { listeners.call(&ColorPalettesControls::Listener::editPaletteRequested, i, paletteComponent->getSelectedSwatchNumber()); };
        addChildComponent(group->getEditButton());

        group->getCloneButton()->getProperties().set("index", i);
        group->getCloneButton()->onClick = [&, i, paletteComponent] { listeners.call(&ColorPalettesControls::Listener::clonePaletteRequested, i); };
        addChildComponent(group->getCloneButton());

        group->getTrashButton()->getProperties().set("index", i);
        group->getTrashButton()->onClick = [&, group, i] { listeners.call(&ColorPalettesControls::Listener::deletePaletteRequested, i); };
        addChildComponent(group->getTrashButton());

        juce::String paletteName = palettes[i].getName();
        auto label = paletteLabels.add(new juce::Label("Label_" + paletteName, paletteName));
        label->setJustificationType(juce::Justification::centred);
        label->getProperties().set("index", i);
        // label->getProperties().set(LumatoneEditorStyleIDs::labelMaximumLineCount, 2);
        addAndMakeVisible(*label);

        controlGroupHitBoxes.set(i, juce::Rectangle<int>());
    }

    numCols = juce::jmin(getColourPalettes().size(), maxNumColumns);
    numRows = 1;

    if (numCols > 1)
    {
        numRows = ceil((float)allPalettes.size() / (float)numCols);
    }

    updateGridBounds();
}

ColourPaletteComponent *ColorPalettesControls::getPaletteComponent(int paletteIndex)
{
    return allPalettes[paletteIndex];
}

void ColorPalettesControls::updateGridBounds()
{
    // update margins

    itemWidth     = juce::roundToInt(itemHeight * itemAspect);
    controlHeight = juce::roundToInt(itemHeight * controlHeightAspect);

    float itemScale = sqrtf(itemWidth * itemHeight);

    float topMargin         = itemScale * topMarginScalar;
    float bottomMargin      = itemScale * bottomMarginScalar + controlHeight;
    float horizontalMargin  = itemScale * horizontalMarginScalar * 0.5;
    paletteItemMargins = juce::FlexItem::Margin(topMargin, horizontalMargin, bottomMargin, horizontalMargin);

    int unitWidth = itemWidth + paletteItemMargins.left + paletteItemMargins.right;
    int unitHeight = itemHeight + paletteItemMargins.top + paletteItemMargins.bottom;

    if (numCols > 0)
        viewableWidth = (numCols) * unitWidth;
    else
        viewableWidth = allPalettes.size() * (unitWidth);

    viewableWidth += unitWidth;// * itemToButtonWidth;

    viewableHeight = numRows * unitHeight;

    if (autoSize)
    {
        setSize(viewableWidth, viewableHeight);
    }
}

void ColorPalettesControls::setControlsVisibleForPalette(int paletteIndex, bool areVisible)
{
    // auto group = controlGroups.getUnchecked(paletteIndex);
    // group->getEditButton()->setVisible(areVisible);
    // group->getCloneButton()->setVisible(areVisible);
    // group->getTrashButton()->setVisible(areVisible);
}

void ColorPalettesControls::colourChangedCallback(ColourSelectionBroadcaster *source, juce::Colour newColour)
{
    // bool isChild = false;
    // for (juce::Component* child : getChildren())
    // {
    //     for (const ColourPaletteComponent* palette : allPalettes)
    //     {
    //         if (palette == source)
    //         {
    //             isChild = true;
    //             break;
    //         }
    //     }
    // }

    // if (!isChild)
    //     for (ColourPaletteComponent* palette : allPalettes)
    //         palette->deselectColour();
}
