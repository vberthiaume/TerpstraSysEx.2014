/*
  ==============================================================================

    TuningTable.cpp
    Created: 8 May 2022 4:19:29pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "TuningTable.h"

TuningTable::TuningTable(
    TuningReference tuningReference,
    juce::Array<Note> noteList,
    juce::String nameIn,
    juce::String descriptionIn)
    : 
    reference(tuningReference),
    period(initializeNotes(noteList)),
    name(nameIn),
    description(descriptionIn)
{

}

Note TuningTable::initializeNotes(juce::Array<Note>& noteList)
{
    bool validScale = true;

    // Check if scale degrees make sense
    for (int i = 0; i < noteList.size(); i++)
    {
        auto note = noteList[i];
        if (note.getScaleDegree() != (i + 1))
        {
            validScale = false;
            error = juce::String("Expected scale degree ") + juce::String(i + 1) + " but got " + juce::String(note.getScaleDegree());
            break;
        }
    }

    if (validScale)
        notes = juce::Array<Note>(noteList.data(), noteList.size() - 1);

    scaleSize = noteList.size();

    return noteList.getLast();
}

Note TuningTable::getScaleDegree(int degree) const
{
    int d = negativeAwareModulo(degree, notes.size());
    return notes[d];
}

TuningTable::Pitch TuningTable::getPitch(int index) const
{
    auto note = getScaleDegree(index);
    int periodNum = index / scaleSize;

    juce::String name;
    if (notation != nullptr)
        name = notation->getNoteNameString(note.getScaleDegree());

    return Pitch(note, period, periodNum, name);
}

juce::String TuningTable::getPitchName(int index) const
{
    auto degree = negativeAwareModulo(index, scaleSize);
    int periodNum = index / scaleSize;

    if (notation != nullptr)
        return notation->getPitchNameString(degree, periodNum);

    return juce::String();
}
