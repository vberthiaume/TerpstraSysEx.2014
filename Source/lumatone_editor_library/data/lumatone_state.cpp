/*
  ==============================================================================

    lumatone_state.cpp
    Created: 4 Jun 2023 4:57:50pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "lumatone_state.h"

#include "../mapping/lumatone_output_map.h"

juce::Array<juce::Identifier> LumatoneState::getLumatoneStateProperties()
{
    juce::Array<juce::Identifier> properties;
    properties.add(LumatoneStateProperty::LastConnectedSerialNumber);
    properties.add(LumatoneStateProperty::LastConnectedFirmwareVersion);
    properties.add(LumatoneStateProperty::LastConnectedNumBoards);

    properties.add(LumatoneStateProperty::MappingData);

    return properties;
}

LumatoneState::LumatoneState(juce::String nameIn, juce::ValueTree stateIn, juce::UndoManager* undoManagerIn)
    : LumatoneStateBase(nameIn)
    , undoManager(undoManagerIn)
{
    state = loadStateProperties(stateIn);
    state.addListener(this);

    mappingData = std::make_shared<LumatoneLayout>();
    midiKeyMap = std::make_shared<LumatoneOutputMap>(mappingData.get());

    juce::ValueTree layoutState = juce::ValueTree(juce::Identifier(nameIn));
    layoutState.appendChild(mappingData->getState(), nullptr);
    state.appendChild(layoutState, nullptr);
}

LumatoneState::LumatoneState(juce::String nameIn, const LumatoneState& stateToCopy, juce::UndoManager* undoManagerIn)
    : LumatoneState(nameIn, stateToCopy.state, undoManagerIn)
{
    mappingData = stateToCopy.mappingData;
    midiKeyMap = stateToCopy.midiKeyMap;
}

LumatoneState::~LumatoneState()
{
    state.removeListener(this);

    midiKeyMap = nullptr;
    mappingData = nullptr;
}

juce::ValueTree LumatoneState::loadStateProperties(juce::ValueTree stateIn)
{
    juce::ValueTree newState = (stateIn.hasType(LumatoneStateProperty::StateTree))
                             ? stateIn
                             : juce::ValueTree(LumatoneStateProperty::StateTree);

    for (auto property : getLumatoneStateProperties())
    {
        if (newState.hasProperty(property))
            handleStatePropertyChange(newState, property);
    }

    return newState;
}

void LumatoneState::handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier& property)
{
    if (property == LumatoneStateProperty::LastConnectedSerialNumber)
    {
        connectedSerialNumber = stateIn.getProperty(property).toString();
    }
    else if (property == LumatoneStateProperty::LastConnectedFirmwareVersion)
    {
        setLumatoneVersion(
            LumatoneFirmware::ReleaseVersion((int)stateIn.getProperty(property, (int)LumatoneFirmware::ReleaseVersion::FUTURE_VERSION))
            );
        firmwareVersion = LumatoneFirmware::Version::fromReleaseVersion(determinedVersion);
    }
    else if (property == LumatoneStateProperty::MappingData)
    {
        // juce::String mappingString = stateIn.getProperty(property).toString();
        // if (mappingString.isEmpty())
        //     return;

        // auto stringArray = juce::StringArray::fromLines(mappingString);
        // LumatoneLayout loadedLayout(getNumBoards(), getOctaveBoardSize());
        // loadedLayout.fromStringArray(stringArray);

        // if (!loadedLayout.isEmpty())
        // {
        //     mappingData.reset(new LumatoneLayout(loadedLayout));
        // }
    }
}

void LumatoneState::setConnectedSerialNumber(juce::String serialNumberIn)
{
    connectedSerialNumber = serialNumberIn;
    state.setPropertyExcludingListener(
        this,
        LumatoneStateProperty::LastConnectedSerialNumber,
        connectedSerialNumber,
        undoManager);

    // numBoards = 5;

    if (connectedSerialNumber == SERIAL_55_KEYS)
    {
        setLumatoneVersion(LumatoneFirmware::ReleaseVersion::VERSION_55_KEYS);
    }
}

void LumatoneState::setFirmwareVersion(LumatoneFirmware::Version& versionIn, bool writeToState)
{
    firmwareVersion = LumatoneFirmware::Version(versionIn);
    setLumatoneVersion(firmwareSupport.getReleaseVersion(firmwareVersion), writeToState);
}

void LumatoneState::setLumatoneVersion(LumatoneFirmware::ReleaseVersion versionIn, bool writeToState)
{
    determinedVersion = versionIn;

    // numBoards = 5;

    switch (determinedVersion)
    {
    case LumatoneFirmware::ReleaseVersion::VERSION_55_KEYS:
        mappingData->setOctaveBoardSize(55);
        break;
    default:
        mappingData->setOctaveBoardSize(56);
        break;
    }

    if (writeToState)
    {
        state.setPropertyExcludingListener(
            this,
            LumatoneStateProperty::LastConnectedFirmwareVersion,
            (int)determinedVersion,
            undoManager);
    }
}

void LumatoneState::setCompleteConfig(const LumatoneLayout &layoutIn)
{
    *mappingData = layoutIn;
}

void LumatoneState::setLayout(const LumatoneLayout &layoutIn)
{
    for (int i = 0; i < layoutIn.getNumBoards(); i++)
    {
        mappingData->setBoard(layoutIn.getBoard(i), i);
    }
}

void LumatoneState::setBoard(const LumatoneBoard &boardIn, int boardId)
{
    mappingData->setBoard(boardIn, boardId-1);
}

void LumatoneState::setKey(const LumatoneKey &keyIn, int boardId, int keyIndex)
{
    mappingData->setKey(keyIn, boardId-1, keyIndex);
}

void LumatoneState::setKeyConfig(const LumatoneKey& keyIn, int boardId, int keyIndex)
{
    mappingData->setKeyConfig(keyIn, boardId-1, keyIndex);
}

void LumatoneState::setKeyColour(juce::Colour colour, int boardId, int keyIndex)
{
    mappingData->setKeyColour(colour, boardId-1, keyIndex);
}

void LumatoneState::sendSelectionParam(const juce::Array<MappedLumatoneKey>& selection, bool signalEditorListeners, bool bufferKeyUpdates)
{
    for (auto mappedKey : selection)
    {
        mappingData->setKey(mappedKey);
    }
}

void LumatoneState::sendSelectionColours(const juce::Array<MappedLumatoneKey>& selection, bool signalEditorListeners, bool bufferKeyUpdates)
{
    for (auto mappedKey : selection)
    {
        mappingData->setKeyColour(mappedKey.getColour(), mappedKey.boardIndex, mappedKey.keyIndex);
    }
}

void LumatoneState::setAftertouchEnabled(bool enabled)
{
    mappingData->setAftertouchEnabled(enabled);
}

void LumatoneState::setLightOnKeyStrokes(bool enabled)
{
    mappingData->setLightOnKeyStrokes(enabled);
}

void LumatoneState::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    //if (treeWhosePropertyHasChanged == state)
    //{
        DBG("LumatoneState::valueTreePropertyChanged("
            + treeWhosePropertyHasChanged.getType().toString() + ", "
            + property.toString() + ")");

    if (treeWhosePropertyHasChanged == state)
    {
        handleStatePropertyChange(state, property);
    }
}

LumatoneFirmware::ReleaseVersion LumatoneState::getLumatoneVersion() const
{
    return determinedVersion;
}

LumatoneFirmware::Version LumatoneState::getFirmwareVersion() const
{
    return firmwareVersion;
}

juce::String LumatoneState::getSerialNumber() const
{
    return connectedSerialNumber;
}

LumatoneLayout* LumatoneState::getMappingData() const
{
    return mappingData.get();
}

const LumatoneBoard& LumatoneState::getBoard(int boardIndex) const
{
    return mappingData->getBoard(boardIndex);
}
const LumatoneKey& LumatoneState::getKey(int boardIndex, int keyIndex) const
{
    return mappingData->getKey(boardIndex, keyIndex);
}

const LumatoneKey& LumatoneState::getKey(LumatoneKeyCoord coord) const
{
    return getKey(coord.boardIndex, coord.keyIndex);
}

const LumatoneOutputMap* LumatoneState::getMidiKeyMap() const
{
    return midiKeyMap.get();
}

const FirmwareSupport& LumatoneState::getFirmwareSupport() const
{
    return firmwareSupport;
}

void LumatoneState::setInvertExpression(bool invert)
{
    mappingData->setInvertExpression(invert);
}

void LumatoneState::setInvertSustain(bool invert)
{
    mappingData->setInvertSustain(invert);
}

void LumatoneState::setExpressionSensitivity(juce::uint8 sensitivity)
{
    mappingData->setExpressionSensitivity(sensitivity);
}

void LumatoneState::setConfigTable(LumatoneConfigTable::TableType type, const LumatoneConfigTable& table)
{
    mappingData->setConfigTable(type, table.velocityValues);
}

//  bool LumatoneState::loadLayoutFromFile(const juce::File& layoutFile)
// {
//     bool fileOpened = false;
//     bool fileParsed = false;

//     if (layoutFile.existsAsFile())
//     {
//         fileOpened = true;

//         juce::StringArray stringArray;
//         layoutFile.readLines(stringArray);

//         LumatoneLayout newLayout(getNumBoards(), getOctaveBoardSize(), true);
//         newLayout.fromStringArray(stringArray);

//         // TODO: something if boards/size don't match?
//         fileParsed = true;


//         if (fileParsed)
//         {
//             *mappingData = LumatoneLayout(newLayout);

//             auto layoutString = mappingData->toStringArray().joinIntoString(juce::newLine);
//             DBG("Loaded: " + layoutString);

//             writeStringProperty(LumatoneStateProperty::MappingData, layoutString, undoManager);

//             invertSustain = mappingData->invertSustain;
//             writeBoolProperty(LumatoneStateProperty::InvertSustain, invertSustain, undoManager);

//             invertExpression = mappingData->invertExpression;
//             writeBoolProperty(LumatoneStateProperty::InvertExpression, invertExpression, undoManager);

//             expressionSensitivity = mappingData->expressionSensitivity;
//             writeIntProperty(LumatoneStateProperty::ExpressionSensitivity, expressionSensitivity, undoManager);

//             // Mark file as unchanged
//             //setHasChangesToSave(false);

//             // Clear undo history
//             //undoManager.clearUndoHistory();

//             // Add file to recent files list
//             //recentFiles.addFile(currentFile);

//             return true;
//         }
//     }

//     if (fileOpened)
//     {
//         // Show error message
//         juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon, "Open File Error", "The file " + layoutFile.getFullPathName() + " could not be opened.");

//         // XXX Update Window title in any case? Make file name empty/make data empty in case of error?
//     }

//     return false;
// }
