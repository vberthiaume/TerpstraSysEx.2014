/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "MidiEditArea.h"
#include "../style/LumatoneEditorLookAndFeel.h"
#include "../lumatone_editor_library/device/lumatone_controller.h"
#include "../lumatone_editor_library/graphics/view_constants.h"
//[/Headers]



//[MiscUserDefs] You can add your own user definitions and misc code here...

//[/MiscUserDefs]

//==============================================================================
MidiEditArea::MidiEditArea (const LumatoneEditorState& stateIn)
    : LumatoneEditorState("MidiEditArea", stateIn)
	, LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
	, LumatoneApplicationState::DeviceController(static_cast<LumatoneApplicationState&>(*this))
{
    //[Constructor_pre] You can add your own custom stuff here..
	setName("MidiEditArea");

	lumatoneLabel.reset(new juce::Label("LumatoneLabel", "lumatone"));
	lumatoneLabel->setFont(getAppFonts().getFont(LumatoneEditorFont::UniviaProBold));
	lumatoneLabel->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::TitlePink));
	lumatoneLabel->setJustificationType(Justification::centred);
	addAndMakeVisible(lumatoneLabel.get());

    noteOnLabel.reset (new juce::Label ("noteOnLabel", ""));
    noteOnLabel->setFont (getAppFonts ().getFont (LumatoneEditorFont::CourierNew));
    noteOnLabel->setColour (juce::Label::ColourIds::textColourId, getEditorLookAndFeel ().findColour (LumatoneEditorColourIDs::ActiveText));
    noteOnLabel->setColour (juce::Label::ColourIds::backgroundColourId, getEditorLookAndFeel ().findColour (LumatoneEditorColourIDs::HeaderBackground));
    noteOnLabel->setJustificationType (Justification::centred);
    addAndMakeVisible (noteOnLabel.get ());

	liveEditorBtn.reset(new juce::TextButton("LiveEditorButton"));
	getEditorLookAndFeel().setupRadioTextButton(*liveEditorBtn, 10, false);
	liveEditorBtn->setButtonText(translate("LiveEditor"));
	liveEditorBtn->setConnectedEdges(juce::Button::ConnectedOnRight);
	liveEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontOverride, LumatoneEditorFont::UniviaProBold);
	liveEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, editModeFontScalar);
	addChildComponent(liveEditorBtn.get());
	liveEditorBtn->onClick = [&]()
	{
		if (liveEditorBtn->getToggleState())
		{
			setEditMode(EditorMode::ONLINE);
			editModeChangedCallback();
		}
	};

	offlineEditorBtn.reset(new juce::TextButton("OfflineEditorButton"));
	getEditorLookAndFeel().setupRadioTextButton(*offlineEditorBtn, 10, true);
	offlineEditorBtn->setButtonText(translate("OfflineEditor"));
	offlineEditorBtn->setConnectedEdges(juce::Button::ConnectedOnLeft);
	offlineEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontOverride, LumatoneEditorFont::UniviaProBold);
	offlineEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, editModeFontScalar);
	addChildComponent(offlineEditorBtn.get());
	offlineEditorBtn->onClick = [&]()
	{
		if (offlineEditorBtn->getToggleState())
		{
			setEditMode(EditorMode::OFFLINE);
			editModeChangedCallback();
		}
	};

    // liveEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, 0.6f);
    // offlineEditorBtn->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, 0.6f);


	pleaseConnectLabel.reset(new juce::Label("PleaseConnectLabel", translate("PleaseConnect")));
	pleaseConnectLabel->setFont(getAppFonts().getFont(LumatoneEditorFont::UniviaProBold));
	pleaseConnectLabel->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ActiveText));
	addAndMakeVisible(*pleaseConnectLabel);

	offlineMsgLabel.reset(new juce::Label("DirectionsLabel", translate("OfflineMessage")));
	offlineMsgLabel->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
	offlineMsgLabel->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
	addAndMakeVisible(*offlineMsgLabel);

	logomark.reset(new juce::Component());
	addAndMakeVisible(logomark.get());

	logomarkPath = createLogomark();

    cbMidiInput.reset (new juce::ComboBox ("cbMidiInput"));
    addAndMakeVisible (cbMidiInput.get());
    cbMidiInput->setTooltip (juce::translate("Receives answers to sent SysEx commands and the current configuration from controller "));
    cbMidiInput->setEditableText (false);
    cbMidiInput->setJustificationType (juce::Justification::centredLeft);
    cbMidiInput->setTextWhenNothingSelected (juce::translate("Select MIDI Input"));
    cbMidiInput->setTextWhenNoChoicesAvailable (juce::translate("(no choices)"));
    cbMidiInput->onChange = [&]()
	{
		midiInputChangedCallback();
	};

    cbMidiOutput.reset (new juce::ComboBox ("cbMidiOutput"));
    addAndMakeVisible (cbMidiOutput.get());
    cbMidiOutput->setTooltip (juce::translate("Key mappings are sent to this port. This happens automatically if a valid MIDI port is selected."));
    cbMidiOutput->setEditableText (false);
    cbMidiOutput->setJustificationType (juce::Justification::centredLeft);
    cbMidiOutput->setTextWhenNothingSelected (juce::translate("Select MIDI Output"));
    cbMidiOutput->setTextWhenNoChoicesAvailable (juce::translate("(no choices)"));
	cbMidiOutput->onChange = [&]()
	{
		midiOutputChangedCallback();
	};

    lblConnectionState.reset (new juce::Label ("lblConnectionState",
                                               juce::translate("Disconnected")));
    addAndMakeVisible (lblConnectionState.get());
    lblConnectionState->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblConnectionState->setJustificationType (juce::Justification::centredLeft);
    lblConnectionState->setEditable (false, false, false);
    lblConnectionState->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblConnectionState->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    lblEditMode.reset (new juce::Label ("lblEditMode",
                                        juce::translate("Edit Mode:")));
    addAndMakeVisible (lblEditMode.get());
    lblEditMode->setFont (juce::Font (18.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    lblEditMode->setJustificationType (juce::Justification::centredLeft);
    lblEditMode->setEditable (false, false, false);
    lblEditMode->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    lblEditMode->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    btnAutoConnect.reset (new juce::TextButton ("btnAutoConnect"));
    addAndMakeVisible (btnAutoConnect.get());
    btnAutoConnect->setTooltip (juce::translate("Toggle between automatic or manual connection to Lumatone"));
    btnAutoConnect->setButtonText (juce::translate("auto"));
	btnAutoConnect->setClickingTogglesState(true);
	btnAutoConnect->onClick = [&]()
	{
		onAutoConnectionChanged();
	};


	// Set up styles

	connectedColours.add(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DisconnectedRed));
	connectedColours.add(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ConnectedGreen));

	cbMidiInput->getProperties().set(LumatoneEditorStyleIDs::popupMenuTargetWidth, 1);
	cbMidiInput->getProperties().set(LumatoneEditorStyleIDs::popupMenuBackgroundColour, getEditorLookAndFeel().findColour(ComboBox::ColourIds::backgroundColourId).toString());
	cbMidiInput->setVisible(false);

	cbMidiOutput->getProperties().set(LumatoneEditorStyleIDs::popupMenuTargetWidth, 1);
	cbMidiOutput->getProperties().set(LumatoneEditorStyleIDs::popupMenuBackgroundColour, getEditorLookAndFeel().findColour(ComboBox::ColourIds::backgroundColourId).toString());
	cbMidiOutput->setVisible(false);

	lblConnectionState->setFont(getAppFonts().getFont(LumatoneEditorFont::UniviaProBold));
	lblConnectionState->setColour(juce::Label::ColourIds::textColourId, connectedColours[isConnected()]);
    lblConnectionState->getProperties().set(LumatoneEditorStyleIDs::labelMaximumLineCount, 2);

	lblEditMode->setFont(getAppFonts().getFont(LumatoneEditorFont::UniviaProBold));
	lblEditMode->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LabelPink));

	ioAreaFlexBox.alignContent = juce::FlexBox::AlignContent::center;
	ioAreaFlexBox.alignItems = juce::FlexBox::AlignItems::center;
	ioAreaFlexBox.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;


	refreshInputMenuAndSetSelected(-1, dontSendNotification);
	refreshOutputMenuAndSetSelected(-1, dontSendNotification);
	setConnectivity(false);

	addEditorListener(this);
	addStatusListener(this);

    // btnAutoConnect->setToggleState(isAutoConnectionEnabled(), sendNotificationSync);
	bool enableAutoConnection = getBoolProperty(LumatoneEditorProperty::AutoConnectDevice, true);
    btnAutoConnect->setToggleState(enableAutoConnection, dontSendNotification);
	onAutoConnectionChanged(false);

    addMidiListener (this);
}

MidiEditArea::~MidiEditArea()
{
	liveEditorBtn = nullptr;
	offlineEditorBtn = nullptr;
	offlineMsgLabel = nullptr;
	pleaseConnectLabel = nullptr;
	logomark = nullptr;

    cbMidiInput = nullptr;
    cbMidiOutput = nullptr;
    lblConnectionState = nullptr;
    lblEditMode = nullptr;
    btnAutoConnect = nullptr;
}

bool MidiEditArea::isConnected() const
{
	return getConnectionState() == ConnectionState::ONLINE;
}

//==============================================================================
void MidiEditArea::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xffbad0de));

	g.fillAll(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LightBackground));

	// Dark background for title and logomark
	g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::HeaderBackground));
	g.fillRect(lumatoneLabelBounds);
	g.fillRect(connectivityArea);

	// Rounded rect for device boxes
	if (!isConnected())
	{
		g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LightBackground));
		g.fillRoundedRectangle(ioBounds, roundToInt(getHeight() * controlBoundsCornerRadius));
	}

	g.setColour(connectedColours[(int)(isConnected() && liveEditorBtn->getToggleState())]);
	drawPathToFillBounds(g, logomarkPath, logomarkBounds);
}

void MidiEditArea::resized()
{
	float w = getWidth();
	float h = getHeight();

	lumatoneLabelBounds = getBounds().withRight(roundToInt(w * lumatoneLabelAreaWidth));
	resizeLabelWithWidth(lumatoneLabel.get(), lumatoneLabelBounds.proportionOfWidth(lumatoneLabelWidthInArea));
	lumatoneLabel->setCentrePosition(lumatoneLabelBounds.getCentre());

	// Also used to position logomark
	ioBounds.setBounds(
		roundToInt(w * controlBoundsX), roundToInt(h * controlBoundsY),
		roundToInt(w * controlBoundsWidth), roundToInt(h * controlBoundsHeight)
	);

	int logomarkSize = roundToInt(h * logomarkHeight);
	logomarkBounds.setSize(logomarkSize, logomarkSize);
	logomarkBounds.setCentre(ioBounds.getRight() + roundToInt((getWidth() - ioBounds.getRight()) * 0.5f), roundToInt(h * 0.5f));

	if (isConnected())
	{
		int lblHeight = roundToInt(h * editModeHeight);
		resizeLabelWithHeight(lblEditMode.get(), lblHeight);
		lblEditMode->setTopLeftPosition(
			lumatoneLabelBounds.getRight() + roundToInt(w * editModeX),
			roundToInt((h - lblEditMode->getHeight()) * 0.5f)
		);

		liveEditorBtn->setSize(roundToInt(w * liveEditButtonWidth), roundToInt(h* editModeButtonHeight));
		liveEditorBtn->setTopLeftPosition(
			lblEditMode->getRight(),
			roundToInt((h - liveEditorBtn->getHeight()) * 0.5f)
		);

		offlineEditorBtn->setBounds(
			liveEditorBtn->getRight(), liveEditorBtn->getY(), roundToInt(w * offlineEditButtonWidth), liveEditorBtn->getHeight()
		);

        noteOnLabel->setBounds (
            offlineEditorBtn->getRight () + 50, offlineEditorBtn->getY (), 300, liveEditorBtn->getHeight ()
        );

		connectivityArea = getBounds().toFloat().withLeft(roundToInt(w * connectedAreaX));

		int logoMargin = w - logomarkBounds.getRight();
		lblConnectionState->setTopLeftPosition(connectivityArea.getX(), roundToInt((h - lblHeight) * 0.5f));
		lblConnectionState->setSize(logomarkBounds.getX() - connectivityArea.getX() - logoMargin, lblHeight);
        lblConnectionState->setJustificationType (juce::Justification::centredRight);
	}
	else
	{
		int controlHeight = roundToInt(ioBounds.getHeight() * midiDeviceControlBoundsHeight);

		connectivityArea = getBounds().toFloat().withLeft(roundToInt(w * disconnectedAreaX));

		int lblMarginX = roundToInt(ioBounds.getWidth() * controlBoundsMarginScalar);
		int lblMarginY = roundToInt((ioBounds.getHeight() - h * connectivityHeight) * 0.5f);

		ioAreaFlexBox.items.clear();
		ioAreaFlexBox.items.add(juce::FlexItem(*btnAutoConnect).withFlex(0).withWidth(controlHeight * 1.6f).withHeight(controlHeight));

		int deviceBoxWidth = roundToInt(ioBounds.getWidth() * midiDeviceControlBoundsWidth);

		if (btnAutoConnect->getToggleState())
		{
			lblConnectionState->setJustificationType(juce::Justification::centredLeft);

			// Make sure these are never 0 size
			cbMidiInput->setSize(deviceBoxWidth, controlHeight);
			cbMidiOutput->setSize(deviceBoxWidth, controlHeight);
		}
		else
		{
			ioAreaFlexBox.items.add(juce::FlexItem(*cbMidiInput).withFlex(0).withWidth(deviceBoxWidth).withHeight(controlHeight));
			ioAreaFlexBox.items.add(juce::FlexItem(*cbMidiOutput).withFlex(0).withWidth(deviceBoxWidth).withHeight(controlHeight));

			lblConnectionState->setJustificationType(juce::Justification::centredRight);
		}

		// Not sure why AlignSelf is necessary...
		ioAreaFlexBox.items.add(juce::FlexItem(*lblConnectionState).withFlex(1).withHeight(h * connectivityHeight).withAlignSelf(FlexItem::AlignSelf::center));

		float itemMargin = lblMarginX * 0.5f;
		for (int i = 0; i < ioAreaFlexBox.items.size(); i++)
		{
			ioAreaFlexBox.items.getReference(i).margin = juce::FlexItem::Margin(0, itemMargin, 0, 0);
		}

		ioAreaFlexBox.performLayout(
			ioBounds.reduced(lblMarginX, lblMarginY)
		);

		pleaseConnectLabel->setTopLeftPosition(roundToInt(w * pleaseConnectX), roundToInt(h * pleaseConnectY));
		resizeLabelWithHeight(pleaseConnectLabel.get(), roundToInt(h * pleaseConnectHeight));

		offlineMsgLabel->setTopLeftPosition(roundToInt(w * connectionDirectionsX), roundToInt(h * connectionDirectionsY));
		offlineMsgLabel->setSize(connectivityArea.getX() - pleaseConnectLabel->getX(), roundToInt(h * connectionDirectionsHeight));
		offlineMsgLabel->setFont(offlineMsgLabel->getFont().withHeight(offlineMsgLabel->getHeight()));
	}
}

void MidiEditArea::setConnectivity(bool isConnectedIn, juce::String connectionStatus)
{
	bool isNotConnected = !isConnectedIn;

	btnAutoConnect->setVisible(isNotConnected);
	cbMidiInput->setVisible(isNotConnected && !btnAutoConnect->getToggleState());
	cbMidiOutput->setVisible(isNotConnected && !btnAutoConnect->getToggleState());
	pleaseConnectLabel->setVisible(isNotConnected);
	offlineMsgLabel->setVisible(isNotConnected);

	lblEditMode->setVisible(isConnectedIn);
	liveEditorBtn->setVisible(isConnectedIn);
	offlineEditorBtn->setVisible(isConnectedIn);

	btnAutoConnect->setEnabled(!isConnectedIn);

	if (isConnectedIn)
	{
		if (liveEditorBtn->getToggleState())
		{
			if (connectionStatus.isEmpty())
				connectionStatus = "Connected";
			lblConnectionState->setText(translate(connectionStatus), dontSendNotification);
		}
		else
		{
			if (connectionStatus.isEmpty())
				connectionStatus = "Offline";
			lblConnectionState->setText(translate(connectionStatus), dontSendNotification);
		}
	}
	else
	{
		if (btnAutoConnect->getToggleState())
		{
			if (connectionStatus.isEmpty())
				connectionStatus = "Searching for Lumatone...";
			lblConnectionState->setText(translate(connectionStatus), dontSendNotification);
		}
		else
		{
			if (connectionStatus.isEmpty())
				connectionStatus = "Disconnected";
			lblConnectionState->setText(translate(connectionStatus), dontSendNotification);
			startTimer(deviceRefreshTimeoutMs);
		}
	}

	lblConnectionState->setColour(juce::Label::ColourIds::textColourId, connectedColours[isConnectedIn]);

	resized();
}

void MidiEditArea::onAutoConnectionChanged(bool writeToPropertiesFile)
{
	bool isAuto = btnAutoConnect->getToggleState();
	bool isNotAuto = !isAuto;

	if (writeToPropertiesFile)
		savePropertyBoolValue(LumatoneEditorProperty::AutoConnectDevice, isAuto);

	cbMidiInput->setVisible(isNotAuto);
	cbMidiOutput->setVisible(isNotAuto);

	setAutoConnectionEnabled(isAuto);

	if (isAuto)
	{
		lblConnectionState->setText(translate("Searching for Lumatone..."), dontSendNotification);
	}
	else
	{
		lblConnectionState->setText(translate("Disconnected"), dontSendNotification);
		startTimer(deviceRefreshTimeoutMs);
	}

	resized();
}

void MidiEditArea::editModeChangedCallback()
{
	if (getEditorMode() == EditorMode::ONLINE)
	{
		liveEditorBtn->setToggleState(true, juce::NotificationType::sendNotification);
		lblConnectionState->setText("Connected", juce::NotificationType::dontSendNotification);

		if (getHasChangesToSave())
            onOpenConnectionToDevice(translate("Switch to Live Mode with unsaved changes"));
	}
	else
	{
		offlineEditorBtn->setToggleState(true, juce::NotificationType::sendNotification);
		lblConnectionState->setText("Offline", juce::NotificationType::dontSendNotification);
	}

	lblConnectionState->setColour(juce::Label::ColourIds::textColourId, connectedColours[(int)liveEditorBtn->getToggleState()]);
    repaint();
}

void MidiEditArea::midiInputChangedCallback()
{
	if (cbMidiInput->getSelectedItemIndex() >= 0)
		setMidiInput(cbMidiInput->getSelectedItemIndex());

	if (cbMidiInput->getSelectedItemIndex() < 0 || cbMidiOutput->getSelectedItemIndex() < 0)
	{
		setConnectivity(false);
	}
	else
	{
		jassert(!isConnected());
		lblConnectionState->setText("Connecting...", juce::NotificationType::dontSendNotification);
	}
}

void MidiEditArea::midiOutputChangedCallback()
{
	if (cbMidiOutput->getSelectedItemIndex() >= 0)
		setMidiOutput(cbMidiOutput->getSelectedItemIndex());

	if (cbMidiInput->getSelectedItemIndex() < 0 || cbMidiOutput->getSelectedItemIndex() < 0)
	{
		setConnectivity(false);
	}
	else
	{
		jassert(!isConnected());
		lblConnectionState->setText("Connecting...", juce::NotificationType::dontSendNotification);
	}
}

void MidiEditArea::connectionFailed()
{
	setConnectivity(false, "No answer");
	//errorVisualizer.setErrorLevel(
    //    *lblConnectionState.get(),
    //    HajuErrorVisualizer::ErrorLevel::error,
    //    "No answer...");
	// getLumatoneController()->setMidiInput(-1);
	// getLumatoneController()->setMidiOutput(-1);
}

void MidiEditArea::connectionStateChanged(ConnectionState state)
{
	if (state == ConnectionState::ONLINE)
	{
		refreshInputMenuAndSetSelected(getMidiInputIndex(), dontSendNotification);
		refreshOutputMenuAndSetSelected(getMidiOutputIndex(), dontSendNotification);
		setConnectivity(true);

		onOpenConnectionToDevice();
	}
	else if (state == ConnectionState::DISCONNECTED)
	{
		if (!btnAutoConnect->getToggleState())
			startTimer(deviceRefreshTimeoutMs);

		else
		{
			refreshInputMenuAndSetSelected(-1, juce::NotificationType::dontSendNotification);
			refreshOutputMenuAndSetSelected(-1, juce::NotificationType::sendNotificationAsync);
		}

		setConnectivity(false);
	}
}

void MidiEditArea::editorModeChanged(EditorMode editModeIn)
{
	editModeChangedCallback();
}

void MidiEditArea::onOpenConnectionToDevice(juce::String dialogTitle)
{
	//jassert(cbMidiInput->getSelectedItemIndex() >= 0 && cbMidiOutput->getSelectedItemIndex() >= 0);

	if (dialogTitle.length() == 0)
		dialogTitle = translate("Connection Established!");

    jassert(!isWaitingForUserChoice);
    if (isWaitingForUserChoice)
    {
        DBG("Bad connection loop detected");
        return;
    }

    isWaitingForUserChoice = true;

	auto alertOptions = juce::MessageBoxOptions().withTitle(dialogTitle)
                                                 .withMessage(translate("Do you want to send the current setup to your Lumatone?"))
                                                 .withIconType(juce::AlertWindow::AlertIconType::QuestionIcon)
                                                 .withAssociatedComponent(getParentComponent())
                                                 .withButton("Send Editor Layout")
                                                 .withButton("Keep Editing Offline")
                                                 .withButton("Import From Lumatone");

	juce::AlertWindow::showAsync(alertOptions, [&](int retc)
	{
		isWaitingForUserChoice = false;

		if (retc == 0) // Import
		{
			// TODO non getLumatoneController call
			setEditMode(EditorMode::ONLINE);
			LumatoneEditorState::Controller::requestCompleteDeviceConfig();
		}
		else if (retc == 1) // Send
		{
			setEditMode(EditorMode::ONLINE);
			LumatoneEditorState::setCompleteConfig(*getMappingData());
		}
		else if (retc == 2) // Offline
		{
			setEditMode(EditorMode::OFFLINE);
		}

		editModeChangedCallback();
	});
}

void MidiEditArea::refreshInputMenuAndSetSelected(int inputDeviceIndex, juce::NotificationType notificationType)
{
	cbMidiInput->clear(juce::NotificationType::dontSendNotification);
	int i = 1;
	for (auto device : getMidiInputList())
		cbMidiInput->addItem(device.name, i++);

	if (inputDeviceIndex >= 0)
		cbMidiInput->setSelectedId(inputDeviceIndex, notificationType);
}

void MidiEditArea::refreshOutputMenuAndSetSelected(int outputDeviceIndex, juce::NotificationType notificationType)
{
	cbMidiOutput->clear(juce::NotificationType::dontSendNotification);
	int i = 1;
	for (auto device : getMidiOutputList())
		cbMidiOutput->addItem(device.name, i++);

	if (outputDeviceIndex >= 0)
		cbMidiOutput->setSelectedId(outputDeviceIndex, notificationType);
}

void MidiEditArea::timerCallback()
{
	if (isConnected())
	{
		stopTimer();
	}
	else
	{
		refreshInputMenuAndSetSelected(
			cbMidiInput->getSelectedId(),
			juce::NotificationType::dontSendNotification
		);

		refreshOutputMenuAndSetSelected(
			cbMidiOutput->getSelectedId(),
			juce::NotificationType::dontSendNotification
		);
	}
}
void MidiEditArea::handleNoteOn (int midiChannel, int midiNote, juce::uint8 velocity)
{
    const auto text = juce::String ("NOTE ON: CHANNEL " + juce::String (midiChannel) + ", NOTE " + juce::String (midiNote) << ", VELOCITY " << juce::String (velocity));
    noteOnLabel->setText (text, juce::dontSendNotification);

    if (!noteLabelClearTimer)
    {
        noteLabelClearTimer = std::make_unique<OneShotTimer> ();
        noteLabelClearTimer->callback = [label = juce::Component::SafePointer<juce::Label> (noteOnLabel.get())]()
            {
                if (label)
                    label->setText ({}, juce::dontSendNotification);
            };
    }

    noteLabelClearTimer->start (2000); // restart the 2-second countdown
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MidiEditArea" componentName=""
                 parentClasses="public Component, public TerpstraMidiDriver::Listener, public juce::ChangeListener, public Timer"
                 constructorParams="LumatoneEditorLookAndFeel&amp; lookAndFeelIn, DeviceActivityMonitor&amp; deviceMonitorIn"
                 variableInitialisers="lookAndFeel(lookAndFeelIn),errorVisualizer(lookAndFeelIn), deviceMonitor(deviceMonitorIn)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="1024" initialHeight="48">
  <BACKGROUND backgroundColour="ffbad0de"/>
  <COMBOBOX name="cbMidiInput" id="aad3610c2aac943b" memberName="cbMidiInput"
            virtualName="" explicitFocusOrder="0" pos="232 8 184 24" tooltip="Receives answers to sent SysEx commands and the current configuration from controller "
            editable="0" layout="33" items="" textWhenNonSelected="Select MIDI Input"
            textWhenNoItems="(no choices)"/>
  <COMBOBOX name="cbMidiOutput" id="f3f3544c4916f527" memberName="cbMidiOutput"
            virtualName="" explicitFocusOrder="0" pos="432 8 184 24" tooltip="Key mappings are sent to this port. This happens automatically if a valid MIDI port is selected."
            editable="0" layout="33" items="" textWhenNonSelected="Select MIDI Output"
            textWhenNoItems="(no choices)"/>
  <LABEL name="lblConnectionState" id="132031ea466ea84f" memberName="lblConnectionState"
         virtualName="" explicitFocusOrder="0" pos="624 8 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Disconnected" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15.0" kerning="0.0" bold="0" italic="0" justification="33"/>
  <LABEL name="lblEditMode" id="ea4ef68f1765a3d4" memberName="lblEditMode"
         virtualName="" explicitFocusOrder="0" pos="8 8 96 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Edit Mode:" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="18.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="btnAutoConnect" id="381f46e171b5df06" memberName="btnAutoConnect"
              virtualName="" explicitFocusOrder="0" pos="184 8 39 24" tooltip="Toggle between automatic or manual connection to Lumatone"
              buttonText="auto" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
