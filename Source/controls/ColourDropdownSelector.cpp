#include "ColourDropdownSelector.h"

#include "../style/LumatoneEditorStyleCommon.h"

ColourDropdownSelector::ColourDropdownSelector(juce::String name, bool editable)
    : juce::Component(name)
{
    colourEditorBox = std::make_unique<juce::ComboBox>("ColourEditorBox");
    colourEditorBox->setEditableText(editable);
    colourEditorBox->getProperties().set(LumatoneEditorStyleIDs::comboBoxEditorRestrictedChars, "0123456789ABCDEFabcdef");
    colourEditorBox->getProperties().set(LumatoneEditorStyleIDs::comboBoxEditorRestrictedLength, 6);
    colourEditorBox->getProperties().set(LumatoneEditorStyleIDs::comboBoxRenderColourItems, true);
    colourEditorBox->onChange = [&](){ valueChangedCallback(); };
    addAndMakeVisible(colourEditorBox.get());

    colourPickerButton = std::make_unique<juce::TextButton>(name + "_ColourPickerButton", juce::translate("Select colour by clicking on key"));
    colourPickerButton->setButtonText("");
    colourPickerButton->setClickingTogglesState(true);
    colourPickerButton->onClick = [&](){ togglePickerListenForColour(colourPickerButton->getToggleState()); };
    colourPickerButton->getProperties().set(LumatoneEditorStyleIDs::textButtonIconHashCode, LumatoneEditorIcon::ColourPicker);
    addChildComponent(colourPickerButton.get());
}

ColourDropdownSelector::~ColourDropdownSelector()
{
    colourPickerButton = nullptr;
    colourEditorBox = nullptr;
}

void ColourDropdownSelector::paint(juce::Graphics &g)
{

}

void ColourDropdownSelector::resized()
{
    if (showPicker)
    {
        colourEditorBox->setBounds(getLocalBounds().withTrimmedRight(getHeight()));
    }
    else
    {
        colourEditorBox->setBounds(getLocalBounds());
    }

    colourPickerButton->setBounds(getLocalBounds().withLeft(getLocalBounds().getWidth() - getHeight()));
}

void ColourDropdownSelector::setColourOptions(const juce::Array<juce::Colour> &colours)
{
    colourEditorBox->clear(juce::NotificationType::dontSendNotification);
    int id = 1;
    for (const juce::Colour& c : colours)
    {
        colourEditorBox->addItem(c.toDisplayString(false), id++);
    }

    findIdealComboBoxNumColumns(colourEditorBox.get(), colourEditorBox->getNumItems());
}

void ColourDropdownSelector::setShowDropdown(bool show)
{
    showDropdown = show;
}

void ColourDropdownSelector::setShowPicker(bool show)
{
    showPicker = show;
    colourPickerButton->setVisible(show);
}

void ColourDropdownSelector::setSelectedColour(juce::Colour newColour, bool setText)
{
    if (!newColour.isOpaque())
        return; // Don't allow transparency

    if (newColour != lastSetColour)
    {
        lastSetColour = newColour;

        colourPickerButton->setColour(juce::TextButton::ColourIds::buttonColourId, newColour);
        colourPickerButton->setColour(juce::TextButton::ColourIds::buttonOnColourId, newColour);

        selectorListeners.call(&ColourSelectionListener::colourChangedCallback, this, lastSetColour);
    }

    if (setText)
    {
        colourEditorBox->setText(lastSetColour.toDisplayString(false), juce::NotificationType::dontSendNotification);
    }

    callbackColourChanged();
}

void ColourDropdownSelector::setOnValueChangeCallback(std::function<void()> callbackIn)
{
    callbackColourChanged = callbackIn;
}

void ColourDropdownSelector::setColourPickerChangedCallback(std::function<void()> callback)
{
    callbackPickerChanged = callback;
}

juce::Array<juce::Colour> ColourDropdownSelector::getColourOptions() const
{
    juce::Array<juce::Colour> options;
    for (int i = 0; i < colourEditorBox->getNumItems(); i++)
    {
        options.add(juce::Colour::fromString("ff" + colourEditorBox->getItemText(i)));
    }

    return options;
}

void ColourDropdownSelector::colourChangedCallback(ColourSelectionBroadcaster* source, juce::Colour newColour)
{
    // todo
    if (pickerIsListening)
    {
        setSelectedColour(newColour);

        colourPickerButton->setToggleState(false, juce::NotificationType::sendNotification);
        pickerIsListening = false;

        // callbackPickerChanged();
    }
}

juce::Colour ColourDropdownSelector::getSelectedColour()
{
    return lastSetColour;
}

void ColourDropdownSelector::deselectColour()
{
    colourEditorBox->setSelectedId(0, juce::NotificationType::sendNotification);
}

juce::Colour ColourDropdownSelector::parseInput() const
{
    juce::String text = colourEditorBox->getText();

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

    if (colourEditorBox->getSelectedId() == 0)
    {
        juce::Colour parsedColour = parseInput();
        if (parsedColour.isOpaque())
        {
            selectedColour = parsedColour;

            // force lowercase
            colourEditorBox->setText(parsedColour.toDisplayString(false), juce::NotificationType::dontSendNotification);
        }
        else
        {
            // ignore
            return;
        }
    }
    else
    {
        selectedColour = juce::Colour::fromString("ff" + colourEditorBox->getText());
    }

    setSelectedColour(selectedColour, false);
}

void ColourDropdownSelector::togglePickerListenForColour(bool listening)
{
    pickerIsListening = listening;
    if (pickerIsListening)
    {
        colourPickerButton->setColour(LumatoneEditorColourIDs::OutlineColourId, juce::Colours::white);
        callbackPickerChanged();
    }
    else
    {
        colourPickerButton->setColour(LumatoneEditorColourIDs::OutlineColourId, juce::Colour());
    }
}
