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
class TerpstraSysExApplication 	: public juce::JUCEApplication
{
public:
	//==============================================================================
	TerpstraSysExApplication();

	const juce::String getApplicationName() override		{ return ProjectInfo::projectName; }
	const juce::String getApplicationVersion() override   { return ProjectInfo::versionString; }
	bool moreThanOneInstanceAllowed() override      { return true; }

	void initialise(const String& commandLine) override;
	void shutdown() override;
	void systemRequestedQuit() override;
	void anotherInstanceStarted(const String& commandLine) override;

	void loadPropertiesFile();

	// Menu functionality
	void getAllCommands(juce::Array <juce::CommandID>& commands) override;
	void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
	bool perform(const juce::ApplicationCommandTarget::InvocationInfo& info) override;

	bool openSysExMapping();
	bool saveSysExMapping(std::function<void(bool success)> saveFileCallback = CHOOSE_FILE_NOOP);
	bool saveSysExMappingAs(std::function<void(bool success)> saveFileCallback = CHOOSE_FILE_NOOP);
	bool resetSysExMapping();

	bool saveCurrentFile(std::function<void(bool success)> saveFileCallback = CHOOSE_FILE_NOOP);

	//bool saveColourPalette(LumatoneEditorColourPalette& palette, juce::File pathToPalette=juce::File());

	bool deleteSubBoardData();
	bool copySubBoardData();
	bool pasteSubBoardData();
    bool pasteModifiedSubBoardData(juce::CommandID commandID);
    bool canPasteSubBoardData() const;

	bool performUndoableAction(juce::UndoableAction* editAction, bool newTransaction=true);
	bool undo();
	bool redo();

	bool toggleDeveloperMode();

	// bool generalOptionsDialog();
	// bool noteOnOffVelocityCurveDialog();
	// bool faderVelocityCurveDialog();
	// bool aftertouchVelocityCurveDialog();

	// void sendCurrentConfigurationToDevice();
	bool requestConfigurationFromDevice();

	void updateMainTitle();

	void setOpenDialogWindow(juce::DialogWindow* dialogWindowIn);

	bool aboutTerpstraSysEx();

private:
	LumatoneFirmwareDriver	firmwareDriver;
	juce::UndoManager 		undoManager;

	LumatoneEditorStateController	state;
	std::unique_ptr<ApplicationCommandManager> commandManager;
	
	MainContentComponent* mainComponent;
	std::unique_ptr<MainWindow> mainWindow;
	std::unique_ptr<DialogWindow> dialogWindow;

	juce::TooltipWindow		tooltipWindow;
	std::unique_ptr<juce::FileChooser> fileChooser;
};
