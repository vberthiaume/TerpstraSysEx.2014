#include "ColourDropdownSelector.h"

#include "../LumatoneEditorStyleCommon.h"

ColourDropdownSelector::ColourDropdownSelector(juce::String name, bool editable)
    : juce::ComboBox(name)
{
    setEditableText(editable);
    getProperties().set(LumatoneEditorStyleIDs::comboBoxEditorRestrictedChars, "0123456789ABCDEFabcdef");
    getProperties().set(LumatoneEditorStyleIDs::comboBoxEditorRestrictedLength, 6);
    getProperties().set(LumatoneEditorStyleIDs::comboBoxRenderColourItems, true);

    onChange = [&](){ valueChangedCallback(); };
}

ColourDropdownSelector::~ColourDropdownSelector()
{
}

void ColourDropdownSelector::paint(juce::Graphics &g)
{
    juce::ComboBox::paint(g);
}

void ColourDropdownSelector::resized()
{
    juce::ComboBox::resized();
}

void ColourDropdownSelector::setColourOptions(const juce::Array<juce::Colour> &colours)
{
    clear(juce::NotificationType::dontSendNotification);
    int id = 1;
    for (const juce::Colour& c : colours)
    {
        addItem(c.toDisplayString(false), id++);
    }

    findIdealComboBoxNumColumns(this, getNumItems());
}

void ColourDropdownSelector::setShowDropdown(bool show)
{
    showDropdown = show;
}

void ColourDropdownSelector::setShowPicker(bool show)
{
    showPicker = show;
}

void ColourDropdownSelector::setOnValueChangeCallback(std::function<void()> callbackIn)
{
    callback = callbackIn;
}

juce::Array<juce::Colour> ColourDropdownSelector::getColourOptions() const
{
    juce::Array<juce::Colour> options;
    for (int i = 0; i < getNumItems(); i++)
    {
        options.add(juce::Colour::fromString("ff" + getItemText(i)));
    }

    return options;
}

void ColourDropdownSelector::colourChangedCallback(ColourSelectionBroadcaster* source, juce::Colour newColour)
{
    // todo
}

juce::Colour ColourDropdownSelector::getSelectedColour()
{
    return lastSetColour;
}

void ColourDropdownSelector::deselectColour()
{
    setSelectedId(0, juce::NotificationType::sendNotification);
}

juce::Colour ColourDropdownSelector::parseInput() const
{
    juce::String text = getText();

    // Skip odd-numbered lengths and those less than 6 for RGB
    if (text.length() % 2 == 1 || text.length() != 6)
    {
        return juce::Colour();
    }

    juce::Colour colour = juce::Colour::fromString("ff" + text);

    if (!colour.isOpaque())
    {
        jassertfalse;
    }

    return colour;
}

void ColourDropdownSelector::valueChangedCallback()
{
    juce::Colour selectedColour = lastSetColour;

    if (getSelectedId() == 0)
    {
        juce::Colour parsedColour = parseInput();
        if (parsedColour.isOpaque())
        {
            selectedColour = parsedColour;

            // force lowercase
            setText(parsedColour.toDisplayString(false), juce::NotificationType::dontSendNotification);
        }
        else
        {
            // ignore
            return;
        }
    }
    else
    {
        selectedColour = juce::Colour::fromString("ff" + getText());
    }

    if (selectedColour != lastSetColour)
    {
        lastSetColour = selectedColour;
        selectorListeners.call(&ColourSelectionListener::colourChangedCallback, this, lastSetColour);
    }

    callback();
}
