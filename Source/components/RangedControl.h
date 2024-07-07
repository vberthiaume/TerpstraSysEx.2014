/*
  ==============================================================================

    RangedControl.h
    Created: 07 July 2024
    Author:  Vincenzo

    Wrapper around juce Slider & ComboBox for ranged controls

  ==============================================================================
*/

#ifndef LUMATONE_RANGED_CONTROL_H
#define LUMATONE_RANGED_CONTROL_H

#include <JuceHeader.h>

class RangedControl : public juce::Component
{
public:

    enum class Style
    {
        IncDecButtons = 1,
        DropdownBox
        // Rotary
    };


public:

    RangedControl(juce::String name, int minValue, int maxValue, Style style = RangedControl::Style::IncDecButtons);

    ~RangedControl() override;

    // void paint(juce::Graphics& g) override;
    void resized() override;

    void setStyle(Style newStyle);

    void setRange(juce::Range<int> newRange);
    void setRange(int min, int max);

    // Sets control to explicit value, if in range
    void setValue(int newValue, juce::NotificationType notify=juce::NotificationType::sendNotification);

    void setValueChangedCallback(std::function<void()> callback);

    void setTooltip(juce::String text);

    Style getStyle() const { return style; }
    juce::Range<int> getRange() const { return range; }

    int getValue() const;

public:
    bool valueIsNull(int checkValue) const;
private:
    bool updateNull(int newValue);

private:

    std::unique_ptr<juce::Slider>   slider;
    std::unique_ptr<juce::ComboBox> box;

    juce::Component* component = nullptr;

    bool isNull = true;

    Style style;
    juce::Range<int> range;

    std::function<void()> valueChangedCallback;

    juce::String tooltip;
};

#endif // LUMATONE_RANGED_CONTROL_H
