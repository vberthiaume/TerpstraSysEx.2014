/*
  ==============================================================================

    colour_palette_window.cpp
    Created: 14 Dec 2020 11:32:03pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "ColourSelectorPanel.h"

#include "../../lumatone_editor_library/palettes/colour_picker_panel.h"
#include "../../lumatone_editor_library/palettes/colour_selection_group.h"

#include "../../controls/colour_palette_component.h"
#include "./PaletteEditPanel.h"

#include "../../style/LumatoneEditorLookAndFeel.h"


//==============================================================================
// ColourSelectorPanel Definitions

ColourSelectorPanel::ColourSelectorPanel(const LumatoneEditorState& stateIn)
    : LumatoneEditorState("ColourSelectorPanel", stateIn)
    , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
{
    setName("ColourSelectorPanel");
    setLookAndFeel(&getEditorLookAndFeel());

    loadColourPalettesFromFile();

    // colourSelectorGroup.reset(new ColourSelectionGroup());

    newPaletteBtn = std::make_unique<juce::TextButton>("newPaletteBtn");
    newPaletteBtn->setButtonText(juce::translate("new"));
    newPaletteBtn->onClick = [&]()
    {
        newPaletteRequested();
    };
    addAndMakeVisible(*newPaletteBtn);
    editPaletteBtn = std::make_unique<juce::TextButton>("editPaletteBtn");
    editPaletteBtn->setButtonText(juce::translate("edit"));
    editPaletteBtn->onClick = [&]()
    {
        // editPaletteRequested(lastSelec)
    };
    addAndMakeVisible(*editPaletteBtn);
    deletePaletteBtn = std::make_unique<juce::TextButton>("deletePaletteBtn");
    deletePaletteBtn->setButtonText(juce::translate("delete"));
    deletePaletteBtn->onClick = [&]()
    {

    };
    addAndMakeVisible(*deletePaletteBtn);

    palettePanel.reset(new ColorPalettesControls(stateIn));
    palettePanel->addListener(this);
    palettePanel->setParams(-1, true);


    palettePanelViewport.reset(new juce::Viewport("PalettePanelViewport"));
    palettePanelViewport->setViewedComponent(palettePanel.get(), false);
    palettePanelViewport->setScrollBarsShown(true, true, false, false);
    palettePanelViewport->getVerticalScrollBar().setColour(juce::ScrollBar::ColourIds::thumbColourId, juce::Colour(0xff2d3135));
    palettePanelViewport->getHorizontalScrollBar().setColour(juce::ScrollBar::ColourIds::thumbColourId, juce::Colour(0xff2d3135));
    addChildComponent(palettePanelViewport.get());

    customPickerPanel.reset(new CustomPickerPanel());
    addChildComponent(customPickerPanel.get());
    addColourSelectionBroadcaster(customPickerPanel.get());
    addColourSelectionListener(customPickerPanel.get());
    // colourSelectorGroup->addSelector(customPickerPanel.get());
    // colourSelectorGroup->addColourSelectionListener(customPickerPanel.get());

    colourToolTabs.reset(new juce::TabbedComponent(juce::TabbedButtonBar::Orientation::TabsAtTop));
    colourToolTabs->setName("ColourSelectionToolTabs");
    // colourToolTabs->addTab(juce::translate("ColourPalettes"), juce::Colour(), palettePanelViewport.get(), false);
    // colourToolTabs->addTab(juce::translate("CustomPicker"), juce::Colour(), customPickerPanel.get(), false);
    colourToolTabs->addTab(juce::translate("ColourPalettes"), juce::Colour(), nullptr, false);
    colourToolTabs->addTab(juce::translate("CustomPicker"), juce::Colour(), nullptr, false);
    colourToolTabs->setColour(juce::TabbedComponent::ColourIds::outlineColourId, juce::Colour());
    colourToolTabs->getTabbedButtonBar().getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, 0.75f);
    colourToolTabs->getTabbedButtonBar().addChangeListener(this);
    addAndMakeVisible(*colourToolTabs);

    const int firstTabIndex = getProperty(LumatoneEditorProperty::LastColourWindowTab, "0").getIntValue();
    colourToolTabs->setCurrentTabIndex(firstTabIndex);
    colourToolTabChanged(firstTabIndex, false);
}

ColourSelectorPanel::~ColourSelectorPanel()
{
    setLookAndFeel(nullptr);

    deletePaletteBtn        = nullptr;
    editPaletteBtn          = nullptr;
    newPaletteBtn           = nullptr;

    paletteEditPanel        = nullptr;

    colourToolTabs          = nullptr;

    // colourSelectorGroup->removeSelector(customPickerPanel.get());
    removeColourSelectionListener(customPickerPanel.get());
    removeColourSelectionBroadcaster(customPickerPanel.get());
    customPickerPanel       = nullptr;

    palettePanelViewport    = nullptr;
    palettePanel            = nullptr;

    // colourSelectorGroup     = nullptr;
}

void ColourSelectorPanel::paint(juce::Graphics &g)
{
    g.setColour(backgroundColour);

    // auto controlBounds = getLocalBounds().withTop(colourToolTabs->getTabbedButtonBar().getHeight())
    //                                      .withTrimmedTop(indent)
    //                                      .withTrimmedBottom(indent*0.5)
    //                                      .toFloat();

    g.fillRoundedRectangle(controlArea.toFloat(), getRoundedRectCornerSize());

    // g.setColour(juce::Colours::green);
    // g.drawRect(getLocalBounds());
}

void ColourSelectorPanel::resized()
{
    float area = sqrtf(getWidth() * getHeight());

    indent = juce::roundToInt(area * viewportIndentScalar);
    colourToolTabs->setIndent(indent);

    colourToolTabs->setTabBarDepth(tabBarDepth);
    colourToolTabs->setBounds(getLocalBounds().withHeight(tabBarDepth));


    // int controlAreaWidth = getWidth() - ;
    // controlArea = juce::Rectangle<int>(newPaletteBtn->getRight() + indent, tabBarDepth + indent, , getHeight() - (indent * 2) - tabBarDepth);
    controlArea = getLocalBounds().withTop(colourToolTabs->getTabbedButtonBar().getHeight()).reduced(indent, indent);

    int buttonAreaHeight = controlArea.getHeight() - indent;
    int buttonHeight = juce::roundToInt(buttonAreaHeight * 0.25f);
    juce::Font textButtonFont = getEditorLookAndFeel().getTextButtonFont(*newPaletteBtn, buttonHeight);
    int buttonWidth = juce::jmax(textButtonFont.getStringWidth(juce::String("_")+newPaletteBtn->getButtonText())
                                 , textButtonFont.getStringWidth(juce::String("_")+editPaletteBtn->getButtonText())
                                 , textButtonFont.getStringWidth(juce::String("_")+deletePaletteBtn->getButtonText())
                                 );

    int paletteButtonY = controlArea.getY() + indent;
    float buttonHeightUnit = buttonAreaHeight / 3.0;
    newPaletteBtn->setBounds(controlArea.getX() + indent, paletteButtonY, buttonWidth, buttonHeight);
    editPaletteBtn->setBounds(controlArea.getX() + indent, paletteButtonY + buttonHeightUnit, buttonWidth, buttonHeight);
    deletePaletteBtn->setBounds(controlArea.getX() + indent, paletteButtonY + buttonHeightUnit * 2, buttonWidth, buttonHeight);

    int scrollSize = juce::roundToInt(area * viewportScrollbarAreaScalar);
    palettePanelViewport->setScrollBarThickness(scrollSize);

    juce::Rectangle<int> paletteSelectorArea = controlArea.withTrimmedLeft(buttonWidth + indent);
    // float windowHeight = getWindowBounds().getHeight();
    palettePanel->setParams(juce::roundToInt(buttonAreaHeight * 0.9f), true, 0);
    // palettePanel->setParams(juce::roundToInt(getWindowBounds().getHeight() * paletteControlWindowH), true, 0);
    palettePanelViewport->setBounds(paletteSelectorArea);
    palettePanel->updateGrid();
    palettePanel->resized();

    palettePanelViewport->setScrollBarsShown(false, true, palettePanel->getNumRows() > 1, palettePanel->getNumCols() > 1);

    if (paletteEditPanel.get())
        paletteEditPanel->setBounds(paletteSelectorArea);

    customPickerPanel->setBounds(controlArea.reduced(indent));
}

void ColourSelectorPanel::startEditingPalette(int paletteIndexIn, int selectedSwatchIndex)
{
    paletteIndexEditing = paletteIndexIn;
    paletteEditPanel.reset(new PaletteEditPanel(*this, LumatoneEditorState::getColourPalettes()[paletteIndexIn]));
    paletteEditPanel->setBounds(getLocalBounds());
    paletteEditPanel->setLookAndFeel(&getLookAndFeel());
    addAndMakeVisible(*paletteEditPanel);
    paletteEditPanel->addChangeListener(this);

    // Retain selected swatch
    if (selectedSwatchIndex >= 0)
        paletteEditPanel->setSelectedSwatch(selectedSwatchIndex);
}

void ColourSelectorPanel::duplicatePalette(int paletteIndexIn)
{
    auto palettes = getColourPalettes();
    auto copiedPalette = getColourPalettes()[paletteIndexIn].clone();

    if (! copiedPalette.saveToFile())
        return; // TODO

    // TODO this is a bit hacky
    palettes.insert(paletteIndexIn + 1, copiedPalette);
    setColourPalettes(palettes);

    palettePanel->updateGrid();
}

void ColourSelectorPanel::removePalette(int paletteIndexToRemove)
{
    // Remove loaded colour palette
    auto palettes = getColourPalettes();
    auto deletedPalette = palettes[paletteIndexToRemove];

    if (!deletedPalette.deleteFile())
        return; // TODO

    // TODO this is a bit hacky
    palettes.remove(paletteIndexToRemove);
    setColourPalettes(palettes);

    palettePanel->updateGrid();
}

void ColourSelectorPanel::customPickerRequested()
{
    palettePanel->setVisible(false);
    palettePanelViewport->setVisible(false);
    newPaletteBtn->setVisible(false);
    editPaletteBtn->setVisible(false);
    deletePaletteBtn->setVisible(false);
    customPickerPanel->setVisible(true);
}

void ColourSelectorPanel::selectPaletteRequested()
{
    customPickerPanel->setVisible(false);
    palettePanel->setVisible(true);
    palettePanelViewport->setVisible(true);
    newPaletteBtn->setVisible(true);
    editPaletteBtn->setVisible(true);
    deletePaletteBtn->setVisible(true);
}

void ColourSelectorPanel::editPaletteRequested(int paletteIndex, int selectedSwatchIndex)
{
    if (paletteIndex >= 0 && paletteIndex < getColourPalettes().size())
    {
        startEditingPalette(paletteIndex, selectedSwatchIndex);
    }
    else
        jassert(true); // Something bad happened!
}

void ColourSelectorPanel::clonePaletteRequested(int paletteIndex)
{
    if (paletteIndex >= 0 && paletteIndex < getColourPalettes().size())
    {
        duplicatePalette(paletteIndex);
    }
    else
        jassert(true); // Something bad happened!
}

void ColourSelectorPanel::deletePaletteRequested(int paletteIndex)
{
    if (paletteIndex >= 0 && paletteIndex < getColourPalettes().size())
    {
        removePalette(paletteIndex);
    }
    else
        jassert(true); // Something bad happened!
}

void ColourSelectorPanel::newPaletteRequested()
{
    paletteEditingIsNew = true;

    auto palettes = getColourPalettes();
    auto newPalette = LumatoneEditorColourPalette();
    newPalette.saveToFile();
    palettes.insert(0, newPalette);

    setColourPalettes(palettes);
    startEditingPalette(0, 0);
}

void ColourSelectorPanel::selectionChanged(int paletteIndex, int selectedSwatchIndex)
{
    // getColourSelectionGroup()->setCurrentSelector(palettePanel->getPaletteComponent(paletteIndex));
}

void ColourSelectorPanel::colourToolTabChanged(int newTab, bool write)
{
    if (newTab == 0)
        selectPaletteRequested();
    else if (newTab == 1)
        customPickerRequested();

    if (write)
        getPropertiesFile()->setValue(LumatoneEditorProperty::LastColourWindowTab, juce::String(newTab));
}

void ColourSelectorPanel::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    // Palette editing finished
    if (source == paletteEditPanel.get())
    {
        if (paletteEditPanel->wasSaveRequested())
        {
            if (paletteIndexEditing >= 0 && paletteIndexEditing < getColourPalettes().size())
            {
                auto palettes = getColourPalettes();
                auto palette = palettes.getReference(paletteIndexEditing);
                palette.setColours(paletteEditPanel->getCurrentPalette());

                juce::String newName = paletteEditPanel->getPaletteName();
                palette.setName(newName);

                palette.saveToFile();
                setColourPalettes(palettes);
            }
            else
                jassert(true); // Something bad happened!


            palettePanel->updateGrid();
        }
        else if (paletteEditingIsNew)
            removePalette(paletteIndexEditing);

        paletteIndexEditing = -1;
        paletteEditingIsNew = false;
        paletteEditPanel = nullptr;
    }

    else if (source == &colourToolTabs->getTabbedButtonBar())
    {
        const int newTab = colourToolTabs->getCurrentTabIndex();
        colourToolTabChanged(newTab);
    }
}

void ColourSelectorPanel::setIndentSize(int size, bool resize)
{
    indent = size;
    if (resize)
        resized();
}

void ColourSelectorPanel::setTabBarDepth(int height, bool resize)
{
    tabBarDepth = height;
    if (resize)
        resized();
}

void ColourSelectorPanel::setBackgroundColour(juce::Colour backgroundIn)
{
    backgroundColour = backgroundIn;
    repaint();
}
