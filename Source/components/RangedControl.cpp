#include "RangedControl.h"

#include "../LumatoneEditorStyleCommon.h"
#include "../ScaleStructureController/Common.h"

RangedControl::RangedControl(juce::String name, int minValueIn, int maxValueIn, Style style)
    : juce::Component(name)
{
    range = juce::Range<int>(minValueIn, maxValueIn);
    valueChangedCallback = [](){}; // no operation
    setStyle(style);
    setValue(minValueIn - 1);
}

RangedControl::~RangedControl()
{
    component = nullptr;
    box = nullptr;
    slider = nullptr;
}

void RangedControl::resized()
{
    component->setBounds(getLocalBounds());
}

void RangedControl::setTextBoxStyle(juce::Slider::TextEntryBoxPosition position, bool readOnly, int boxWidth, int boxHeight)
{
    if (slider)
    {
        slider->setTextBoxStyle(position, readOnly, boxWidth, boxHeight);
    }
}

void RangedControl::setStyle(Style newStyle)
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

void RangedControl::setRange(juce::Range<int> newRange)
{
    setRange(newRange.getStart(), newRange.getEnd());
}

void RangedControl::setRange(int min, int max)
{
    range = juce::Range<int>(min, max);

    if (box)
    {
        box->clear();

        int id = 1;
        for (int i = range.getStart(); i <= range.getEnd(); i++)
        {
            box->addItem(juce::String(i), id++);
        }

    int numItems = range.getEnd() - range.getStart() + 1;
    double sqNumItems = sqrt((double)numItems);
    int maxColumns = (int)(sqNumItems);

    if (maxColumns != sqNumItems)
    {
        // get prime factors
        auto factors = getFactors(numItems);

        // find largest exponent
        int maxF = 1;
        int maxExp = 1;
        for (int i = 0; i < jmin(factors.size(), 100); i++)
        {
            int iExp = factors[i];
            if (iExp > maxExp)
            {
                maxExp = iExp;
                maxF = (int)pow(PRIMES[i], maxExp);
            }
        }

        if (maxExp > 4)
            maxColumns = 8;
        else if (maxExp > 2)
            maxColumns = 4;
        else if (maxExp > 1)
            maxColumns = 2;
        else
            maxColumns = 1;
    }


    box->getProperties().set(LumatoneEditorStyleIDs::popupMenuMaxColumns, juce::var(maxColumns));
    }

    else if (slider)
    {
        // Minus 1 to reserve for null value
        slider->setRange(range.getStart() - 1, range.getEnd(), 1);
    }
}

void RangedControl::setValue(int newValue, juce::NotificationType notify)
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

void RangedControl::setValueChangedCallback(std::function<void()> callback)
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

void RangedControl::setTooltip(juce::String text)
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

int RangedControl::getValue() const
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

bool RangedControl::isValueNull() const
{
    return isNull;
}

bool RangedControl::valueIsNull(int checkValue) const
{
    return checkValue < range.getStart() || checkValue > range.getEnd();
}

bool RangedControl::updateNull(int newValue)
{
    isNull = valueIsNull(newValue);
    return isNull;
}
