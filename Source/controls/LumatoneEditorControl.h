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

#include "../data/LumatoneEditorState.h"

class ColourDropdownSelector;
class LumatoneEditorLookAndFeel;
class LumatoneEditorControl : public LumatoneEditorState, // only read - parent class writes to state
                              public juce::Component
{
public:

    enum class Style
    {
        IncDecButtons = 1,
        DropdownBox,
        ColourDropdownInput
        // Rotary
    };

    enum class LabelLocation
    {
        None = 0,
        Left,
        Top
    };

    enum ColourIds
    {
        outline = 0x501000,
    };

public:

    LumatoneEditorControl(const LumatoneEditorState& stateIn, juce::String name, int minValue, int maxValue, LumatoneEditorControl::Style style = LumatoneEditorControl::Style::IncDecButtons, bool hasClearButton = false);
    LumatoneEditorControl(const LumatoneEditorState& stateIn, juce::String name, LumatoneEditorControl::Style style = LumatoneEditorControl::Style::IncDecButtons, bool hasClearButton = false);

    ~LumatoneEditorControl() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setTextBoxStyle(juce::Slider::TextEntryBoxPosition position, bool readOnly, int boxWidth, int boxHeight);

    void setLabelOptions(juce::String labelText, LabelLocation location, int labelWidth=0, int labelHeight=0);

    void setStyle(Style newStyle);

    void setRange(juce::Range<int> newRange);
    void setRange(int min, int max);
    void addOption(const juce::String& name, int id, bool reshapeMenu=true);
    void clearOptions();

    void allowTextInput(bool allowInput);

    // Sets control to explicit value, if in range
    void setValue(int newValue, juce::NotificationType notify=juce::NotificationType::sendNotification);
    void setColourValue(juce::Colour newColour, juce::NotificationType notify=juce::NotificationType::sendNotification);
    void setValueChangedCallback(std::function<void()> callback);

    void clearValue(juce::NotificationType notify = juce::NotificationType::sendNotification);

    void setTooltip(juce::String text);

    void setShowClearButton(bool hasClearButton);
    void setLastColour(const juce::Colour& lastColour);

    Style getStyle() const { return style; }
    juce::Range<int> getRange() const { return range; }

    int getLength() const { return range.getLength(); }

    int getValue() const;
    juce::String getValueText() const;
    juce::String getOptionText(int index) const;
    juce::String getLabelText() const;

    juce::Colour getColourValue() const;

    bool isValueNull() const;

    juce::Font getFont() const;

    juce::Slider* getSlider() const { return slider.get();}
    juce::ComboBox* getComboBox() const { return box.get(); }
    ColourDropdownSelector* getColourSelector() const { return colourDropdownInput.get(); }

public:
    bool valueIsNull(int checkValue) const;
private:
    bool updateNull(int newValue);

private:
    void createClearButton();

    void updateColourHistory(const juce::Colour& newColour);

private:

    std::unique_ptr<juce::Slider>           slider;
    std::unique_ptr<juce::ComboBox>         box;

    std::unique_ptr<ColourDropdownSelector> colourDropdownInput;
    juce::Array<Colour>                     colourHistory;

    std::unique_ptr<juce::Label>            label;
    std::unique_ptr<juce::TextButton>       clearButton;

    juce::Component* component = nullptr;

    bool isNull = true;

    bool showClearButton = false;
    LumatoneEditorControl::Style style;
    juce::Range<int> range;

    LabelLocation labelLocation = LabelLocation::None;
    int labelWidthInput = 0;
    int labelHeightInput = 0;

    juce::FlexBox flexLayout;

    std::function<void()> valueChangedCallback;

    juce::String tooltip;

    const float clearButtonMarginScalar = 0.2f;
};

#endif // LUMATONE_RANGED_CONTROL_H
