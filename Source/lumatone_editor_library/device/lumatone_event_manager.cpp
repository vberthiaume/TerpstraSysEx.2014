/*
  ==============================================================================

    lumatone_event_manager.cpp
    Created: 5 Jun 2023 8:19:04pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "lumatone_event_manager.h"

#include "../lumatone_midi_driver/lumatone_midi_driver.h"
#include "../lumatone_midi_driver/firmware_sysex.h"

#include "../listeners/editor_listener.h"
#include "../listeners/firmware_listener.h"

LumatoneEventManager::LumatoneEventManager(const LumatoneApplicationState& stateIn, LumatoneFirmwareDriver& midiDriverIn)
    : LumatoneApplicationState("LumatoneEventManager", stateIn)
    , LumatoneApplicationState::Controller(static_cast<LumatoneApplicationState&>(*this))
    , midiDriver(midiDriverIn)
{
    midiDriver.addDriverListener(this);

    // juce::MidiMessageCollector::reset(bufferReadTimeoutMs);
}

LumatoneEventManager::~LumatoneEventManager()
{
    midiDriver.removeDriverListener(this);
}

//=============================================================================
// Communication and broadcasting

FirmwareSupport::Error LumatoneEventManager::handleOctaveConfigResponse(
    const juce::MidiMessage& midiMessage,
    std::function<FirmwareSupport::Error(const juce::MidiMessage&, int&, juce::uint8, int*)> unpackFunction,
    std::function<void(int, void*)> callbackFunctionIfNoError)
{
    int boardId = -1;
    int channelData[56];

    int boardSize = getOctaveBoardSize();
    auto errorCode = unpackFunction(midiMessage, boardId, boardSize, channelData);
    if (errorCode == FirmwareSupport::Error::noError)
    {
        callbackFunctionIfNoError(boardId, channelData);
    }

    return errorCode;
}

FirmwareSupport::Error LumatoneEventManager::handleTableConfigResponse(
    const juce::MidiMessage& midiMessage,
    std::function<FirmwareSupport::Error(const juce::MidiMessage&, int*)> unpackFunction,
    std::function<void(void*)> callbackFunctionIfNoError)
{
    int veloctiyData[128];
    auto errorCode = unpackFunction(midiMessage, veloctiyData);
    if (errorCode == FirmwareSupport::Error::noError)
        callbackFunctionIfNoError(veloctiyData);

    return errorCode;
}

FirmwareSupport::Error LumatoneEventManager::handleLEDConfigResponse(const juce::MidiMessage& midiMessage)
{
    int boardId = -1;
    int colourData[56];

    int cmd = firmwareSupport.getCommandNumber(midiMessage);
    FirmwareSupport::Error errorCode;

    auto version = getLumatoneVersion();
    auto boardSize = getOctaveBoardSize();

    // Use correct unpacking function
    if (version < LumatoneFirmware::ReleaseVersion::VERSION_1_0_11)
    {
        errorCode = LumatoneSysEx::unpackGetLEDConfigResponse_Version_1_0_0(midiMessage, boardId, boardSize, colourData);
    }
    else
    {
        errorCode = LumatoneSysEx::unpackGetLEDConfigResponse(midiMessage, boardId, colourData);
    }

    if (errorCode == FirmwareSupport::Error::noError)
    {
        int colourCode = cmd - GET_RED_LED_CONFIG;

        switch (colourCode)
        {
            default:
                jassertfalse;
                break;
            case 0:
                getReceivedLayoutStatus().numRedConfigReceived++;
                break;
            case 1:
                getReceivedLayoutStatus().numGreenConfigReceived++;
                break;
            case 2:
                getReceivedLayoutStatus().numBlueConfigReceived++;
                break;
        }

        int boardIndex = boardId - 1;

        for (int keyIndex = 0; keyIndex < getOctaveBoardSize(); keyIndex++)
        {
            auto newValue = colourData[keyIndex];

            juce::Colour colour = getKey(boardIndex, keyIndex).getColour();
            if (colourCode == 0)
            {
                colour = juce::Colour(newValue, colour.getGreen(), colour.getBlue());
            }
            else if (colourCode == 1)
            {
                colour = juce::Colour(colour.getRed(), newValue, colour.getBlue());
            }
            else if (colourCode == 2)
            {
                colour = juce::Colour(colour.getRed(), colour.getGreen(), newValue);
            }
            else
            {
                jassertfalse;
            }

            LumatoneState::setKeyColour(colour, boardId, keyIndex);
        }

        getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::octaveColourConfigReceived, boardId, colourCode, colourData);
        getEditorListeners()->call(&LumatoneEditor::EditorListener::boardChanged, getBoard(boardIndex));

        if (getReceivedLayoutStatus().completed())
        {
            getEditorListeners()->call(&LumatoneEditor::EditorListener::layoutImported, *getMappingData());
        }
    }

    return errorCode;
}

FirmwareSupport::Error LumatoneEventManager::handleChannelConfigResponse(const juce::MidiMessage& midiMessage)
{
    auto unpack = [&](const juce::MidiMessage& msg, int& boardId, juce::uint8 numKeys, int* data) {
        auto errorCode = LumatoneSysEx::unpackGetChannelConfigResponse(msg, boardId, numKeys, data);
        if (errorCode == FirmwareSupport::Error::noError)
            for (int i = 0; i < numKeys; i++)
                data[i]++; // MIDI Channels are 1-based
        return errorCode;
    };
    auto callback = [&](int boardId, void* data)
    {
        int boardIndex = boardId - 1;
        const int* channelData = (int*) data;

        for (int keyIndex = 0; keyIndex < getOctaveBoardSize(); keyIndex++)
        {
            juce::uint8 ch = channelData[keyIndex];
            if (ch == 0 || ch > 16)
                ch = 1;

            auto key = getKey(boardIndex, keyIndex);
            key.setChannelNumber(ch);
            LumatoneState::setKeyConfig(key, boardId, keyIndex);
        }

        getReceivedLayoutStatus().numChannelConfigReceived++;

        getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::octaveChannelConfigReceived, boardId, channelData);
        getEditorListeners()->call(&LumatoneEditor::EditorListener::boardChanged, getBoard(boardIndex));
    };
    return handleOctaveConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneEventManager::handleNoteConfigResponse(const juce::MidiMessage& midiMessage)
{
    auto unpack = [&](const juce::MidiMessage& msg, int& boardId, juce::uint8 numKeys, int* data) {
        return LumatoneSysEx::unpackGetNoteConfigResponse(msg, boardId, numKeys, data);
    };
    auto callback = [&](int boardId, void* data)
    {
        int boardIndex = boardId - 1;
        const int* noteData = (int*)data;
        for (int keyIndex = 0; keyIndex < getOctaveBoardSize(); keyIndex++)
        {
            int note = noteData[keyIndex];
            if (note < 0 || note > 127)
                note = 0;

            auto key = getKey(boardIndex, keyIndex);
            key.setNoteOrCC(noteData[keyIndex]);
            LumatoneState::setKeyConfig(key, boardId, keyIndex);
        }

        getReceivedLayoutStatus().numNoteConfigReceived++;
        getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::octaveNoteConfigReceived, boardId, noteData);
        getEditorListeners()->call(&LumatoneEditor::EditorListener::boardChanged, getBoard(boardIndex));
    };
    return handleOctaveConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneEventManager::handleKeyTypeConfigResponse(const juce::MidiMessage& midiMessage)
{
    auto unpack = [&](const juce::MidiMessage& msg, int& boardId, juce::uint8 numKeys, int* data) {
        return LumatoneSysEx::unpackGetTypeConfigResponse(msg, boardId, numKeys, data);
    };
    auto callback = [&](int boardId, void* data)
    {
        int boardIndex = boardId - 1;
        const int* keyTypeData = (int*)data;
        for (int keyIndex = 0; keyIndex < getOctaveBoardSize(); keyIndex++)
        {
            auto type = LumatoneKeyType(keyTypeData[keyIndex]);

            auto key = getKey(boardIndex, keyIndex);
            key.setKeyType(type);
            LumatoneState::setKeyConfig(key, boardId, keyIndex);
        }

        getReceivedLayoutStatus().numKeyTypeConfigReceived++;
        getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::keyTypeConfigReceived, boardId, keyTypeData);
        getEditorListeners()->call(&LumatoneEditor::EditorListener::boardChanged, getBoard(boardIndex));
    };
    return handleOctaveConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneEventManager::handleVelocityConfigResponse(const juce::MidiMessage& midiMessage)
{
    auto unpack = [&](const juce::MidiMessage& msg, int* data) {
        return LumatoneSysEx::unpackGetVelocityConfigResponse(msg, data);
    };
    auto callback = [&](void* data)
    {
        getReceivedLayoutStatus().receivedVelocityTable = true;
        getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::velocityConfigReceived, (int*)data);
    };
    return handleTableConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneEventManager::handleAftertouchConfigResponse(const juce::MidiMessage& midiMessage)
{
    auto unpack = [&](const juce::MidiMessage& msg, int* data) {
        return LumatoneSysEx::unpackGetAftertouchConfigResponse(msg, data);
    };
    auto callback = [&](void* data)
    {
        getReceivedLayoutStatus().receivedAftertouchTable = true;
        getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::aftertouchConfigReceived, (int*)data);
    };
    return handleTableConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneEventManager::handleVelocityIntervalConfigResponse(const juce::MidiMessage& midiMessage)
{
    auto unpack = [&](const juce::MidiMessage& msg, int* data) {
        return LumatoneSysEx::unpackGetVelocityIntervalConfigResponse(msg, data);
    };
    auto callback = [&](void* data)
    {
        getReceivedSettingsStatus().receivedVelocityIntervalTable = true;
        getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::velocityIntervalConfigReceived, (int*)data);
    };
    return handleTableConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneEventManager::handleFaderConfigResponse(const juce::MidiMessage& midiMessage)
{
    auto unpack = [&](const juce::MidiMessage& msg, int* data) {
        return LumatoneSysEx::unpackGetFaderConfigResponse(msg, data);
    };
    auto callback = [&](void* data)
    {
        getReceivedLayoutStatus().receivedFaderTable = true;
        getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::faderConfigReceived, (int*)data);
    };
    return handleTableConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneEventManager::handleFaderTypeConfigResponse(const juce::MidiMessage& midiMessage)
{
    auto unpack = [&](const juce::MidiMessage& msg, int& boardId, juce::uint8 numKeys, int* data) {
        return LumatoneSysEx::unpackGetTypeConfigResponse(msg, boardId, numKeys, data);
    };
    auto callback = [&](int boardId, void* data)
    {
        getReceivedLayoutStatus().numFaderTypeConfigReceived++;
        getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::faderTypeConfigReceived, boardId, (int*)data);
    };
    return handleOctaveConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneEventManager::handleSerialIdentityResponse(const juce::MidiMessage& midiMessage)
{
    int serialBytes[6];
    auto errorCode = LumatoneSysEx::unpackGetSerialIdentityResponse(midiMessage, serialBytes);
    if (errorCode != FirmwareSupport::Error::noError)
        return errorCode;

    setConnectedSerialNumber(firmwareSupport.serialIdentityToString(serialBytes));
    getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::serialIdentityReceived, serialBytes);

    return errorCode;
}

FirmwareSupport::Error LumatoneEventManager::handleFirmwareRevisionResponse(const juce::MidiMessage& midiMessage)
{
    int major, minor, revision;
    auto errorCode = LumatoneSysEx::unpackGetFirmwareRevisionResponse(midiMessage, major, minor, revision);
    if (errorCode != FirmwareSupport::Error::noError)
        return errorCode;

    auto version = LumatoneFirmware::Version(major, minor, revision);
    auto releaseVersion = getFirmwareSupport().getReleaseVersion(version);
    switch (releaseVersion)
    {
    case LumatoneFirmware::ReleaseVersion::VERSION_55_KEYS:
        mappingData->setOctaveBoardSize(55);
        break;
    default:
        mappingData->setOctaveBoardSize(56);
        break;
    }

    setLumatoneVersion(releaseVersion, true);

    DBG("Firmware version is: " + version.toString());
    getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::firmwareRevisionReceived, version);

    return FirmwareSupport::Error::noError;
}

FirmwareSupport::Error LumatoneEventManager::handleLumatouchConfigResponse(const juce::MidiMessage& midiMessage)
{
    auto unpack = [&](const juce::MidiMessage& msg, int* data) {
        return LumatoneSysEx::unpackGetLumatouchConfigResponse(msg, data);
    };
    auto callback = [&](void* data) { getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::lumatouchConfigReceived, (int*)data); };
    return handleTableConfigResponse(midiMessage, unpack, callback);
}

FirmwareSupport::Error LumatoneEventManager::handlePingResponse(const juce::MidiMessage& midiMessage)
{
    unsigned int value = 0;
    auto errorCode = LumatoneSysEx::unpackPingResponse(midiMessage, value);

    if (errorCode != FirmwareSupport::Error::noError)
        return errorCode;

    getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::pingResponseReceived, value);

    return errorCode;
}

FirmwareSupport::Error LumatoneEventManager::handleGetPeripheralChannelResponse(const juce::MidiMessage& midiMessage)
{
    auto channelSettings = LumatoneFirmware::PeripheralChannelSettings();
    auto errorCode = LumatoneSysEx::unpackGetPeripheralChannelsResponse(midiMessage,
        channelSettings.pitchWheel,
        channelSettings.modWheel,
        channelSettings.expressionPedal,
        channelSettings.sustainPedal
    );

    getReceivedSettingsStatus().receivedPeripheralChannels = true;

    getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::peripheralMidiChannelsReceived, channelSettings);

    return errorCode;
}

FirmwareSupport::Error LumatoneEventManager::handleGetPresetFlagsResponse(const juce::MidiMessage& midiMessage)
{
    auto presetFlags = LumatoneFirmware::PresetFlags();
    auto errorCode = LumatoneSysEx::unpackGetPresetFlagsResponse(midiMessage,
        presetFlags.expressionPedalInverted,
        presetFlags.lightsOnKeystroke,
        presetFlags.polyphonicAftertouch,
        presetFlags.sustainPedalInverted
    );

    LumatoneState::setLightOnKeyStrokes(presetFlags.lightsOnKeystroke);
    LumatoneState::setAftertouchEnabled(presetFlags.polyphonicAftertouch);
    LumatoneState::setInvertSustain(presetFlags.sustainPedalInverted);
    LumatoneState::setInvertExpression(presetFlags.expressionPedalInverted);

    getReceivedLayoutStatus().receivedPresetFlags = true;

    getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::presetFlagsReceived, presetFlags);

    getEditorListeners()->call(&LumatoneEditor::EditorListener::lightOnKeyStrokesChanged, presetFlags.lightsOnKeystroke);
    getEditorListeners()->call(&LumatoneEditor::EditorListener::aftertouchToggled, presetFlags.polyphonicAftertouch);
    getEditorListeners()->call(&LumatoneEditor::EditorListener::invertSustainToggled, presetFlags.sustainPedalInverted);
    getEditorListeners()->call(&LumatoneEditor::EditorListener::invertFootControllerChanged, presetFlags.expressionPedalInverted);

    return errorCode;
}

FirmwareSupport::Error LumatoneEventManager::handleGetExpressionPedalSensitivityResponse(const juce::MidiMessage& midiMessage)
{
    int sensitivity = 127;
    auto errorCode = LumatoneSysEx::unpackGetExpressionPedalSensitivityResponse(midiMessage, sensitivity);

    LumatoneState::setExpressionSensitivity(sensitivity);

    getReceivedLayoutStatus().receivedExprPedalSensitivity = true;

    getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::expressionPedalSensitivityReceived, sensitivity);
    getEditorListeners()->call(&LumatoneEditor::EditorListener::expressionPedalSensitivityChanged, sensitivity);

    return errorCode;
}

FirmwareSupport::Error LumatoneEventManager::handleGetMacroLightIntensityResponse(const juce::MidiMessage &midiMessage)
{
    juce::Colour activeColour;
    juce::Colour inactiveColour;

    auto errorCode = LumatoneSysEx::unpackGetMacroLightIntensityResponse(midiMessage, activeColour, inactiveColour);
    if (errorCode != FirmwareSupport::Error::noError)
        return errorCode;

    LumatoneState::setActiveMacroButtonColour(activeColour);
    LumatoneState::setInactiveMacroButtonColour(inactiveColour);

    getReceivedSettingsStatus().receivedMacroButtonColours = true;

    getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::macroButtonColoursReceived, inactiveColour, activeColour);

    getEditorListeners()->call(&LumatoneEditor::EditorListener::macroButtonActiveColourChanged, activeColour);
    getEditorListeners()->call(&LumatoneEditor::EditorListener::macroButtonInactiveColourChanged, inactiveColour);

    return FirmwareSupport::Error::noError;
}

FirmwareSupport::Error LumatoneEventManager::handlePeripheralCalibrationData(const juce::MidiMessage& midiMessage)
{
    int mode = -1;
    auto errorCode = LumatoneSysEx::unpackPeripheralCalibrationMode(midiMessage, mode);

    if (errorCode != FirmwareSupport::Error::noError)
        return errorCode;

    switch (mode)
    {
    case LumatoneFirmware::PeripheralCalibrationDataMode::ExpressionPedal:
        errorCode = handleExpressionPedalCalibrationData(midiMessage);
        break;
    case LumatoneFirmware::PeripheralCalibrationDataMode::PitchAndModWheels:
        errorCode = handleWheelsCalibrationData(midiMessage);
        break;
    default:
        errorCode = FirmwareSupport::Error::messageIsNotResponseToCommand;
        break;
    }

    if (errorCode != FirmwareSupport::Error::noError)
        return errorCode;

    return errorCode;
}

FirmwareSupport::Error LumatoneEventManager::handleExpressionPedalCalibrationData(const juce::MidiMessage& midiMessage)
{
    int minBound = 0;
    int maxBound = 0;
    bool isValid = false;

    auto errorCode = LumatoneSysEx::unpackExpressionPedalCalibrationPayload(midiMessage, minBound, maxBound, isValid);

    getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::pedalCalibrationDataReceived, minBound, maxBound, isValid);

    return errorCode;
}

FirmwareSupport::Error LumatoneEventManager::handleWheelsCalibrationData(const juce::MidiMessage& midiMessage)
{
    LumatoneFirmware::WheelsCalibrationData calibrationData;
    auto errorCode = LumatoneSysEx::unpackWheelsCalibrationPayload(midiMessage,
        calibrationData.centerPitch,
        calibrationData.minPitch,
        calibrationData.maxPitch,
        calibrationData.minMod,
        calibrationData.maxMod
    );

    getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::wheelsCalibrationDataReceived, calibrationData);
    return errorCode;
}

FirmwareSupport::Error LumatoneEventManager::handleResponse(const juce::MidiMessage& midiMessage)
{
    auto sysExData = midiMessage.getSysExData();
    unsigned int cmd = sysExData[CMD_ID];

    switch (cmd)
    {
    case GET_RED_LED_CONFIG:
        return handleLEDConfigResponse(midiMessage);

    case GET_GREEN_LED_CONFIG:
        return handleLEDConfigResponse(midiMessage);

    case GET_BLUE_LED_CONFIG:
        return handleLEDConfigResponse(midiMessage);

    case GET_CHANNEL_CONFIG:
        return handleChannelConfigResponse(midiMessage);

    case GET_NOTE_CONFIG:
        return handleNoteConfigResponse(midiMessage);

    case GET_KEYTYPE_CONFIG:
        return handleKeyTypeConfigResponse(midiMessage);

        // TODO

    case GET_VELOCITY_CONFIG:
        return handleVelocityConfigResponse(midiMessage);

    case GET_FADER_TYPE_CONFIGURATION:
        return handleFaderTypeConfigResponse(midiMessage);

    case GET_SERIAL_IDENTITY:
        return handleSerialIdentityResponse(midiMessage);

    case CALIBRATE_PITCH_MOD_WHEEL:
        getFirmwareListeners()->call(&LumatoneEditor::FirmwareListener::calibratePitchModWheelAnswer, (LumatoneFirmware::ReturnCode)sysExData[MSG_STATUS]);
        return FirmwareSupport::Error::noError;

    case GET_LUMATOUCH_CONFIG:
        return handleLumatouchConfigResponse(midiMessage);

    case GET_FIRMWARE_REVISION:
        return handleFirmwareRevisionResponse(midiMessage);

    case LUMA_PING:
        return handlePingResponse(midiMessage);

    case GET_PERIPHERAL_CHANNELS:
        return handleGetPeripheralChannelResponse(midiMessage);

    case PERIPHERAL_CALBRATION_DATA:
        return handlePeripheralCalibrationData(midiMessage);

    case GET_PRESET_FLAGS:
        return handleGetPresetFlagsResponse(midiMessage);

    case GET_EXPRESSION_PEDAL_SENSITIVIY:
        return handleGetExpressionPedalSensitivityResponse(midiMessage);

    case SET_VELOCITY_CONFIG:
        DBG("Send layout complete.");
        // loadRandomMapping(1000, 1); // uncomment for test sequence
        return FirmwareSupport::Error::noError;

    case GET_MACRO_LIGHT_INTENSITY:
        return handleGetMacroLightIntensityResponse(midiMessage);

    default:
        jassert(sysExData[MSG_STATUS] == LumatoneFirmware::ReturnCode::ACK);
        if (midiMessage.getRawDataSize() <= 8)
        {
            // Simple confirmation
            return FirmwareSupport::Error::noError;
        }
        else
        {
            // Returned data
            DBG("WARNING: UNIMPLEMENTED RESPONSE HANDLING CMD " + juce::String(sysExData[CMD_ID]));
            return FirmwareSupport::Error::commandNotImplemented;
        }
    }

    return FirmwareSupport::Error::unknownCommand;
}

void LumatoneEventManager::handleResponseError(FirmwareSupport::Error errorToHandle, int commandReceived, const juce::MidiMessage& msg)
{
    switch (errorToHandle)
    {
    case FirmwareSupport::Error::noError:
    case FirmwareSupport::Error::messageIsAnEcho:
    case FirmwareSupport::Error::commandNotImplemented:
        return;

    // case FirmwareSupport::Error::messageHasInvalidStatusByte:
    //     return;

    case FirmwareSupport::Error::deviceIsBusy:
        DBG("DEVICE IS BUSY from command " + juce::String::toHexString(commandReceived));
        return;

    default:
        DBG("ERROR from command " + juce::String::toHexString(commandReceived) + ": " + FirmwareSupport::errorToString(errorToHandle));
        break;
    }

    jassertfalse;
}
