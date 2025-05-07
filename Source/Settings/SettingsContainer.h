/*
  ==============================================================================

    SettingsContainer.h
    Created: 5 Feb 2021 7:32:32pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include "../LumatoneEditorLookAndFeel.h"
#include "CalibrationDlg.h"
#include "FirmwareDlg.h"
#include "DeveloperDlg.h"
#include "PresetSettingsDlg.h"
#include "MidiSettingsDlg.h"

typedef enum {
    Calibration = 0,
    Firmware = 1,
    Midi        = 2,
    Presets     = 3,
    Developer = 4
} LumatoneEditorSettingCategories;

class SettingsCategoryModel : public ListBoxModel, public ChangeBroadcaster
{
public:

    SettingsCategoryModel();

    void refreshCategories();

    void setDeveloperMode(bool enableDeveloperMode);

    //=========================================================================
    // juce::ListBoxModel Implementation

    int getNumRows() override { return categories.size(); }

    void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) override;

    void selectedRowsChanged(int lastRowSelected) override { sendChangeMessage(); }


    //=========================================================================
private:
    Array<String> categories;

    bool showDeveloperPanel = false;
};

class SettingsContainer : public Component, protected ChangeListener
{
public:

    SettingsContainer();

    // Send change signal when destructed
    ~SettingsContainer();

    void paint(Graphics& g) override;

    void resized() override;

    void lookAndFeelChanged() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    //=========================================================================

    void showPanel(int editorSettingCategory);

    void setDeveloperMode(bool enableDeveloperMode);

private:

    SettingsCategoryModel model;
    std::unique_ptr<ListBox> categoryList;
    std::unique_ptr<Component> settingsPanel;

};
