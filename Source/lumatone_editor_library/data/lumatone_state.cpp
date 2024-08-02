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

    properties.add(LumatoneStateProperty::LightsOnAfterKeystroke);
    properties.add(LumatoneStateProperty::AftertouchEnabled);
    properties.add(LumatoneStateProperty::InvertExpression);
    properties.add(LumatoneStateProperty::InvertSustain);

    properties.add(LumatoneStateProperty::ExpressionSensitivity);

    properties.add(LumatoneStateProperty::InactiveMacroButtonColour);
    properties.add(LumatoneStateProperty::ActiveMacroButtonColour);

    properties.add(LumatoneStateProperty::MappingData);

    return properties;
}

LumatoneState::LumatoneState(juce::ValueTree stateIn, juce::UndoManager *undoManagerIn)
    : LumatoneStateBase(stateIn.getType().toString())
    , undoManager(undoManagerIn)
{
    mappingData = std::make_shared<LumatoneLayout>();
    midiKeyMap = std::make_shared<LumatoneOutputMap>(mappingData.get());

    state = loadStateProperties(stateIn);
    state.addListener(this);

    auto mappingState = mappingData->getState();
    state.appendChild(mappingState, nullptr);
}

LumatoneState::LumatoneState(juce::String nameIn, const LumatoneState &stateIn)
    : LumatoneStateBase(nameIn)
    , undoManager(stateIn.undoManager)
    , mappingData(stateIn.mappingData)
    , midiKeyMap(stateIn.midiKeyMap)
{
    state = loadStateProperties(stateIn.state);
    state.addListener(this);
}

LumatoneState::LumatoneState(const LumatoneState &stateIn)
    : LumatoneState(stateIn.name + "Copy", stateIn)
{
}

LumatoneState::~LumatoneState()
{
    state.removeListener(this);

    midiKeyMap = nullptr;
    mappingData = nullptr;
}

juce::ValueTree LumatoneState::loadStateProperties(juce::ValueTree stateIn)
{
    juce::ValueTree newState = stateIn.isValid()
                             ? stateIn
                             : juce::ValueTree(LumatoneStateProperty::DefaultState);

    for (auto property : getLumatoneStateProperties())
    {
        if (newState.hasProperty(property))
            handleStatePropertyChange(newState, property);
    }

    return newState;
}

void LumatoneState::handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier& property)
{
    // Only acknowledge changes to this node 
    if (stateIn != state)
        return;

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
    else if (property == LumatoneStateProperty::InactiveMacroButtonColour)
    {
        auto readColour = juce::Colour::fromString(stateIn[property].toString());
        inactiveMacroButtonColour = readColour;
    }
    else if (property == LumatoneStateProperty::ActiveMacroButtonColour)
    {
        auto readColour = juce::Colour::fromString(stateIn[property].toString());
        activeMacroButtonColour = readColour;
    }
}

void LumatoneState::loadPropertiesFile(juce::PropertiesFile *properties)
{
    // No global properties for now
}

void LumatoneState::setConnectedSerialNumber(juce::String serialNumberIn)
{
    connectedSerialNumber = serialNumberIn;
    state.setPropertyExcludingListener(
        this,
        LumatoneStateProperty::LastConnectedSerialNumber,
        connectedSerialNumber,
        undoManager);


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

    if (writeToState)
    {
        setStateProperty(LumatoneStateProperty::LastConnectedFirmwareVersion, (int)determinedVersion);
    }
}

void LumatoneState::setCompleteConfig(const LumatoneLayout &layoutIn)
{
    if (mappingData.get() != &layoutIn)
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

void LumatoneState::setInactiveMacroButtonColour(juce::Colour buttonColour)
{
    inactiveMacroButtonColour = buttonColour;
    setStateProperty(LumatoneStateProperty::InactiveMacroButtonColour, buttonColour.toString());
}

void LumatoneState::setActiveMacroButtonColour(juce::Colour buttonColour)
{
    activeMacroButtonColour = buttonColour;
    setStateProperty(LumatoneStateProperty::InactiveMacroButtonColour, buttonColour.toString());
}
