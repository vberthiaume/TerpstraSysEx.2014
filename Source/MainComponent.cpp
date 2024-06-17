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

#include "mapping_editors/KeySelectionController.h"
#include "mapping_editors/KeyEditorPanel.h"
#include "mapping_editors/MappingSettingsPanel.h"
#include "GlobalSettingsArea.h"

#include "./ColourPaletteWindow.h"

#include "./lumatone_editor_library/palettes/colour_edit_textbox.h"
#include "./lumatone_editor_library/ui/keyboard_component.h"
#include "./lumatone_editor_library/graphics/view_constants.h"

#include "./lumatone_editor_library/actions/edit_actions.h"


//==============================================================================
MainContentComponent::MainContentComponent(const LumatoneEditorState& stateIn, juce::ApplicationCommandManager* commandManager)
	: LumatoneEditorState("MainComponent", stateIn)
	, copiedSubBoardData(std::make_unique<LumatoneBoard>())
{
	setName("MainContentComponent");

	// Midi input + output
	midiEditArea.reset(new MidiEditArea(stateIn));
	addAndMakeVisible(midiEditArea.get());

	// All keys overview
	allKeysOverview.reset(new LumatoneKeyboardComponent(*this));
	allKeysOverview->setUiMode(LumatoneKeyboardComponent::UiMode::Controller);
	addAndMakeVisible(allKeysOverview.get());

	// Listens to key selection and creates selection edit actions
	keySelectionController = std::make_unique<KeySelectionController>(*this, allKeysOverview.get());

	// Edit function area
	// noteEditArea.reset(new NoteEditArea(stateIn));
	// noteEditArea->getOctaveBoardSelectorTab()->addChangeListener(this);
	// noteEditArea->getColourViewComponent()->addListener(this); // Open up ColourPaletteWindow

	keyEditorPanel = std::make_unique<KeyEditorPanel>(stateIn);
	mappingSettingsComponent = std::make_unique<MappingSettingsPanel>(stateIn);

	// curvesArea.reset(new CurvesArea(stateIn));
	// addAndMakeVisible(curvesArea.get());

	globalSettingsArea.reset(new GlobalSettingsArea(stateIn));
	addAndMakeVisible(globalSettingsArea.get());
	globalSettingsArea->listenToColourEditButtons(this);

	lblEditTitle.reset(new Label("lblEditTitle", "Edit Mapping"));
	lblEditTitle->setFont(getAppFonts().getFont(LumatoneEditorFont::UniviaProBold));
	lblEditTitle->setColour(Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LabelBlue));
	lblEditTitle->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));
	addAndMakeVisible(lblEditTitle.get());

	sectionTabs = std::make_unique<juce::TabbedComponent>(juce::TabbedButtonBar::TabsAtTop);
	sectionTabs->setColour(juce::TabbedComponent::ColourIds::outlineColourId, juce::Colours::transparentBlack);
	addAndMakeVisible(*sectionTabs);
	sectionTabs->addTab("Key Editor", juce::Colour(), keyEditorPanel.get(), false);
	sectionTabs->addTab("AutoGenerator", juce::Colour(), mappingSettingsComponent.get(), false);
	sectionTabs->addTab("Advanced", juce::Colour(), mappingSettingsComponent.get(), false);
	sectionTabs->addTab("Mapping Settings", juce::Colour(), mappingSettingsComponent.get(), false);

	sectionTabNames = sectionTabs->getTabNames();
	sectionTabBar = &sectionTabs->getTabbedButtonBar();
	sectionTabBar->addChangeListener(this);

	btnLoadFile.reset(new juce::TextButton("btnLoadFile"));
	addAndMakeVisible(btnLoadFile.get());
	btnLoadFile->setButtonText(juce::translate("LoadFile"));
	btnLoadFile->setCommandToTrigger(commandManager, Lumatone::Menu::openSysExMapping, true);

	btnSaveFile.reset(new juce::TextButton("btnSaveFile"));
	addAndMakeVisible(btnSaveFile.get());
	btnSaveFile->setButtonText(juce::translate("SaveFile"));
	btnSaveFile->setCommandToTrigger(commandManager, Lumatone::Menu::saveSysExMappingAs, true);

	btnImportFile.reset(new juce::TextButton("buttonReceive"));
	addAndMakeVisible(btnImportFile.get());
	btnImportFile->setTooltip(juce::translate("ImportTooltip"));
	btnImportFile->setButtonText(juce::translate("Import from Lumatone"));
	btnImportFile->setCommandToTrigger(commandManager, Lumatone::Menu::importSysExMapping, true);


	lblAppName.reset(new Label("lblAppName", getApplicationName()));
	lblAppName->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
	lblAppName->setColour(Label::ColourIds::textColourId, Colour(0xff777777));
	addAndMakeVisible(lblAppName.get());

	lblAppVersion.reset(new Label("lblAppVersion", "v" + getApplicationVersion()));
	lblAppVersion->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
	lblAppVersion->setColour(Label::ColourIds::textColourId, Colour(0xff777777));
	addAndMakeVisible(lblAppVersion.get());


	addStatusListener(this);

	// Initial size
	// setSize(DEFAULTMAINWINDOWWIDTH, DEFAULTMAINWINDOWHEIGHT);

	// Select first board and first key
	// noteEditArea->getOctaveBoardSelectorTab()->setCurrentTabIndex(0, true);
	// The above call is supposed to update changeListener - but apparently doesn't... Call it manually then. XXX
	// changeListenerCallback(noteEditArea->getOctaveBoardSelectorTab());
    // noteEditArea->changeSingleKeySelection(0);

	btnLoadFile->getProperties().set(LumatoneEditorStyleIDs::textButtonIconHashCode, LumatoneEditorIcon::LoadIcon);
	btnSaveFile->getProperties().set(LumatoneEditorStyleIDs::textButtonIconHashCode, LumatoneEditorIcon::SaveIcon);
	btnImportFile->getProperties().set(LumatoneEditorStyleIDs::textButtonIconHashCode, LumatoneEditorIcon::ArrowUp);
	btnImportFile->getProperties().set(LumatoneEditorStyleIDs::textButtonIconPlacement, LumatoneEditorStyleIDs::TextButtonIconPlacement::RightOfText);

	// Only enable when connected
	btnImportFile->setEnabled(false);

	// DEBUG
	// sectionTabs->setCurrentTabIndex(3);

    // Initialize mapping structure
    //deleteAll();
}

MainContentComponent::~MainContentComponent()
{
	keySelectionController = nullptr;

	copiedSubBoardData = nullptr;

	lblAppVersion = nullptr;
	lblAppName = nullptr;

	btnImportFile = nullptr;
	btnSaveFile = nullptr;
	btnLoadFile = nullptr;

	sectionTabs = nullptr;
	lblEditTitle = nullptr;

	globalSettingsArea = nullptr;

	mappingSettingsComponent = nullptr;
	keyEditorPanel = nullptr;

	allKeysOverview = nullptr;
	midiEditArea = nullptr;
}

void MainContentComponent::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	// noteEditArea->saveStateToPropertiesFile(propertiesFile);
	// globalSettingsArea->saveStateToPropertiesFile(propertiesFile);
}

// Set the currentSectionKey mapping to be edited to the value passed in parameter
//void MainContentComponent::setData(LumatoneLayout& newData, bool withRefresh)
//{
//	*mappingData = newData;
//
//	noteEditArea->onSetData(newData);
//
//	if (withRefresh)
//	{
//		refreshAllFields();
//	}
//}
//
//void MainContentComponent::deleteAll(bool withRefresh)
//{
//	LumatoneLayout keyMapping;
//	setData(keyMapping, withRefresh);
//}
//
//// Copy the edited mapping to the variable passed as parameter
//void MainContentComponent::getData(LumatoneLayout& newData)
//{
//	newData = *getMappingData();
//}

// TabbedButtonBar *MainContentComponent::getOctaveBoardSelectorTab()
// {
// 	return  noteEditArea->getOctaveBoardSelectorTab();
// }

// LumatoneAction* MainContentComponent::createDeleteCurrentSectionAction()
// {
// 	auto currentSetSelection = noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex();
// 	if (currentSetSelection >= 0 && currentSetSelection < getOctaveBoardSize())
// 	{
//         auto keySet = LumatoneBoard();
//         return new LumatoneEditAction::SectionEditAction(this, currentSetSelection, keySet);
// 	}
// 	else
// 		return nullptr;
// }

// bool MainContentComponent::copyCurrentSubBoardData()
// {
// 	auto currentSetSelection = noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex();
// 	if (currentSetSelection >= 0 && currentSetSelection < getOctaveBoardSize())
// 	{
// 		*copiedSubBoardData = getBoard(currentSetSelection);
// 		return true;
// 	}
// 	else
// 		return false;
// }

// LumatoneAction* MainContentComponent::createPasteCurrentSectionAction()
// {
// 	auto currentSetSelection = noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex();
// 	if (currentSetSelection >= 0 && currentSetSelection < getNumBoards()
// 		&& !copiedSubBoardData->isEmpty())
// 	{
// 		return new LumatoneEditAction::SectionEditAction(this, currentSetSelection, *copiedSubBoardData);
// 	}
// 	else
// 		return nullptr;
// }

// LumatoneAction* MainContentComponent::createModifiedPasteCurrentSectionAction(CommandID commandID)
// {
//     auto currentSetSelectionIndex = noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex();
//     if (currentSetSelectionIndex >= 0 && currentSetSelectionIndex < getNumBoards()
//         && !copiedSubBoardData->isEmpty())
//     {

// 		auto modifiedSection = getBoard(currentSetSelectionIndex);
// 		auto octaveSize = getOctaveBoardSize();

//         for (int i = 0; i < octaveSize; i++)
//         {
//             LumatoneKey copiedKey = copiedSubBoardData->getKey(i);
//             LumatoneKey currentSectionKey = getKey(currentSetSelectionIndex, i);

//             switch (commandID)
//             {
//             case Lumatone::Menu::commandIDs::pasteOctaveBoardNotes:
//                 currentSectionKey.setNoteOrCC(copiedKey.getMidiNumber());
//                 break;

//             case Lumatone::Menu::commandIDs::pasteOctaveBoardChannels:
//                 currentSectionKey.setChannelNumber(copiedKey.getMidiChannel());
//                 break;

//             case Lumatone::Menu::commandIDs::pasteOctaveBoardColours:
//                 currentSectionKey.setColour(copiedKey.getColour());
//                 break;

//             case Lumatone::Menu::commandIDs::pasteOctaveBoardTypes:
//                 currentSectionKey.setKeyType(copiedKey.getType());
// 				currentSectionKey.setDefaultCCFader(copiedKey.isCCFaderDefault());
//                 break;

//             default:
//                 jassertfalse;
// 				currentSectionKey = copiedKey;
// 				break;
//             }

// 			modifiedSection.setKey(currentSectionKey, i);
//         }

//         return new LumatoneEditAction::SectionEditAction(this, currentSetSelectionIndex, modifiedSection);
//     }
//     else
//         return nullptr;
// }

bool MainContentComponent::canPasteCopiedSubBoard() const
{
    return !copiedSubBoardData->isEmpty();
}

void MainContentComponent::updateDeveloperMode()
{
	// TODO This can just be implemented individually in handleStatePropertyChange
	// curvesArea->setDeveloperMode(inDeveloperMode);
    // globalSettingsArea->setDeveloperMode(inDeveloperMode);
}

void MainContentComponent::connectionStateChanged(ConnectionState state)
{
	btnImportFile->setEnabled(state == ConnectionState::ONLINE);
}

//void MainContentComponent::octaveColourConfigReceived(int octaveIndex, uint8 rgbFlag, const int* colourData)
//{
//	for (int keyIndex = 0; keyIndex < getOctaveBoardSize(); keyIndex++)
//	{
//		// LumatoneKey& keyData = getBoard(octaveIndex - 1)->theKeys[keyIndex];
//		// auto newValue = colourData[keyIndex];
//
//		// if (rgbFlag == 0)
//		// {
//		// 	keyData.colour = Colour(newValue, keyData.colour.getGreen(), keyData.colour.getBlue());
//		// }
//		// else if (rgbFlag == 1)
//		// {
//		// 	keyData.colour = Colour(keyData.colour.getRed(), newValue, keyData.colour.getBlue());
//		// }
//		// else if (rgbFlag == 2)
//		// {
//		// 	keyData.colour = Colour(keyData.colour.getRed(), keyData.colour.getGreen(), newValue);
//		// }
//		// else
//		// 	jassertfalse;
//	}
//
//	refreshKeyDataFields();
//}
//
//void MainContentComponent::octaveChannelConfigReceived(int octaveIndex, const int* channelData)
//{
//	for (int keyIndex = 0; keyIndex < getOctaveBoardSize(); keyIndex++)
//	{
//		// Check channel values?
//		// getBoard(octaveIndex - 1)->theKeys[keyIndex].channelNumber = channelData[keyIndex];
//	}
//
//	refreshKeyDataFields();
//}
//
//void MainContentComponent::octaveNoteConfigReceived(int octaveIndex, const int* noteData)
//{
//	for (int keyIndex = 0; keyIndex < getOctaveBoardSize(); keyIndex++)
//	{
//		// Check note values?
//		// getBoard(octaveIndex - 1)->theKeys[keyIndex].noteNumber = noteData[keyIndex];
//	}
//
//	refreshKeyDataFields();
//}
//
//void MainContentComponent::keyTypeConfigReceived(int octaveIndex, const int* keyTypeData)
//{
//	for (int keyIndex = 0; keyIndex < getOctaveBoardSize(); keyIndex++)
//	{
//		// Check type values?
//		// getBoard(octaveIndex - 1)->theKeys[keyIndex].keyType = LumatoneKeyType(keyTypeData[keyIndex]);
//	}
//
//	refreshKeyDataFields();
//}
//
//void MainContentComponent::velocityConfigReceived(const int* velocityData)
//{
//	// mappingData.velocityTable.editStrategy = LumatoneConfigTable::DrawMode::freeDrawing;
//	// for (int i = 0; i < 128; i++)
//	// 	mappingData.velocityTable.velocityValues[i] = velocityData[127 - i]; // Reversed
//	curvesArea->loadFromMapping();
//}
//
//void MainContentComponent::aftertouchConfigReceived(const int* aftertouch)
//{
//	// mappingData.afterTouchTable.editStrategy = LumatoneConfigTable::DrawMode::freeDrawing;
//	// memmove(mappingData.afterTouchTable.velocityValues, aftertouch, sizeof(int) * 128);
//	curvesArea->loadFromMapping();
//}
//
//void MainContentComponent::velocityIntervalConfigReceived(const int* velocityData)
//{
//	// memmove(mappingData.table, velocityData, sizeof(int) * VELOCITYINTERVALTABLESIZE);
//	curvesArea->loadFromMapping();
//}
//
//void MainContentComponent::faderConfigReceived(const int* faderData)
//{
//	// mappingData.faderTable.editStrategy = LumatoneConfigTable::DrawMode::freeDrawing;
//	// memmove(mappingData.faderTable.velocityValues, faderData, sizeof(int) * 128);
//	curvesArea->loadFromMapping();
//}
//
//void MainContentComponent::faderTypeConfigReceived(int octaveIndex, const int* faderTypeData)
//{
//	for (int keyIndex = 0; keyIndex < getOctaveBoardSize(); keyIndex++)
//	{
//		// getBoard(octaveIndex - 1)->theKeys[keyIndex].ccFaderDefault = faderTypeData[keyIndex];
//	}
//}
//
//void MainContentComponent::lumatouchConfigReceived(const int* lumatouchData)
//{
//	// mappingData.lumaTouchTable.editStrategy = LumatoneConfigTable::DrawMode::freeDrawing;
//	// memmove(mappingData.lumaTouchTable.velocityValues, lumatouchData, sizeof(int) * 128);
//	curvesArea->loadFromMapping();
//}

//void MainContentComponent::firmwareRevisionReceived(LumatoneFirmware::Version version)
//{
//	// Make sure changes happen in proper order
//	//noteEditArea->resetOctaveSize();
//	//allKeysOverview->resetOctaveSize();
//}

void MainContentComponent::changeListenerCallback(ChangeBroadcaster *source)
{
	// if (source == noteEditArea->getOctaveBoardSelectorTab())
	// {
	// 	// allKeysOverview->setCurrentSetSelection(noteEditArea->getOctaveBoardSelectorTab()->getCurrentTabIndex());
	// }

	// Probably not the cleanest way to do this
	if (source == sectionTabBar)
	{
		resizeEditSectionTabs();
	}
}

void MainContentComponent::buttonClicked(Button* btn)
{
	ColourViewComponent* colourEdit = dynamic_cast<ColourViewComponent*>(btn);

	if (colourEdit)
	{
		// May be better asynchronous on a timer
		// TerpstraSysExApplication::getApp().loadColourPalettesFromFile();

		auto palettes = getColourPalettes();
		ColourPaletteWindow* paletteWindow = new ColourPaletteWindow(*this);
		paletteWindow->setSize(proportionOfWidth(popupWidth), proportionOfHeight(popupHeight));

        // if (btn == noteEditArea->getColourViewComponent())
        // {
        //     colourEdit = noteEditArea->getColourViewComponent();

		// 	auto colourTextEditor = noteEditArea->getSingleNoteColourTextEditor();
        //     paletteWindow->listenToColourSelection(static_cast<ColourSelectionListener*>(colourTextEditor));

        //     // Shouldn't be necessary when Isomorphic is moved from dev to public
        //     auto isomorphicPanel = noteEditArea->getIsomorphicMassAssignPanel();
        //     if (isomorphicPanel != nullptr)
        //         paletteWindow->listenToColourSelection(static_cast<ColourSelectionListener*>(isomorphicPanel));

		// 	paletteWindow->addColourSelectorToGroup(noteEditArea.get());
		// 	paletteWindow->setCurrentColourSelector(noteEditArea->getSingleNoteColourTextEditor());
        // }

		Rectangle<int> componentArea = colourEdit->getScreenBounds().translated(-getScreenX(), -getScreenY());

		juce::CallOutBox::launchAsynchronously(
			std::unique_ptr<juce::Component>(paletteWindow),
			componentArea,
			this
		);

		// else, a preset button colour button was pressed
		paletteWindow->listenToColourSelection(colourEdit);
		// TODO: Set swatch # or custom colour as currentSectionKey colour
	}
}

void MainContentComponent::paint (Graphics& g)
{
	g.fillAll(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::MediumBackground));

	g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LightBackground));
	g.fillRect(controlsAreaBackground);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	int newWidth = getWidth();
	// int newHeight = getHeight();

	contentWidth = proportionOfWidth(contentWidthRatio);
	contentMargin = juce::roundToInt((newWidth - contentWidth) * 0.5);

	// Logo, MIDI edit area and connection state
	int midiAreaHeight = proportionOfHeight(headerHeight);
	midiEditArea->setBounds(0, 0, getWidth(), midiAreaHeight);

	// Bounds for controls, where background is darker
	int footerY = proportionOfHeight(footerAreaY);
	int footerHeight = getHeight() - footerY;

	controlsAreaBackground = getBounds().withTop(proportionOfHeight(controlsAreaY)).withBottom(footerY);
	controlsArea = controlsAreaBackground.withSizeKeepingCentre(contentWidth, controlsAreaBackground.getHeight());

	controlsLabelYPos = controlsArea.getY() - proportionOfHeight(controlsLabelHeight);

	allKeysOverview->setBounds(contentMargin, midiAreaHeight, contentWidth, controlsLabelYPos - midiAreaHeight);

	int btnHeight = roundToInt(getHeight() * fileButtonH);
	int btnMargin = roundToInt(getWidth() * saveloadMarginW);
	int saveLoadWidth = roundToInt(getWidth() * saveLoadW);
	int btnY = allKeysOverview->getY() - roundToInt(getHeight() * btnYFromImageTop);

	int halfWidthX = roundToInt(getWidth() * 0.5f);

	btnLoadFile->setBounds(halfWidthX - btnMargin - saveLoadWidth, btnY, saveLoadWidth, btnHeight);
	btnSaveFile->setBounds(halfWidthX + btnMargin, btnY, saveLoadWidth, btnHeight);

	int importY = allKeysOverview->getY() - roundToInt(getHeight() * importYFromImageTop);
	int importWidth = roundToInt(getWidth() * importW);
	btnImportFile->setBounds(allKeysOverview->getRight() - importWidth, importY, importWidth, btnHeight);

	lblEditTitle->setTopLeftPosition(contentMargin, controlsLabelYPos);
	resizeLabelWithHeight(lblEditTitle.get(), (controlsArea.getY() - controlsLabelYPos) * 0.8f);

	sectionTabs->setBounds(contentMargin, controlsLabelYPos, contentWidth, footerY - controlsLabelYPos);
	resizeEditSectionTabs();

	globalSettingsArea->setBounds(getLocalBounds().withTop(roundToInt(getHeight() * footerAreaY)));

	resizeLabelWithHeight(lblAppName.get(), roundToInt(footerHeight * lumatoneVersionHeight), 1.0f, " ");
	lblAppName->setTopLeftPosition(
		proportionOfWidth(lumatoneVersionMarginX),
		footerY + juce::roundToInt((footerHeight - lblAppName->getHeight()) * 0.5f));

	resizeLabelWithHeight(lblAppVersion.get(), roundToInt(lblAppName->getHeight() * 0.75f));
	lblAppVersion->setTopLeftPosition(lblAppName->getRight(), lblAppName->getBottom() - lblAppVersion->getHeight());
}
void MainContentComponent::resizeEditSectionTabs()
{
	int sectionTabsMargin = proportionOfWidth(sectionTabsMarginW);
	int sectionTabsWidth = 0;

	auto font = getEditorLookAndFeel().getTabBarFont((float)sectionTabBar->getHeight() * 0.7f);
	for (int i = 0; i < sectionTabNames.size(); i++)
	{
		sectionTabsWidth += font.getStringWidth(sectionTabNames[i]) + sectionTabsMargin;
	}

	int leftMargin = lblEditTitle->getWidth() + roundToInt(sectionTabsMargin * 0.5f);
	sectionTabBar->setBounds(sectionTabBar->getBounds().withTrimmedLeft(leftMargin).withWidth(sectionTabsWidth));
}

//
// void MainContentComponent::refreshKeyDataFields()
//{
//	noteEditArea->refreshKeyFields();
//	// allKeysOverview->mappingUpdateCallback();
//	// juce::Timer::callAfterDelay(1, [&]() { allKeysOverview->refreshMappingData(); });
//}
//
// void MainContentComponent::refreshAllFields()
//{
//	refreshKeyDataFields();
//	generalOptionsArea->loadFromMapping();
//	pedalSensitivityDlg->loadFromMapping();
//	curvesArea->loadFromMapping();
//	curvesArea->repaint();
//}

void MainContentComponent::handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier& property)
{
	LumatoneEditorState::handleStatePropertyChange(stateIn, property);

	if (property == LumatoneEditorProperty::DeveloperModeOn)
	{
		updateDeveloperMode();
	}
}
