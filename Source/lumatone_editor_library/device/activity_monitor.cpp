/*
  ==============================================================================

    activity_monitor.cpp
    Created: 11 Feb 2021 9:15:43pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "activity_monitor.h"

#include "../lumatone_midi_driver/lumatone_midi_driver.h"
#include "../lumatone_midi_driver/firmware_sysex.h"
#include "../listeners/status_listener.h"
#include "./lumatone_controller.h"


DeviceActivityMonitor::DeviceActivityMonitor(const LumatoneApplicationState& stateIn, LumatoneFirmwareDriver& midiDriverIn)
    :   LumatoneApplicationState("DeviceActivityMonitor", stateIn)
    ,   LumatoneApplicationState::DeviceController(static_cast<LumatoneApplicationState&>(*this))
    ,   firmwareDriver(midiDriverIn)
{
    detectDevicesIfDisconnected = getBoolProperty(LumatoneApplicationProperty::DetectDeviceIfDisconnected, true);
    checkConnectionOnInactivity = getBoolProperty(LumatoneApplicationProperty::CheckConnectionIfInactive, true);
    responseTimeoutMs = getIntProperty(LumatoneApplicationProperty::DetectDevicesTimeout, detectRoutineTimeoutMs);

    firmwareDriver.addDriverListener(this);
}

DeviceActivityMonitor::~DeviceActivityMonitor()
{
    firmwareDriver.removeDriverListener(this);
}

void DeviceActivityMonitor::setDetectDeviceIfDisconnected(bool doDetection)
{
    detectDevicesIfDisconnected = doDetection;
    setStateProperty(LumatoneApplicationProperty::DetectDeviceIfDisconnected, detectDevicesIfDisconnected);

    if (!detectDevicesIfDisconnected)
    {
        deviceConnectionMode = DetectConnectionMode::idle;
        firmwareDriver.closeTestingDevices();
    }
    else
    {
        startTimer(detectRoutineTimeoutMs);
    }
}

void DeviceActivityMonitor::setCheckForInactivity(bool monitorActivity)
{
    checkConnectionOnInactivity = monitorActivity;
    setStateProperty(LumatoneApplicationProperty::CheckConnectionIfInactive, checkConnectionOnInactivity);

    if (checkConnectionOnInactivity && isConnectionEstablished())
    {
        startTimer(inactivityTimeoutMs);
    }
}

void DeviceActivityMonitor::pingAllDevices()
{
    DBG("Ping all devices!");

    deviceDetectInProgress = true;
    checkingDeviceIsLumatone = true;
    waitingForResponse = true;

    outputDevices = firmwareDriver.getMidiOutputList();
    int maxDevices = juce::jmin(outputDevices.size(), 128);
    if (firmwareDriver.getHostMode() == LumatoneFirmwareDriver::HostMode::Plugin)
        maxDevices = 1;

    outputPingIds.clear();
    for (int i = 0; i < maxDevices; i++)
    {
        unsigned int id = (unsigned int)(i + 1);
        firmwareDriver.ping(id, i);
        outputPingIds.add(id);
    }

    startTimer(responseTimeoutMs);
}

// bool DeviceActivityMonitor::testLastConnectedDevice()
// {
//     juce::String inputId = getStringProperty(LumatoneApplicationProperty::LastInputDeviceId);
//     if (inputId.length() <= 0)
//         return false;

//     int inputIndex = midiDriver->findIndexOfInputDevice(inputId);
//     if (inputIndex >= 0)
//     {
//         juce::String outputId = getStringProperty(LumatoneApplicationProperty::LastOutputDeviceId);
//         if (outputId.length() <= 0)
//             return false;

//         int outputIndex = midiDriver->findIndexOfOutputDevice(outputId);
//         if (outputIndex >= 0)
//         {
//             deviceDetectInProgress = true;
//             waitingForResponse = true;

//             DBG("Testing last connected device");
//             midiDriver->setMidiInput(inputIndex);
//             midiDriver->setMidiOutput(outputIndex);
//             midiDriver->sendGetSerialIdentityRequest();
//             return true;
//         }
//     }

//     return false;
// }

void DeviceActivityMonitor::startIndividualDetection()
{
    jassert(outputPingIds.size() == 0);

    testOutputIndex = -1;
    outputDevices = firmwareDriver.getMidiOutputList();
    inputDevices = firmwareDriver.getMidiInputList();

    if (outputDevices.size() > 0 && inputDevices.size() > 0)
    {
        deviceDetectInProgress = true;
        testNextOutput();
    }
    else
    {
        DBG("No input and output MIDI device combination could be made.");
        deviceDetectInProgress = false;
        startTimer(detectRoutineTimeoutMs);
    }
}

void DeviceActivityMonitor::testNextOutput()
{
    checkingDeviceIsLumatone = true;

    if (firmwareDriver.getHostMode() == LumatoneFirmwareDriver::HostMode::Driver)
    {
        testOutputIndex++;

        if (testOutputIndex < 0)
            testOutputIndex = 0;

        if (testOutputIndex >= outputDevices.size())
        {
            startTimer(10);
            return;
        }
    }

    #if JUCE_DEBUG
        juce::String deviceName = firmwareDriver.getHostMode() == LumatoneFirmwareDriver::HostMode::Driver ? outputDevices[testOutputIndex].name : "Host";
        juce::String command = sendCalibratePitchModOff ? "Calibrate" : "GetSerial";
        DBG("Test via " + command + ": " + deviceName);
    #endif

    testCurrentOutput();
}

void DeviceActivityMonitor::startDeviceDetection()
{
    // if (midiDriver->getHostMode() != LumatoneFirmwareDriver::HostMode::Driver)
    //     return; // Only allow in Driver mode

    // Belongs somewhere else?
    if (!firmwareDriver.hasDevicesDefined())
    {
        confirmedInputIndex = -1;
        confirmedOutputIndex = -1;
    }

    if (detectDevicesIfDisconnected)
    {
        deviceConnectionMode = DetectConnectionMode::lookingForDevice;
        deviceDetectInProgress = false; // Don't start until first test response is sent
        startTimer(10);
    }
}

void DeviceActivityMonitor::startActivityMonitoring()
{
    if (firmwareDriver.getHostMode() != LumatoneFirmwareDriver::HostMode::Driver)
        return; // Only allow in Driver mode

    deviceConnectionMode = DetectConnectionMode::waitingForInactivity;
    startTimer(inactivityTimeoutMs);
}

void DeviceActivityMonitor::stopDeviceDetection()
{
    deviceConnectionMode = DetectConnectionMode::idle;
    deviceDetectInProgress = false;
}

void DeviceActivityMonitor::stopMonitoringDevice()
{
    stopTimer();
    waitingForResponse = false;
    deviceConnectionMode = DetectConnectionMode::noDeviceMonitoring;
}

void DeviceActivityMonitor::setMidiInput(int deviceIndex, bool test)
{
    // const bool changed = firmwareDriver.getMidiInputIndex() != deviceIndex;
    firmwareDriver.setMidiInput(deviceIndex);

    // if (changed)
    //     currentDevicePairConfirmed = false;

    if (test && deviceIndex >= 0)
    {
        onDisconnection();
        checkingDeviceIsLumatone = true;
        testCurrentOutput();
    }
    else if (deviceIndex < 0)
    {
        onDisconnection();
    }
}

void DeviceActivityMonitor::setMidiOutput(int deviceIndex, bool test)
{
    // const bool changed = firmwareDriver.getMidiOutputIndex() != deviceIndex;
    firmwareDriver.setMidiOutput(deviceIndex);

    // if (changed)
    //     currentDevicePairConfirmed = false;

    if (test && deviceIndex >= 0)
    {
        onDisconnection();
        checkingDeviceIsLumatone = true;
        testCurrentOutput();
    }
    else if (deviceIndex < 0)
    {
        onDisconnection();
    }
}

bool DeviceActivityMonitor::initializeConnectionTest()
{
    bool isIdle = !firmwareDriver.isWaitingForResponse();
    if (isIdle)
    {
        testCurrentOutput();
        startTimer(inactivityTimeoutMs);
    }

    return isIdle;
}

int DeviceActivityMonitor::getPingIdFromResponse(const juce::MidiMessage &msg)
{
    unsigned int pingId = 0;

    auto errorCode = LumatoneSysEx::unpackPingResponse(msg, pingId);

    if (errorCode != FirmwareSupport::Error::noError)
    {
        if (errorCode == FirmwareSupport::Error::messageIsAnEcho)
            return -1;

        DBG("WARNING: Ping response error in auto connection routine detected");
        jassertfalse;
    }

    return (int)pingId - 1;
}

void DeviceActivityMonitor::removeFailedPingDevice(const juce::MidiMessage &msg)
{
    unsigned int pingId = 0;
    LumatoneSysEx::unpackPingResponse(msg, pingId);
    outputDevices.remove(outputPingIds.indexOf(pingId));
}

void DeviceActivityMonitor::checkDetectionStatus()
{
    // Successful
    if (isConnectionEstablished())
    {
        deviceDetectInProgress = false;
        outputPingIds.clear();

        if (!waitingForFirmwareVersion && checkConnectionOnInactivity)
            startActivityMonitoring();
        else
            deviceConnectionMode = DetectConnectionMode::noDeviceMonitoring;

        return;
    }

    if (firmwareDriver.getHostMode() == LumatoneFirmwareDriver::HostMode::Plugin)
    {
        if (deviceDetectInProgress)
        {
            DBG("Detect device timeout.");
            deviceDetectInProgress = false;
            waitingForResponse = false;
            startTimer(detectRoutineTimeoutMs);

            getStatusListeners()->call(&LumatoneEditor::StatusListener::connectionFailed);
        }
        else
        {
            deviceDetectInProgress = true;
            if (sendCalibratePitchModOff)
            {
                firmwareDriver.sendCalibratePitchModWheel(false, testOutputIndex);
            }
            else
            {
                firmwareDriver.sendGetSerialIdentityRequest(testOutputIndex);
            }

            waitingForResponse = true;
            startTimer(responseTimeoutMs);
        }
    }
    else if (firmwareDriver.getHostMode() == LumatoneFirmwareDriver::HostMode::Driver)
    {
        if (deviceDetectInProgress)
        {
            // Failed last-device test
            if (firmwareDriver.hasDevicesDefined())
            {
                DBG("Unable to connect to last device.");
                waitingForResponse = false;
                firmwareDriver.closeMidiInput();
                firmwareDriver.closeMidiOutput();
                pingAllDevices();
            }

            // Failed ping routine
            else if (outputPingIds.size() > 0)
            {
                waitingForResponse = false;
                outputPingIds.clear();
                startIndividualDetection();
            }

            // Ongoing GetSerial routine
            else if (testOutputIndex >= 0 && testOutputIndex < outputDevices.size())
            {
                testNextOutput();
            }

            // Set timeout for next attempt
            else
            {
                DBG("Detect device timeout.");
                deviceDetectInProgress = false;
                waitingForResponse = false;
                startTimer(detectRoutineTimeoutMs);

                getStatusListeners()->call(&LumatoneEditor::StatusListener::connectionFailed);
            }
        }

        // Start detection
        else
        {
            firmwareDriver.refreshDeviceLists();
            firmwareDriver.openAvailableDevicesForTesting();
            deviceConnectionMode = DetectConnectionMode::lookingForDevice;

            // If there's no last-connected-device, skip to pinging
            //if (!testLastConnectedDevice())
            //{
                pingAllDevices();
            //}
        }
    }
}

void DeviceActivityMonitor::testCurrentOutput()
{
    if (sendCalibratePitchModOff)
    {
        firmwareDriver.sendCalibratePitchModWheel(false, testOutputIndex);
    }
    else
    {
        getLumatoneController()->testCurrentDeviceConnection();
    }

    waitingForResponse = true;
    startTimer(responseTimeoutMs);
}

bool DeviceActivityMonitor::handleSerialIdentityResponse(const juce::MidiMessage& msg)
{
    int serialBytes[6];
    auto errorCode = LumatoneSysEx::unpackGetSerialIdentityResponse(msg, serialBytes);
    if (errorCode != FirmwareSupport::Error::noError)
        return false;

    waitingForResponse = false;

    juce::String readSerial = firmwareSupport.serialIdentityToString(serialBytes);
    if (confirmedSerial != readSerial)
    {
        confirmedVersion = LumatoneFirmware::ReleaseVersion::NO_VERSION;
        waitingForFirmwareVersion = true;
    }
    confirmedSerial = readSerial;

    if (waitingForFirmwareVersion)
    {
        if (confirmedSerial == SERIAL_55_KEYS)
        {
            confirmedVersion = LumatoneFirmware::ReleaseVersion::VERSION_55_KEYS;
            waitingForFirmwareVersion = false;
        }
        else
        {
            waitingForResponse = true;
            firmwareDriver.sendGetFirmwareRevisionRequest();
        }
    }

    return true;
}


void DeviceActivityMonitor::timerCallback()
{
    stopTimer();

    switch (deviceConnectionMode)
    {
    case DetectConnectionMode::idle:
    case DetectConnectionMode::lookingForDevice:
        if (detectDevicesIfDisconnected)
        {
            checkDetectionStatus();
            break;
        }

        stopDeviceDetection();
        break;

    case DetectConnectionMode::noDeviceMonitoring:
    case DetectConnectionMode::waitingForInactivity:
        if (!checkConnectionOnInactivity)
        {
            stopMonitoringDevice();
            break;
        }

        if (!waitingForResponse)
        {
            if (getSendQueueSize() > 0)
            {
                // Skip test because there's already a message being sent
                startTimer(inactivityTimeoutMs);
                break;
            }

            if (deviceConnectionMode == DetectConnectionMode::noDeviceMonitoring)
                deviceConnectionMode = DetectConnectionMode::waitingForInactivity;

            initializeConnectionTest();
        }
        else
        {
            DBG("DAM: testing connection...");
        }

        break;

    default:
        jassertfalse;
    }
}

void DeviceActivityMonitor::handleResponse(int inputDeviceIndex, const juce::MidiMessage& msg)
{
    if (!msg.isSysEx())
        return;

    auto sysExData = msg.getSysExData();
    auto cmd = sysExData[CMD_ID];
    auto description = FirmwareSupport::getCommandDescription(msg);

    if (cmd == PERIPHERAL_CALBRATION_DATA && !isConnectionEstablished())
    {
        DBG("DAM: Unexpected calibration data received");
        sendCalibratePitchModOff = true;
        // startTimer(100);
        return;
    }

    if (waitingForResponse) switch (sysExData[MSG_STATUS])
    {
        // Ignore or mark as a failed ping
        case TEST_ECHO:
        {
            DBG("DAM ignoring feedback from device " + juce::String(inputDeviceIndex) + " with message: " + msg.getDescription());
            if (cmd == LUMA_PING)
                removeFailedPingDevice(msg);
            return;
        }

        case LumatoneFirmware::ReturnCode::ACK:
        {
            int establishOutIndex = -1;

            switch (cmd)
            {
            case GET_SERIAL_IDENTITY:
                if (handleSerialIdentityResponse(msg) && !isConnectionEstablished())
                    establishOutIndex = testOutputIndex;
                break;

            case CALIBRATE_PITCH_MOD_WHEEL:
                if (sysExData[PAYLOAD_INIT] != TEST_ECHO)
                {
                    if (!isConnectionEstablished())
                    {
                        sendCalibratePitchModOff = true;
                        jassertfalse;
                    }
                    // waitingForResponse = false; // ??
                }
                break;

            case GET_FIRMWARE_REVISION:
                waitingForFirmwareVersion = false;
                waitingForResponse = false;
                break;

            case LUMA_PING:
                if (outputPingIds.size() == 0)
                    break;

                establishOutIndex = getPingIdFromResponse(msg);
                waitingForResponse = false;
                break;

            default:
                break;
            }

            if (sendCalibratePitchModOff)
            {
                sendCalibratePitchModOff = false;
                juce::MessageManager::callAsync([&]() { checkDetectionStatus(); });
                return;
            }

            switch (deviceConnectionMode)
            {
            case DetectConnectionMode::lookingForDevice:
                if (establishOutIndex >= 0)
                {
                    establishConnection(inputDeviceIndex, establishOutIndex);
                }
                break;

            case DetectConnectionMode::waitingForInactivity:
                if (!waitingForFirmwareVersion)
                    startTimer(inactivityTimeoutMs);
                break;

            default:
                break;
            }

            break;
        }

        case LumatoneFirmware::ReturnCode::STATE:
        {
            // TODO set warning flag ?
            break;
        }
    }

    // Edge case if we're disconnected but get a response
    else if (!isConnectionEstablished())
    {
        establishConnection(firmwareDriver.getMidiInputIndex(), firmwareDriver.getMidiOutputIndex());
    }
    else if (!waitingForFirmwareVersion && checkConnectionOnInactivity)
    {
        startTimer(inactivityTimeoutMs);
    }
}

//=========================================================================
// LumatoneFirmwareDriver::Listener Implementation

void DeviceActivityMonitor::midiMessageReceived(juce::MidiInput* source, const juce::MidiMessage& msg)
{
    if (!msg.isSysEx())
        return;

    int deviceIndex = (source == nullptr) ? -1
                                          : firmwareDriver.getMidiInputList().indexOf(source->getDeviceInfo());

    stopTimer();
    handleResponse(deviceIndex, msg);
}

void DeviceActivityMonitor::noAnswerToMessage(juce::MidiDeviceInfo expectedDevice, const juce::MidiMessage& midiMessage)
{
    stopTimer();

    if (waitingForResponse && deviceConnectionMode < DetectConnectionMode::noDeviceMonitoring)
    {
        waitingForResponse = false;
        auto sysExData = midiMessage.getSysExData();
        if (sysExData[CMD_ID] == LUMA_PING && outputPingIds.size() > 0)
        {
            removeFailedPingDevice(midiMessage);
        }
        else
        {
            startTimer(threadDelayMs);
        }
    }

    else if (isConnectionEstablished())
    {
        onDisconnection();
    }
}

void DeviceActivityMonitor::onDisconnection(bool redetectIfEnabled)
{
    DBG("DISCONNECTION DETECTED");

    confirmedInputIndex = -1;
    confirmedOutputIndex = -1;

    waitingForResponse = false;

    setConnectionState(ConnectionState::DISCONNECTED);

    if (redetectIfEnabled && detectDevicesIfDisconnected)
    {
        startDeviceDetection();
    }
    else
    {
        deviceConnectionMode = DetectConnectionMode::idle;
        stopTimer();
    }
}

void DeviceActivityMonitor::establishConnection(int inputIndex, int outputIndex)
{
    stopTimer();

    DBG("DAM: Devices confirmed.");

    if (firmwareDriver.hasDevicesDefined())
    {
        confirmedOutputIndex = firmwareDriver.getMidiOutputIndex();
        confirmedInputIndex = firmwareDriver.getMidiInputIndex();

        DBG("\tInput: " + firmwareDriver.getMidiInputInfo().name);
        DBG("\tOutput: " + firmwareDriver.getMidiOutputInfo().name);
    }
    else if (firmwareDriver.getHostMode() == LumatoneFirmwareDriver::HostMode::Driver)
    {
        firmwareDriver.setMidiInput(inputIndex);
        confirmedInputIndex = inputIndex;

        firmwareDriver.setMidiOutput(outputIndex);
        confirmedOutputIndex = outputIndex;

        DBG("\tInput: " + firmwareDriver.getMidiInputInfo().name);
        DBG("\tOutput: " + firmwareDriver.getMidiOutputInfo().name);
    }
    else
    {
        confirmedInputIndex = 0;
        confirmedOutputIndex = 0;

        DBG("\tPlugin host mode.");
    }

    checkingDeviceIsLumatone = false;

    if (confirmedSerial.isEmpty())
    {
        waitingForResponse = true;
        firmwareDriver.sendGetSerialIdentityRequest();
    }
    else if (confirmedSerial != SERIAL_55_KEYS)
    {
        waitingForFirmwareVersion = true;
        waitingForResponse = true;
        firmwareDriver.sendGetFirmwareRevisionRequest();
    }
    else
    {
        waitingForFirmwareVersion = false;
    }

    setConnectionState(ConnectionState::ONLINE);

    startTimer(threadDelayMs);
}
