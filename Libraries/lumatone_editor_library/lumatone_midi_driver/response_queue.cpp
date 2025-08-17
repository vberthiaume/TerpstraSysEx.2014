#include "response_queue.h"

LumatoneResponseQueueReader::LumatoneResponseQueueReader()
{
    reset(1000);
}

void LumatoneResponseQueueReader::timerCallback()
{
    stopTimer();

    juce::MidiBuffer readBuffer;
    removeNextBlockOfMessages(readBuffer, bufferReadSize);
    for (auto event : readBuffer)
    {
        bool configMsg = false;
        auto midiMessage = event.getMessage();
        if (midiMessage.isSysEx())
        {
            auto sysExData = midiMessage.getSysExData();
            auto cmd = sysExData[CMD_ID];
            auto description = FirmwareSupport::getCommandDescription(midiMessage);

            configMsg = cmd < 0x2;

            #if JUCE_DEBUG
                if (verbose > 1)
                    DBG("READ: " + midiMessage.getDescription());
            #endif

            auto errorCode = FirmwareSupport::parseResponseError(sysExData);
            handleResponseError(errorCode, cmd, midiMessage);

            if (sysExData[MSG_STATUS] == 1)
            {
                errorCode = handleResponse(midiMessage);
                handleResponseError(errorCode, cmd, midiMessage);
            }
        }

        // Ignore non-sysex messages
    }

    auto bufferSize = juce::jlimit(0, 999999, readQueueSize.load() - bufferReadSize);
    readQueueSize.store(bufferSize);

    if (bufferSize != 0)
        startTimer(bufferReadTimeoutMs);

    bufferReadRequested = false;
}

void LumatoneResponseQueueReader::midiMessageReceived(juce::MidiInput *source, const juce::MidiMessage& midiMessage)
{
    if (midiMessage.isSysEx())
    {
        addMessageToQueue(midiMessage);
        readQueueSize.store(readQueueSize.load() + 1);

        if (!bufferReadRequested)
        {
            bufferReadRequested = true;
            startTimer(bufferReadTimeoutMs);
        }
    }
}

