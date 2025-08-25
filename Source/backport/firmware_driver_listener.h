/*
  ==============================================================================

    firmware_driver_listener.h
    Created: 30 Sep 2023
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_FIRMWARE_DRIVER_LISTENER_H
#define LUMATONE_FIRMWARE_DRIVER_LISTENER_H

#include <JuceHeader.h>

class LumatoneFirmwareDriverListener
{
    juce::MidiBuffer messagesSentQueue;

public:
    virtual ~LumatoneFirmwareDriverListener() {}
    
    // Called asynchronously on message thread
    virtual void midiMessageReceived(juce::MidiInput* source, const juce::MidiMessage& message) = 0;

    // Called asynchronously on message thread
    virtual void midiMessageSent(juce::MidiOutput* target, const juce::MidiMessage& message) { }


    virtual void midiSendQueueSize(int size) { sendQueueSize = size; }
    // virtual void generalLogMessage(juce::String textMessage, ErrorLevel errorLevel) {}

    // Realtime message - not for heavy processing!
    virtual void noAnswerToMessage(juce::MidiDeviceInfo expectedDevice, const juce::MidiMessage& message) { }


    int getSendQueueSize() { return sendQueueSize; }

protected:
    int sendQueueSize = 0;
};

#endif
