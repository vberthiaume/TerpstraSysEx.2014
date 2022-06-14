/*
  ==============================================================================

    TuningDefinition.h
    Created: 8 May 2022 5:25:47pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "Interval.h"

// Describes an interval within a Tuning
class Note
{
protected:

    juce::String name;
    Interval interval;
    int scaleDegree = 0;

public:

    Note() {}

    Note(Interval intervalIn, int degreeIn)
        : interval(intervalIn), scaleDegree(degreeIn) {}

    Note(juce::String noteName, Interval intervalIn, int degreeIn)
        : name(noteName), interval(intervalIn), scaleDegree(degreeIn) {}

    juce::String getName() const { return name; }

    Interval getInterval() const { return interval; }

    int getScaleDegree() const { return scaleDegree; }

    double getIntervalCents() const { return interval.getCents(); }
};

class TuningReference
{
protected:

    double frequency = 261.6255653;
    int channel = 1;
    int note = 60;

public:

    double getFrequency() const { return frequency; }

    int getChannel() const { return channel; }

    int getNote() const { return note; }
};


