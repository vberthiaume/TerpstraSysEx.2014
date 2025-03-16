#include "LumatoneEditorControl.h"

#include "../style/LumatoneEditorStyleCommon.h"
#include "../lumatone_editor_library/common/math.h"

LumatoneEditorControl::LumatoneEditorControl(juce::String name, int minValueIn, int maxValueIn, LumatoneEditorControl::Style style, bool hasClearButton)
    : juce::Component(name)
    , range(juce::Range<int>(minValueIn, maxValueIn))
    , valueChangedCallback([]() {}) // no operation
{
    setShowClearButton(hasClearButton);
    setStyle(style);
    setValue(range.getStart() - 1);
}

LumatoneEditorControl::LumatoneEditorControl(juce::String name, LumatoneEditorControl::Style style, bool hasClearButton)
    : LumatoneEditorControl(name, 0, 0, style, hasClearButton)
{
}

LumatoneEditorControl::~LumatoneEditorControl()
{
    clearButton = nullptr;
    component = nullptr;
    box = nullptr;
    slider = nullptr;
}

void LumatoneEditorControl::resized()
{
    component->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, CONTROLBOXFONTHEIGHTSCALAR);

    juce::Rectangle<int> controlBounds = getLocalBounds();
    if (showClearButton)
    {
        int margin = juce::roundToInt(getHeight() * 0.2f);
        controlBounds = controlBounds.withTrimmedRight(controlBounds.getHeight() + margin);

        int buttonX = controlBounds.getRight() + margin;
        clearButton->setBounds(buttonX, 0,  getWidth() - buttonX, getHeight());
    }

    component->setBounds(controlBounds);
}

void LumatoneEditorControl::setTextBoxStyle(juce::Slider::TextEntryBoxPosition position, bool readOnly, int boxWidth, int boxHeight)
{
    if (slider)
    {
        slider->setTextBoxStyle(position, readOnly, boxWidth, boxHeight);
    }
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

void LumatoneEditorControl::addOption(const juce::String &name, int id)
{
    jassert(style == LumatoneEditorControl::Style::DropdownBox);
    jassert(box.get() != nullptr);

    if (box)
    {
        box->addItem(name, id);
        // setRange(1, box->getNumItems());
        range = juce::Range<int>(0, box->getNumItems());
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
    // Shouldn't be possible to get null from UI
    updateNull(newValue);

    if (slider)
    {
        slider->getProperties().set(LumatoneEditorStyleIDs::sliderValueNull, isNull);
        slider->setValue(newValue, notify);

        // does not update if null is same value

        // Kludge to hide label when null
        if (isNull)
        {
            dynamic_cast<juce::Label*>(slider->getChildComponent(0))->setText("", juce::NotificationType::dontSendNotification);
        }
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
}

void LumatoneEditorControl::setValueChangedCallback(std::function<void()> callback)
{
    valueChangedCallback = callback;

    if (box)
    {
        box->onChange = [&]() {
            valueChangedCallback();
            updateNull(box->getText().getIntValue());
        };
    }
    else if (slider)
    {
        slider->onValueChange =[&]() {
            // Kludge fix for incrementing from null -- side effects??
            // if (isNull && slider->getValue() == (range.getStart() + 1))
            //     slider->setValue(range.getStart(), juce::NotificationType::dontSendNotification);

            valueChangedCallback();
            updateNull(slider->getValue());
        };
    }
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

bool LumatoneEditorControl::isValueNull() const
{
    return isNull;
}

bool LumatoneEditorControl::valueIsNull(int checkValue) const
{
    return checkValue < range.getStart() || checkValue > range.getEnd();
}

bool LumatoneEditorControl::updateNull(int newValue)
{
    isNull = valueIsNull(newValue);
    return isNull;
}

void LumatoneEditorControl::createClearButton()
{
    clearButton = std::make_unique<juce::TextButton>(getName() + "_clear", "Clear " + getName());
    clearButton->setButtonText("x");
    addChildComponent(*clearButton);

    clearButton->onClick = [&]()
    {
        setValue(range.getStart() - 1);
    };
}
