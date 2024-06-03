/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "LumatoneEditorLookAndFeel.h"

#include "./Settings/SettingsContainer.h"

//[/Headers]

#include "GlobalSettingsArea.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GlobalSettingsArea::GlobalSettingsArea (const LumatoneEditorState& stateIn)
    : LumatoneEditorState("GlobalSettingsArea", stateIn)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    lblPresetButtonColours.reset (new juce::Label ("lblPresetButtonColours",
                                                   juce::translate("Preset Button Colours:")));
    addAndMakeVisible (lblPresetButtonColours.get());
    lblPresetButtonColours->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblPresetButtonColours->setJustificationType (juce::Justification::centredLeft);
    lblPresetButtonColours->setEditable (false, false, false);
    lblPresetButtonColours->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblPresetButtonColours->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblColourInactiveMacroButton.reset (new juce::Label ("lblColourInactiveMacroButton",
                                                         juce::translate("inactive")));
    addAndMakeVisible (lblColourInactiveMacroButton.get());
    lblColourInactiveMacroButton->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblColourInactiveMacroButton->setJustificationType (juce::Justification::centredLeft);
    lblColourInactiveMacroButton->setEditable (false, false, false);
    lblColourInactiveMacroButton->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblColourInactiveMacroButton->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblColourActiveMacroButton.reset (new juce::Label ("lblColourActiveMacroButton",
                                                       juce::translate("active")));
    addAndMakeVisible (lblColourActiveMacroButton.get());
    lblColourActiveMacroButton->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblColourActiveMacroButton->setJustificationType (juce::Justification::centredLeft);
    lblColourActiveMacroButton->setEditable (false, false, false);
    lblColourActiveMacroButton->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblColourActiveMacroButton->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    settingsButton.reset (new juce::TextButton ("buttonCalibrate"));
    addAndMakeVisible (settingsButton.get());
    settingsButton->setTooltip (juce::translate("Show controls for calibration, setting controller MIDI channels, and updating firmware"));
    settingsButton->setButtonText (juce::translate("Settings"));
    settingsButton->addListener (this);


    //[UserPreSize]
    activeMacroButtonColourEdit.reset(new ColourViewComponent());
    addAndMakeVisible(activeMacroButtonColourEdit.get());
    activeMacroButtonColourEdit->addChangeListener(this);
    activeMacroButtonColourEdit->setColour(getActiveMacroButtonColour().toString(), false);

    inactiveMacroButtonColourEdit.reset(new ColourViewComponent());
    addAndMakeVisible(inactiveMacroButtonColourEdit.get());
    inactiveMacroButtonColourEdit->addChangeListener(this);
    inactiveMacroButtonColourEdit->setColour(getInactiveMacroButtonColour().toString(), false);

    lblDeveloperMode.reset(new juce::Label("DeveloperModeLabel", "Developer Mode"));
    addChildComponent(lblDeveloperMode.get());
    setDeveloperMode(getInDeveloperMode());

    lblPresetButtonColours->setFont(getAppFonts().getFont(LumatoneEditorFont::UniviaProBold));
    lblColourActiveMacroButton->setFont(getAppFonts().getFont(LumatoneEditorFont::GothamNarrowMedium));
    lblPresetButtonColours->setFont(getAppFonts().getFont(LumatoneEditorFont::UniviaProBold));

    addStatusListener(this);
    addEditorListener(this);

    settingsButton->setEnabled(false);

    lblPresetButtonColours->setColour(Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LabelPink));

    lblColourActiveMacroButton->setColour(Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    lblColourInactiveMacroButton->setColour(Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));

    settingsButton->setColour(TextButton::ColourIds::buttonColourId, Colour(0xff383b3d));
    settingsButton->setColour(TextButton::ColourIds::textColourOffId, Colour(0xffffffff));

    connectionStateChanged(getConnectionState());

    /* We don't want a resize here
    /*
    //[/UserPreSize]

    setSize (456, 64);


    //[Constructor] You can add your own custom stuff here..
    */


	// Set values according to the properties files
	// restoreStateFromPropertiesFile(TerpstraSysExApplication::getApp().getPropertiesFile());
    //[/Constructor]
}

GlobalSettingsArea::~GlobalSettingsArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
	inactiveMacroButtonColourEdit = nullptr;
	activeMacroButtonColourEdit = nullptr;
    //[/Destructor_pre]

    lblPresetButtonColours = nullptr;
    lblColourInactiveMacroButton = nullptr;
    lblColourActiveMacroButton = nullptr;
    settingsButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

void GlobalSettingsArea::macroButtonInactiveColourChanged(juce::Colour colour)
{
    inactiveMacroButtonColourEdit->setColour(colour.toString(), false);
}

void GlobalSettingsArea::macroButtonActiveColourChanged(juce::Colour colour)
{
    activeMacroButtonColourEdit->setColour(colour.toString(), false);
}

//==============================================================================
void GlobalSettingsArea::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::MediumBackground));
    g.fillAll();

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GlobalSettingsArea::resized()
{
    //[UserPreResize] Add your own custom resize code here..

    // Build right-to-left

    int calbrateBtnHeight = roundToInt(getHeight() * calibrateHeight);
    int calibrateWidth = getLookAndFeel().getTextButtonWidthToFitText(*settingsButton, calbrateBtnHeight);

    float controlsMargin = roundToInt(getHeight() * 0.1f);
    float colourEditHeight = proportionOfHeight(controlsHeight);
    float controlY = proportionOfHeight((1 - controlsHeight) / 2.0f);
    float colourButtonWidth = colourEditHeight * colourButtonAspect;
    Font colourLabelsFont = getAppFonts().getFont(LumatoneEditorFont::FranklinGothic, colourEditHeight * 1.1f);

    int startMargin = proportionOfWidth(marginXRatio);
    settingsButton->setSize(calibrateWidth, calbrateBtnHeight);
    settingsButton->setTopRightPosition(getWidth() - startMargin, roundToInt((getHeight() - settingsButton->getHeight()) * 0.5f));

    lblColourInactiveMacroButton->setFont(colourLabelsFont);
    resizeLabelWithHeight(lblColourInactiveMacroButton.get(), colourEditHeight);
    lblColourInactiveMacroButton->setTopRightPosition(settingsButton->getX() - controlsMargin, controlY);

    inactiveMacroButtonColourEdit->setSize(colourButtonWidth, colourEditHeight);
    inactiveMacroButtonColourEdit->setTopRightPosition(lblColourInactiveMacroButton->getX() - controlsMargin, controlY);

    lblColourActiveMacroButton->setFont(colourLabelsFont);
    resizeLabelWithHeight(lblColourActiveMacroButton.get(), colourEditHeight);
    lblColourActiveMacroButton->setTopRightPosition(inactiveMacroButtonColourEdit->getX() - controlsMargin, controlY);

    activeMacroButtonColourEdit->setSize(colourButtonWidth, colourEditHeight);
    activeMacroButtonColourEdit->setTopRightPosition(lblColourActiveMacroButton->getX() - controlsMargin, controlY);

    resizeLabelWithHeight(lblPresetButtonColours.get(), colourEditHeight);
    lblPresetButtonColours->setTopRightPosition(activeMacroButtonColourEdit->getX() - controlsMargin, controlY);

    resizeLabelWithHeight(lblDeveloperMode.get(), getHeight());
    lblDeveloperMode->setCentrePosition(getLocalBounds().getCentre());

    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GlobalSettingsArea::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == settingsButton.get())
    {
        //[UserButtonCode_buttonCalibrate] -- add your button handler code here..

		auto settingsComponent = new SettingsContainer(*this);
        settingsComponent->setLookAndFeel(&getLookAndFeel());

		DialogWindow::LaunchOptions launchOptions;
		launchOptions.content.setOwned(settingsComponent);
		launchOptions.content->setSize(480, 240);

		launchOptions.dialogTitle = "Settings";
		launchOptions.escapeKeyTriggersCloseButton = true;
		launchOptions.useNativeTitleBar = false;
		launchOptions.resizable = false;

        launchOptions.dialogBackgroundColour = Colour();

		auto settingsDialog = launchOptions.launchAsync();
        settingsDialog->setLookAndFeel(static_cast<juce::LookAndFeel_V4*>(&getEditorLookAndFeel().compactWindowStyle));
        settingsDialog->centreWithSize(548, 240);

        // TerpstraSysExApplication::getApp().setOpenDialogWindow(settingsDialog);

        //[/UserButtonCode_buttonCalibrate]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void GlobalSettingsArea::changeListenerCallback(ChangeBroadcaster *source)
{
	if (source == inactiveMacroButtonColourEdit.get())
	{
        setInactiveMacroButtonColour(inactiveMacroButtonColourEdit->getColourAsObject());
	}
	else if (source == activeMacroButtonColourEdit.get())
	{
        setActiveMacroButtonColour(activeMacroButtonColourEdit->getColourAsObject());
	}
}

void GlobalSettingsArea::listenToColourEditButtons(Button::Listener* listenerIn)
{
    inactiveMacroButtonColourEdit->addListener(listenerIn);
    activeMacroButtonColourEdit->addListener(listenerIn);
}

void GlobalSettingsArea::setDeveloperMode(bool devModeOn)
{
    showDeveloperMode = devModeOn;
    lblDeveloperMode->setVisible(showDeveloperMode);
    if (devModeOn)
        settingsButton->setEnabled(true);
    repaint();
}

void GlobalSettingsArea::connectionStateChanged(ConnectionState state)
{
    settingsButton->setEnabled(state == ConnectionState::ONLINE);
    inactiveMacroButtonColourEdit->setEnabled(state == ConnectionState::ONLINE);
    activeMacroButtonColourEdit->setEnabled(state == ConnectionState::ONLINE);
}

// void GlobalSettingsArea::connectionFaile()
// {
//     settingsButton->setEnabled(false);
// }

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GlobalSettingsArea" componentName=""
                 parentClasses="public juce::Component, public ChangeListener, public LumatoneEditor::StatusListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="456"
                 initialHeight="64">
  <BACKGROUND backgroundColour="0"/>
  <LABEL name="lblPresetButtonColours" id="988da53c99fc73f2" memberName="lblPresetButtonColours"
         virtualName="" explicitFocusOrder="0" pos="0 8 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Preset Button Colours:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblColourInactiveMacroButton" id="9c5cddfcc2966280" memberName="lblColourInactiveMacroButton"
         virtualName="" explicitFocusOrder="0" pos="152 32 64 24" edTextCol="ff000000"
         edBkgCol="0" labelText="inactive" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblColourActiveMacroButton" id="79f93468a20f174" memberName="lblColourActiveMacroButton"
         virtualName="" explicitFocusOrder="0" pos="152 8 56 24" edTextCol="ff000000"
         edBkgCol="0" labelText="active" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="settingsButton" id="47242594c34a8de9" memberName="settingsButton"
              virtualName="" explicitFocusOrder="0" pos="336 8 112 24" tooltip="Show controls for calibration, setting controller MIDI channels, and updating firmware"
              buttonText="Settings" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
