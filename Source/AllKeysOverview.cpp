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
#include "ViewComponents.h"
#include "MainComponent.h"
#include "Main.h"
#include "LumatoneMenu.h"

#include "backport/lumatone_render.h"
#include "backport/colour_model.h"
#include "KeyCalibrationPanel.h"
#include "DebugCalibrationDragView.h"

//[/Headers]

#include "AllKeysOverview.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...


//==============================================================================
KeyMiniDisplayInsideAllKeysOverview::KeyMiniDisplayInsideAllKeysOverview(int newBoardIndex, int newKeyIndex)
	: Component("AllKeysOverview_Key" + String(newKeyIndex) + "," + String(newBoardIndex))
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	boardIndex = newBoardIndex;
	keyIndex = newKeyIndex;

	TerpstraSysExApplication::getApp().getLumatoneController()->addMidiListener(this);
}

KeyMiniDisplayInsideAllKeysOverview::~KeyMiniDisplayInsideAllKeysOverview()
{
	TerpstraSysExApplication::getApp().getLumatoneController()->removeMidiListener(this);
}

void KeyMiniDisplayInsideAllKeysOverview::paint(Graphics& g)
{
    // Rendering handled by AllKeysOverview::paint() at float precision; this component exists for mouse hit-testing only
}

void KeyMiniDisplayInsideAllKeysOverview::resized()
{

}

void KeyMiniDisplayInsideAllKeysOverview::mouseDown(const MouseEvent& e)
{
	Component::mouseDown(e);

	// Select this octave board
	jassert(getParentComponent() != nullptr);
	jassert(getParentComponent()->getParentComponent() != nullptr);
	dynamic_cast<MainContentComponent*>(getParentComponent()->getParentComponent())->
		getOctaveBoardSelectorTab()->setCurrentTabIndex(boardIndex);

	isHighlighted = true;
	repaint();

	if (e.mods.isRightButtonDown())
	{
		// Right mouse click: popup menu
		PopupMenu menu;
		TerpstraSysExApplication::getApp().getMainMenu()->createEditMenu(menu);
		menu.showMenuAsync(PopupMenu::Options());
	}

	// TODO integrate interaction through LumatoneController

	//else
	//{
	//	// NoteOn MIDI message
	//	auto keyData = getKeyData();
	//	if (keyData != nullptr && keyData->channelNumber > 0)
	//	{
	//		if (keyData->keyType == LumatoneKeyType::noteOnNoteOff)
	//		{
	//			// Send "note on" event
	//			//TerpstraSysExApplication::getApp().getMidiDriver().sendNoteOnMessage(keyData->noteNumber, keyData->channelNumber, 60);
	//		}
	//		// ToDo if keyType is "continuous controller": send controller event?
	//	}
	//}
}

void KeyMiniDisplayInsideAllKeysOverview::mouseUp(const MouseEvent& e)
{
	Component::mouseDown(e);

	isHighlighted = false;
	repaint();

	// TODO integrate interaction through LumatoneController

	//// NoteOff MIDI message
	//auto keyData = getKeyData();
	//if (keyData != nullptr && keyData->channelNumber > 0)
	//{
	//	if (keyData->keyType == TerpstraKey::noteOnNoteOff)
	//	{
	//		// Send "note off" event
	//		TerpstraSysExApplication::getApp().getMidiDriver().sendNoteOffMessage(keyData->noteNumber, keyData->channelNumber, 60);
	//	}
	//}
}

void KeyMiniDisplayInsideAllKeysOverview::handleMidiMessage(const MidiMessage& msg)
{
	auto keyData = getKeyData();
	if (keyData != nullptr && msg.getChannel() == keyData->channelNumber && msg.getNoteNumber() == keyData->noteNumber)
	{
		isHighlighted = msg.isNoteOn();
		repaint();
	}
}

const TerpstraKey* KeyMiniDisplayInsideAllKeysOverview::getKeyData() const
{
	if (boardIndex >= 0 && boardIndex < NUMBEROFBOARDS && keyIndex >= 0 && keyIndex < TerpstraSysExApplication::getApp().getOctaveBoardSize())
	{
		jassert(getParentComponent() != nullptr);
		jassert(getParentComponent()->getParentComponent() != nullptr);
		return &dynamic_cast<MainContentComponent*>(getParentComponent()->getParentComponent())
			->getMappingInEdit().sets[boardIndex].theKeys[keyIndex];
	}

	return nullptr;
}

Colour KeyMiniDisplayInsideAllKeysOverview::getKeyColour() const
{
	juce::Colour colour = juce::Colours::transparentBlack;

	auto keyData = getKeyData();
	if (keyData != nullptr)
	{
		auto state = &TerpstraSysExApplication::getApp();
		if (state->useColourModel())
			colour = state->getColourModel()->getModelColour(keyData->colour);
		else
			colour = keyData->colour;

        if (isHighlighted)
            colour = colour.contrasting (.4f);
	}

	return colour;
}

void KeyMiniDisplayInsideAllKeysOverview::setKeyGraphics(Image& colourGraphicIn, Image& shadowGraphicIn)
{
	colourGraphic = colourGraphicIn;
	shadowGraphic = shadowGraphicIn;
}

//[/MiscUserDefs]

//==============================================================================
AllKeysOverview::AllKeysOverview ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("AllKeysOverview");
    btnLoadFile.reset (new juce::TextButton ("btnLoadFile"));
    addAndMakeVisible (btnLoadFile.get());
    btnLoadFile->setButtonText (TRANS("LoadFile"));
    btnLoadFile->addListener (this);

    btnLoadFile->setBounds (368, 8, 96, 24);

    btnSaveFile.reset (new juce::TextButton ("btnSaveFile"));
    addAndMakeVisible (btnSaveFile.get());
    btnSaveFile->setButtonText (TRANS("SaveFile"));
    btnSaveFile->addListener (this);

    btnSaveFile->setBounds (472, 8, 96, 24);

    buttonReceive.reset (new juce::TextButton ("buttonReceive"));
    addAndMakeVisible (buttonReceive.get());
    buttonReceive->setTooltip (TRANS("ImportTooltip"));
    buttonReceive->setButtonText (TRANS("Import from Lumatone"));
    buttonReceive->addListener (this);

    buttonReceive->setBounds (584, 8, 176, 24);

	btnColourModel.reset(new juce::TextButton("btnColourModel"));
	addAndMakeVisible(btnColourModel.get());
	btnColourModel->setTooltip(TRANS("Toggle realistic display colors"));
	btnColourModel->setClickingTogglesState(true);
	btnColourModel->addListener(this);
	btnColourModel->setButtonText("");
	btnColourModel->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, 1.5f);

    //[UserPreSize]
	btnLoadFile->getProperties().set(LumatoneEditorStyleIDs::textButtonIconHashCode, LumatoneEditorIcon::LoadIcon);
	btnSaveFile->getProperties().set(LumatoneEditorStyleIDs::textButtonIconHashCode, LumatoneEditorIcon::SaveIcon);
	buttonReceive->getProperties().set(LumatoneEditorStyleIDs::textButtonIconHashCode, LumatoneEditorIcon::ArrowUp);
	buttonReceive->getProperties().set(LumatoneEditorStyleIDs::textButtonIconPlacement, LumatoneEditorStyleIDs::TextButtonIconPlacement::RightOfText);
	btnColourModel->getProperties().set(LumatoneEditorStyleIDs::textButtonIconHashCode, LumatoneEditorIcon::ColourModelIcon);

	lblFirmwareVersion.reset(new Label("FirmwareVersionLabel"));
	addChildComponent(lblFirmwareVersion.get());

    velocityMeter.reset (new VelocityMeter());
    addAndMakeVisible (velocityMeter.get ());

    btnCalibrate.reset(new juce::TextButton("Calibrate Keys"));
    addChildComponent(btnCalibrate.get());
    btnCalibrate->onClick = [this]() { openCalibrationWindow(); };

	// tilingGeometry.setColumnAngle(LUMATONEGRAPHICCOLUMNANGLE);
	// tilingGeometry.setRowAngle(LUMATONEGRAPHICROWANGLE);

	// lumatoneRenderState = std::make_unique<LumatoneState>(juce::ValueTree("LumatoneEditor"), nullptr);
	lumatoneRender = std::make_unique<LumatoneRender>(TerpstraSysExApplication::getApp().getLumatoneController());

	TerpstraSysExApplication::getApp().getLumatoneController()->addStatusListener(this);
	TerpstraSysExApplication::getApp().getLumatoneController()->addFirmwareListener(this);
    TerpstraSysExApplication::getApp ().getLumatoneController ()->addMidiListener (this);

	resetOctaveSize();

	// Initialize the colour model button state
	btnColourModel->setToggleState(TerpstraSysExApplication::getApp().useColourModel(), juce::dontSendNotification);

    //[/UserPreSize]

    setSize (928, 214);


    //[Constructor] You can add your own custom stuff here..
	currentSetSelection = -1;
    buttonReceive->setVisible(false);
    showDeveloperMode(TerpstraSysExApplication::getApp().getPropertiesFile()->getBoolValue("DeveloperMode", false));
    //[/Constructor]
}

AllKeysOverview::~AllKeysOverview()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    btnLoadFile = nullptr;
    btnSaveFile = nullptr;
    buttonReceive = nullptr;
	btnColourModel = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
	imageProcessor = nullptr;
    //[/Destructor]
}

//==============================================================================
void AllKeysOverview::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..

	g.drawImageAt(lumatoneGraphic, lumatoneBounds.getX(), lumatoneBounds.getY());

    const float bX = (float)lumatoneBounds.getX();
    const float bY = (float)lumatoneBounds.getY();
    const float bW = (float)lumatoneBounds.getWidth();
    const float bH = (float)lumatoneBounds.getHeight();
    const float kW = bW * keyW * lumatoneRender->keyScaleFactor;
    const float kH = bH * keyH * lumatoneRender->keyScaleFactor;

    // Draw key colours at float positions — avoids per-component roundToInt snapping
    if (keyCentres.size() > 0 && keyShapeGraphic.isValid() && keyShadowGraphic.isValid())
    {
        auto* controller  = TerpstraSysExApplication::getApp().getLumatoneController();
        auto* colourModel = TerpstraSysExApplication::getApp().useColourModel()
                                ? TerpstraSysExApplication::getApp().getColourModel()
                                : nullptr;

        const int shapeW  = keyShapeGraphic.getWidth();
        const int shapeH  = keyShapeGraphic.getHeight();
        const int shadowW = keyShadowGraphic.getWidth();
        const int shadowH = keyShadowGraphic.getHeight();

        const int octaveBoardSize = TerpstraSysExApplication::getApp().getOctaveBoardSize();

        for (int i = 0; i < keyCentres.size(); i++)
        {
            juce::Colour keyColour = controller->getKey(i / octaveBoardSize, i % octaveBoardSize).getColour();
            if (colourModel != nullptr)
                keyColour = colourModel->getModelColour(keyColour);

            const float kx = keyCentres[i].x * bW + bX - kW * 0.5f;
            const float ky = keyCentres[i].y * bH + bY - kH * 0.5f;

            if (!keyColour.isTransparent())
            {
                g.setColour(keyColour);
                g.drawImage(keyShapeGraphic, kx, ky, kW, kH, 0, 0, shapeW, shapeH, true);
            }

            g.drawImage(keyShadowGraphic, kx, ky, kW, kH, 0, 0, shadowW, shadowH);
        }
    }

    // Calibration overlay: key centre dots, anchor triangle, anchor markers
    if (lumatoneRender->showCalibrationOverlay && keyCentres.size() > 0)
    {
        // Dot at every key centre
        const float dotR = 1.5f;
        g.setColour(juce::Colours::white.withAlpha(0.8f));
        for (int i = 0; i < keyCentres.size(); i++)
        {
            const float px = keyCentres[i].x * bW + bX;
            const float py = keyCentres[i].y * bH + bY;
            g.fillEllipse(px - dotR, py - dotR, dotR * 2.0f, dotR * 2.0f);
        }

        auto& c = lumatoneRender->calibration;

        // Pixel positions of the 3 anchors
        const float ax1 = c.oct1Key1X  * bW + bX,  ay1 = c.oct1Key1Y  * bH + bY;
        const float ax2 = c.oct1Key56X * bW + bX,  ay2 = c.oct1Key56Y * bH + bY;
        const float ax3 = c.oct5Key7X  * bW + bX,  ay3 = c.oct5Key7Y  * bH + bY;

        // Triangle connecting the 3 anchor points
        g.setColour(juce::Colours::yellow.withAlpha(0.6f));
        g.drawLine(ax1, ay1, ax2, ay2, 1.0f);
        g.drawLine(ax2, ay2, ax3, ay3, 1.0f);
        g.drawLine(ax3, ay3, ax1, ay1, 1.0f);

        // Crosshair + circle at each anchor
        auto drawAnchor = [&](float px, float py, juce::Colour colour)
        {
            const float radius = 5.0f;
            const float arm    = 9.0f;
            g.setColour(colour);
            g.drawLine(px - arm, py, px + arm, py, 1.5f);
            g.drawLine(px, py - arm, px, py + arm, 1.5f);
            g.drawEllipse(px - radius, py - radius, radius * 2.0f, radius * 2.0f, 1.5f);
        };

        drawAnchor(ax1, ay1, juce::Colours::red);
        drawAnchor(ax2, ay2, juce::Colours::limegreen);
        drawAnchor(ax3, ay3, juce::Colours::cyan);
    }

	// Draw a line under the selected sub board
	if (currentSetSelection >= 0 && currentSetSelection < NUMBEROFBOARDS)
	{
		Path selectionMarkPath;
		selectionMarkPath.startNewSubPath(octaveBoards[currentSetSelection]->leftPos, octaveLineY);
		selectionMarkPath.lineTo(octaveBoards[currentSetSelection]->rightPos, octaveLineY);

		Colour lineColour = findColour(TerpstraKeyEdit::outlineColourId);
		g.setColour(lineColour);
		g.strokePath(selectionMarkPath, PathStrokeType(1.0f));
	}


    //[/UserPaint]
}

void AllKeysOverview::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

	// Prepare position helpers for graphics
	int graphicHeight = roundToInt(getHeight() * imageHeight);
	int graphicWidth = roundToInt(imageAspect * graphicHeight);

	lumatoneBounds.setBounds(
		roundToInt((getWidth() - graphicWidth) / 2.0f), roundToInt(getHeight() * imageY),
		graphicWidth, graphicHeight
	);

	int btnHeight = roundToInt(getHeight() * saveLoadH);
	int btnMargin = roundToInt(getWidth() * saveloadMarginW);
	int saveLoadWidth = roundToInt(getWidth() * saveLoadW);
	int btnY = lumatoneBounds.getY() - roundToInt(getHeight() * btnYFromImageTop);

	int halfWidthX = roundToInt(getWidth() * 0.5f);

	btnLoadFile->setBounds(halfWidthX - btnMargin - saveLoadWidth, btnY, saveLoadWidth, btnHeight);
	btnSaveFile->setBounds(halfWidthX + btnMargin, btnY, saveLoadWidth, btnHeight);

	octaveLineY = lumatoneBounds.getBottom() + roundToInt(getHeight() * octaveLineYRatio);

	int importY = lumatoneBounds.getY() - roundToInt(getHeight() * importYFromImageTop);
	int importWidth = roundToInt(getWidth() * importW);

	// Colour model toggle: above graphics, right-aligned with the graphic edge
	btnColourModel->setBounds(lumatoneBounds.getRight() - btnHeight, importY, btnHeight, btnHeight);

	// Import button: to the left of the colour model toggle
	buttonReceive->setBounds(lumatoneBounds.getRight() - btnHeight - btnMargin - importWidth, importY, importWidth, btnHeight);

    // Calibrate button sits next to the colour model button (only visible in developer mode)
    btnCalibrate->setBounds(lumatoneBounds.getX() + btnHeight + btnMargin, lumatoneBounds.getY() - btnHeight * 1.08f, saveLoadWidth, btnHeight);

	resizeLabelWithHeight(lblFirmwareVersion.get(), btnHeight * 0.6f);
	lblFirmwareVersion->setTopLeftPosition(lumatoneBounds.getX(), lumatoneBounds.getY() - btnHeight * 0.6f);

    const auto velocityMeterW = 25;
    velocityMeter->setBounds ((lumatoneBounds.getX() - velocityMeterW) / 2, lumatoneBounds.getY (), velocityMeterW, lumatoneBounds.getHeight ());

	int keyWidth  = roundToInt(lumatoneBounds.getWidth()  * keyW * lumatoneRender->keyScaleFactor);
	int keyHeight = roundToInt(lumatoneBounds.getHeight() * keyH * lumatoneRender->keyScaleFactor);

	// Scale key graphics once
	// lumatoneGraphic = imageProcessor->resizeImage(ImageCache::getFromHashCode(LumatoneEditorAssets::LumatoneGraphic), lumatoneBounds.getWidth(), lumatoneBounds.getHeight());
	// keyShapeGraphic = imageProcessor->resizeImage(ImageCache::getFromHashCode(LumatoneEditorAssets::KeyShape), keyWidth, keyHeight);
	// keyShadowGraphic = imageProcessor->resizeImage(ImageCache::getFromHashCode(LumatoneEditorAssets::KeyShadow), keyWidth, keyHeight);

	lumatoneGraphic = lumatoneRender->getResizedAsset(LumatoneAssets::ID::LumatoneGraphic, lumatoneBounds.getWidth(), lumatoneBounds.getHeight());
	keyShapeGraphic = lumatoneRender->getResizedAsset(LumatoneAssets::ID::KeyShape, keyWidth, keyHeight);
    keyShadowGraphic = lumatoneRender->getResizedAsset(LumatoneAssets::ID::KeyShadow, keyWidth, keyHeight, true);

	// oct1Key1 = Point<float>(oct1Key1X * lumatoneBounds.getWidth() + lumatoneBounds.getX(), oct1Key1Y * lumatoneBounds.getHeight() + lumatoneBounds.getY());
	// oct1Key56 = Point<float>(oct1Key56X * lumatoneBounds.getWidth() + lumatoneBounds.getX(), oct1Key56Y * lumatoneBounds.getHeight() + lumatoneBounds.getY());
	// oct5Key7 = Point<float>(oct5Key7X * lumatoneBounds.getWidth() + lumatoneBounds.getX(), oct5Key7Y * lumatoneBounds.getHeight() + lumatoneBounds.getY());

	// tilingGeometry.fitSkewedTiling(oct1Key1, oct1Key56, 10, oct5Key7, 24, false);

	// keyCentres = tilingGeometry.getHexagonCentresSkewed(boardGeometry, 0, NUMBEROFBOARDS);
	// jassert(keyCentres.size() == TerpstraSysExApplication::getApp().getOctaveBoardSize() * NUMBEROFBOARDS);

	int octaveIndex = 0;
    int octaveX = keyCentres[0].getX() * lumatoneBounds.getWidth() + lumatoneBounds.getX();
	octaveBoards[octaveIndex]->leftPos = octaveX - keyWidth * 0.5;

	const int octaveBoardSize = TerpstraSysExApplication::getApp().getOctaveBoardSize();
	const int numBoards = TerpstraSysExApplication::getApp().getNumBoards();

	for (int keyIndex = 0; keyIndex < keyCentres.size(); keyIndex++)
	{
		int keyOctaveIndex = keyIndex % octaveBoardSize;

        juce::Point<int> keyPos = juce::Point<int>(
            juce::roundToInt(keyCentres[keyIndex].x * lumatoneBounds.getWidth() + lumatoneBounds.getX() - keyWidth * 0.5f),
            juce::roundToInt(keyCentres[keyIndex].y * lumatoneBounds.getHeight() + lumatoneBounds.getY() - keyHeight * 0.5f)
        );

        auto key = octaveBoards[octaveIndex]->keyMiniDisplay[keyOctaveIndex];
        key->setKeyGraphics(keyShapeGraphic, keyShadowGraphic);
        key->setBounds(keyPos.x, keyPos.y, keyWidth, keyHeight);

        if (keyOctaveIndex + 1 == octaveBoardSize)
        {
            octaveBoards[octaveIndex]->rightPos = key->getRight();
            octaveIndex++;

            if (octaveIndex < numBoards)
                octaveBoards[octaveIndex]->leftPos = key->getX();
        }
	}

    //[/UserResized]
}

void AllKeysOverview::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnLoadFile.get())
    {
        //[UserButtonCode_btnLoadFile] -- add your button handler code here..
		TerpstraSysExApplication::getApp().openSysExMapping();
        //[/UserButtonCode_btnLoadFile]
    }
    else if (buttonThatWasClicked == btnSaveFile.get())
    {
        //[UserButtonCode_btnSaveFile] -- add your button handler code here..
		TerpstraSysExApplication::getApp().saveSysExMappingAs();
        //[/UserButtonCode_btnSaveFile]
    }
    else if (buttonThatWasClicked == buttonReceive.get())
    {
        //[UserButtonCode_buttonReceive] -- add your button handler code here..
		TerpstraSysExApplication::getApp().requestConfigurationFromDevice();
        //[/UserButtonCode_buttonReceive]
    }
	else if (buttonThatWasClicked == btnColourModel.get())
	{
		//[UserButtonCode_btnColourModel] -- add your button handler code here..
		TerpstraSysExApplication::getApp().toggleUseColourModel();
		//[/UserButtonCode_btnColourModel]
	}

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void AllKeysOverview::setFirmwareVersion(LumatoneFirmware::Version versionIn)
{
	if (versionIn.isValid())
	{
		if (versionIn.major == 0 && versionIn.minor == 0)
		{
			if (versionIn.revision == 55)
			{
				lblFirmwareVersion->setText("55-keys Prototype", NotificationType::dontSendNotification);
			}
		}
		else
		{
#if JUCE_DEBUG
			lblFirmwareVersion->setText("Firmware version: " + versionIn.toString(), NotificationType::dontSendNotification);
#else
			lblFirmwareVersion->setText("Firmware version: " + versionIn.toDisplayString(), NotificationType::dontSendNotification);
#endif
		}

		lblFirmwareVersion->setVisible(true);
		resized();
	}
    else
    {
        lblFirmwareVersion->setVisible(false);
    }

    repaint();
}

void AllKeysOverview::showDeveloperMode(bool developerModeOn)
{
	if (developerModeOn)
		buttonReceive->setVisible(true);

    btnCalibrate->setVisible(developerModeOn);

    repaint();
}

struct CalibrationDocWindow : public juce::DocumentWindow
{
    CalibrationDocWindow(const juce::String& title, juce::Colour bg, std::function<void()> onCloseCb)
        : juce::DocumentWindow(title, bg, juce::DocumentWindow::closeButton)
        , onClose(std::move(onCloseCb)) {}

    void closeButtonPressed() override { onClose(); }

    std::function<void()> onClose;
};

// Combined container: drag view on the left, nudge panel on the right.
// Both share the same LumatoneRender and are kept in sync via a shared callback.
struct CalibrationEditorComponent : public juce::Component
{
    CalibrationEditorComponent(LumatoneRender* render,
                               std::function<void()> overviewCallback)
    {
        // Shared callback: syncs drag view display, panel readouts, and AllKeysOverview
        auto sharedCb = [this, overviewCallback]()
        {
            dragView->refresh();
            panel->refreshValueLabels();
            if (overviewCallback) overviewCallback();
        };

        panel    = std::make_unique<KeyCalibrationPanel>(render, sharedCb);
        dragView = std::make_unique<DebugCalibrationDragView>(
            render,
            TerpstraSysExApplication::getApp().getLumatoneController(),
            sharedCb);

        addAndMakeVisible(*dragView);
        addAndMakeVisible(*panel);

        panel->setKeyScaleCallbacks(
            [this]()        { return dragView->getKeyScaleFactor(); },
            [this, overviewCallback](float f) {
                dragView->setKeyScaleFactor(f);
                if (overviewCallback) overviewCallback();  // rebuild main view key graphics
            }
        );
    }

    void resized() override
    {
        const int panelW = KeyCalibrationPanel::preferredWidth() + 8;
        panel->setBounds(getWidth() - panelW, 0, panelW, getHeight());
        dragView->setBounds(0, 0, getWidth() - panelW, getHeight());
    }

    std::unique_ptr<DebugCalibrationDragView>  dragView;
    std::unique_ptr<KeyCalibrationPanel>  panel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CalibrationEditorComponent)
};

void AllKeysOverview::openCalibrationWindow()
{
    if (calibrationWindow != nullptr)
    {
        calibrationWindow->toFront(true);
        return;
    }

    auto* editor = new CalibrationEditorComponent(
        lumatoneRender.get(),
        [this]()
        {
            keyCentres = lumatoneRender->getKeyCentres();
            resized();
            repaint();
        }
    );

    // Default: drag view ~2x the original image width (≈1060) + panel sidebar
    const int panelW    = KeyCalibrationPanel::preferredWidth() + 8;
    const int contentW  = 1060 + panelW;   // ~1340
    const int contentH  = 430;
    editor->setSize(contentW, contentH);

    calibrationWindow = std::make_unique<CalibrationDocWindow>(
        "Key Calibration",
        juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId),
        [this]() { calibrationWindow.reset(); }
    );
    calibrationWindow->setContentOwned(editor, true);
    calibrationWindow->setResizable(true, false);
    calibrationWindow->setUsingNativeTitleBar(true);
    calibrationWindow->centreWithSize(contentW, contentH);
    calibrationWindow->setVisible(true);
}

void AllKeysOverview::connectionEstablished(int, int)
{
	buttonReceive->setVisible(true);
}

void AllKeysOverview::connectionLost()
{
	buttonReceive->setVisible(false);
}

void AllKeysOverview::firmwareRevisionReceived(LumatoneFirmware::Version version)
{
	setFirmwareVersion(version);
}

void AllKeysOverview::handleMidiMessage (const MidiMessage& msg)
{
	if (msg.isNoteOn())
    	velocityMeter->setVelocity (msg.getVelocity());
}

void AllKeysOverview::resetOctaveSize()
{
	int octaveSize = TerpstraSysExApplication::getApp().getOctaveBoardSize();
	if (currentOctaveSize != octaveSize)
	{
		boardGeometry = TerpstraBoardGeometry();
		octaveBoards.clear();

		for (int subBoardIndex = 0; subBoardIndex < NUMBEROFBOARDS; subBoardIndex++)
		{
			OctaveBoard* board = octaveBoards.add(new OctaveBoard());

			for (int keyIndex = 0; keyIndex < octaveSize; keyIndex++)
			{
				auto key = board->keyMiniDisplay.add(new KeyMiniDisplayInsideAllKeysOverview(subBoardIndex, keyIndex));
				addAndMakeVisible(key);
			}

			jassert(board->keyMiniDisplay.size() == octaveSize);
		}

		currentOctaveSize = octaveSize;
	}

	lumatoneRender->resetOctaveSize();
    keyCentres = lumatoneRender->getKeyCentres();

	jassert(octaveBoards.size() == NUMBEROFBOARDS);
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AllKeysOverview" componentName="AllKeysOverview"
                 parentClasses="public juce::Component, public LumatoneEditor::StatusListener, public LumatoneEditor::FirmwareListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="928"
                 initialHeight="214">
  <BACKGROUND backgroundColour="0"/>
  <TEXTBUTTON name="btnLoadFile" id="6c0c074c9f137f23" memberName="btnLoadFile"
              virtualName="" explicitFocusOrder="0" pos="368 8 96 24" buttonText="Load File"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="btnSaveFile" id="abbc33d699ba1e52" memberName="btnSaveFile"
              virtualName="" explicitFocusOrder="0" pos="472 8 96 24" buttonText="Save File"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="buttonReceive" id="6a7ed19ee86a3b97" memberName="buttonReceive"
              virtualName="" explicitFocusOrder="0" pos="584 8 176 24" tooltip="Receive current layout from connected Lumatone"
              buttonText="Import from Lumatone" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

