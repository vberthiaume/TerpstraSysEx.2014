/*
  ==============================================================================

    SettingsContainer.h
    Created: 5 Feb 2021 7:32:32pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "../LumatoneEditorState.h"

typedef enum {
    Calibration = 0,
    Firmware    = 1,
    Midi        = 2,
    Presets     = 3
} LumatoneEditorSettingCategories;

class SettingsCategoryModel : public ListBoxModel, public ChangeBroadcaster
{
public:

    SettingsCategoryModel(StringArray categoryNamesIn, juce::Font fontIn) : font(fontIn)
    {
        for (auto name : categoryNamesIn)
        {
            categories.add(name);
        }
    }

    int getNumRows() override { return categories.size(); }

    void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) override;

    void selectedRowsChanged(int lastRowSelected) override { sendChangeMessage(); }



    //=========================================================================
private:
    Array<String> categories;
    juce::Font font;
};

class SettingsContainer : public Component
                        , private LumatoneEditorState
                        , protected ChangeListener
{
public:

    SettingsContainer(const LumatoneEditorState& stateIn);

    // Send change signal when destructed
    ~SettingsContainer();

    void paint(Graphics& g) override;

    void resized() override;

    void lookAndFeelChanged() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    //=========================================================================

    void showPanel(int editorSettingCategory);

private:

    SettingsCategoryModel model;
    std::unique_ptr<ListBox> categoryList;
    std::unique_ptr<Component> settingsPanel;

};
