#include "LumatoneEditSelectionState.h"

static juce::Array<juce::Identifier> getEditSelectionProperties()
{
    juce::Array<juce::Identifier> properties;
    properties.add(LumatoneEditSelectionProperty::AssignKeyColour);
    properties.add(LumatoneEditSelectionProperty::AssignKeyType);
    properties.add(LumatoneEditSelectionProperty::AssignKeyNote);
    properties.add(LumatoneEditSelectionProperty::AssignKeyChannel);
    properties.add(LumatoneEditSelectionProperty::AssignKeyCCFader);
    return properties;
}

LumatoneEditSelectionState::LumatoneEditSelectionState(juce::String nameIn)
    : LumatoneStateBase(nameIn)
{
    state = juce::ValueTree(LumatoneEditSelectionProperty::AssignKeyEdits);
}

LumatoneEditSelectionState::LumatoneEditSelectionState(juce::String nameIn, juce::ValueTree parentStateIn)
    : LumatoneStateBase(nameIn)
{
    state = juce::ValueTree(LumatoneEditSelectionProperty::AssignKeyEdits);
    parentStateIn.addChild(state, -1, nullptr);
}

LumatoneEditSelectionState::LumatoneEditSelectionState(juce::String nameIn,  juce::ValueTree parentStateIn, const LumatoneEditSelectionState &stateToCopy)
    : LumatoneStateBase(nameIn)
{
    state = loadStateProperties(stateToCopy.state.getChildWithName(LumatoneEditSelectionProperty::AssignKeyEdits));
    parentStateIn.addChild(state, -1, nullptr);
}

juce::ValueTree LumatoneEditSelectionState::loadStateProperties(juce::ValueTree stateIn)
{
    juce::ValueTree newState = (stateIn.hasType(LumatoneEditSelectionProperty::AssignKeyEdits))
                             ? stateIn
                             : juce::ValueTree(LumatoneEditSelectionProperty::AssignKeyEdits);

    for (auto property : getEditSelectionProperties())
    {
        handleStatePropertyChange(newState, property);
    }

    return newState;
}

void LumatoneEditSelectionState::handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier &property)
{
    bool setProperty = stateIn.hasProperty(property);
    juce::var value = stateIn.getProperty(property);

    if (property == LumatoneEditSelectionProperty::AssignKeyColour)
    {
        if (setProperty && value.isString())
        {
            data.setColour = true;
            data.colour = juce::Colour::fromString(value.toString());
        }
        else
        {
            data.setColour = false;
        }
    }
    else if (property == LumatoneEditSelectionProperty::AssignKeyType)
    {
        if (setProperty && value.isInt())
        {
            data.setType = true;
            data.type = LumatoneKeyType((int)value);
        }
        else
        {
            data.setType = false;
        }
    }
    else if (property == LumatoneEditSelectionProperty::AssignKeyNote)
    {
        if (setProperty && value.isInt())
        {
            data.setNote = true;
            data.note = (int)value;
        }
        else
        {
            data.setNote = false;
        }
    }
    else if (property == LumatoneEditSelectionProperty::AssignKeyChannel)
    {
        if (setProperty && value.isInt())
        {
            data.setChannel = true;
            data.channel = (int)value;
        }
        else
        {
            data.setChannel = false;
        }
    }
    else if (property == LumatoneEditSelectionProperty::AssignKeyCCFader)
    {
        if (setProperty && value.isBool())
        {
            data.setCCFaderDefault = true;
            data.ccFaderDefault = (bool)value;
        }
        else
        {
            data.setCCFaderDefault = false;
        }
    }
}

void LumatoneEditSelectionState::setKeyColour(bool set, juce::Colour colourIn)
{
    data.setColour = set;
    data.colour = colourIn;

    if (set)
    {
        state.setProperty(LumatoneEditSelectionProperty::AssignKeyColour, colourIn.toString(), nullptr);
    }
    else
    {
        state.removeProperty(LumatoneEditSelectionProperty::AssignKeyColour, nullptr);
    }
}

void LumatoneEditSelectionState::setKeyType(bool set, LumatoneKeyType typeIn)
{
    data.setType = set;
    data.type = typeIn;

    if (set)
    {
        state.setProperty(LumatoneEditSelectionProperty::AssignKeyType, juce::var((int)typeIn), nullptr);
    }
    else
    {
        state.removeProperty(LumatoneEditSelectionProperty::AssignKeyType, nullptr);
    }
}

void LumatoneEditSelectionState::setKeyNote(bool set, int noteIn)
{
    data.setNote = set;
    data.note = noteIn;

    if (set)
    {
        state.setProperty(LumatoneEditSelectionProperty::AssignKeyNote, juce::var(noteIn), nullptr);
    }
    else
    {
        state.removeProperty(LumatoneEditSelectionProperty::AssignKeyNote, nullptr);
    }
}

void LumatoneEditSelectionState::setKeyChannel(bool set, int channelIn)
{
    data.setChannel = set;
    data.channel = channelIn;

    if (set)
    {
        state.setProperty(LumatoneEditSelectionProperty::AssignKeyChannel, juce::var(channelIn), nullptr);
    }
    else
    {
        state.removeProperty(LumatoneEditSelectionProperty::AssignKeyChannel, nullptr);
    }
}

void LumatoneEditSelectionState::setCCFader(bool set, bool ccFaderDefaultIn)
{
    data.setCCFaderDefault = set;
    data.ccFaderDefault = ccFaderDefaultIn;

    if (set)
    {
        state.setProperty(LumatoneEditSelectionProperty::AssignKeyCCFader, juce::var(ccFaderDefaultIn), nullptr);
    }
    else
    {
        state.removeProperty(LumatoneEditSelectionProperty::AssignKeyCCFader, nullptr);
    }
}
