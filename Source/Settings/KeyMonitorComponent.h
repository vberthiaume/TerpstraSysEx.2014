/*
  ==============================================================================

    KeyMonitorComponent.h

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../LumatoneFirmwareDefinitions.h"
#include "../TerpstraMidiDriver.h"

#include "../Main.h"

class KeyMonitorComponent  : public juce::Component, private TerpstraMidiDriver::Collector, private juce::Timer
{
public:

    KeyMonitorComponent()
    {
        velocityLabel = std::make_unique<juce::Label>("", "");
        addAndMakeVisible(velocityLabel.get());

        maxVelocityLabel = std::make_unique<juce::Label>("", "");
        addAndMakeVisible(maxVelocityLabel.get());

        noteLabel = std::make_unique<juce::Label>("", "");
        addAndMakeVisible(noteLabel.get());

        channelLabel = std::make_unique<juce::Label>("", "");
        addAndMakeVisible(channelLabel.get());

        updateVelocityMeter();
        setSize(250, 150);

        TerpstraSysExApplication::getApp().getLumatoneController()->getMidiDriver().addMessageCollector(this);
    }

    ~KeyMonitorComponent()
    {
        TerpstraSysExApplication::getApp().getLumatoneController()->getMidiDriver().removeMessageCollector(this);
    }

    float levelToRatio(int level) const
    {
        return (float)level / 127.0;
    }

    void paint(juce::Graphics& g) override
    {
        // Current velocity level
        g.setColour(juce::Colours::blue);
        g.fillRect(velocityMeter.withTop(currentVelocityY));

        // Meter outline
        g.setColour(juce::Colours::black);
        g.drawRect(velocityMeter.toFloat());

        // Max velocity line
        g.setColour(juce::Colours::white);
        g.drawLine(maxVelocityLine, 2.0f);
    }

    void resized() override
    {
        int meterX = 0;
        float meterWidth = juce::roundToInt(getWidth() * 0.2f);
        float meterHeight = getHeight() - 50;
        velocityMeter.setBounds(meterX, 0, meterWidth, meterHeight);

        updateVelocityMeter();

        int labelHeight = 24;
        velocityLabel->setBounds(0, meterHeight, 100, labelHeight);

        // noteLabel->setBounds(velocityMeter.getRight(), velocityMeter.getCentreY(), 100, 48);
        // channelLabel->setBounds(velocityMeter.getRight(), noteLabel->getBottom(), 100, 48);
        noteLabel->setBounds(0, velocityLabel->getBottom(), 60, labelHeight);
        channelLabel->setBounds(noteLabel->getRight(), velocityLabel->getBottom(), 100, labelHeight);
    }

    void setNewVelocityLevel(int newVelocity, bool resetHighest)
    {
        currentVelocity = newVelocity;
        if (resetHighest  || (currentVelocity > highestVelocity))
        {
            highestVelocity = currentVelocity;
        }

        startTimer(2000);
        updateVelocityMeter();
        repaint();
    }

    void midiMessageReceived(MidiInput* source, const MidiMessage& message)
    {
        if (message.isNoteOn())
        {
            bool differentKey = midiNote != message.getNoteNumber() || midiChannel != message.getChannel();
            if (differentKey)
            {
                updateCurrentKey(message.getNoteNumber(), message.getChannel());
            }
            setNewVelocityLevel(message.getVelocity(), differentKey);
        }
    }

    void midiMessageSent(MidiOutput* target, const MidiMessage& message) {}
    void midiSendQueueSize(int size) {}
    void generalLogMessage(String textMessage, HajuErrorVisualizer::ErrorLevel errorLevel) {}
    void noAnswerToMessage(MidiInput* expectedDevice, const MidiMessage& message)  {}


    void timerCallback() override
    {
        stopTimer();
        currentVelocity = 0;
        updateVelocityMeter();
        repaint();
    }

private:

    void updateVelocityMeter()
    {
        currentVelocityY = juce::roundToInt(velocityMeter.getHeight() * (1.0f - levelToRatio(currentVelocity)));
        highestVelocityY = juce::roundToInt(velocityMeter.getHeight() * (1.0f - levelToRatio(highestVelocity)));

        maxVelocityLine.setStart(velocityMeter.getX(), highestVelocityY);
        maxVelocityLine.setEnd(velocityMeter.getRight(), highestVelocityY);

        maxVelocityLabel->setBounds(maxVelocityLine.getEndX() + 8, maxVelocityLine.getEndY(), 100, 48);

        velocityLabel->setText(juce::String("Velocity: ") + juce::String(currentVelocity), juce::NotificationType::dontSendNotification);
        maxVelocityLabel->setText(juce::String("Max: ") + juce::String(highestVelocity), juce::NotificationType::dontSendNotification);
    }

    void updateCurrentKey(int newMidiNote, int newMidiChannel)
    {
        midiNote = newMidiNote;
        midiChannel = newMidiChannel;

        noteLabel->setText(juce::String("Note: ") + juce::String(midiNote), juce::NotificationType::dontSendNotification);
        channelLabel->setText(juce::String("Channel: ") + juce::String(midiChannel), juce::NotificationType::dontSendNotification);
    }

private:

    TerpstraMidiDriver* midiDriver;

    juce::Rectangle<int> velocityMeter;
    std::unique_ptr<juce::Label> velocityLabel;
    juce::Line<float> maxVelocityLine;

    int currentVelocity = 0;
    int highestVelocity = 0;
    std::unique_ptr<juce::Label> maxVelocityLabel;

    int currentVelocityY = 0;
    int highestVelocityY = 0;

    int midiNote = 0;
    std::unique_ptr<juce::Label> noteLabel;

    int midiChannel = 0;
    std::unique_ptr<juce::Label> channelLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyMonitorComponent)
};
