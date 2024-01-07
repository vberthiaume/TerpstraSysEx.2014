/*
  ==============================================================================

    LumatoneEditorState.h
    Created: 29 Dec 2023 5:39pm
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_STATE_H
#define LUMATONE_EDITOR_STATE_H

#include "./lumatone_editor_library/data/application_state.h"
#include "./lumatone_editor_library/palettes/colour_palette_file.h"

#define CHOOSE_FILE_NOOP [](bool) -> void {}

struct LumatoneEditorFontLibrary;
class LumatoneEditorLookAndFeel;
class LumatoneController;
class LumatoneEditorColourPalette;

class TerpstraSysExApplication;

namespace LumatoneEditorProperty
{
    static const juce::Identifier StateTree = juce::Identifier("LumatoneEditorState");

    static const juce::Identifier HasChangesToSave = juce::Identifier("HasChangesToSave");
    static const juce::Identifier HasChangesToSend = juce::Identifier("HasChangesToSend");

    static const juce::Identifier InCalibrationMode = juce::Identifier("InCalibrationMode");
    static const juce::Identifier FirmwareUpdatePerformed = juce::Identifier("FirmwareUpdatePerformed");

    static const juce::Identifier ColourPalettes = juce::Identifier("ColourPalettes");
    
    static const juce::Identifier CurrentFile = juce::Identifier("CurrentFile");
    static const juce::Identifier RecentFiles = juce::Identifier("RecentFiles");

    static const juce::Identifier AutoConnectDevice = juce::Identifier("AutoConnectDevice");
    static const juce::Identifier CheckDeviceActivity = juce::Identifier("CheckDeviceActivity");

    static const juce::Identifier UserDocumentsDirectory = juce::Identifier("UserDocumentsDirectory");
    static const juce::Identifier UserMappingsDirectory = juce::Identifier("UserMappingsDirectory");
    static const juce::Identifier UserPalettesDirectory = juce::Identifier("UserPalettesDirectory");

    static const juce::Identifier DeveloperModeOn = juce::Identifier("DeveloperModeOn");

    static const juce::Identifier MainWindowState = juce::Identifier("MainWindowState");
    static const juce::Identifier EditorMode = juce::Identifier("EditorMode");

    static const juce::Identifier SingleNoteNoteSetActive = juce::Identifier("SingleNoteNoteSetActive");
    static const juce::Identifier SingleNoteChannelSetActive = juce::Identifier("SingleNoteChannelSetActive");
    static const juce::Identifier SingleNoteColourSetActive = juce::Identifier("SingleNoteColourSetActive");
    static const juce::Identifier SingleNoteKeyTypeSetActive = juce::Identifier("SingleNoteKeyTypeSetActive");

    static const juce::Identifier LastSettingsPanel = juce::Identifier("LastSettingsPanel");

}

enum class EditorMode
{
    OFFLINE,
    ONLINE
};

static juce::Array<juce::Identifier> GetLumatoneEditorProperties();

class LumatoneEditorState : public LumatoneApplicationState
{
public:

    LumatoneEditorState(juce::String name,  LumatoneFirmwareDriver& driverIn, juce::UndoManager* undoManagerIn);
    LumatoneEditorState(juce::String name, const LumatoneEditorState& stateIn);

    ~LumatoneEditorState() override;

    const juce::String getApplicationName() const { return ProjectInfo::projectName; }
	const juce::String getApplicationVersion() const { return ProjectInfo::versionString; }

    bool getHasChangesToSave() const { return hasChangesToSave; }
    bool getInCalibrationMode() const { return inCalibrationMode; }
    bool firmwareUpdateCompleted() const { return firmwareUpdateWasPerformed; }
    bool getInDeveloperMode() const { return inDeveloperMode; }

    EditorMode getEditorMode() const { return editorMode; }

    LumatoneEditorLookAndFeel& getEditorLookAndFeel() { return *lookAndFeel; }

    //const juce::Array<LumatoneEditorColourPalette>& getColourPalettes() { return colourPalettes; }
    const juce::Array<LumatoneEditorColourPalette>& getColourPalettes();

    const LumatoneEditorFontLibrary& getAppFonts() const { return *appFonts; }

    juce::var getProperty(juce::Identifier propertyId) const;

    // juce::PropertiesFile& getPropertiesFile() { return *propertiesFile; }
    juce::File getCurrentFile() const { return currentFile; }

    juce::RecentlyOpenedFilesList& getRecentFiles();

    juce::File getUserDocumentsDirectory() const;
    juce::File getUserMappingsDirectory() const;
    juce::File getUserPalettesDirectory() const;

    // RecentlyOpenedFilesList& getRecentFileList() { return *recentFiles; }
	// Font getAppFont(LumatoneEditorFont fontIdIn, float height = 12.0f) { return appFonts.getFont(fontIdIn, height); }

public:
    bool doSendChangesToDevice() const override;

protected:
    juce::ValueTree loadStateProperties(juce::ValueTree stateIn) override;
    void handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier& property) override;

    bool resetToCurrentFile();
    bool openRecentFile(int recentFileIndex);

    void addPalette(const LumatoneEditorColourPalette& newPalette);
    bool deletePaletteFile(juce::File pathToPalette);

public:



// Only main app should access these
private:
    void setColourPalettes(const juce::Array<LumatoneEditorColourPalette>& palettesIn);
    void loadColourPalettesFromFile();

    bool setCurrentFile(juce::File fileToOpen);

    void setEditMode(EditorMode editMode);

    void setHasChangesToSave(bool hasChangesToSave);
    void setCalibrationMode(bool calibrationModeOn);
    void setDeveloperMode(bool developerModeOn);

protected:
    bool hasChangesToSave = false;
    bool hasChangesToSend = false;
	bool inCalibrationMode = false;
    bool inDeveloperMode = false;
	bool firmwareUpdateWasPerformed = false;

    EditorMode editorMode = EditorMode::OFFLINE;

private:
    std::shared_ptr<LumatoneEditorFontLibrary>      appFonts;
	std::shared_ptr<LumatoneEditorLookAndFeel>      lookAndFeel;
	std::shared_ptr<juce::Array<LumatoneEditorColourPalette>>        colourPalettes;

	juce::File                      currentFile;
	std::shared_ptr<juce::RecentlyOpenedFilesList>	recentFiles;

    std::shared_ptr<juce::PropertiesFile>   propertiesFile;

    friend class TerpstraSysExApplication;
};

#endif LUMATONE_EDITOR_STATE_H
