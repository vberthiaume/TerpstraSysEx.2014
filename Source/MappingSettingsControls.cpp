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
    labelGeneralSettingslTitle.reset (new juce::Label ("labelGeneralSettingslTitle", juce::translate("General Settings")));
    labelGeneralSettingslTitle->setFont(getAppFonts().getFont(LumatoneEditorFont::UniviaProBold));
    labelGeneralSettingslTitle->setJustificationType (juce::Justification::centredLeft);
    labelGeneralSettingslTitle->setEditable (false, false, false);
    labelGeneralSettingslTitle->setColour(Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LabelBlue));
    labelGeneralSettingslTitle->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));
    addAndMakeVisible (labelGeneralSettingslTitle.get());

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

    labelExprContrSensitivity.reset (new juce::Label ("labelExprContrSensitivity", juce::translate("Sensitivity")));
    labelExprContrSensitivity->setFont(getAppFonts().getFont(LumatoneEditorFont::GothamNarrowMedium));
    labelExprContrSensitivity->setJustificationType (juce::Justification::centred);
    labelExprContrSensitivity->setEditable (false, false, false);
    labelExprContrSensitivity->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelExprContrSensitivity->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));
    addAndMakeVisible (labelExprContrSensitivity.get());

    btnInvertExpression.reset (new juce::ToggleButton ("btnInvertExpression"));
    btnInvertExpression->setButtonText (juce::translate("Invert"));
    btnInvertExpression->onClick = [&]()
    {
        setInvertExpression(btnInvertExpression->getToggleState());
    };
    addAndMakeVisible (btnInvertExpression.get());

    lblExpression.reset (new juce::Label ("lblExpression", juce::translate("Expression")));
    lblExpression->setFont(getAppFonts().getFont(LumatoneEditorFont::UniviaProBold));
    lblExpression->setJustificationType (juce::Justification::centredLeft);
    lblExpression->setEditable (false, false, false);
    lblExpression->setColour (juce::Label::textColourId, juce::Colour (0xff61acc8));
    lblExpression->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblExpression->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));
    addAndMakeVisible (lblExpression.get());

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
    btnInvertSustain->setButtonText (juce::translate("Invert"));
    btnInvertSustain->onClick = [&]()
    {
        setInvertSustain(btnInvertSustain->getToggleState());
    };
    // addAndMakeVisible (btnInvertSustain.get());

    lblSustain.reset (new juce::Label ("lblSustain", juce::translate("Sustain")));
    lblSustain->setFont(getAppFonts().getFont(LumatoneEditorFont::UniviaProBold));
    lblSustain->setJustificationType (juce::Justification::centredLeft);
    lblSustain->setEditable (false, false, false);
    lblSustain->setColour (juce::Label::textColourId, juce::Colour (0xff61acc8));
    lblSustain->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblSustain->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));
    // addAndMakeVisible (lblSustain.get());


    addEditorListener(this);
}

MappingSettingsControls::~MappingSettingsControls()
{
    removeEditorListener(this);

    labelGeneralSettingslTitle = nullptr;
    buttonAfterTouchActive = nullptr;
    buttonLightOnKeyStrokes = nullptr;
}

//==============================================================================
void MappingSettingsControls::paint (juce::Graphics& g)
{
    g.setColour(juce::Colour(0xff212626));
    g.fillRoundedRectangle(getLocalBounds().toFloat().withTop(proportionOfHeight(SETTINGSAREAMARGINHEIGHT)), roundedCornerSize);
}

void MappingSettingsControls::resized()
{
    float w = getWidth();
    float h = getHeight();

    roundedCornerSize = roundToInt(getWindowBounds().getHeight() * ROUNDEDCORNERTOAPPHEIGHT);

    resizeLabelWithHeight(labelGeneralSettingslTitle.get(), roundToInt(getHeight() * SETTINGSLABELHEIGHT));
    labelGeneralSettingslTitle->setTopLeftPosition(roundToInt(getWidth() * SETTINGSLABELMARGINWIDTH), 0);

    int marginX = roundToInt(getParentWidth() * SETTINGSCONTROLMARGINTOAPPWIDTH);

    buttonAfterTouchActive->setBounds(
        marginX, proportionOfHeight(0.3f), proportionOfWidth(1.0f), proportionOfHeight(SETTINGSTOGGLEHEIGHTSCALAR)
    );

    buttonLightOnKeyStrokes->setBounds(
        marginX, proportionOfHeight(0.5f), proportionOfWidth(1.0f), proportionOfHeight(SETTINGSTOGGLEHEIGHTSCALAR)
    );

    roundedCornerSize = roundToInt(getWindowBounds().getHeight() * ROUNDEDCORNERTOAPPHEIGHT);

    int areaMarginWidth = roundToInt(w * sectionMarginWidth) * 0.5f;
    int areaMarginHeight = roundToInt(h * SETTINGSAREAMARGINHEIGHT);

    expressionBounds = getLocalBounds().toFloat().withTop(areaMarginHeight).withRight(roundToInt(w * 0.5f - areaMarginWidth));
    sustainBounds = getLocalBounds().toFloat().withTop(areaMarginHeight).withLeft(roundToInt(w * 0.5f + areaMarginWidth));

    int lblMarginX = roundToInt(w * SETTINGSLABELMARGINWIDTH);
    int lblWidth = roundToInt(w * 0.5f);
    int lblHeight = roundToInt(h * SETTINGSLABELHEIGHT);
    lblExpression->setBounds(expressionBounds.getX() + lblMarginX, 0, lblWidth, lblHeight);
    lblSustain->setBounds(sustainBounds.getX() + lblMarginX, 0, lblWidth, lblHeight);

    int controlMargin = roundToInt(getParentWidth() * SETTINGSCONTROLMARGINTOAPPWIDTH);
    int buttonHeight = roundToInt(h * SETTINGSTOGGLEHEIGHTSCALAR);
    int buttonY = roundToInt(h * 0.3f);
    btnInvertExpression->setBounds(expressionBounds.withTrimmedLeft(controlMargin).withTop(buttonY).withHeight(buttonHeight).toNearestInt());
    btnInvertSustain->setBounds(sustainBounds.withTrimmedLeft(controlMargin).withTop(buttonY).withHeight(buttonHeight).toNearestInt());

    sldExprCtrlSensitivity->setBounds(
          expressionBounds.reduced(expressionBounds.getWidth() * 0.2f, 0)
                          .withTop(btnInvertExpression->getBottom() + buttonHeight)
                          .withTrimmedBottom(buttonHeight * 1.5f)
                          .toNearestInt()
    );

    labelExprContrSensitivity->setBounds(
         expressionBounds.withTop(sldExprCtrlSensitivity->getBottom() + buttonHeight * 0.1f)
                         .withTrimmedBottom(buttonHeight * 0.5f)
                         .toNearestInt()
    );
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
