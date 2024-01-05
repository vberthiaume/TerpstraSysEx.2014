/*
  ==============================================================================

  MainComponent.cpp
  Created: XXX.2014
  Author:  hsstraub

  ==============================================================================
*/

#include "MainComponent.h"

#include "LumatoneEditorLookAndFeel.h"

#include "LumatoneMenu.h"

#include "MidiEditArea.h"
#include "NoteEditArea.h"
#include "IsomorphicMassAssign.h"

#include "GeneralOptionsDlg.h"
#include "CurvesArea.h"
#include "GlobalSettingsArea.h"
#include "PedalSensitivityDlg.h"

#include "./lumatone_editor_library/palettes/colour_palette_window.h"
#include "./lumatone_editor_library/palettes/colour_edit_textbox.h"
#include "./lumatone_editor_library/ui/keyboard_component.h"
#include "./lumatone_editor_library/graphics/view_constants.h"

#include "./lumatone_editor_library/actions/edit_actions.h"


//==============================================================================
MainContentComponent::MainContentComponent(const LumatoneEditorState& stateIn)
	: LumatoneEditorState("MainComponent", stateIn)
	, copiedSubBoardData(std::make_unique<LumatoneBoard>())
{
	setName("MainContentComponent");

	// Midi input + output
	midiEditArea.reset(new MidiEditArea(stateIn));
	addAndMakeVisible(midiEditArea.get());

	// All keys overview
	allKeysOverview.reset(new LumatoneKeyboardComponent(*this));
	addAndMakeVisible(allKeysOverview.get());

	// Edit function area
	noteEditArea.reset(new NoteEditArea(stateIn));
	addAndMakeVisible(noteEditArea.get());
	noteEditArea->getOctaveBoardSelectorTab()->addChangeListener(this);
	noteEditArea->getColourViewComponent()->addListener(this); // Open up ColourPaletteWindow

	generalOptionsArea.reset(new GeneralOptionsDlg(stateIn));
	addAndMakeVisible(generalOptionsArea.get());

	pedalSensitivityDlg.reset(new PedalSensitivityDlg(stateIn));
	addAndMakeVisible(pedalSensitivityDlg.get());

	curvesArea.reset(new CurvesArea(stateIn));
	addAndMakeVisible(curvesArea.get());

	globalSettingsArea.reset(new GlobalSettingsArea(stateIn));
	addAndMakeVisible(globalSettingsArea.get());
	globalSettingsArea->listenToColourEditButtons(this);

	// getLumatoneController()->addFirmwareListener(this);

	//lblAppName.reset(new Label("lblAppName", TerpstraSysExApplication::getApp().getApplicationName()));
	lblAppName.reset(new Label("lblAppName", "lumatone editor"));
	lblAppName->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
	lblAppName->setColour(Label::ColourIds::textColourId, Colour(0xff777777));
	addAndMakeVisible(lblAppName.get());

	lblAppVersion.reset(new Label("lblAppVersion", "v" + getApplicationVersion()));
	lblAppVersion->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
	lblAppVersion->setColour(Label::ColourIds::textColourId, Colour(0xff777777));
	addAndMakeVisible(lblAppVersion.get());

	// Initial size
	setSize(DEFAULTMAINWINDOWWIDTH, DEFAULTMAINWINDOWHEIGHT);

	// Select first board and first key
	noteEditArea->getOctaveBoardSelectorTab()->setCurrentTabIndex(0, true);
	// The above call is supposed to update changeListener - but apparently doesn't... Call it manually then. XXX
	changeListenerCallback(noteEditArea->getOctaveBoardSelectorTab());
    noteEditArea->changeSingleKeySelection(0);

    // Initialize mapping structure
    deleteAll();
}

MainContentComponent::~MainContentComponent()
{
    //TerpstraSysExApplication::getApp().getMidiDriver().removeListener(this);

	midiEditArea = nullptr;
	allKeysOverview = nullptr;
	noteEditArea = nullptr;

	generalOptionsArea = nullptr;
	curvesArea = nullptr;
	globalSettingsArea = nullptr;
	pedalSensitivityDlg = nullptr;

	lblAppName = nullptr;
	lblAppVersion = nullptr;
}

void MainContentComponent::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	noteEditArea->restoreStateFromPropertiesFile(propertiesFile);
}

void MainContentComponent::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	noteEditArea->saveStateToPropertiesFile(propertiesFile);
	globalSettingsArea->saveStateToPropertiesFile(propertiesFile);
}

// Set the current mapping to be edited to the value passed in parameter
void MainContentComponent::setData(LumatoneLayout& newData, bool withRefresh)
{
	*mappingData = newData;

	noteEditArea->onSetData(newData);

	if (withRefresh)
	{
		refreshAllFields();
	}
}

void MainContentComponent::deleteAll(bool withRefresh)
{
	LumatoneLayout keyMapping;
	setData(keyMapping, withRefresh);
}

// Copy the edited mapping to the variable passed as parameter
void MainContentComponent::getData(LumatoneLayout& newData)
{
	newData = *getMappingData();
}

TabbedButtonBar *MainContentComponent::getOctaveBoardSelectorTab()
{
	return  noteEditArea->getOctaveBoardSelectorTab();
}

UndoableAction* MainContentComponent::createDeleteCurrentSectionAction()
{
	auto currentSetSelection = noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex();
	if (currentSetSelection >= 0 && currentSetSelection < getOctaveBoardSize())
	{
        auto keySet = LumatoneBoard();
        return new LumatoneEditAction::SectionEditAction(this, currentSetSelection, keySet);
	}
	else
		return nullptr;
}

bool MainContentComponent::copyCurrentSubBoardData()
{
	auto currentSetSelection = noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex();
	if (currentSetSelection >= 0 && currentSetSelection < getOctaveBoardSize())
	{
		*copiedSubBoardData = getBoard(currentSetSelection);
		return true;
	}
	else
		return false;
}

UndoableAction* MainContentComponent::createPasteCurrentSectionAction()
{
	auto currentSetSelection = noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex();
	if (currentSetSelection >= 0 && currentSetSelection < getNumBoards()
		&& !copiedSubBoardData->isEmpty())
	{
		return new LumatoneEditAction::SectionEditAction(this, currentSetSelection, *copiedSubBoardData);
	}
	else
		return nullptr;
}

UndoableAction* MainContentComponent::createModifiedPasteCurrentSectionAction(CommandID commandID)
{
    auto currentSetSelectionIndex = noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex();
    if (currentSetSelectionIndex >= 0 && currentSetSelectionIndex < getNumBoards()
        && !copiedSubBoardData->isEmpty())
    {
        auto modifiedSubBoardData = *copiedSubBoardData;
        auto octaveSize = getOctaveBoardSize();

        for (int i = 0; i < octaveSize; i++)
        {
            // LumatoneKey currentSectionKey = getKey(currentSetSelectionIndex, i);
            // LumatoneKey modifiedKey = modifiedSubBoardData.getKey(i);
            LumatoneKey modifiedKey = getKey(currentSetSelectionIndex, i);

            switch (commandID)
            {
            case Lumatone::Menu::commandIDs::pasteOctaveBoardNotes:
                modifiedKey.setNoteOrCC(modifiedKey.getMidiNumber());
                break;

            case Lumatone::Menu::commandIDs::pasteOctaveBoardChannels:
                modifiedKey.setChannelNumber(modifiedKey.getMidiChannel());
                break;

            case Lumatone::Menu::commandIDs::pasteOctaveBoardColours:
                modifiedKey.setColour(modifiedKey.getColour());
                break;

            case Lumatone::Menu::commandIDs::pasteOctaveBoardTypes:
                modifiedKey.setKeyType(modifiedKey.getType());
				modifiedKey.setDefaultCCFader(modifiedKey.isCCFaderDefault());
                break;

            default:
                jassertfalse;
				modifiedKey = modifiedSubBoardData.getKey(i);
				break;
            }

            modifiedSubBoardData.setKey(modifiedKey, i);
        }

        return new LumatoneEditAction::SectionEditAction(this, currentSetSelectionIndex, modifiedSubBoardData);
    }
    else
        return nullptr;
}

bool MainContentComponent::canPasteCopiedSubBoard() const
{
    return !copiedSubBoardData->isEmpty();
}

bool MainContentComponent::setDeveloperMode(bool developerModeOn)
{
	curvesArea->setDeveloperMode(developerModeOn);
    globalSettingsArea->setDeveloperMode(developerModeOn);
    // allKeysOverview->showDeveloperMode(developerModeOn);
	return true;
}

void MainContentComponent::octaveColourConfigReceived(int octaveIndex, uint8 rgbFlag, const int* colourData)
{
	for (int keyIndex = 0; keyIndex < getOctaveBoardSize(); keyIndex++)
	{
		// LumatoneKey& keyData = getBoard(octaveIndex - 1)->theKeys[keyIndex];
		// auto newValue = colourData[keyIndex];

		// if (rgbFlag == 0)
		// {
		// 	keyData.colour = Colour(newValue, keyData.colour.getGreen(), keyData.colour.getBlue());
		// }
		// else if (rgbFlag == 1)
		// {
		// 	keyData.colour = Colour(keyData.colour.getRed(), newValue, keyData.colour.getBlue());
		// }
		// else if (rgbFlag == 2)
		// {
		// 	keyData.colour = Colour(keyData.colour.getRed(), keyData.colour.getGreen(), newValue);
		// }
		// else
		// 	jassertfalse;
	}

	refreshKeyDataFields();
}

void MainContentComponent::octaveChannelConfigReceived(int octaveIndex, const int* channelData)
{
	for (int keyIndex = 0; keyIndex < getOctaveBoardSize(); keyIndex++)
	{
		// Check channel values?
		// getBoard(octaveIndex - 1)->theKeys[keyIndex].channelNumber = channelData[keyIndex];
	}

	refreshKeyDataFields();
}

void MainContentComponent::octaveNoteConfigReceived(int octaveIndex, const int* noteData)
{
	for (int keyIndex = 0; keyIndex < getOctaveBoardSize(); keyIndex++)
	{
		// Check note values?
		// getBoard(octaveIndex - 1)->theKeys[keyIndex].noteNumber = noteData[keyIndex];
	}

	refreshKeyDataFields();
}

void MainContentComponent::keyTypeConfigReceived(int octaveIndex, const int* keyTypeData)
{
	for (int keyIndex = 0; keyIndex < getOctaveBoardSize(); keyIndex++)
	{
		// Check type values?
		// getBoard(octaveIndex - 1)->theKeys[keyIndex].keyType = LumatoneKeyType(keyTypeData[keyIndex]);
	}

	refreshKeyDataFields();
}

void MainContentComponent::velocityConfigReceived(const int* velocityData)
{
	// mappingData.velocityTable.editStrategy = LumatoneConfigTable::DrawMode::freeDrawing;
	// for (int i = 0; i < 128; i++)
	// 	mappingData.velocityTable.velocityValues[i] = velocityData[127 - i]; // Reversed
	curvesArea->loadFromMapping();
}

void MainContentComponent::aftertouchConfigReceived(const int* aftertouch)
{
	// mappingData.afterTouchTable.editStrategy = LumatoneConfigTable::DrawMode::freeDrawing;
	// memmove(mappingData.afterTouchTable.velocityValues, aftertouch, sizeof(int) * 128);
	curvesArea->loadFromMapping();
}

void MainContentComponent::velocityIntervalConfigReceived(const int* velocityData)
{
	// memmove(mappingData.table, velocityData, sizeof(int) * VELOCITYINTERVALTABLESIZE);
	curvesArea->loadFromMapping();
}

void MainContentComponent::faderConfigReceived(const int* faderData)
{
	// mappingData.faderTable.editStrategy = LumatoneConfigTable::DrawMode::freeDrawing;
	// memmove(mappingData.faderTable.velocityValues, faderData, sizeof(int) * 128);
	curvesArea->loadFromMapping();
}

void MainContentComponent::faderTypeConfigReceived(int octaveIndex, const int* faderTypeData)
{
	for (int keyIndex = 0; keyIndex < getOctaveBoardSize(); keyIndex++)
	{
		// getBoard(octaveIndex - 1)->theKeys[keyIndex].ccFaderDefault = faderTypeData[keyIndex];
	}
}

void MainContentComponent::lumatouchConfigReceived(const int* lumatouchData)
{
	// mappingData.lumaTouchTable.editStrategy = LumatoneConfigTable::DrawMode::freeDrawing;
	// memmove(mappingData.lumaTouchTable.velocityValues, lumatouchData, sizeof(int) * 128);
	curvesArea->loadFromMapping();
}

void MainContentComponent::firmwareRevisionReceived(LumatoneFirmware::Version version)
{
	// Make sure changes happen in proper order
	noteEditArea->resetOctaveSize();
	allKeysOverview->resetOctaveSize();
}

void MainContentComponent::changeListenerCallback(ChangeBroadcaster *source)
{
	if (source == noteEditArea->getOctaveBoardSelectorTab())
	{
		// allKeysOverview->setCurrentSetSelection(noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex());
	}
}

void MainContentComponent::buttonClicked(Button* btn)
{
	ColourViewComponent* colourEdit = dynamic_cast<ColourViewComponent*>(btn);

	if (colourEdit)
	{
		// May be better asynchronous on a timer
		// TerpstraSysExApplication::getApp().reloadColourPalettes();

		auto palettes = getColourPalettes();
		ColourPaletteWindow* paletteWindow = new ColourPaletteWindow(palettes);
		paletteWindow->setSize(proportionOfWidth(popupWidth), proportionOfHeight(popupHeight));

        if (btn == noteEditArea->getColourViewComponent())
        {
            colourEdit = noteEditArea->getColourViewComponent();

			auto colourTextEditor = noteEditArea->getSingleNoteColourTextEditor();
            paletteWindow->listenToColourSelection(static_cast<ColourSelectionListener*>(colourTextEditor));

            // Shouldn't be necessary when Isomorphic is moved from dev to public
            auto isomorphicPanel = noteEditArea->getIsomorphicMassAssignPanel();
            if (isomorphicPanel != nullptr)
                paletteWindow->listenToColourSelection(static_cast<ColourSelectionListener*>(isomorphicPanel));

			paletteWindow->addColourSelectorToGroup(noteEditArea.get());
			paletteWindow->setCurrentColourSelector(noteEditArea->getSingleNoteColourTextEditor());
        }

		Rectangle<int> componentArea = colourEdit->getScreenBounds().translated(-getScreenX(), -getScreenY());

		CallOutBox::launchAsynchronously(
			std::unique_ptr<Component>(paletteWindow),
			componentArea,
			this
		);

		// else, a preset button colour button was pressed
		paletteWindow->listenToColourSelection(colourEdit);
		// TODO: Set swatch # or custom colour as current colour
	}
}

void MainContentComponent::paint (Graphics& g)
{
	g.fillAll(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::MediumBackground));
	
	g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LightBackground));
	g.fillRect(controlsArea);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	int newWidth = getWidth();
	int newHeight = getHeight();

	// Logo, MIDI edit area and connection state
	int midiAreaHeight = proportionOfHeight(headerHeight);
	midiEditArea->setBounds(0, 0, getWidth(), midiAreaHeight);

	// Bounds for controls, where background is darker
	int footerY = proportionOfHeight(footerAreaY);
	int footerHeight = getHeight() - footerY;
	controlsArea = getBounds().withTop(proportionOfHeight(controlsAreaY)).withBottom(footerY);

	// All keys overview/virtual keyboard playing
	int newKeysOverviewAreaHeight = jmax(controlsArea.getY() - midiAreaHeight, MINIMALTERPSTRAKEYSETAREAHEIGHT);
	int keyboardMarginTop = newKeysOverviewAreaHeight * 0.0625f;
	int keyboardHeight = newKeysOverviewAreaHeight * 0.8f;
	allKeysOverview->setBounds(0, midiAreaHeight + keyboardMarginTop, newWidth, keyboardHeight);

	// Edit function/single key field area
	noteEditArea->setSize(proportionOfWidth(assignWidth), proportionOfHeight(assignHeight));
	noteEditArea->setControlsTopLeftPosition(proportionOfWidth(assignMarginX), controlsArea.getY());

	generalOptionsArea->setBounds(getLocalBounds().toFloat().getProportion(generalSettingsBounds).toNearestInt());
	pedalSensitivityDlg->setBounds(getLocalBounds().toFloat().getProportion(pedalSettingsBounds).toNearestInt());

	curvesArea->setBounds(getLocalBounds().toFloat().getProportion(curvesAreaBounds).toNearestInt());

	globalSettingsArea->setBounds(getLocalBounds()
		.withTop(roundToInt(getHeight() * footerAreaY))
		.withTrimmedRight(footerHeight)
	);

	resizeLabelWithHeight(lblAppName.get(), roundToInt(footerHeight * lumatoneVersionHeight), 1.0f, " ");
	lblAppName->setTopLeftPosition(proportionOfWidth(lumatoneVersionMarginX), footerY + (footerHeight - lblAppName->getHeight()) * 0.5f);

	resizeLabelWithHeight(lblAppVersion.get(), roundToInt(lblAppName->getHeight() * 0.75f));
	lblAppVersion->setTopLeftPosition(lblAppName->getRight(), lblAppName->getBottom() - lblAppVersion->getHeight());
}

void MainContentComponent::refreshKeyDataFields()
{
	noteEditArea->refreshKeyFields();
	// allKeysOverview->mappingUpdateCallback();
	// juce::Timer::callAfterDelay(1, [&]() { allKeysOverview->refreshMappingData(); });
}

void MainContentComponent::refreshAllFields()
{
	refreshKeyDataFields();
	generalOptionsArea->loadFromMapping();
	pedalSensitivityDlg->loadFromMapping();
	curvesArea->loadFromMapping();
	curvesArea->repaint();
}
