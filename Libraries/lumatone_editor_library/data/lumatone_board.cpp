/*
  ==============================================================================

    lumatone_board.cpp
    Created: 6 Aug 2023 2:16:27pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "lumatone_board.h"

/*
==============================================================================
TerpstraKeys class
==============================================================================
*/

LumatoneBoard::LumatoneBoard(LumatoneKeyType newKeyType, int numKeysIn, int boardIndex)
    : numKeys(numKeysIn)
    , board_idx(boardIndex)
{
    for (int i = 0; i < numKeys; i++)
        theKeys[i] = LumatoneKey(newKeyType);

    //key_idx = 0;
    // state = juce::ValueTree(LumatoneBoardProperty::State);
    // updateState();
}

LumatoneBoard::LumatoneBoard(const LumatoneBoard& copyBoard)
{
    // state = juce::ValueTree(LumatoneBoardProperty::State);
    board_idx = copyBoard.board_idx;
    operator=(copyBoard);
}

LumatoneBoard::LumatoneBoard(juce::ValueTree stateIn)
{
    juce::ValueTree stateToCopy(LumatoneBoardProperty::State);
    if (stateIn.hasType(LumatoneBoardProperty::State))
        stateToCopy = stateIn;

    // state = stateToCopy;
    refreshFromState(stateToCopy);
}

void LumatoneBoard::operator=(const LumatoneBoard& copyBoard)
{
    numKeys = copyBoard.numKeys;

    for (int i = 0; i < numKeys; i++)
    {
        theKeys[i] = copyBoard.getKey(i);
    }

    // updateState();
}

bool LumatoneBoard::isEmpty() const
{
    LumatoneKey emptyKeyData = LumatoneKey();

    for (int i = 0; i < numKeys; i++)
        if (theKeys[i] != emptyKeyData)
            return false;

    return true;
}

juce::ValueTree LumatoneBoard::getState() const
{
    juce::ValueTree newState(LumatoneBoardProperty::State);

    newState.setProperty(LumatoneBoardProperty::Id, board_idx, nullptr);
    for (int i = 0; i < numKeys; i++)
    {
        newState.addChild(theKeys[i].getState(), i, nullptr);
    }

    return newState;
}

int LumatoneBoard::getNumKeys() const
{
    return numKeys;
}

const LumatoneKey& LumatoneBoard::getKey(int index) const
{
    return theKeys[index];
}

void LumatoneBoard::setKey(LumatoneKey key, int index)
{
    theKeys[index] = key;
}

void LumatoneBoard::setKey(const MappedLumatoneKey &mappedKey)
{
    theKeys[mappedKey.keyIndex] = static_cast<const LumatoneKey&>(mappedKey);
}

void LumatoneBoard::setKeyConfig(LumatoneKey key, int index)
{
    theKeys[index].setConfig(key);
}

void LumatoneBoard::setKeyColour(juce::Colour colour, int index)
{
    theKeys[index].setColour(colour);
}

juce::Array<juce::Colour> LumatoneBoard::getBoardColours() const
{
    juce::Array<juce::Colour> boardColours;

    for (int i = 0; i < numKeys; i++)
    {
        if (!boardColours.contains(theKeys[i].getColour()))
            boardColours.add(theKeys[i].getColour());
    }

    return boardColours;
}

juce::Array<LumatoneKeyType> LumatoneBoard::getBoardKeyTypes() const
{
    juce::Array<LumatoneKeyType> boardKeyTypes;

    for (int i = 0; i < numKeys; i++)
    {
        if (!boardKeyTypes.contains(theKeys[i].getType()))
            boardKeyTypes.add(theKeys[i].getType());
    }

    return boardKeyTypes;
}

juce::Array<int> LumatoneBoard::getBoardKeyNotes() const
{
    juce::Array<int> keyNotes;
    auto compare = juce::DefaultElementComparator<int>();

    for (int i = 0; i < numKeys; i++)
    {
        if (!keyNotes.contains(theKeys[i].getMidiNumber()))
            keyNotes.addSorted(compare, theKeys[i].getMidiNumber());
    }

    return keyNotes;
}

juce::Array<int> LumatoneBoard::getBoardKeyChannels() const
{
    juce::Array<int> keyChannels;
    auto compare = juce::DefaultElementComparator<int>();

    for (int i = 0; i < numKeys; i++)
    {
        if (!keyChannels.contains(theKeys[i].getMidiChannel()))
            keyChannels.addSorted(compare, theKeys[i].getMidiChannel());
    }

    return keyChannels;
}

juce::Array<LumatoneKeyCoord> LumatoneBoard::getKeysWithProperties(LumatoneKeyPropertyData properties) const
{
    juce::Array<LumatoneKeyCoord> keyCoords;

    for (int i = 0; i < numKeys; i++)
    {
        const LumatoneKey& key = theKeys[i];
        bool matches = true;

        if (matches && properties.useColour)
        {
            matches = matches && key.isColour(properties.colour);
        }

        if (matches && properties.useType)
        {
            matches = matches && key.hasType(properties.type);
        }

        if (matches && properties.useNote)
        {
            matches = matches && key.hasMidiNumber(properties.note);
        }

        if (matches && properties.useChannel)
        {
            matches = matches && key.hasMidiChannel(properties.channel);
        }

        if (matches && properties.useCCFaderDefault)
        {
            matches = matches && key.hasCCFaderFlag(properties.ccFaderDefault);
        }

        if (matches)
            keyCoords.add(LumatoneKeyCoord(board_idx, i));
    }

    return keyCoords;
}

juce::Array<LumatoneKeyCoord> LumatoneBoard::getKeysWithColour(const juce::Colour &c) const
{
    juce::Array<LumatoneKeyCoord> keyCoords;

    for (int i = 0; i < numKeys; i++)
    {
        if (theKeys[i].isColour(c))
            keyCoords.add(LumatoneKeyCoord(board_idx, i));
    }

    return keyCoords;
}

// void LumatoneBoard::updateState()
// {
//     juce::ValueTree newState(LumatoneBoardProperty::State);

//     newState.setProperty(LumatoneBoardProperty::Id, board_idx, nullptr);
//     for (int i = 0; i < numKeys; i++)
//     {
//         newState.addChild(theKeys[i].getState(), i, nullptr);
//     }

//     state.copyPropertiesAndChildrenFrom(newState, nullptr);
// }

void LumatoneBoard::refreshFromState(const juce::ValueTree& state)
{
    juce::var defaultIndex = 0;
    board_idx = (int)state.getProperty(LumatoneBoardProperty::Id, defaultIndex);

    for (int i = 0; i < numKeys; i++)
    {
        auto keyState = state.getChild(i);
        LumatoneKey key = LumatoneKey(keyState);
        theKeys[i] = key;
    }
}
