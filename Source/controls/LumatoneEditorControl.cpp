#include "LumatoneEditorControl.h"
#include "ColourDropdownSelector.h"

#include "../style/LumatoneEditorLookAndFeel.h"
#include "../lumatone_editor_library/common/math.h"


LumatoneEditorControl::LumatoneEditorControl(const LumatoneEditorState& stateIn, juce::String name, int minValueIn, int maxValueIn, LumatoneEditorControl::Style style, bool hasClearButton)
    : LumatoneEditorState(name, stateIn)
    , juce::Component(name)
    , range(juce::Range<int>(minValueIn, maxValueIn))
    , valueChangedCallback([]() {}) // no operation
{
    setShowClearButton(hasClearButton);
    setStyle(style);
    setValue(range.getStart() - 1);

    setColour(ColourIds::outline, juce::Colour());
}

LumatoneEditorControl::LumatoneEditorControl(const LumatoneEditorState& stateIn, juce::String name, LumatoneEditorControl::Style style, bool hasClearButton)
    : LumatoneEditorControl(stateIn, name, 0, 0, style, hasClearButton)
{
}

LumatoneEditorControl::~LumatoneEditorControl()
{
    clearButton = nullptr;
    label = nullptr;
    component = nullptr;
    box = nullptr;
    slider = nullptr;
    colourDropdownInput = nullptr;
}

void LumatoneEditorControl::paint(juce::Graphics &g)
{
    juce::Colour outline = findColour(ColourIds::outline);
    if (outline.isTransparent() == false)
    {
        g.setColour(outline);
        g.drawRect(getLocalBounds(), 1);

        if (label)
            g.drawRect(label->getBounds(), 1);
        g.drawRect(component->getBounds(), 1);
    }
}

void LumatoneEditorControl::resized()
{
    component->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, CONTROLBOXFONTHEIGHTSCALAR);

    juce::Rectangle<int> controlBounds = getLocalBounds();
    if (controlBounds.isEmpty())
        return;

    flexLayout = juce::FlexBox();

    juce::FlexBox controlLayout;
    controlLayout.flexDirection = juce::FlexBox::Direction::row;

    juce::FlexItem controlItem = juce::FlexItem(*component);
    controlItem.flexGrow = 1.0f;
    controlLayout.items.add(controlItem);

    juce::FlexItem controlLayoutItem = juce::FlexItem(controlLayout);
    controlLayoutItem.width = controlBounds.getWidth();
    controlLayoutItem.maxWidth = controlBounds.getWidth();
    controlLayoutItem.height = controlBounds.getHeight();
    controlLayoutItem.maxHeight = controlBounds.getHeight();
    controlLayoutItem.flexGrow = 1.0f;

    int margin = juce::roundToInt(getHeight() * clearButtonMarginScalar);
    int rowHeight = controlBounds.getHeight();

    if (label != nullptr)
    {
        juce::FlexItem labelItem = juce::FlexItem(*label);
        labelItem.margin = juce::FlexItem::Margin(0, margin, 0, 0);

        // int labelHeight = rowHeight * controlLabelFontScalar;
        int labelHeight = rowHeight;

        if (labelLocation == LabelLocation::Left)
        {
            flexLayout.flexDirection = juce::FlexBox::Direction::row;
            flexLayout.alignItems = juce::FlexBox::AlignItems::center;
        }
        else if (labelLocation == LabelLocation::Top)
        {
            flexLayout.flexDirection = juce::FlexBox::Direction::column;

            rowHeight = juce::roundToInt(rowHeight * 0.5f);
            labelHeight = rowHeight * controlLabelFontScalar;

            labelItem.margin.bottom = rowHeight * 0.2f;
            controlLayoutItem.height = rowHeight;
            controlLayoutItem.maxHeight = rowHeight;
        }

        labelItem.height = labelHeight;
        labelItem.maxHeight = labelHeight;
        juce::Font labelFont = label->getFont().withHeight(labelItem.height);
        label->setFont(labelFont);

        int labelWidth = labelWidthInput;
        if (labelWidthInput <= 0)
        {
            labelWidth = juce::roundToInt(labelFont.getStringWidth(label->getText()));
        }

        labelItem.width = labelWidth;
        labelItem.maxWidth = labelWidth;
        labelItem.flexGrow = 1.0f;

        flexLayout.items.add(labelItem);
    }

    if (showClearButton)
    {
        int clearButtonMaxSize = rowHeight * 0.6f;
        int buttonMargin = (rowHeight - clearButtonMaxSize) * 0.5;

        juce::FlexItem clearButtonItem = juce::FlexItem(*clearButton);
        clearButtonItem.margin = juce::FlexItem::Margin(buttonMargin);
        clearButtonItem.width = clearButtonMaxSize;
        clearButtonItem.height = clearButtonMaxSize;

        controlLayout.items.add(clearButtonItem);
    }

    flexLayout.items.add(controlLayoutItem);
    flexLayout.performLayout(controlBounds);
}

void LumatoneEditorControl::setTextBoxStyle(juce::Slider::TextEntryBoxPosition position, bool readOnly, int boxWidth, int boxHeight)
{
    if (slider)
    {
        slider->setTextBoxStyle(position, readOnly, boxWidth, boxHeight);
    }
}

void LumatoneEditorControl::setLabelOptions(juce::String labelText, LabelLocation location, int labelWidth, int labelHeight)
{
    labelLocation = location;
    labelWidthInput = labelWidth;
    labelHeightInput = labelHeight;

    if (label == nullptr)
    {
        label = std::make_unique<juce::Label>(getName() + "_label", labelText);
        addAndMakeVisible(*label);
        // label->setColour(juce::Label::ColourIds::outlineColourId, juce::Colours::white);
    }

    label->setVisible(location != LabelLocation::None);
    label->setText(labelText, juce::NotificationType::dontSendNotification);
    label->setJustificationType(juce::Justification::centredLeft);

    label->setFont(getEditorLookAndFeel().getAppFont(LumatoneEditorFont::FranklinGothic));
    label->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
}

void LumatoneEditorControl::setStyle(Style newStyle)
{
    if (component)
    {
        removeChildComponent(component);
        slider = nullptr;
        box = nullptr;
    }

    style = newStyle;

    switch (style)
    {
    case Style::DropdownBox:
        box = std::make_unique<juce::ComboBox>(getName());
        component = box.get();
        break;

    case Style::ColourDropdownInput:
        colourDropdownInput = std::make_unique<ColourDropdownSelector>(getName());

        colourDropdownInput->setColourOptions(colourHistory);
        colourDropdownInput->setShowPicker(false);
        colourDropdownInput->setEditText(true);

        component = colourDropdownInput.get();
        break;

    case Style::IncDecButtons:
    default:
        slider = std::make_unique<juce::Slider>(getName());
        slider->setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
        component = slider.get();
        break;
    }

    setRange(range);
    setValueChangedCallback(valueChangedCallback);
    setTooltip(tooltip);

    addAndMakeVisible(component);
    resized();
}

void LumatoneEditorControl::setRange(juce::Range<int> newRange)
{
    setRange(newRange.getStart(), newRange.getEnd());
}

void LumatoneEditorControl::setRange(int min, int max)
{
    range = juce::Range<int>(min, max);

    if (box)
    {
        box->clear();

        if (min == max && max == 0) // Define this as empty
            return;

        int id = 1;
        for (int i = range.getStart(); i <= range.getEnd(); i++)
        {
            box->addItem(juce::String(i), id++);
        }

        findIdealComboBoxNumColumns(box.get(), range.getEnd() - range.getStart() + 1);
    }

    else if (slider)
    {
        // Minus 1 to reserve for null value
        slider->setRange(range.getStart() - 1, range.getEnd(), 1);
    }
}

void LumatoneEditorControl::addOption(const juce::String &name, int id,  bool reshapeMenu)
{
    jassert(style == LumatoneEditorControl::Style::DropdownBox);
    jassert(box.get() != nullptr);

    if (box)
    {
        box->addItem(name, id);
        // setRange(1, box->getNumItems());
        range = juce::Range<int>(0, box->getNumItems());

        if (reshapeMenu)
            findIdealComboBoxNumColumns(box.get(), box->getNumItems());
    }
}

void LumatoneEditorControl::clearOptions()
{
    jassert(style == LumatoneEditorControl::Style::DropdownBox);
    jassert(box.get() != nullptr);

    if (box)
    {
        box->clear();
        range = juce::Range<int>(0, 0);
    }
}

void LumatoneEditorControl::allowTextInput(bool allowInput)
{
    if (slider)
    {
        slider->setTextBoxIsEditable(allowInput);
    }
    else if (box)
    {
        box->setEditableText(allowInput);
    }
}

void LumatoneEditorControl::setValue(int newValue, juce::NotificationType notify)
{
    parseNull(newValue);

    if (slider)
    {
        slider->setValue(newValue, notify);
    }
    else if (box)
    {
        if (isNull)
        {
            box->setSelectedId(0, notify);
        }
        else
        {
            int id = newValue - range.getStart() + 1;
            box->setSelectedId(id, notify);
        }
    }
    else if (colourDropdownInput)
    {
        colourDropdownInput->clearColour(notify != juce::NotificationType::dontSendNotification);
    }

    if (isNull)
        updateNullText();
}

void LumatoneEditorControl::setColourValue(juce::Colour newColour, juce::NotificationType notify)
{
    if (colourDropdownInput)
    {
        bool callbacks = notify != juce::NotificationType::dontSendNotification;
        colourDropdownInput->setSelectedColour(newColour, callbacks, callbacks);
    }
}

void LumatoneEditorControl::setValueChangedCallback(std::function<void()> callback)
{
    valueChangedCallback = callback;

    if (box)
    {
        box->onChange = [&]() {
            parseNull(box->getText().getIntValue());
            valueChangedCallback();
        };
    }
    else if (slider)
    {
        slider->onValueChange =[&]() {
            // Kludge fix for incrementing from null -- side effects??
            // if (isNull && slider->getValue() == (range.getStart() + 1))
            //     slider->setValue(range.getStart(), juce::NotificationType::dontSendNotification);

            parseNull(slider->getValue());
            valueChangedCallback();
        };
    }
    else if (colourDropdownInput)
    {
        colourDropdownInput->setOnValueChangeCallback([&]() {
            isNull = colourDropdownInput->getSelectedColour().isTransparent();
            valueChangedCallback();
        });
    }

}

void LumatoneEditorControl::clearValue(juce::NotificationType notify)
{
    setValue(range.getStart() - 1, notify);
}

void LumatoneEditorControl::setNullText(juce::String textToDisplay)
{
    nullText = textToDisplay;

    if (box)
    {
        box->setTextWhenNothingSelected(nullText);
    }
    else if (colourDropdownInput)
    {
        colourDropdownInput->setEmptyText(nullText);
    }
    else if (slider)
    {

    }

    updateNullText();
}

void LumatoneEditorControl::setTooltip(juce::String text)
{
    tooltip = text;
    if (box)
    {
        box->setTooltip(tooltip);
    }
    else if (slider)
    {
        slider->setTooltip(tooltip);
    }
}

void LumatoneEditorControl::setShowClearButton(bool hasClearButton)
{
    showClearButton = hasClearButton;

    if (showClearButton && clearButton.get() == nullptr)
    {
        createClearButton();
    }

    if (clearButton)
        clearButton->setVisible(showClearButton);
}

void LumatoneEditorControl::setLastColour(const juce::Colour &lastColour)
{
    updateColourHistory(lastColour);
}

int LumatoneEditorControl::getValue() const
{
    if (slider)
    {
        return slider->getValue();
    }
    else if (box)
    {
        return range.getStart() + box->getSelectedId() - 1;
    }
}

juce::String LumatoneEditorControl::getValueText() const
{
    if (isNull)
        return juce::String();

    if (slider)
    {
        return slider->getTextFromValue(slider->getValue());
    }
    else if (box || colourDropdownInput)
    {
        return static_cast<juce::ComboBox*>(component)->getText();
    }
}

juce::String LumatoneEditorControl::getOptionText(int index) const
{
    if (!range.contains(index))
        return juce::String();

    if (slider)
    {
        return juce::String(range.getStart() + index);
    }
    else if (box)
    {
        return box->getItemText(index + 1);
    }
    else if (colourDropdownInput)
    {
        return colourDropdownInput->getColourOptions()[index].toDisplayString(false);
    }
}

juce::String LumatoneEditorControl::getLabelText() const
{
    if (label)
    {
        return label->getText();
    }

    return juce::String();
}

juce::Colour LumatoneEditorControl::getColourValue() const
{
    if (colourDropdownInput)
    {
        return colourDropdownInput->getSelectedColour();
    }
}

bool LumatoneEditorControl::isValueNull() const
{
    return isNull;
}

bool LumatoneEditorControl::valueIsNull(int checkValue) const
{
    return checkValue < range.getStart() || checkValue > range.getEnd();
}

bool LumatoneEditorControl::parseNull(int newValue)
{
    isNull = valueIsNull(newValue);
    return isNull;
}

void LumatoneEditorControl::createClearButton()
{
    clearButton = std::make_unique<juce::TextButton>(getName() + "_clear", "Clear " + getName());
    clearButton->setButtonText("x");
    addChildComponent(*clearButton);

    clearButton->onClick = [&]() { clearValue(); };
}

void LumatoneEditorControl::updateColourHistory(const juce::Colour &newColour)
{
    int indexOfColour = colourHistory.indexOf(newColour);
    if (indexOfColour >= 0)
        colourHistory.move(indexOfColour, 0);
    else
        colourHistory.insert(0, newColour);

    colourDropdownInput->setColourOptions(colourHistory);
}

void LumatoneEditorControl::updateNullText()
{
    if (slider)
        dynamic_cast<juce::Label*>(slider->getChildComponent(0))->setText(nullText, juce::NotificationType::dontSendNotification);

    // boxes are automatic
}
