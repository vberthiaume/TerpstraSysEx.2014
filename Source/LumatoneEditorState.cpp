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
#include "./lumatone_editor_library/listeners/editor_listener.h"


static juce::File getDefaultUserDocumentsDirectory()
{
    return File::getSpecialLocation(File::userDocumentsDirectory).getChildFile("Lumatone Editor");
}
static juce::File getDefaultUserMappingsDirectory()
{
    return getDefaultUserDocumentsDirectory().getChildFile("Mappings");
}
static juce::File getDefaultUserPalettesDirectory()
{
    return getDefaultUserDocumentsDirectory().getChildFile("Palettes");
}

juce::Array<juce::Identifier> GetLumatoneEditorProperty()
{
    juce::Array<juce::Identifier> properties;
    properties.add(LumatoneEditorProperty::HasChangesToSave);
    properties.add(LumatoneEditorProperty::HasChangesToSend);
    properties.add(LumatoneEditorProperty::InCalibrationMode);
    properties.add(LumatoneEditorProperty::FirmwareUpdatePerformed);
    properties.add(LumatoneEditorProperty::ColourPalettes);
    properties.add(LumatoneEditorProperty::CurrentFile);
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

    loadPropertiesFile(nullptr);

    colourPalettes = std::make_shared<juce::Array<LumatoneEditorColourPalette>>();
}

LumatoneEditorState::LumatoneEditorState(juce::String name, const LumatoneEditorState &stateIn)
    : LumatoneApplicationState(name, stateIn)
    , appFonts(stateIn.appFonts)
    , lookAndFeel(stateIn.lookAndFeel)
    , recentFiles(stateIn.recentFiles)
    , propertiesFile(stateIn.propertiesFile)
    , colourPalettes(stateIn.colourPalettes)
{
}

LumatoneEditorState::~LumatoneEditorState()
{
    recentFiles = nullptr;
    propertiesFile = nullptr;
    lookAndFeel = nullptr;
    appFonts = nullptr;
}

juce::String LumatoneEditorState::getProperty(juce::Identifier propertyId, juce::String fallbackValue) const
{
    return propertiesFile->getValue(propertyId, fallbackValue);
}

juce::RecentlyOpenedFilesList& LumatoneEditorState::getRecentFiles()
{
    return *recentFiles;
}

const juce::Array<LumatoneEditorColourPalette>& LumatoneEditorState::getColourPalettes()
{
    return *colourPalettes;
}

juce::File LumatoneEditorState::getUserDocumentsDirectory() const
{
    juce::String possibleDirectory = propertiesFile->getValue(LumatoneEditorProperty::UserDocumentsDirectory);
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

    juce::String possibleDirectory = propertiesFile->getValue(LumatoneEditorProperty::UserMappingsDirectory);
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

    juce::String possibleDirectory = propertiesFile->getValue(LumatoneEditorProperty::UserPalettesDirectory);
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
    setStateProperty(LumatoneEditorProperty::HasChangesToSave, hasChangesToSave);
}

bool LumatoneEditorState::doSendChangesToDevice() const
{
    return LumatoneApplicationState::doSendChangesToDevice() && editorMode == EditorMode::ONLINE;
}

void LumatoneEditorState::Controller::setEditMode(EditorMode editMode)
{
    editorState.editorMode = editMode;
    editorState.setStateProperty(LumatoneEditorProperty::EditorMode, (int)editorState.editorMode);
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
    LumatoneApplicationState::handleStatePropertyChange(stateIn, property);

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
    else if (property == LumatoneEditorProperty::CurrentFile)
    {
        currentFile = juce::File(stateIn[property]);
    }
    else if (property == LumatoneEditorProperty::DeveloperModeOn)
    {
        inDeveloperMode = (bool)stateIn[property];
    }
    else if (property == LumatoneEditorProperty::EditorMode)
    {
        editorMode = EditorMode((int)stateIn[property]);
    }
}

void LumatoneEditorState::loadPropertiesFile(juce::PropertiesFile *propertiesIn)
{
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
    DBG(propertiesFile->createXml("LumatoneEditorSettings")->toString());
    jassert(propertiesFile != nullptr);

    recentFiles = std::make_shared<juce::RecentlyOpenedFilesList>();
    recentFiles->restoreFromString(propertiesFile->getValue(LumatoneEditorProperty::RecentFiles));
    recentFiles->removeNonExistentFiles();

    LumatoneApplicationState::loadPropertiesFile(propertiesFile.get());

    setStateProperty(LumatoneEditorProperty::UserDocumentsDirectory, propertiesFile->getValue(LumatoneEditorProperty::UserDocumentsDirectory.toString(), getDefaultUserDocumentsDirectory().getFullPathName()));
    setStateProperty(LumatoneEditorProperty::UserMappingsDirectory, propertiesFile->getValue(LumatoneEditorProperty::UserMappingsDirectory.toString(), getDefaultUserMappingsDirectory().getFullPathName()));
    setStateProperty(LumatoneEditorProperty::UserPalettesDirectory, propertiesFile->getValue(LumatoneEditorProperty::UserPalettesDirectory.toString(), getDefaultUserPalettesDirectory().getFullPathName()));
    setStateProperty(LumatoneEditorProperty::RecentFiles, propertiesFile->getValue(LumatoneEditorProperty::RecentFiles.toString(), juce::String()));
    setStateProperty(LumatoneEditorProperty::MainWindowState, propertiesFile->getValue(LumatoneEditorProperty::MainWindowState.toString(), juce::String()));
    
    setStateProperty(LumatoneEditorProperty::AutoConnectDevice, propertiesFile->getBoolValue(LumatoneEditorProperty::AutoConnectDevice.toString(), true));

    setStateProperty(LumatoneEditorProperty::SingleNoteKeyTypeSetActive, propertiesFile->getBoolValue(LumatoneEditorProperty::SingleNoteKeyTypeSetActive.toString(), true));
    setStateProperty(LumatoneEditorProperty::SingleNoteNoteSetActive, propertiesFile->getBoolValue(LumatoneEditorProperty::SingleNoteNoteSetActive.toString(), true));
    setStateProperty(LumatoneEditorProperty::SingleNoteChannelSetActive, propertiesFile->getBoolValue(LumatoneEditorProperty::SingleNoteChannelSetActive.toString(), true));
    setStateProperty(LumatoneEditorProperty::SingleNoteColourSetActive, propertiesFile->getBoolValue(LumatoneEditorProperty::SingleNoteColourSetActive.toString(), true));
    setStateProperty(LumatoneEditorProperty::SingleNoteCCFaderIsDefault, propertiesFile->getBoolValue(LumatoneEditorProperty::SingleNoteCCFaderIsDefault.toString(), false));
    setStateProperty(LumatoneEditorProperty::SingleNoteAutoIncNoteActive, propertiesFile->getBoolValue(LumatoneEditorProperty::SingleNoteAutoIncNoteActive.toString(), true));
    setStateProperty(LumatoneEditorProperty::SingleNoteAutoIncChannelActive, propertiesFile->getBoolValue(LumatoneEditorProperty::SingleNoteAutoIncChannelActive.toString(), true));
    setStateProperty(LumatoneEditorProperty::SingleNoteAutoIncChannelAfterNumNotes, propertiesFile->getIntValue(LumatoneEditorProperty::SingleNoteAutoIncChannelAfterNumNotes.toString(), 127));

    setStateProperty(LumatoneEditorProperty::IsomorphicMassAssign, propertiesFile->getBoolValue(LumatoneEditorProperty::IsomorphicMassAssign.toString(), false));

    setStateProperty(LumatoneEditorProperty::LastSettingsPanel, propertiesFile->getIntValue(LumatoneEditorProperty::LastSettingsPanel.toString(), 1));
    setStateProperty(LumatoneEditorProperty::LastColourWindowTab, propertiesFile->getIntValue(LumatoneEditorProperty::LastColourWindowTab.toString(), 1));
    setStateProperty(LumatoneEditorProperty::LastFirmwareBinPath, propertiesFile->getValue(LumatoneEditorProperty::LastFirmwareBinPath.toString()
        , juce::File::getSpecialLocation(juce::File::SpecialLocationType::userHomeDirectory).getFullPathName()
        ));

    setStateProperty(LumatoneEditorProperty::DeveloperModeOn, propertiesFile->getBoolValue(LumatoneEditorProperty::DeveloperModeOn, false));
}

void LumatoneEditorState::Controller::setColourPalettes(const juce::Array<LumatoneEditorColourPalette> &palettesIn)
{
    *editorState.colourPalettes = palettesIn;
    // TODO
    // editorState.setPropertyExcludingListener(this, LumatoneEditorProperty::ColourPalettes, "", nullptr);
}

void LumatoneEditorState::Controller::loadColourPalettesFromFile()
{
    auto directory = editorState.getUserPalettesDirectory();
    auto foundPaletteFiles = directory.findChildFiles(juce::File::TypesOfFileToFind::findFiles, true, '*' + juce::String(PALETTEFILEEXTENSION));

    juce::Array<LumatoneEditorColourPalette> newPalettes;

    auto paletteSorter = LumatoneEditorPaletteSorter();
    for (auto file : foundPaletteFiles)
    {
        LumatoneEditorColourPalette palette = LumatoneEditorColourPalette::loadFromFile(file);
        newPalettes.addSorted(paletteSorter, palette);
    }

    setColourPalettes(newPalettes);
}

void LumatoneEditorState::Controller::addPalette(const LumatoneEditorColourPalette &newPalette)
{
    editorState.colourPalettes->add(newPalette);
    // TODO
    // editorState.setPropertyExcludingListener(this, LumatoneEditorProperty::ColourPalettes, "", nullptr);
}

bool LumatoneEditorState::Controller::deletePaletteFile(juce::File pathToPalette)
{
	bool success = false;

	if (pathToPalette.existsAsFile())
	{
		success = pathToPalette.deleteFile();
	}

	return success;
}

bool LumatoneEditorState::Controller::performAction(LumatoneAction *action, bool undoable, bool newTransaction)
{
    if (LumatoneApplicationState::Controller::performAction(action, undoable, newTransaction))
    {
        setHasChangesToSave(true);
        return true;
    }

    return false;
}

// Open a SysEx mapping from the file specified in currentFile
bool LumatoneEditorState::Controller::resetToCurrentFile()
{
    getEditorListeners()->call(&LumatoneEditor::EditorListener::newFileLoaded, editorState.getCurrentFile());

    if (editorState.getCurrentFile().getFullPathName().isEmpty())
    {
        // Replace with blank file
		LumatoneLayout defaultLayout;
        editorState.setCompleteConfig(defaultLayout);
        editorState.setHasChangesToSave(false);
        return true;
    }

    if (editorState.getCurrentFile().existsAsFile())
	{
		// XXX StringArray format: platform-independent?
		juce::StringArray stringArray;
		editorState.getCurrentFile().readLines(stringArray);
		LumatoneLayout keyMapping(stringArray);

		// Send configuration to controller, if connected
        editorState.setCompleteConfig(keyMapping);

		// Mark file as unchanged
        editorState.setHasChangesToSave(false);

        // Clear undo history
		editorState.undoManager->clearUndoHistory();

		// Add file to recent files list
		editorState.recentFiles->addFile(editorState.currentFile);

		return true;
	}

	// Show error message
	AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::WarningIcon, "Open File Error", "The file " + editorState.getCurrentFile().getFullPathName() + " could not be opened.");

	// XXX Update Window title in any case? Make file name empty/make data empty in case of error?
	return false;
}

bool LumatoneEditorState::Controller::setCurrentFile(File fileToOpen)
{
    editorState.currentFile = fileToOpen;
    editorState.setStateProperty(LumatoneEditorProperty::CurrentFile, editorState.currentFile.getFullPathName());
    return resetToCurrentFile();
}

// open a file from the "recent files" menu
bool LumatoneEditorState::Controller::openRecentFile(int recentFileIndex)
{
    jassert(recentFileIndex >= 0 && recentFileIndex < editorState.recentFiles->getNumFiles());
    return setCurrentFile(editorState.recentFiles->getFile(recentFileIndex));
}

bool LumatoneEditorState::Controller::requestCompleteConfigFromDevice()
{
    setHasChangesToSave(false);
    editorState.undoManager->clearUndoHistory();
    return LumatoneApplicationState::Controller::requestCompleteConfigFromDevice();
}

bool LumatoneEditorState::Controller::saveMappingToFile(juce::File fileToSave)
{
    juce::StringArray stringArray = editorState.getMappingData()->toStringArray();
    juce::String fileText = stringArray.joinIntoString("\n");

    bool success = false;

    if (fileToSave.existsAsFile())
        success = fileToSave.replaceWithText(fileText, false, false);

    else if (fileToSave.create().ok())
    {
        success = fileToSave.appendText(fileText, false, false);
    }

    if (success && editorState.getCurrentFile() != fileToSave)
    {
        // TODO skip certain updates?
        setCurrentFile(fileToSave);
    }

    return success;
}

bool LumatoneEditorState::Controller::savePropertiesFile() const
{
    // TODO Save documents directories (Future: provide option to change them and save after changed by user)
    //propertiesFile->setValue(LumatoneEditorProperty::UserDocumentsDirectory, getUserDocumentsDirectory().getFullPathName());
    //propertiesFile->setValue(LumatoneEditorProperty::UserMappingsDirectory, getUserMappingsDirectory().getFullPathName());
    //propertiesFile->setValue(LumatoneEditorProperty::UserPalettesDirectory, getUserPalettesDirectory().getFullPathName());

    // Save recent files list
    editorState.recentFiles->removeNonExistentFiles();
    jassert(editorState.propertiesFile != nullptr);
    editorState.propertiesFile->setValue(LumatoneEditorProperty::RecentFiles, editorState.recentFiles->toString());

    return editorState.propertiesFile->saveIfNeeded();
}

void LumatoneEditorState::Controller::savePropertyBoolValue(const juce::Identifier &id, bool value)
{
    editorState.propertiesFile->setValue(id.toString(), juce::var(value));
    savePropertiesFile();
    editorState.setStateProperty(id, juce::var(value));
}

void LumatoneEditorState::Controller::savePropertyIntValue(const juce::Identifier &id, int value)
{
    editorState.propertiesFile->setValue(id.toString(), juce::var(value));
    savePropertiesFile();
    editorState.setStateProperty(id, juce::var(value));
}

void LumatoneEditorState::Controller::savePropertyStringValue(const juce::Identifier &id, juce::String value)
{
    editorState.propertiesFile->setValue(id.toString(), juce::var(value));
    savePropertiesFile();
    editorState.setStateProperty(id, juce::var(value));
}

void LumatoneEditorState::Controller::setCalibrationMode(bool calibrationModeOn)
{
    editorState.inCalibrationMode = calibrationModeOn;
    editorState.getLumatoneController()->setCalibratePitchModWheel(calibrationModeOn);
    editorState.setStateProperty(LumatoneEditorProperty::InCalibrationMode, editorState.inCalibrationMode);
    savePropertyBoolValue(LumatoneEditorProperty::InCalibrationMode, calibrationModeOn);
}

void LumatoneEditorState::Controller::setDeveloperMode(bool developerModeOn)
{
    editorState.inDeveloperMode = developerModeOn;
    editorState.setStateProperty(LumatoneEditorProperty::DeveloperModeOn, editorState.inDeveloperMode);
    savePropertyBoolValue(LumatoneEditorProperty::DeveloperModeOn, developerModeOn);
}
