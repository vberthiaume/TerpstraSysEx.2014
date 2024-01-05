/*
  ==============================================================================

    PresetSettingsDlg.cpp
    Created: 29 Jun 2021 9:10:00pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "PresetSettingsDlg.h"
#include "../lumatone_editor_library/device/lumatone_controller.h"

PresetSettingsDlg::PresetSettingsDlg(const LumatoneEditorState& stateIn)
    : LumatoneEditorState("PresetSettingsDlg", stateIn)
{
    resetPresetsBtn.reset(new TextButton(
        juce::translate("Reset Presets To Factory Default"),
        juce::translate("Clear the mapping presets stored on the device and replace with factory mappings.")
    ));
    addAndMakeVisible(resetPresetsBtn.get());
    resetPresetsBtn->addListener(this);

    // getLumatoneController()->addFirmwareListener(this);
    setSupportedControls(getFirmwareVersion());

    flexBox.justifyContent = FlexBox::JustifyContent::flexStart;
    flexBox.alignContent = FlexBox::AlignContent::flexStart;
}

PresetSettingsDlg::~PresetSettingsDlg()
{
    // getLumatoneController()->removeFirmwareListener(this);
    resetPresetsBtn = nullptr;
}

void PresetSettingsDlg::paint(Graphics& g)
{

}

void PresetSettingsDlg::resized()
{
    juce::Font btnFont = getLookAndFeel().getTextButtonFont(*resetPresetsBtn.get(), buttonHeight);

    int btnWidth = btnFont.getStringWidth(resetPresetsBtn->getButtonText()) * 1.2f;

    flexBox.items.clear();

    juce::FlexItem btnItem = juce::FlexItem().withWidth(btnWidth).withHeight(buttonHeight);
    btnItem.associatedComponent = resetPresetsBtn.get();
    flexBox.items.add(btnItem);

    flexBox.performLayout(getLocalBounds().reduced(margin));
}

void PresetSettingsDlg::buttonClicked(Button* btn)
{
    if (btn == resetPresetsBtn.get())
    {
        getLumatoneController()->resetPresetsToFactoryDefault();
    }
}

void PresetSettingsDlg::firmwareRevisionReceived(LumatoneFirmware::Version version)
{
    setSupportedControls(version);
}

void PresetSettingsDlg::setSupportedControls(LumatoneFirmware::Version version)
{
    if (getFirmwareSupport().versionAcknowledgesCommand(version, RESET_DEFAULT_PRESETS))
    {
        resetPresetsBtn->setEnabled(true);
        resetPresetsBtn->setTooltip(juce::translate("Clear the mapping presets stored on the device and replace with factory mappings."));
    }
    else
    {
        resetPresetsBtn->setEnabled(false);
        resetPresetsBtn->setTooltip(juce::translate("This feature is not supported by your Lumatone firmware version."));
        // TODO: better approach for changing tooltips
    }
}
