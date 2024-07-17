#include "BatchToolsColourControls.h"

#include "../LumatoneEditorStyleCommon.h"

BatchToolsColourControls::BatchToolsColourControls(const LumatoneEditorState & stateIn)
    : LumatoneEditorState("BatchToolsColourControls", stateIn)
    , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
    , juce::Component("BatchToolsColourControls")
{
    brightness = std::make_unique<juce::Slider>("BatchToolsBrightnessSlider");
    brightness->setRange (0, 127, 1);
    brightness->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    brightness->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    brightness->getProperties().set(LumatoneEditorStyleIDs::sliderRotaryColourGradient, (int)LumatoneEditorColourGradients::BrightnessSlider);
    brightness->onValueChange = [&] () {};
    addAndMakeVisible (brightness.get());

    hue = std::make_unique<juce::Slider>("BatchToolsHueSlider");
    hue->setRange (0, 127, 1);
    hue->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    hue->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    hue->getProperties().set(LumatoneEditorStyleIDs::sliderRotaryColourGradient, (int)LumatoneEditorColourGradients::HueSlider);
    hue->onValueChange = [&] () {};
    addAndMakeVisible (hue.get());

    temperature = std::make_unique<juce::Slider>("BatchToolsTemperatureSlider");
    temperature->setRange (0, 127, 1);
    temperature->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    temperature->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    temperature->getProperties().set(LumatoneEditorStyleIDs::sliderRotaryColourGradient, (int)LumatoneEditorColourGradients::TemperatureSlider);
    temperature->onValueChange = [&] () {};
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
    addAndMakeVisible(applyButton.get());

    resetButton = std::make_unique<juce::TextButton>("Reset", "Reset all colour adjustments.");
    addAndMakeVisible(resetButton.get());
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
