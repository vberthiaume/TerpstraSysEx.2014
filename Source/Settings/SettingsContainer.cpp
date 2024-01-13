/*
  ==============================================================================

    SettingsContainer.cpp
    Created: 5 Feb 2021 7:32:32pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "SettingsContainer.h"

#include "../LumatoneEditorLookAndFeel.h"

#include "CalibrationDlg.h"
#include "FirmwareDlg.h"
#include "PresetSettingsDlg.h"
#include "MidiSettingsDlg.h"

void SettingsCategoryModel::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    juce::Colour backgroundColour = (rowIsSelected) ? Colours::teal : juce::Colour();
    juce::Rectangle<int> rowBounds(0, 0, width, height);

    if (rowIsSelected)
    {
        g.setColour(backgroundColour);
        g.fillRect(rowBounds);
    }

    g.setColour(Colours::white);

    g.setFont(font.withHeight(height));
    g.drawFittedText(categories[rowNumber], rowBounds.withLeft(8), juce::Justification::left, 1, 1.0f);
}

//=========================================================================

SettingsContainer::SettingsContainer(const LumatoneEditorState& stateIn)
    : Component("SettingsContainer")
    , LumatoneEditorState("SettingsContainer", stateIn)
    , model({
        translate("Calibrate"),
        translate("Firmware"),
        translate("MIDI"),
        translate("Presets")
      }, stateIn.getAppFonts().getFont(LumatoneEditorFont::GothamNarrowMedium)
       )
{
    categoryList.reset(new juce::ListBox("CategoryList"));
    categoryList->setModel(&model);

    addAndMakeVisible(categoryList.get());
    model.addChangeListener(this);

    auto lastPanelIndex = getProperty(LumatoneEditorProperty::LastSettingsPanel).getIntValue();
    categoryList->selectRow(lastPanelIndex);
}

SettingsContainer::~SettingsContainer()
{
    settingsPanel = nullptr;
    categoryList = nullptr;
}

void SettingsContainer::paint(Graphics& g)
{
    g.fillAll(findColour(ResizableWindow::ColourIds::backgroundColourId));
}

void SettingsContainer::resized()
{
    categoryList->setBounds(getLocalBounds().withRight(proportionOfWidth(0.2)));

    if (settingsPanel.get())
    {
        settingsPanel->setBounds(getLocalBounds().withLeft(categoryList->getRight()).reduced(0.033333f));
    }
}

void SettingsContainer::lookAndFeelChanged()
{
    setColour(juce::ResizableWindow::ColourIds::backgroundColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LightBackground));
    categoryList->setColour(juce::ListBox::ColourIds::backgroundColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::MediumBackground));
}
void SettingsContainer::changeListenerCallback(ChangeBroadcaster* source)
{
    auto panelIndex = categoryList->getSelectedRow();
    showPanel(panelIndex);
}

void SettingsContainer::showPanel(int editorSettingCategory)
{
    juce:: Component* newPanel = nullptr;
    switch (editorSettingCategory)
    {
    case LumatoneEditorSettingCategories::Calibration:
        newPanel = new CalibrationDlg(*this);
        break;

    case LumatoneEditorSettingCategories::Firmware:
        newPanel = new FirmwareDlg(*this);
        break;

    case LumatoneEditorSettingCategories::Midi:
        newPanel = new MidiSettingsDlg(*this);
        break;

    case LumatoneEditorSettingCategories::Presets:
        newPanel = new PresetSettingsDlg(*this);
        break;
    }

    if (newPanel)
    {
        removeChildComponent(settingsPanel.get());
        settingsPanel = nullptr;
        settingsPanel.reset(std::move(newPanel));
        addAndMakeVisible(settingsPanel.get());
        settingsPanel->setLookAndFeel(&getLookAndFeel());

        state.setPropertyExcludingListener(this, LumatoneEditorProperty::LastSettingsPanel, editorSettingCategory, nullptr);

        resized();
    }
}
