#pragma once

#include "./data/lumatone_layout.h"

#include "TerpstraMidiDriver.h"

class LumatoneKeyUpdateBuffer : private juce::Timer
{
public:

    LumatoneKeyUpdateBuffer(TerpstraMidiDriver& midiDriver);
    ~LumatoneKeyUpdateBuffer() override;

    void sendKeyConfig(int boardId, int keyIndex, const LumatoneKey& noteDataConfig, bool signalEditorListeners = true);
    void sendKeyConfig(int boardIndex, int keyIndex, int noteOrCCNum, int channel, int keyType, bool faderUpIsNull = true);

    void sendKeyColourConfig(int boardId, int keyIndex, juce::Colour colour, bool signalEditorListeners = true);

    void timerCallback() override;

private:

    int getKeyNum(int boardIndex, int keyIndex) const;
    LumatoneKeyCoord getKeyCoord(int keyNum) const;

    void updateKeyConfig(int boardIndex, int keyIndex, const LumatoneKey& config);
    void updateKeyColour(int boardIndex, int keyIndex, juce::Colour colour);

private:

    juce::CriticalSection lock;

    TerpstraMidiDriver& midiDriver;

    juce::HashMap<int, MappedLumatoneKey> keysToUpdate;

    LumatoneLayout preUpdateLayout;

    int updateMs = 300;
};
