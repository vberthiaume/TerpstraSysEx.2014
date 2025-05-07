/*
  ==============================================================================

    TuningTable.h
    Created: 8 May 2022 4:19:29pm
    Author:  Vincenzo

    Classes related to tunings

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "./Interval.h"
#include "./TuningDefinition.h"
#include "./NotationTable.h"

class TuningTable
{
public:
    class Pitch : private Note
    {
        Interval value;

        juce::String pitchName;

    public:

        Pitch(Note noteIn, double periodCents, int numberOfPeriods, juce::String pitchName=juce::String()) 
            : Note(noteIn)
        {
            double cents = noteIn.getIntervalCents() + periodCents * numberOfPeriods;
            value = Interval::CentsInterval(cents);
        }

        Pitch(Note noteIn, Note period, int numberOfPeriods, juce::String pitchName = juce::String())
            : Pitch(noteIn, period.getIntervalCents(), numberOfPeriods, pitchName) {}

        Note getNote() const { return Note(name, interval, scaleDegree); }

        Interval getValue() const { return value; }

        double getCents() const { return value.getCents(); }

        juce::String getNoteName() const { return name; }

        juce::String getPitchName() const { return pitchName; }
    }; 

private:
        
    TuningReference reference;
    juce::Array<Note> notes;
    Note period;

    int scaleSize = 0;

    juce::String name;
    juce::String description;

    NotationTable* notation = nullptr;

    juce::String error;

private:

    Note initializeNotes(juce::Array<Note>& noteList);

public:

    // Note list should start on first interval and end on period
    TuningTable(
        TuningReference tuningReference,
        juce::Array<Note> noteList,
        juce::String nameIn = juce::String(),
        juce::String descriptionIn = juce::String());

    TuningReference getReference() const { return reference; }

    double getReferenceFrequency() const { return reference.getFrequency(); }

    juce::String getName() const { return name; }

    juce::String getDescription() const { return description; }

    Note getPeriod() const { return period; }

    double getPeriodCents() const { return period.getIntervalCents(); }

    bool hasError() const { return error.isNotEmpty(); }

    juce::String getError() const { return error; }


    Note getScaleDegree(int degree) const;

    Pitch getPitch(int index) const;

    juce::String getPitchName(int index) const;
};
