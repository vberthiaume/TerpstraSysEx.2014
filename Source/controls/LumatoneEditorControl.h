/*
  ==============================================================================

    LumatoneEditorControl.h
    Created: 07 July 2024
    Author:  Vincenzo

    Wrapper around juce Slider & ComboBox for ranged controls

  ==============================================================================
*/

#ifndef LUMATONE_RANGED_CONTROL_H
#define LUMATONE_RANGED_CONTROL_H

#include <JuceHeader.h>

class LumatoneEditorControl : public juce::Component
{
public:

    enum class Style
    {
        IncDecButtons = 1,
        DropdownBox
        // Rotary
    };

public:

    LumatoneEditorControl(juce::String name, int minValue, int maxValue, LumatoneEditorControl::Style style = LumatoneEditorControl::Style::IncDecButtons, bool hasClearButton = false);
    LumatoneEditorControl(juce::String name, LumatoneEditorControl::Style style = LumatoneEditorControl::Style::IncDecButtons, bool hasClearButton = false);

    ~LumatoneEditorControl() override;

    // void paint(juce::Graphics& g) override;
    void resized() override;

    void setTextBoxStyle(juce::Slider::TextEntryBoxPosition position, bool readOnly, int boxWidth, int boxHeight);

    void setStyle(Style newStyle);

    void setRange(juce::Range<int> newRange);
    void setRange(int min, int max);
    void addOption(const juce::String& name, int id);

    void allowTextInput(bool allowInput);

    // Sets control to explicit value, if in range
    void setValue(int newValue, juce::NotificationType notify=juce::NotificationType::sendNotification);

    void setValueChangedCallback(std::function<void()> callback);

    void setTooltip(juce::String text);

    void setShowClearButton(bool hasClearButton);

    Style getStyle() const { return style; }
    juce::Range<int> getRange() const { return range; }

    int getValue() const;

    bool isValueNull() const;

public:
    bool valueIsNull(int checkValue) const;
private:
    bool updateNull(int newValue);

    void createClearButton();

private:

    std::unique_ptr<juce::Slider>       slider;
    std::unique_ptr<juce::ComboBox>     box;

    std::unique_ptr<juce::TextButton>   clearButton;

    juce::Component* component = nullptr;

    bool isNull = true;

    bool showClearButton = false;
    LumatoneEditorControl::Style style;
    juce::Range<int> range;

    std::function<void()> valueChangedCallback;

    juce::String tooltip;
};

#endif // LUMATONE_RANGED_CONTROL_H
