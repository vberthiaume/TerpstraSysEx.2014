#include "BatchToolsColourControls.h"

#include "../../style/LumatoneEditorStyleCommon.h"

#include "../../actions/BatchColourActions.h"
#include "../../lumatone_editor_library/actions/edit_actions.h"

BatchToolsColourControls::BatchToolsColourControls(const LumatoneEditorState & stateIn)
    : LumatoneEditorState("BatchToolsColourControls", stateIn)
    , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
    , juce::Component("BatchToolsColourControls")
    , lastSavedLayout(new LumatoneLayout())
{
    brightness = std::make_unique<juce::Slider>("BatchToolsBrightnessSlider");
    brightness->setRange(0, 1.0, 0.001);
    brightness->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    brightness->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    brightness->getProperties().set(LumatoneEditorStyleIDs::sliderRotaryColourGradient, (int)LumatoneEditorColourGradients::BrightnessSlider);
    brightness->onValueChange = [&] ()
    {
        performAction(SetBatchColourSettingsAction::NewSetBrightnessValue(*this, brightness->getValue()));
        if (isSetToDefault())
            resetButtonCallback();
        else
        {
            performAction(new ApplyBatchColourAdjustments(*this, lastSavedLayout, getBatchColourEditData(), *getSelectedKeys()));
            setHasChanges(true);
        }
    };
    brightness->valueFromTextFunction = [](const juce::String& text)
    {
        double textValue = text.upToFirstOccurrenceOf("%", false, false).getDoubleValue();
        return textValue / 200.0;
    };
    brightness->textFromValueFunction = [](double value)
    {
        double percent = juce::roundToInt(value * 200 * 100) * 0.01;
        return juce::String(percent) + "%";
    };
    brightness->setValue(0.5, juce::NotificationType::dontSendNotification);
    addAndMakeVisible (brightness.get());

    hue = std::make_unique<juce::Slider>("BatchToolsHueSlider");
    hue->setRange (0, 1.0, 0.001);
    hue->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    hue->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    hue->getProperties().set(LumatoneEditorStyleIDs::sliderRotaryColourGradient, (int)LumatoneEditorColourGradients::HueSlider);
    hue->onValueChange = [&] ()
    {

        performAction(SetBatchColourSettingsAction::NewSetHueValue(*this, hue->getValue()));
        if (isSetToDefault())
            resetButtonCallback();
        else
        {
            performAction(new ApplyBatchColourAdjustments(*this, lastSavedLayout, getBatchColourEditData(), *getSelectedKeys()));
            setHasChanges(true);
        }
    };
    hue->valueFromTextFunction = [](const juce::String& text)
    {
        double textValue = text.upToFirstOccurrenceOf("%", false, false).getDoubleValue();
        return (textValue + 100.0) * 0.5;
    };
    hue->textFromValueFunction = [](double value)
    {
        double percent = juce::roundToInt(value * 200 * 100) * 0.01 - 100.0;
        juce::String prefix = percent > 0 ? "+" : "";
        return prefix + juce::String(percent) + "%";
    };
    hue->setValue(0.5, juce::NotificationType::dontSendNotification);
    addAndMakeVisible (hue.get());

    temperature = std::make_unique<juce::Slider>("BatchToolsTemperatureSlider");
    temperature->setRange (0, 1, 0.001);
    temperature->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    temperature->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    temperature->getProperties().set(LumatoneEditorStyleIDs::sliderRotaryColourGradient, (int)LumatoneEditorColourGradients::TemperatureSlider);
    temperature->onValueChange = [&] ()
    {
        performAction(SetBatchColourSettingsAction::NewSetTemperatureValue(*this, temperature->getValue()));
        if (isSetToDefault())
            resetButtonCallback();
        else
        {
            performAction(new ApplyBatchColourAdjustments(*this, lastSavedLayout, getBatchColourEditData(), *getSelectedKeys()));
            setHasChanges(true);
        }
    };
    temperature->valueFromTextFunction = [](const juce::String& text)
    {
        double textValue = text.upToFirstOccurrenceOf("%", false, false).getDoubleValue();
        return (textValue + 10.0) * 0.05;
    };
    temperature->textFromValueFunction = [](double value)
    {
        double display = (double)juce::roundToInt(value * 20.0 * 1000) * 0.001 - 10.0;
        juce::String prefix = display > 0 ? "+" : "";
        return juce::String(display);
    };
    temperature->setValue(0.5, juce::NotificationType::dontSendNotification);
    addAndMakeVisible (temperature.get());


    brightnessLabel = std::make_unique<juce::Label>("brightnessLabel", "Brightness");
    brightnessLabel->setJustificationType(juce::Justification::centred);
    brightnessLabel->attachToComponent(brightness.get(), false);
    addAndMakeVisible(brightnessLabel.get());

    hueLabel = std::make_unique<juce::Label>("hueLabel", "Hue Shift");
    hueLabel->setJustificationType(juce::Justification::centred);
    hueLabel->attachToComponent(hue.get(), false);
    addAndMakeVisible(hueLabel.get());

    temperatureLabel = std::make_unique<juce::Label>("temperatureLabel", "Temp. Shift");
    temperatureLabel->setJustificationType(juce::Justification::centred);
    temperatureLabel->attachToComponent(temperature.get(), false);
    addAndMakeVisible(temperatureLabel.get());


    applyButton = std::make_unique<juce::TextButton>("Apply", "Commit colour changes on selection");
    applyButton->onClick = [&]() { applyButtonCallback(); };
    applyButton->setEnabled(false);
    addAndMakeVisible(applyButton.get());

    resetButton = std::make_unique<juce::TextButton>("Reset", "Reset all colour adjustments.");
    resetButton->onClick = [&]() { resetButtonCallback(); };
    resetButton->setEnabled(false);
    addAndMakeVisible(resetButton.get());

    addEditorListener(this);
}

BatchToolsColourControls::~BatchToolsColourControls()
{
    resetButton = nullptr;
    applyButton = nullptr;

    temperatureLabel = nullptr;
    hueLabel = nullptr;
    brightnessLabel = nullptr;

    temperature = nullptr;
    hue = nullptr;
    brightness = nullptr;
}

void BatchToolsColourControls::resized()
{
    int windowH = getWindowBounds().getHeight();

    int marginX = juce::roundToInt((float)windowH * contentMarginWidthWindowH);
    contentMarginX = juce::roundToInt(windowH * contentMarginWidthWindowH);
    contentMarginH = juce::roundToInt(windowH * controlContentSmallMarginHeightWindowH * 1.2f);

    int controlHeight = juce::roundToInt(windowH * controlHeightWindowH);

    int sliderMargin = juce::roundToInt((float)marginX * 0.5f);

    int sliderWidth = juce::roundToInt((getWidth() - marginX * 2) / 3) - sliderMargin;
    int sliderHeight = juce::roundToInt(getHeight() * sliderH);

    resizeLabelWithHeight(brightnessLabel.get(), controlHeight, controlLabelFontScalar);
    resizeLabelWithHeight(hueLabel.get(), controlHeight, controlLabelFontScalar);
    resizeLabelWithHeight(temperatureLabel.get(), controlHeight, controlLabelFontScalar);

    int sliderY = contentMarginH + controlHeight / 2;
    hue->setBounds(marginX, sliderY, sliderWidth, sliderHeight);
    brightness->setBounds(marginX + sliderWidth + sliderMargin, sliderY + getHeight() * 0.25f, sliderWidth, sliderHeight);
    temperature->setBounds(marginX + (sliderWidth + sliderMargin) * 2, sliderY, sliderWidth, sliderHeight);

    int buttonHeight = juce::roundToInt((float)controlHeight * 1.2f);
    int buttonY = getHeight() - buttonHeight - contentMarginH;

    int applyWidth = getLookAndFeel().getTextButtonFont(*applyButton, buttonHeight)
                                     .getStringWidth(applyButton->getButtonText() + "____") ;
    int resetWidth = getLookAndFeel().getTextButtonFont(*resetButton, buttonHeight)
                                     .getStringWidth(resetButton->getButtonText() + "____");

    // int buttonMargin = (getWidth() - buttonWidth * 2) / 3;


    applyButton->setBounds(marginX, buttonY, applyWidth, buttonHeight);
    resetButton->setBounds(getWidth() - resetWidth - marginX, buttonY, resetWidth, buttonHeight);
}

bool BatchToolsColourControls::isSetToDefault() const
{
    return brightness->getValue() == 0.5
        && hue->getValue() == 0.5
        && temperature->getValue() == 0.5
        ;
}

void BatchToolsColourControls::setHasChanges(bool changes)
{
    hasChanges = changes;

    if (!hasChanges)
    {
        performAction(new SetBatchColourSettingsAction(*this, true, true, true));
    }

    applyButton->setEnabled(hasChanges);
    resetButton->setEnabled(hasChanges);
}

void BatchToolsColourControls::applyButtonCallback()
{
    *lastSavedLayout = *getMappingData();
    setHasChanges(false);
}

void BatchToolsColourControls::resetButtonCallback()
{
    performAction(new LumatoneEditAction::LayoutUpdateAction(this, *lastSavedLayout, false));
    setHasChanges(false);
}

void BatchToolsColourControls::handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier &property)
{
    LumatoneEditorState::handleStatePropertyChange(stateIn, property);

    juce::var value = stateIn.getProperty(property);

    if (property == LumatoneEditorBatchColourProperty::BrightnessMultiplier)
    {
        brightness->setValue((double)value, juce::NotificationType::dontSendNotification);
    }
    else if (property == LumatoneEditorBatchColourProperty::HueShiftAmount)
    {
        hue->setValue((double)value, juce::NotificationType::dontSendNotification);
    }
    else if (property == LumatoneEditorBatchColourProperty::TemperatureShiftValue)
    {
        temperature->setValue((double)value, juce::NotificationType::dontSendNotification);
    }
}

void BatchToolsColourControls::layoutWasUpdated()
{
    auto desc = undoManager->getUndoDescription();
    if (!desc.contains("BatchColour"))
        *lastSavedLayout = *getMappingData();
    // if (!getHasChangesToSave())
    //     *lastSavedLayout = *getMappingData();

}
