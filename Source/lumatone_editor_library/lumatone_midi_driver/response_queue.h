#ifndef LUMATONE_RESPONSE_QUEUE_H
#define LUMATONE_RESPONSE_QUEUE_H

#include "./firmware_driver_listener.h"
#include "./firmware_support.h"

class LumatoneResponseQueueReader : public LumatoneFirmwareDriverListener
                                  , protected juce::MidiMessageCollector
                                  , public juce::Timer
{
public:

    LumatoneResponseQueueReader();
    virtual ~LumatoneResponseQueueReader() { }

    virtual FirmwareSupport::Error handleResponse(const juce::MidiMessage& msg) = 0;
    virtual void handleResponseError(FirmwareSupport::Error errorToHandle, int commandReceived, const juce::MidiMessage& msg) { }

    // juce::Timer implementation
    virtual void timerCallback() override;

protected:

    virtual void midiMessageReceived(juce::MidiInput* source, const juce::MidiMessage& message) override;

private:

    const int                   bufferReadTimeoutMs = 30;
    const int                   bufferReadSize = 16;
    bool                        bufferReadRequested = false;
    std::atomic<int>            readQueueSize;

    int verbose = 0;
};

#endif // LUMATONE_RESPONSE_QUEUE_H
