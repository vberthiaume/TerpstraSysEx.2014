#include "key_update_buffer.h"
#include "Main.h"

LumatoneKeyUpdateBuffer::LumatoneKeyUpdateBuffer(TerpstraMidiDriver& driverIn)
    : midiDriver(driverIn)
{
    keysToUpdate.remapTable(280);
}

LumatoneKeyUpdateBuffer::~LumatoneKeyUpdateBuffer()
{
    
}

int LumatoneKeyUpdateBuffer::getKeyNum(int boardIndex, int keyIndex) const
{
    return boardIndex * TerpstraSysExApplication::getApp().getOctaveBoardSize() + keyIndex;
}

LumatoneKeyCoord LumatoneKeyUpdateBuffer::getKeyCoord(int keyNum) const
{
    return LumatoneKeyCoord(keyNum / TerpstraSysExApplication::getApp().getOctaveBoardSize(), 
                            keyNum % TerpstraSysExApplication::getApp().getOctaveBoardSize());
}

void LumatoneKeyUpdateBuffer::sendKeyConfig(int boardId, int keyIndex, const LumatoneKey& noteDataConfig, bool signalEditorListeners)
{
    updateKeyConfig(boardId - 1, keyIndex, noteDataConfig);
}

void LumatoneKeyUpdateBuffer::sendKeyConfig(int boardIndex, int keyIndex, int noteOrCCNum, int channel, int keyType, bool faderUpIsNull)
{
    auto config = LumatoneKey(LumatoneKeyType(keyType), channel, noteOrCCNum, juce::Colour(), faderUpIsNull);
    updateKeyConfig(boardIndex, keyIndex, config);
}

void LumatoneKeyUpdateBuffer::sendKeyColourConfig(int boardId, int keyIndex, juce::Colour colour, bool signalEditorListeners)
{
    updateKeyColour(boardId - 1, keyIndex, colour);
}

void LumatoneKeyUpdateBuffer::updateKeyConfig(int boardIndex, int keyIndex, const LumatoneKey& config)
{
    lock.enter();
    // while (!lock.tryEnter()) {}

    auto keyNum = getKeyNum(boardIndex, keyIndex);
    MappedLumatoneKey currentUpdateData = keysToUpdate[keyNum];

    auto currentKey = TerpstraSysExApplication::getApp().getMappingData()->readKey(boardIndex, keyIndex);
    preUpdateLayout.getBoard(boardIndex)->theKeys[keyIndex] = *currentKey;

    if (*currentKey == config)
    {
        keysToUpdate.set(keyNum, MappedLumatoneKey());
    }
    else if (currentUpdateData.boardIndex < 0 || currentUpdateData.keyIndex < 0)
    {
        keysToUpdate.set(keyNum, MappedLumatoneKey(config.withColour(currentKey->colour), boardIndex, keyIndex));
    }
    else if (!currentUpdateData.configIsEqual(config))
    {
        keysToUpdate.set(keyNum, MappedLumatoneKey(config.withColour(currentUpdateData.colour), boardIndex, keyIndex));
    }

    lock.exit();
    startTimer(updateMs);
}

void LumatoneKeyUpdateBuffer::updateKeyColour(int boardIndex, int keyIndex, juce::Colour colour)
{
    lock.enter();
    // while (!lock.tryEnter()) {}

    auto keyNum = getKeyNum(boardIndex, keyIndex);
    MappedLumatoneKey currentUpdateData = keysToUpdate[keyNum];
    auto currentKey = TerpstraSysExApplication::getApp().getMappingData()->readKey(boardIndex, keyIndex);
    preUpdateLayout.getBoard(boardIndex)->theKeys[keyIndex] = *currentKey;
    
    if (currentKey->colour == colour && currentKey->configIsEqual(currentUpdateData))
    {
        keysToUpdate.set(keyNum, MappedLumatoneKey());
    }
    else if (currentUpdateData.boardIndex < 0 || currentUpdateData.keyIndex < 0)
    {
        keysToUpdate.set(keyNum, MappedLumatoneKey(currentKey->withColour(colour), boardIndex, keyIndex));
    }
    else if (currentUpdateData.colour != colour)
    {
        keysToUpdate.set(keyNum, MappedLumatoneKey(currentUpdateData.withColour(colour), boardIndex, keyIndex));
    }

    lock.exit();
    startTimer(updateMs);
}

void LumatoneKeyUpdateBuffer::timerCallback()
{
    stopTimer();

    if (! lock.tryEnter())
        return;

    int keyNum = 0;
    for (int board = 0; board < TerpstraSysExApplication::getApp().getNumBoards(); board++)
    {
        int boardId = board + 1;
        for (int keyIndex = 0; keyIndex < TerpstraSysExApplication::getApp().getOctaveBoardSize(); keyIndex++)
        {
            auto keyUpdate = keysToUpdate[keyNum];
            if (keyUpdate.boardIndex >= 0 && keyUpdate.keyIndex >= 0)
            {
                // auto currentKey = getKey(board, keyIndex);
                auto currentKey = preUpdateLayout.getBoard(board)->theKeys[keyIndex];
                if (!currentKey.configIsEqual(keyUpdate))
                    midiDriver.sendKeyFunctionParameters(boardId, keyIndex, keyUpdate.noteNumber, keyUpdate.channelNumber, keyUpdate.keyType, keyUpdate.ccFaderDefault);
                
                if (!currentKey.colourIsEqual(keyUpdate))
                {
                    auto colour = keyUpdate.colour;
                    if (TerpstraSysExApplication::getApp().getLumatoneVersion() >= LumatoneFirmwareVersion::VERSION_1_0_11)
                        midiDriver.sendKeyLightParameters(boardId, keyIndex, colour.getRed(), colour.getGreen(), colour.getBlue());
                    else
                        midiDriver.sendKeyLightParameters_Version_1_0_0(boardId, keyIndex, colour.getRed() * 0.5f, colour.getGreen() * 0.5f, colour.getBlue() * 0.5f);
                }
            }

            keysToUpdate.set(keyNum, MappedLumatoneKey());
            keyNum++;
        }
    }

    lock.exit();
}
