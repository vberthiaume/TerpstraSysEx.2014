#include "BatchToolsColourControls.h"
#include "BatchColourActions.h"

#include "../LumatoneEditorStyleCommon.h"
#include "../LumatoneController.h"

// #include "./edit_actions.h"

BatchToolsColourControls::BatchToolsColourControls(LumatoneController* stateIn)
    // : LumatoneApplicationState("BatchToolsColourControls", stateIn)
    // , LumatoneApplicationState::Controller(static_cast<LumatoneEditorState&>(state))
    : juce::Component("BatchToolsColourControls")
    // , lastSavedLayout(new LumatoneLayout())
    , state(stateIn)
{
    brightness = std::make_unique<juce::Slider>("BatchToolsBrightnessSlider");
    brightness->setRange(0, 1.0, 0.001);
    brightness->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    brightness->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    brightness->getProperties().set(LumatoneEditorStyleIDs::sliderRotaryColourGradient, (int)LumatoneEditorColourGradients::BrightnessSlider);
    brightness->onDragStart = [&]() { isDragging = true;  state->beginNewUndoTransaction(); };
    brightness->onDragEnd   = [&]() { isDragging = false; };
    brightness->onValueChange = [&] ()
    {
        auto currentData = state->getBatchColourEditState().getData();
        state->performAction(SetBatchColourSettingsAction::NewSetBrightnessValue(state, brightness->getValue()), true, !isDragging);
        if (isSetToDefault())
            resetButtonCallback();
        else
        {
            state->performAction(new ApplyBatchColourAdjustments(state, currentData), true, false);
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
        double percent = juce::roundToInt(value * 200);
        return juce::String(percent) + "%";
    };
    brightness->setValue(0.5, juce::NotificationType::dontSendNotification);
    addAndMakeVisible (brightness.get());

    hue = std::make_unique<juce::Slider>("BatchToolsHueSlider");
    hue->setRange (0, 1.0, 0.001);
    hue->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    hue->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    hue->getProperties().set(LumatoneEditorStyleIDs::sliderRotaryColourGradient, (int)LumatoneEditorColourGradients::HueSlider);
    hue->onDragStart = [&]() { isDragging = true;  state->beginNewUndoTransaction(); };
    hue->onDragEnd   = [&]() { isDragging = false; };
    hue->onValueChange = [&] ()
    {
        auto currentData = state->getBatchColourEditState().getData();
        state->performAction(SetBatchColourSettingsAction::NewSetHueValue(state, hue->getValue()), true, !isDragging);
        if (isSetToDefault())
            resetButtonCallback();
        else
        {
            state->performAction(new ApplyBatchColourAdjustments(state, currentData), true, false);
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
        int percent = juce::roundToInt(value * 200) - 100;
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
    temperature->onDragStart = [&]() { isDragging = true;  state->beginNewUndoTransaction(); };
    temperature->onDragEnd   = [&]() { isDragging = false; };
    temperature->onValueChange = [&] ()
    {
        auto currentData = state->getBatchColourEditState().getData();
        state->performAction(SetBatchColourSettingsAction::NewSetTemperatureValue(state, temperature->getValue()), true, !isDragging);
        if (isSetToDefault())
            resetButtonCallback();
        else
        {
            state->performAction(new ApplyBatchColourAdjustments(state, currentData), true, false);
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
        int display = juce::roundToInt(value * 20.0) - 10.0;
        juce::String prefix = display > 0 ? "+" : "";
        return juce::String(display);
    };
    temperature->setValue(0.5, juce::NotificationType::dontSendNotification);
    addAndMakeVisible (temperature.get());


    brightnessLabel = std::make_unique<juce::Label>("brightnessLabel", "Brightness");
    brightnessLabel->setJustificationType(juce::Justification::centred);
    addAndMakeVisible(brightnessLabel.get());

    hueLabel = std::make_unique<juce::Label>("hueLabel", "Hue Shift");
    hueLabel->setJustificationType(juce::Justification::centred);
    addAndMakeVisible(hueLabel.get());

    temperatureLabel = std::make_unique<juce::Label>("temperatureLabel", "Temp. Shift");
    temperatureLabel->setJustificationType(juce::Justification::centred);
    addAndMakeVisible(temperatureLabel.get());


    applyButton = std::make_unique<juce::TextButton>("Save", "Commit colour changes on selection");
    applyButton->onClick = [&]() { applyButtonCallback(); };
    applyButton->setEnabled(false);
    addAndMakeVisible(applyButton.get());

    sendButton = std::make_unique<juce::TextButton>("Send", "Commit colour changes on selection");
    sendButton->onClick = [&]() { sendButtonCallback(); };
    sendButton->setEnabled(false);
    addAndMakeVisible(sendButton.get());

    resetButton = std::make_unique<juce::TextButton>("Reset", "Reset all colour adjustments.");
    resetButton->onClick = [&]() { resetButtonCallback(); };
    resetButton->setEnabled(false);
    addAndMakeVisible(resetButton.get());

    // addEditorListener(this);
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
    float contentMarginWidthWindowH = 0.008f;
    float controlLabelFontScalar = 0.8f;

    int windowH = state->getWindowHeight();

    contentMarginX = juce::roundToInt(windowH * contentMarginWidthWindowH);
    contentMarginH = juce::roundToInt(windowH * 0.03f);

    int controlHeight = juce::roundToInt(windowH * 0.024f);

    int sliderMargin = juce::roundToInt((float)contentMarginX * 0.5f);

    int sliderWidth = juce::roundToInt((getWidth() - contentMarginX * 2) / 3) - sliderMargin;
    int sliderHeight = juce::roundToInt(getHeight() * sliderH);

    int sliderY = contentMarginH + controlHeight / 2;
    hue->setBounds(contentMarginX, sliderY, sliderWidth, sliderHeight);
    brightness->setBounds(contentMarginX + sliderWidth + sliderMargin, sliderY, sliderWidth, sliderHeight);
    temperature->setBounds(contentMarginX + (sliderWidth + sliderMargin) * 2, sliderY, sliderWidth, sliderHeight);

    int labelHeight = juce::roundToInt(controlHeight * controlLabelFontScalar);
    int labelMargin = juce::roundToInt(labelHeight * 0.8f);
    hueLabel->setSize(sliderWidth, labelHeight);
    hueLabel->setCentrePosition(hue->getBounds().getCentre().withY(hue->getY() - labelMargin));

    brightnessLabel->setSize(sliderWidth, labelHeight);
    brightnessLabel->setCentrePosition(brightness->getBounds().getCentre().withY(brightness->getY() - labelMargin));

    temperatureLabel->setSize(sliderWidth, labelHeight);
    temperatureLabel->setCentrePosition(temperature->getBounds().getCentre().withY(temperature->getY() - labelMargin));


    int buttonHeight = juce::roundToInt((float)controlHeight * 1.2f);
    int buttonY = getHeight() - buttonHeight - contentMarginH + labelHeight / 2;

    int applyWidth = getLookAndFeel().getTextButtonFont(*applyButton, buttonHeight)
                                     .getStringWidth(applyButton->getButtonText() + "____") ;
    int sendWidth = getLookAndFeel().getTextButtonFont(*sendButton, buttonHeight)
                                     .getStringWidth(sendButton->getButtonText() + "____");
    int resetWidth = getLookAndFeel().getTextButtonFont(*resetButton, buttonHeight)
                                     .getStringWidth(resetButton->getButtonText() + "____");


    int btnMarginX = contentMarginX * 3; // compensate for extra slider margin
    applyButton->setBounds(btnMarginX, buttonY, applyWidth, buttonHeight);
    resetButton->setBounds(getWidth() - resetWidth - btnMarginX, buttonY, resetWidth, buttonHeight);
    sendButton->setBounds(applyButton->getRight() + (resetButton->getX() - applyButton->getRight()) * 0.5 - sendWidth * 0.5, buttonY, sendWidth, buttonHeight);
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
        // performAction(new SetBatchColourSettingsAction(state, true, true, true));
    }

    applyButton->setEnabled(hasChanges);
    sendButton->setEnabled(hasChanges);
    resetButton->setEnabled(hasChanges);
}

void BatchToolsColourControls::updateFromState()
{
    auto data = state->getBatchColourEditState().getData();
    brightness->setValue(data.brightnessMultiplier, juce::NotificationType::dontSendNotification);
    hue->setValue(data.hueShiftAmount, juce::NotificationType::dontSendNotification);
    temperature->setValue(data.temperatureShiftValue, juce::NotificationType::dontSendNotification);
    
    setHasChanges(!isSetToDefault());
}

void BatchToolsColourControls::applyButtonCallback()
{
    // *lastSavedLayout = *getMappingData();
    state->applyBatchColours();
    updateFromState();
    setHasChanges(false);
}

void BatchToolsColourControls::sendButtonCallback()
{
    state->sendCurrentMapping();
}

void BatchToolsColourControls::resetButtonCallback()
{
    state->resetBatchColours();
    updateFromState();
    setHasChanges(false);
    // state->performAction(new LumatoneEditAction::LayoutUpdateAction(this, *lastSavedLayout, false));
}

// void BatchToolsColourControls::handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier &property)
// {
    // LumatoneEditorState::handleStatePropertyChange(stateIn, property);

    // juce::var value = stateIn.getProperty(property);

    // if (property == LumatoneEditorBatchColourProperty::BrightnessMultiplier)
    // {
    //     brightness->setValue((double)value, juce::NotificationType::dontSendNotification);
    // }
    // else if (property == LumatoneEditorBatchColourProperty::HueShiftAmount)
    // {
    //     hue->setValue((double)value, juce::NotificationType::dontSendNotification);
    // }
    // else if (property == LumatoneEditorBatchColourProperty::TemperatureShiftValue)
    // {
    //     temperature->setValue((double)value, juce::NotificationType::dontSendNotification);
    // }
// }

// void BatchToolsColourControls::layoutWasUpdated()
// {
//     auto desc = undoManager->getUndoDescription();
//     if (!desc.contains("BatchColour"))
//         *lastSavedLayout = *getMappingData();
//     // if (!getHasChangesToSave())
//     //     *lastSavedLayout = *getMappingData();

// }
