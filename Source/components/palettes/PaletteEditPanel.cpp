/*
  ==============================================================================

    ColourSelectionPanels.h
    Created: 21 Dec 2020 9:48:08pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "./PaletteEditPanel.h"

#include "../../style/LumatoneEditorLookAndFeel.h"
#include "../../controls/colour_palette_component.h"

PaletteEditPanel::PaletteEditPanel(const LumatoneEditorState& stateIn, const LumatoneEditorColourPalette& paletteIn)
    : LumatoneEditorState("PaletteEditPanel", stateIn)
    , colourPalette(paletteIn)
{
    colourPicker.reset(new juce::ColourSelector(
        juce::ColourSelector::ColourSelectorOptions::editableColour
        + juce::ColourSelector::ColourSelectorOptions::showColourAtTop
        + juce::ColourSelector::ColourSelectorOptions::showColourspace
    ));
    addAndMakeVisible(*colourPicker);

    paletteControl.reset(new TenHexagonPalette());
    paletteControl->setColourPalette(*colourPalette.getColours());
    paletteControl->attachColourSelector(colourPicker.get());
    addAndMakeVisible(*paletteControl);

    editPaletteLabel.reset(new juce::Label("EditPaletteLabel", juce::translate("EditPalette")));
    editPaletteLabel->setJustificationType(juce::Justification::centred);
    addAndMakeVisible(*editPaletteLabel);

    paletteNameEditor.reset(new juce::Label("PaletteNameEditor"));
    paletteNameEditor->setJustificationType(juce::Justification::centred);
    paletteNameEditor->setEditable(true);
    paletteNameEditor->addListener(this);
    paletteNameEditor->setColour(juce::Label::ColourIds::backgroundColourId, juce::Colour());
    paletteNameEditor->setText(colourPalette.getName(), juce::dontSendNotification);
    paletteNameEditor->getProperties().set(LumatoneEditorStyleIDs::labelMaximumLineCount, 3);
    addAndMakeVisible(*paletteNameEditor);
    labelTextChanged(paletteNameEditor.get()); // force update


    saveButton.reset(new juce::TextButton("SaveButton", juce::translate("SavePaletteTip")));
    saveButton->setButtonText("Save");
    saveButton->addListener(this);
    addAndMakeVisible(*saveButton);

    cancelButton.reset(new juce::TextButton("CancelButton", juce::translate("CancelPaletteTip")));
    cancelButton->setButtonText("Cancel");
    cancelButton->addListener(this);
    addAndMakeVisible(*cancelButton);
}

PaletteEditPanel::~PaletteEditPanel()
{
    paletteControl = nullptr;
}

void PaletteEditPanel::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void PaletteEditPanel::resized()
{
    int leftWidth = juce::roundToInt(getWidth() * leftColumnWidth);
    int leftCenter = juce::roundToInt(leftWidth * 0.5f);

    auto labelText = editPaletteLabel->getText() + " ";
    int labelWidth = editPaletteLabel->getFont().getStringWidth(labelText);
    resizeLabelWithHeight(editPaletteLabel.get(), proportionOfHeight(editPaletteHeight));
    editPaletteLabel->setSize(labelWidth, proportionOfHeight(editPaletteHeight));
    editPaletteLabel->setCentrePosition(leftCenter, juce::roundToInt(editPaletteLabel->getHeight() * 0.5f + proportionOfHeight(editPaletteLabelY)));

    int paletteWidth = proportionOfWidth(paletteWidthScalar);
    int paletteHeight = proportionOfHeight(paletteHeightScalar);
    paletteControl->setSize(paletteWidth, paletteHeight);
    paletteControl->setCentrePosition(leftCenter, juce::roundToInt(paletteHeight * 0.5f + proportionOfHeight(paletteY)));

    saveButton->setSize(proportionOfWidth(buttonWidth), proportionOfHeight(buttonHeight));
    saveButton->setCentrePosition(leftCenter, juce::roundToInt(saveButton->getHeight() * 0.5f + proportionOfHeight(buttonY)));

    int cancelMarginY = juce::roundToInt(saveButton->getHeight() * 1.125f);
    cancelButton->setBounds(saveButton->getBounds().translated(0, cancelMarginY));

    colourPicker->setSize(proportionOfWidth(pickerWidth), proportionOfHeight(pickerHeight));
    colourPicker->setTopLeftPosition(leftWidth, juce::roundToInt((getHeight() - colourPicker->getHeight()) * 0.5f));

    int leftMargin = juce::roundToInt(colourPicker->getRight() * 0.03f * 0.5f);
    paletteNameEditor->setBounds(juce::Rectangle<int>(
        juce::Point<int>(juce::roundToInt(leftMargin), paletteControl->getBottom()),
        juce::Point<int>(colourPicker->getX() - leftMargin, saveButton->getY())
        ));
    paletteNameEditor->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, editPaletteLabel->getHeight() / (float)paletteNameEditor->getHeight());

    juce::Font labelFont = (paletteUnnamed)
        ? getAppFonts().getFont(LumatoneEditorFont::UniviaProBold)
        : getAppFonts().getFont(LumatoneEditorFont::GothamNarrowMedium);
    paletteNameEditor->setFont(labelFont);
}

//==============================================================================

void PaletteEditPanel::buttonClicked(juce::Button* btn)
{
    if (btn == saveButton.get())
    {
        saveRequested = true;
    }

    sendChangeMessage();
}

//==============================================================================

void PaletteEditPanel::labelTextChanged(juce::Label* label)
{
    // Italics and font override here is a bit of a hack
    if (label == paletteNameEditor.get())
    {
        bool nameIsEmpty = paletteNameEditor->getText() == "";

        if (!nameIsEmpty)
        {
            paletteUnnamed = false;
            colourPalette.setName(paletteNameEditor->getText());
                paletteNameEditor->getProperties().remove(LumatoneEditorStyleIDs::fontOverrideTypefaceStyle);
                paletteNameEditor->getProperties().set(LumatoneEditorStyleIDs::fontOverride, LumatoneEditorFont::GothamNarrowMedium);
        }

        else if (nameIsEmpty && !paletteUnnamed)
        {
            paletteUnnamed = true;
        }

        if (paletteUnnamed)
        {
            paletteNameEditor->setText("unnamed", juce::NotificationType::dontSendNotification);
            paletteNameEditor->getProperties().set(LumatoneEditorStyleIDs::fontOverrideTypefaceStyle, "Italic");
            paletteNameEditor->getProperties().remove(LumatoneEditorStyleIDs::fontOverride);
        }

        //resized();
    }
}

void PaletteEditPanel::editorShown(juce::Label*, juce::TextEditor& editor)
{
    editor.setColour(juce::TextEditor::ColourIds::backgroundColourId, juce::Colours::black);
    editor.setJustification(juce::Justification::centred);
    editor.setMultiLine(true);
}

//==============================================================================

juce::Array<juce::Colour> PaletteEditPanel::getCurrentPalette() const
{
    return paletteControl->getColourPalette();
}

void PaletteEditPanel::setSelectedSwatch(int selectedSwatchNumber)
{
    if (selectedSwatchNumber >= 0 && selectedSwatchNumber < paletteControl->getNumberOfSwatches())
        paletteControl->setSelectedSwatchNumber(selectedSwatchNumber);
}

juce::String PaletteEditPanel::getPaletteName()
{
    return colourPalette.getName();
}

bool PaletteEditPanel::wasSaveRequested() const
{
    return saveRequested;
}
