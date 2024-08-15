#ifndef LUMATONE_UPDATE_BUFFER_H

#include "../data/lumatone_state.h"

class LumatoneFirmwareDriver;

class LumatoneKeyUpdateBuffer : public LumatoneState,
                                private juce::Timer
{
public:

    LumatoneKeyUpdateBuffer(LumatoneFirmwareDriver& firmwareDriver, LumatoneState state);
    ~LumatoneKeyUpdateBuffer() override;

    void sendKeyConfig(int boardId, int keyIndex, const LumatoneKey& noteDataConfig, bool signalEditorListeners = true);
    void sendKeyColourConfig(int boardId, int keyIndex, juce::Colour colour, bool signalEditorListeners = true);

    void timerCallback() override;

private:

    int getKeyNum(int boardIndex, int keyIndex) const;
    LumatoneKeyCoord getKeyCoord(int keyNum) const;

    void updateKeyConfig(int boardIndex, int keyIndex, const LumatoneKey& config);
    void updateKeyColour(int boardIndex, int keyIndex, juce::Colour colour);

    void processUpdates();

private:

    void handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier& property) override;

private:

    juce::CriticalSection lock;

    LumatoneFirmwareDriver& firmwareDriver;

    const LumatoneKey* keysToUpdate[MAX_LUMATONE_KEYS];
    LumatoneKey lastUpdatedKeys[MAX_LUMATONE_KEYS];

    int updateMs = 700;
};

#endif LUMATONE_UPDATE_BUFFER_H
