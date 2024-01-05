/*
==============================================================================

    LumatoneEditorState.cpp
    Created: 29 Dec 2023 5:39pm
    Author:  Vincenzo

==============================================================================
*/

#include "LumatoneEditorState.h"

#include "./LumatoneEditorLookAndFeel.h"
#include "./KeyEditComponent.h"

#include "./lumatone_editor_library/device/lumatone_controller.h"

juce::Array<juce::Identifier> GetLumatoneEditorProperty()
{
    juce::Array<juce::Identifier> properties;
    properties.add(LumatoneEditorProperty::HasChangesToSave);
    properties.add(LumatoneEditorProperty::HasChangesToSend);
    properties.add(LumatoneEditorProperty::InCalibrationMode);
    properties.add(LumatoneEditorProperty::FirmwareUpdatePerformed);
    properties.add(LumatoneEditorProperty::ColourPalettes);
    properties.add(LumatoneEditorProperty::RecentFiles);
    properties.add(LumatoneEditorProperty::UserDocumentsDirectory);
    properties.add(LumatoneEditorProperty::UserMappingsDirectory);
    properties.add(LumatoneEditorProperty::UserPalettesDirectory);
    properties.add(LumatoneEditorProperty::DeveloperModeOn);
    properties.add(LumatoneEditorProperty::EditorMode);
    return properties;
}


LumatoneEditorState::LumatoneEditorState(juce::String name, LumatoneFirmwareDriver& driverIn, juce::UndoManager *undoManagerIn)
    : LumatoneApplicationState("LumatoneEditor", driverIn, juce::ValueTree(), undoManagerIn)
{
    appFonts = std::make_shared<LumatoneEditorFontLibrary>();
    lookAndFeel = std::make_shared<LumatoneEditorLookAndFeel>(*appFonts, true);

    juce::PropertiesFile::Options options;
    options.applicationName = "LumatoneSetup";
    options.filenameSuffix = "settings";
    options.osxLibrarySubFolder = "Application Support";
#if JUCE_LINUX
    options.folderName = "~/.config/LumatoneSetup";
#else
    options.folderName = "LumatoneSetup";
#endif

    propertiesFile = std::make_shared<juce::PropertiesFile>(options);
    jassert(propertiesFile != nullptr);
}

LumatoneEditorState::LumatoneEditorState(juce::String name, const LumatoneEditorState &stateIn)
    : LumatoneApplicationState(name, stateIn)
    , appFonts(stateIn.appFonts)
    , lookAndFeel(stateIn.lookAndFeel)
    , propertiesFile(stateIn.propertiesFile)
{
}

LumatoneEditorState::~LumatoneEditorState()
{
}

juce::var LumatoneEditorState::getProperty(juce::Identifier propertyId) const
{
    return propertiesFile->getValue(propertyId.toString());
}

juce::RecentlyOpenedFilesList LumatoneEditorState::getRecentFiles() const
{
    juce::RecentlyOpenedFilesList recentFiles;
	recentFiles.restoreFromString(propertiesFile->getValue("RecentFiles"));
	recentFiles.removeNonExistentFiles();
    return recentFiles;
}

juce::File LumatoneEditorState::getUserDocumentsDirectory() const
{
    juce::String possibleDirectory = propertiesFile->getValue("UserDocumentsDirectory");
    juce::File directory;

    if (juce::File::isAbsolutePath(possibleDirectory))
    {
        directory = juce::File(possibleDirectory);
    }
    if (!directory.exists() || directory.existsAsFile())
    {
        directory = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("Lumatone Editor");

        // User pref?
        directory.createDirectory();
    }
    return directory;
}

juce::File LumatoneEditorState::getUserMappingsDirectory() const
{
    juce::File parentFolder = getUserDocumentsDirectory();

    juce::String possibleDirectory = propertiesFile->getValue("UserMappingsDirectory");
    juce::File directory;

    if (juce::File::isAbsolutePath(possibleDirectory))
    {
        directory = juce::File(possibleDirectory);
    }
    if (!directory.exists() || parentFolder.isDirectory())
    {
        directory = parentFolder.getChildFile("Mappings");

        // User pref?
        directory.createDirectory();
    }
    return directory;
}

juce::File LumatoneEditorState::getUserPalettesDirectory() const
{
    juce::File parentFolder = getUserDocumentsDirectory();

    juce::String possibleDirectory = propertiesFile->getValue("UserPalettesDirectory");
    juce::File directory;

    if (juce::File::isAbsolutePath(possibleDirectory))
    {
        directory = juce::File(possibleDirectory);
    }
    if (!directory.exists() || parentFolder.isDirectory())
    {
        directory = parentFolder.getChildFile("Palettes");

        // User pref?
        directory.createDirectory();
    }
    return directory;
}

void LumatoneEditorState::setHasChangesToSave(bool hasChangesToSaveIn)
{
    hasChangesToSave = hasChangesToSaveIn;
    state.setPropertyExcludingListener(this, LumatoneEditorProperty::HasChangesToSave, hasChangesToSave, nullptr);
}

void LumatoneEditorState::setCalibrationMode(bool calibrationModeOn)
{
    calibrationModeOn = calibrationModeOn;
    getLumatoneController()->setCalibratePitchModWheel(calibrationModeOn);
    // writeBoolProperty(LumatoneEditorProperty::InCalibrationMode, calibrationModeOn, nullptr);
    state.setPropertyExcludingListener(this, LumatoneEditorProperty::InCalibrationMode, calibrationModeOn, nullptr);
}

void LumatoneEditorState::setDeveloperMode(bool developerModeOn)
{
    inDeveloperMode = developerModeOn;
    // writeBoolProperty(LumatoneEditorProperty::DeveloperModeOn, inDeveloperMode, undoManager);
    state.setPropertyExcludingListener(this, LumatoneEditorProperty::DeveloperModeOn, inDeveloperMode, undoManager);
}

bool LumatoneEditorState::sendSysExToDevice() const
{
    return LumatoneApplicationState::sendSysExToDevice() && editorMode == EditorMode::ONLINE;
}

juce::ValueTree LumatoneEditorState::loadStateProperties(juce::ValueTree stateIn)
{
    juce::ValueTree newState = (stateIn.hasType(LumatoneEditorProperty::StateTree))
                             ? stateIn
                             : juce::ValueTree(LumatoneEditorProperty::StateTree);

    // TODO load editor properties

    // DBG("LumatoneApplicationState::loadStateProperties:\n" + newState.toXmlString());
    for (auto property : getLumatoneApplicationProperties())
    {
        if (newState.hasProperty(property))
            handleStatePropertyChange(newState, property);
    }

    LumatoneState::loadStateProperties(newState);

    return newState;
}

void LumatoneEditorState::handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier &property)
{
    if (property == LumatoneEditorProperty::HasChangesToSave)
    {
        hasChangesToSave = (bool)stateIn[property];
    }
    else if (property == LumatoneEditorProperty::HasChangesToSend)
    {
        hasChangesToSend = (bool)stateIn[property];
    }
    else if (property == LumatoneEditorProperty::InCalibrationMode)
    {
        inCalibrationMode =  (bool)stateIn[property];
    }
    else if (property == LumatoneEditorProperty::FirmwareUpdatePerformed)
    {
        firmwareUpdateWasPerformed = (bool)stateIn[property];
    }
    else if (property == LumatoneEditorProperty::ColourPalettes)
    {

    }
    else if (property == LumatoneEditorProperty::RecentFiles)
    {

    }
    else if (property == LumatoneEditorProperty::UserDocumentsDirectory)
    {

    }
    else if (property == LumatoneEditorProperty::UserMappingsDirectory)
    {

    }
    else if (property == LumatoneEditorProperty::UserPalettesDirectory)
    {

    }
    else if (property == LumatoneEditorProperty::DeveloperModeOn)
    {

    }
    else if (property == LumatoneEditorProperty::EditorMode)
    {

    }
    else
    {
        LumatoneApplicationState::handleStatePropertyChange(stateIn, property);
    }
}

void LumatoneEditorState::setColourPalettes(const juce::Array<LumatoneEditorColourPalette> &palettesIn)
{
    colourPalettes = palettesIn;
}

void LumatoneEditorState::addPalette(const LumatoneEditorColourPalette &newPalette)
{
    colourPalettes.add(newPalette);
}

bool LumatoneEditorState::deletePaletteFile(juce::File pathToPalette)
{
	bool success = false;

	if (pathToPalette.existsAsFile())
	{
		success = pathToPalette.deleteFile();
	}

	return success;
}

// Open a SysEx mapping from the file specified in currentFile
bool LumatoneEditorState::openFromCurrentFile()
{
    if (getCurrentFile().getFullPathName().isEmpty())
    {
        // Replace with blank file
		LumatoneLayout defaultLayout;
        setCompleteConfig(defaultLayout);
        return true;
    }

    if (getCurrentFile().existsAsFile())
	{
		// XXX StringArray format: platform-independent?
		juce::StringArray stringArray;
		getCurrentFile().readLines(stringArray);
		LumatoneLayout keyMapping(stringArray);

		// ((MainContentComponent*)(mainWindow->getContentComponent()))->setData(keyMapping);

        // TODO FIX
		// Window title
		// updateMainTitle();

		// Send configuration to controller, if connected
		// controller->sendCurrentCompleteConfig();

        setCompleteConfig(keyMapping);
		// Mark file as unchanged
		setHasChangesToSave(false);

		// Clear undo history
		// undoManager.clearUndoHistory();

		// Add file to recent files list
		recentFiles.addFile(currentFile);

		return true;
	}

	// Show error message
	AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon, "Open File Error", "The file " + getCurrentFile().getFullPathName() + " could not be opened.");

	// XXX Update Window title in any case? Make file name empty/make data empty in case of error?
	return false;
}

bool LumatoneEditorState::setCurrentFile(File fileToOpen)
{
    currentFile = fileToOpen;
    return openFromCurrentFile();
}

void LumatoneEditorState::setEditMode(EditorMode editMode)
{
    editorMode = editMode;
    state.setPropertyExcludingListener(this, LumatoneEditorProperty::EditorMode, (int)editorMode, nullptr);
}
