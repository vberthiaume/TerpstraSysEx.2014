/*
  ==============================================================================

    lumatone_event_manager.h
    Created: 5 Jun 2023 8:19:04pm
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_EVENT_MANAGER_H
#define LUMATONE_EVENT_MANAGER_H

#include "../data/application_state.h"
#include "../midi/lumatone_midi_state.h"
#include "../lumatone_midi_driver/response_queue.h"

class LumatoneEventManager : public LumatoneApplicationState
                           , private LumatoneApplicationState::Controller
                           , public LumatoneResponseQueueReader
{

public:
    LumatoneEventManager(const LumatoneApplicationState& stateIn, LumatoneFirmwareDriver& midiDriver);
    ~LumatoneEventManager() override;

protected:
    //============================================================================
    // LumatoneResponseQueue implementation
    FirmwareSupport::Error handleResponse(const juce::MidiMessage& midiMessage) override;
    void handleResponseError(FirmwareSupport::Error errorToHandle, int commandReceived, const juce::MidiMessage& msg) override;

private:
    //============================================================================
    // Lower-level message unpacking and handling

    FirmwareSupport::Error handleOctaveConfigResponse(
        const juce::MidiMessage& midiMessage,
        std::function <FirmwareSupport::Error(const juce::MidiMessage&, int&, juce::uint8, int*)> unpackFunction,
        std::function <void(int, void*)> callbackFunctionIfNoError = {}
    );

    FirmwareSupport::Error handleTableConfigResponse(
        const juce::MidiMessage& midiMessage,
        std::function<FirmwareSupport::Error(const juce::MidiMessage&, int*)> unpackFunction,
        std::function <void(void*)> callbackFunctionIfNoError = {}
    );

    FirmwareSupport::Error handleLEDConfigResponse(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handleChannelConfigResponse(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handleNoteConfigResponse(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handleKeyTypeConfigResponse(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handleVelocityConfigResponse(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handleAftertouchConfigResponse(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handleVelocityIntervalConfigResponse(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handleFaderConfigResponse(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handleFaderTypeConfigResponse(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handleSerialIdentityResponse(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handleFirmwareRevisionResponse(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handleLumatouchConfigResponse(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handlePingResponse(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handlePeripheralCalibrationData(const juce::MidiMessage& midiMessage);
    FirmwareSupport::Error handleExpressionPedalCalibrationData(const juce::MidiMessage& midiMessage);
    FirmwareSupport::Error handleWheelsCalibrationData(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handleGetPeripheralChannelResponse(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handleGetPresetFlagsResponse(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handleGetExpressionPedalSensitivityResponse(const juce::MidiMessage& midiMessage);

    FirmwareSupport::Error handleGetMacroLightIntensityResponse(const juce::MidiMessage& midiMessage);

private:
    LumatoneFirmwareDriver&     midiDriver;
};

#endif // LUMATONE_EVENT_MANAGER_H
