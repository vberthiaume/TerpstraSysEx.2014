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

class ColourDropdownSelector : public juce::ComboBox
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

    void setOnValueChangeCallback(std::function<void()> callback);

    juce::Array<juce::Colour> getColourOptions() const;

    // ColourSelectionListener implementation
    void colourChangedCallback(ColourSelectionBroadcaster* source, juce::Colour newColour) override;

    // ColourSelectionBroadcaster implementation
    juce::Colour getSelectedColour() override;
    void deselectColour() override;

private:
    juce::Colour parseInput() const;

    void valueChangedCallback();

private:

    using juce::ComboBox::onChange; // don't allow public mutation


    // juce::Array<juce::Colour> colourOptions;

    juce::Colour lastSetColour;

    std::function<void()> callback = [](){};

    bool showDropdown = true;
    bool showPicker = true;

};


#endif // LUMATONE_COLOUR_DROPDOWN_SELECTOR_H
