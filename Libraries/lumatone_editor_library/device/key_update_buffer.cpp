#include "key_update_buffer.h"
#include "../lumatone_midi_driver/lumatone_midi_driver.h"

LumatoneKeyUpdateBuffer::LumatoneKeyUpdateBuffer(LumatoneFirmwareDriver& driverIn, LumatoneState state)
    : firmwareDriver(driverIn)
    , LumatoneState("LumatoneKeyUpdateBuffer", state)
{
    // updatedLayout.remapTable(280);

    for (int b = 0; b < MAX_LUMATONE_BOARDS; b++)
    {
        if (b < getNumBoards())
        {
            const LumatoneBoard& board = getBoard(b);
            for (int k = 0; k < MAX_LUMATONE_BOARD_KEYS; k++)
            {
                if (k < board.getNumKeys())
                {
                    const LumatoneKey& key = board.getKey(k);
                    int index = b * MAX_LUMATONE_BOARD_KEYS + k;
                    keysToUpdate[index] = nullptr;
                    lastUpdatedKeys[index] = LumatoneKey();
                }
            }
        }
    }
}

LumatoneKeyUpdateBuffer::~LumatoneKeyUpdateBuffer()
{

}

int LumatoneKeyUpdateBuffer::getKeyNum(int boardIndex, int keyIndex) const
{
    return boardIndex * getOctaveBoardSize() + keyIndex;
}

LumatoneKeyCoord LumatoneKeyUpdateBuffer::getKeyCoord(int keyNum) const
{
    return LumatoneKeyCoord(keyNum / getOctaveBoardSize(), keyNum % getOctaveBoardSize());
}

void LumatoneKeyUpdateBuffer::sendKeyConfig(int boardId, int keyIndex, const LumatoneKey& noteDataConfig, bool signalEditorListeners)
{
    updateKeyConfig(boardId - 1, keyIndex, noteDataConfig);
}

void LumatoneKeyUpdateBuffer::sendKeyColourConfig(int boardId, int keyIndex, juce::Colour colour, bool signalEditorListeners)
{
    updateKeyColour(boardId - 1, keyIndex, colour);
}

void LumatoneKeyUpdateBuffer::updateKeyConfig(int boardIndex, int keyIndex, const LumatoneKey& config)
{
    // juce::ScopedLock l(lock);
    // while (!lock.tryEnter()) {}

    const LumatoneKey& currentKey = getKey(boardIndex, keyIndex);

    int index = boardIndex * MAX_LUMATONE_BOARD_KEYS + keyIndex;
    const LumatoneKey& lastUpdatedKey = lastUpdatedKeys[index];
    const LumatoneKey** keyPtr = &keysToUpdate[index];

    // if (/*currentKey == config && */lastUpdatedKey == config)
    // {
    //     *keyPtr = nullptr;
    // }
    // else
    // {
        *keyPtr = &currentKey;
    // }

    if (!isTimerRunning())
    {
        // processUpdates();
        startTimer(updateMs);
    }
}

void LumatoneKeyUpdateBuffer::updateKeyColour(int boardIndex, int keyIndex, juce::Colour colour)
{
    // juce::ScopedLock l(lock);
    // while (!lock.tryEnter()) {}

    const LumatoneKey& currentKey = getKey(boardIndex, keyIndex);

    int index = boardIndex * MAX_LUMATONE_BOARD_KEYS + keyIndex;
    const LumatoneKey& lastUpdatedKey = lastUpdatedKeys[index];
    const LumatoneKey** keyPtr = &keysToUpdate[boardIndex * MAX_LUMATONE_BOARD_KEYS + keyIndex];

    // if (/*currentKey.isColour(colour) &&*/ lastUpdatedKey.isColour(colour))
    // {
    //     *keyPtr = nullptr;
    // }
    // else
    // {
        *keyPtr = &currentKey;
    // }

    if (!isTimerRunning())
    {
        // processUpdates();
        startTimer(updateMs);
    }
}

void LumatoneKeyUpdateBuffer::processUpdates()
{
    // DBG("<<<< process updates >>>>");

    for (int board = 0; board < MAX_LUMATONE_BOARDS; board++)
    {
        int boardId = board + 1;
        for (int keyIndex = 0; keyIndex < MAX_LUMATONE_BOARD_KEYS; keyIndex++)
        {
            int index = board * MAX_LUMATONE_BOARD_KEYS + keyIndex;
            const LumatoneKey** key = &keysToUpdate[index];
            if (key == nullptr || *key == nullptr)
                continue; 

            const LumatoneKey& keyUpdate = **key;

            // const LumatoneKey& currentKey = getKey(board, keyIndex);
            const LumatoneKey& lastUpdatedKey = lastUpdatedKeys[index];
            if (!lastUpdatedKey.configIsEqual(keyUpdate))
                firmwareDriver.sendKeyFunctionParameters(boardId, keyIndex, keyUpdate.getMidiNumber(), keyUpdate.getMidiChannel(), keyUpdate.getType(), keyUpdate.isCCFaderDefault());

            if (!lastUpdatedKey.colourIsEqual(keyUpdate))
            {
                auto colour = keyUpdate.getColour();
                if (getLumatoneVersion() >= LumatoneFirmware::ReleaseVersion::VERSION_1_0_11)
                    firmwareDriver.sendKeyLightParameters(boardId, keyIndex, colour.getRed(), colour.getGreen(), colour.getBlue());
                else
                    firmwareDriver.sendKeyLightParameters_Version_1_0_0(boardId, keyIndex, colour.getRed() * 0.5f, colour.getGreen() * 0.5f, colour.getBlue() * 0.5f);
            }

            *key = nullptr;
            lastUpdatedKeys[index] = keyUpdate;
        }
    }
}

void LumatoneKeyUpdateBuffer::handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier &property)
{
    LumatoneState::handleStatePropertyChange(stateIn, property);

    // juce::ValueTree lumatoneStateTree = state.getChildWithName(LumatoneConfigProperty::State);
    // if (stateIn ==  lumatoneStateTree || stateIn.isAChildOf(lumatoneStateTree))
    // {
    //     // layout updated

    //     DBG("key update buffer layout update");
    // }
}

void LumatoneKeyUpdateBuffer::timerCallback()
{
    stopTimer();

    if (! lock.tryEnter())
        return;

    // DBG(">>>> buffer timer callback <<<<");
    processUpdates();
}
