/*
  ==============================================================================

    Main.h
    Created: 26 Jul 2015 8:39:46pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "./lumatone_editor_library/lumatone_midi_driver/lumatone_midi_driver.h"

#include "./LumatoneEditorState.h"

#include "LocalisationMap.h"

#define CHOOSE_FILE_NOOP [](bool) -> void {}

class LumatoneController;
class MainWindow;
class MainContentComponent;
class DeviceActivityMonitor;

//==============================================================================
class TerpstraSysExApplication 	: public JUCEApplication
{
public:
	//==============================================================================
	TerpstraSysExApplication();

	//LumatoneFirmwareDriver& initializeDriver();

	const String getApplicationName() override		{ return ProjectInfo::projectName; }
	const String getApplicationVersion() override   { return ProjectInfo::versionString; }
	bool moreThanOneInstanceAllowed() override      { return true; }

	void initialise(const String& commandLine) override;
	void shutdown() override;
	void systemRequestedQuit() override;
	void anotherInstanceStarted(const String& commandLine) override;

	void loadPropertiesFile();

	// Menu functionality
	void getAllCommands(Array <CommandID>& commands) override;
	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
	bool perform(const InvocationInfo& info) override;

	bool openSysExMapping();
	bool saveSysExMapping(std::function<void(bool success)> saveFileCallback = CHOOSE_FILE_NOOP);
	bool saveSysExMappingAs(std::function<void(bool success)> saveFileCallback = CHOOSE_FILE_NOOP);
	bool resetSysExMapping();

	bool setCurrentFile(juce::File file);
	bool saveCurrentFile(std::function<void(bool success)> saveFileCallback = CHOOSE_FILE_NOOP);

	bool saveColourPalette(LumatoneEditorColourPalette& palette, juce::File pathToPalette=juce::File());

	bool deleteSubBoardData();
	bool copySubBoardData();
	bool pasteSubBoardData();
    bool pasteModifiedSubBoardData(CommandID commandID);
    bool canPasteSubBoardData() const;

	bool performUndoableAction(UndoableAction* editAction, bool newTransaction=true);
	bool undo();
	bool redo();

	bool toggleDeveloperMode();

	// bool generalOptionsDialog();
	// bool noteOnOffVelocityCurveDialog();
	// bool faderVelocityCurveDialog();
	// bool aftertouchVelocityCurveDialog();

	// void sendCurrentConfigurationToDevice();
	void requestConfigurationFromDevice();

	void updateMainTitle();

	void setOpenDialogWindow(juce::DialogWindow* dialogWindowIn);

	bool aboutTerpstraSysEx();

	MainContentComponent* getMainContentComponent() const;

private:
	LumatoneFirmwareDriver	firmwareDriver;
	juce::UndoManager 		undoManager;

	LumatoneEditorState		state;

	std::unique_ptr<DeviceActivityMonitor> activityMonitor;

	std::unique_ptr<ApplicationCommandManager> commandManager;
	
	std::unique_ptr<MainWindow> mainWindow;
	std::unique_ptr<DialogWindow> dialogWindow;

	std::unique_ptr<juce::ComponentBoundsConstrainer> boundsConstrainer;


	juce::TooltipWindow		tooltipWindow;
	std::unique_ptr<juce::FileChooser> fileChooser;
};
