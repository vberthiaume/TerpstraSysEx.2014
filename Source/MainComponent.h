/*
  ==============================================================================

    This file was auto-generated!
	Created: XXX.2014
	Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include "LumatoneEditorState.h"

#include "./lumatone_editor_library/data/lumatone_layout.h"
#include "./lumatone_editor_library/listeners/status_listener.h"
#include "./lumatone_editor_library/listeners/firmware_listener.h"
#include "./lumatone_editor_library/lumatone_midi_driver/firmware_types.h"

class LumatoneKeyboardComponent;

class MidiEditArea;
class NoteEditArea;
class GeneralOptionsDlg;
class CurvesArea;
class GlobalSettingsArea;
class PedalSensitivityDlg;

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent : public juce::Component
						   , public LumatoneEditorState
						   , public LumatoneEditor::StatusListener
						   , public LumatoneEditor::FirmwareListener
						   , public juce::ChangeListener
						   , public juce::Button::Listener
{
public:
	//==============================================================================
	MainContentComponent(const LumatoneEditorState& stateIn, juce::ApplicationCommandManager* commandManager);
	~MainContentComponent();

	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);

	// Transfer of data
	void setData(LumatoneLayout& newData, bool withRefresh = true);
	void deleteAll(bool withRefresh = true);

	void getData(LumatoneLayout& newData);
	// LumatoneLayout&	getMappingInEdit() { return getMapping; }

	juce::TabbedButtonBar* getOctaveBoardSelectorTab();
	CurvesArea* getCurvesArea() { return curvesArea.get(); }

	// Board edit operations
	juce::UndoableAction* createDeleteCurrentSectionAction();
	bool copyCurrentSubBoardData();
	juce::UndoableAction* createPasteCurrentSectionAction();
    juce::UndoableAction* createModifiedPasteCurrentSectionAction(CommandID commandID);
    bool canPasteCopiedSubBoard() const;

	// Implementation of ChangeListener
	void changeListenerCallback(juce::ChangeBroadcaster *source) override;

	// Implementation of Button::Listener
	void buttonClicked(juce::Button* btn) override;

	// GUI implementation
    void paint (juce::Graphics&) override;
    void resized() override;

	void refreshKeyDataFields();
	void refreshAllFields();

	// Implementation of LumatoneEditor::StatusListener
	void connectionStateChanged(ConnectionState state) override;

	// Implementation of LumatoneEditor::FirmwareListener

	void octaveColourConfigReceived(int octaveIndex, uint8 rgbFlag, const int* colourData) override;

	void octaveChannelConfigReceived(int octaveIndex, const int* channelData) override;

	void octaveNoteConfigReceived(int octaveIndex, const int* noteData) override;

	void keyTypeConfigReceived(int octaveIndex, const int* keyTypeData) override;

	void velocityConfigReceived(const int* velocityData) override;

	void aftertouchConfigReceived(const int* aftertouchData) override;

	void velocityIntervalConfigReceived(const int* velocityData) override;

	void faderConfigReceived(const int* faderData) override;

	void faderTypeConfigReceived(int octaveIndex, const int* faderTypeData) override;

	void lumatouchConfigReceived(const int* lumatouchData) override;

	void firmwareRevisionReceived(LumatoneFirmware::Version version) override;

	// Implementation of LumatoneState
	void handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier& property) override;


private:

	void updateDeveloperMode();


private:
    //==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)

	//==============================================================================
	// GUI components

	// Midi devices and connection state
	std::unique_ptr<MidiEditArea>			midiEditArea;

	// Sets of 55/56 keys
	std::unique_ptr<LumatoneKeyboardComponent> 	allKeysOverview;

	// Edit fields for setting key and button parameters, and edits for single keys
	std::unique_ptr<NoteEditArea>			noteEditArea;
	std::unique_ptr<GeneralOptionsDlg>		generalOptionsArea;
	std::unique_ptr<CurvesArea> 			curvesArea;
	std::unique_ptr<GlobalSettingsArea> 	globalSettingsArea;
	std::unique_ptr<PedalSensitivityDlg> 	pedalSensitivityDlg;

	std::unique_ptr<juce::TextButton>		btnLoadFile;
	std::unique_ptr<juce::TextButton>		btnSaveFile;
	std::unique_ptr<juce::TextButton>		btnImportFile;

	// Version signature in bottom left of window
	std::unique_ptr<juce::Label> 					lblAppName;
	std::unique_ptr<juce::Label> 					lblAppVersion;

	// Buffer for copy/paste of sub board data
	std::unique_ptr<LumatoneBoard>			copiedSubBoardData;

	//==============================================================================
	// Position and Size helpers

    juce::Rectangle<int> controlsArea;

    const float headerHeight                = 0.0776f;

    const float controlsAreaY               = 0.5273f;
    const float controlsAreaHeight          = 0.43f;

    const float assignMarginX               = 0.043f;
    const float assignWidth                 = 0.5385f;
    const float assignHeight                = 0.44f;

    const float footerAreaY                 = 0.96f;

	const float lumatoneGraphicMarginTop	= 0.1f;
	const float lumatoneGraphicH			= 0.8f;

	const float fileButtonH					= 0.025f;
	const float importYFromImageTop			= 0.4f;
	const float importW						= 0.1f;

	const float btnYFromImageTop			= 0.02;
	const float saveLoadW					= 0.08f;
	const float saveloadMarginW				= 0.0034f;


    const float popupWidth                  = 0.4f;
    const float popupHeight                 = 0.333f;

    const float lumatoneVersionMarginX      = 0.02f;
	const float lumatoneVersionWidth        = 0.2f;
	const float lumatoneVersionHeight       = 0.5f;

    const float presetButtonColoursX        = 0.7237f;
    const float presetButtonColoursHeight   = 0.2258f;

    const float settingsColumnX             = 0.597826f;
    const float settingsAreaY               = 0.546875f;
    const float settingsAreaHeight          = 0.148148f;

    const juce::Rectangle<float> generalSettingsBounds = { settingsColumnX, settingsAreaY, 0.17f, settingsAreaHeight };
    const juce::Rectangle<float>   pedalSettingsBounds = { 0.777778f,       settingsAreaY, 0.18f, settingsAreaHeight };
    const juce::Rectangle<float>      curvesAreaBounds = { settingsColumnX, 0.7174f,       0.3626f, 0.21f };

};
