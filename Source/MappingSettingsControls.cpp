/*
==============================================================================

    MappingSettingsControls.h
    Created: 15 Jan 2023
    Author:  Vincenzo

==============================================================================
*/

#include "MappingSettingsControls.h"

#include "LumatoneEditorLookAndFeel.h"

#include "./lumatone_editor_library/graphics/view_constants.h"

//==============================================================================
MappingSettingsControls::MappingSettingsControls (const LumatoneEditorState& stateIn)
    : LumatoneEditorState("MappingSettingsControls", stateIn)
{
    lblGeneralSettings.reset (new juce::Label ("lblGeneralSettings", juce::translate("General Settings")));
    lblGeneralSettings->setJustificationType (juce::Justification::centredLeft);
    lblGeneralSettings->setFont(getAppFonts().getFont(LumatoneEditorFont::UniviaProBold));
    lblGeneralSettings->setEditable (false, false, false);
    lblGeneralSettings->setColour(Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LabelPink));
    lblGeneralSettings->setColour (juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
    addAndMakeVisible (lblGeneralSettings.get());

    buttonAfterTouchActive.reset (new juce::ToggleButton ("buttonAfterTouchActive"));
    buttonAfterTouchActive->setButtonText (juce::translate("Polyphonic Aftertouch"));
    buttonAfterTouchActive->onClick = [&]()
    {
        setAftertouchEnabled(buttonAfterTouchActive->getToggleState());
    };
    addAndMakeVisible (buttonAfterTouchActive.get());

    buttonLightOnKeyStrokes.reset (new juce::ToggleButton ("buttonLightOnKeyStrokes"));
    buttonLightOnKeyStrokes->setButtonText (juce::translate("Light on Keystrokes"));
    buttonLightOnKeyStrokes->onClick = [&]()
    {
		    setLightOnKeyStrokes(buttonLightOnKeyStrokes->getToggleState());
    };
    addAndMakeVisible (buttonLightOnKeyStrokes.get());

    lblExprSensitivity.reset (new juce::Label ("lblExprSensitivity", juce::translate("Expression Sensitivity")));
    lblExprSensitivity->setFont(getAppFonts().getFont(LumatoneEditorFont::GothamNarrowMedium));
    lblExprSensitivity->setJustificationType (juce::Justification::centred);
    lblExprSensitivity->setEditable (false, false, false);
    lblExprSensitivity->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblExprSensitivity->setColour (juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
    addAndMakeVisible (lblExprSensitivity.get());

    btnInvertExpression.reset (new juce::ToggleButton ("btnInvertExpression"));
    btnInvertExpression->setButtonText (juce::translate("Invert Expression"));
    btnInvertExpression->onClick = [&]()
    {
        setInvertExpression(btnInvertExpression->getToggleState());
    };
    addAndMakeVisible (btnInvertExpression.get());

    lblPedalSettings.reset (new juce::Label ("lblPedalSettings", juce::translate("Pedal Settings")));
    lblPedalSettings->setFont(getAppFonts().getFont(LumatoneEditorFont::UniviaProBold));
    lblPedalSettings->setJustificationType (juce::Justification::centredLeft);
    lblPedalSettings->setEditable (false, false, false);
    lblPedalSettings->setColour (juce::Label::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LabelPink));
    lblPedalSettings->setColour (juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
    addAndMakeVisible (lblPedalSettings.get());

    sldExprCtrlSensitivity.reset (new juce::Slider ("sldExprCtrlSensitivity"));
    sldExprCtrlSensitivity->setRange (0, 127, 1);
    sldExprCtrlSensitivity->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    sldExprCtrlSensitivity->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 60, 20);
    sldExprCtrlSensitivity->onValueChange = [&] ()
    {
        exprSensitivitySliderCallback();
    };
    addAndMakeVisible (sldExprCtrlSensitivity.get());

    btnInvertSustain.reset (new juce::ToggleButton ("btnInvertSustain"));
    btnInvertSustain->setButtonText (juce::translate("Invert Sustain"));
    btnInvertSustain->onClick = [&]()
    {
        setInvertSustain(btnInvertSustain->getToggleState());
    };
    addAndMakeVisible (btnInvertSustain.get());

    // lblSustain.reset (new juce::Label ("lblSustain", juce::translate("Sustain")));
    // lblSustain->setFont(getAppFonts().getFont(LumatoneEditorFont::UniviaProBold));
    // lblSustain->setJustificationType (juce::Justification::centredLeft);
    // lblSustain->setEditable (false, false, false);
    // lblSustain->setColour (juce::Label::textColourId, juce::Colour (0xff61acc8));
    // lblSustain->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    // lblSustain->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));
    // addAndMakeVisible (lblSustain.get());


    addEditorListener(this);
}

MappingSettingsControls::~MappingSettingsControls()
{
    removeEditorListener(this);

    lblGeneralSettings = nullptr;
    buttonAfterTouchActive = nullptr;
    buttonLightOnKeyStrokes = nullptr;
}

//==============================================================================
void MappingSettingsControls::paint (juce::Graphics& g)
{
    g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlAreaBackground));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), roundedCornerSize);

    g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlBoxBackground));
    g.fillRoundedRectangle(generalSettingsBounds, roundedCornerSize);
    g.fillRoundedRectangle(pedalBounds, roundedCornerSize);
}

void MappingSettingsControls::resized()
{
    float w = (float)getWidth();
    float h = (float)getHeight();

    roundedCornerSize = roundToInt(getWindowBounds().getHeight() * ROUNDEDCORNERTOAPPHEIGHT);

    int settingsMarginX = roundToInt(w * controlAreaXMargin);
    int settingsYMargin = roundToInt(h * settingsAreaMarginH);

    int generalSettingsRightX = roundToInt(w * 0.5f - settingsMarginX);
    int pedalSettingsX = roundToInt(w * 0.5f);

    int lblHeight = roundToInt(h * settingsLabelH);

    int controlAreaY = lblGeneralSettings->getBottom() + settingsYMargin;
    int settingsHeight = h - controlAreaY - settingsYMargin;

    generalSettingsBounds = juce::Rectangle<float>(settingsMarginX, controlAreaY, generalSettingsRightX - settingsMarginX, settingsHeight);
    pedalBounds = juce::Rectangle<float>(pedalSettingsX, controlAreaY, w - settingsMarginX - pedalSettingsX, settingsHeight);

    resizeLabelWithHeight(lblGeneralSettings.get(), lblHeight);
    lblGeneralSettings->setTopLeftPosition(generalSettingsBounds.getX(), settingsYMargin);

    resizeLabelWithHeight(lblPedalSettings.get(), lblHeight);
    lblPedalSettings->setTopLeftPosition(pedalBounds.getX(), lblGeneralSettings->getY());

    int buttonHeight = roundToInt(h * controlHeightScalar);
    int buttonY = roundToInt(h * 0.3f);
    int buttonMarginY = roundToInt(buttonHeight * controlYMarginToControlHeight);

    int controlMarginX = proportionOfWidth(controlXMargin);

    buttonAfterTouchActive->setBounds(
        generalSettingsBounds.withTrimmedLeft(controlMarginX).withTrimmedTop(buttonMarginY).withHeight(buttonHeight).toNearestInt());

    buttonLightOnKeyStrokes->setBounds(
        buttonAfterTouchActive->getBounds().translated(0, buttonHeight + buttonMarginY));

    btnInvertSustain->setBounds(pedalBounds.withTrimmedLeft(controlMarginX)
                                           .withTop(buttonY)
                                           .withHeight(buttonHeight)
                                           .toNearestInt());

    btnInvertExpression->setBounds(pedalBounds.withTrimmedLeft(controlMarginX)
                                              .withTop(btnInvertSustain->getBottom() + settingsYMargin)
                                              .withHeight(buttonHeight).toNearestInt());

    lblExprSensitivity->setBounds(pedalBounds.withTop(btnInvertExpression->getBottom() + settingsYMargin)
                                             .withHeight(buttonHeight)
                                             .toNearestInt());

    sldExprCtrlSensitivity->setBounds(pedalBounds.withTop(lblExprSensitivity->getBottom() + buttonMarginY * 0.25f)
                                                 .withTrimmedBottom(buttonMarginY)
                                                 .toNearestInt());
}

void MappingSettingsControls::loadFromMapping()
{
  	buttonAfterTouchActive->setToggleState(getAftertouchOn(), juce::NotificationType::dontSendNotification);
    buttonLightOnKeyStrokes->setToggleState(getLightOnKeyStrokes(), juce::NotificationType::dontSendNotification);
  	btnInvertExpression->setToggleState(getInvertExpression(), juce::NotificationType::dontSendNotification);
    btnInvertSustain->setToggleState(getInvertSustain(), juce::NotificationType::dontSendNotification);
    sldExprCtrlSensitivity->setValue(getExpressionSensitivity(), juce::NotificationType::dontSendNotification);
}

void MappingSettingsControls::exprSensitivitySliderCallback()
{
    int newSensitvity = sldExprCtrlSensitivity->getValue();
    // ToDo value checking: encapsulate in keyboard data structure?
    if (newSensitvity < 0)
    {
        newSensitvity = 0;
        sldExprCtrlSensitivity->setValue(newSensitvity);
    }

    if (newSensitvity > 0x7f)
    {
        newSensitvity = 0x7f;
        sldExprCtrlSensitivity->setValue(newSensitvity);
    }

    setExpressionSensitivity(newSensitvity);
}

void MappingSettingsControls::lightOnKeyStrokesChanged(bool lightOn)
{
    buttonLightOnKeyStrokes->setToggleState(getLightOnKeyStrokes(), dontSendNotification);
}

void MappingSettingsControls::aftertouchToggled(bool enabled)
{
    buttonAfterTouchActive->setToggleState(getAftertouchOn(), dontSendNotification);
}

void MappingSettingsControls::expressionPedalSensitivityChanged(unsigned char value)
{
    sldExprCtrlSensitivity->setValue(getExpressionSensitivity(), dontSendNotification);
}

void MappingSettingsControls::invertFootControllerChanged(bool inverted)
{
    btnInvertExpression->setToggleState(getInvertExpression(), dontSendNotification);
}

void MappingSettingsControls::invertSustainToggled(bool inverted)
{
    btnInvertSustain->setToggleState(getInvertSustain(), dontSendNotification);
}

void MappingSettingsControls::firmwareVersionChanged()
{
    if (getFirmwareSupport().versionAcknowledgesCommand(getLumatoneVersion(), INVERT_SUSTAIN_PEDAL))
    {
        btnInvertSustain->setEnabled(true);
        btnInvertSustain->setTooltip("");
    }
    else
    {
        btnInvertSustain->setEnabled(false);
        btnInvertSustain->setTooltip("This feature is not supported by the firmware version of your Lumatone.");
    }
}
