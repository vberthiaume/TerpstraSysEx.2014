/*
  ==============================================================================

    ColourDropdownSelector.h
    Created: 11 July 2024
    Author:  Vincenzo

    Colour selector with text editing and optional dropdown

  ==============================================================================
*/

#ifndef LUMATONE_COLOUR_DROPDOWN_SELECTOR_H
#define LUMATONE_COLOUR_DROPDOWN_SELECTOR_H

#include "../lumatone_editor_library/palettes/colour_selection_group.h"

class ColourDropdownSelector : public juce::Component
                             , public ColourSelectionBroadcaster
                             , public ColourSelectionListener
{
public:

    ColourDropdownSelector(juce::String name, bool editable=true);
    ~ColourDropdownSelector() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setColourOptions(const juce::Array<juce::Colour>& colours);
    void setShowDropdown(bool show);
    void setShowPicker(bool show);

    void setSelectedColour(juce::Colour newColour, bool sendNotification = true, bool doCallback = true);
    void clearColour(bool sendNotification = true);

    void setOnValueChangeCallback(std::function<void()> callback);
    void setColourPickerChangedCallback(std::function<void()> callback);

    juce::Array<juce::Colour> getColourOptions() const;

    // ColourSelectionListener implementation
    void colourChangedCallback(ColourSelectionBroadcaster* source, juce::Colour newColour) override;

    // ColourSelectionBroadcaster implementation
    juce::Colour getSelectedColour() override;
    void deselectColour() override;

    void setEditText(bool enableTextEdit);

private:
    juce::Colour parseInput() const;

    void valueChangedCallback();

    void togglePickerListenForColour(bool listening);

private:

    std::unique_ptr<juce::ComboBox>     colourEditorBox;
    std::unique_ptr<juce::TextButton>   colourPickerButton;

    juce::Colour lastSetColour;
    bool pickerIsListening = false;

    std::function<void()> callbackColourChanged = [](){};
    std::function<void()> callbackPickerChanged = [](){};

    bool showDropdown = true;
    bool showPicker = false;

};


#endif // LUMATONE_COLOUR_DROPDOWN_SELECTOR_H
