                                                                                                                                                                                                                                                                                                                                                                                                                   #include "LumatoneEditSelectionState.h"
#include "../lumatone_editor_library/data/lumatone_layout.h"
#include "../data/LumatoneEditorState.h"

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

LumatoneEditSelectionState::LumatoneEditSelectionState(juce::String nameIn, juce::ValueTree parentStateIn)
    : LumatoneStateBase(nameIn)
{
    state = parentStateIn.getChildWithName(LumatoneEditSelectionProperty::AssignKeyEdits);

    if (!state.isValid())
    {
        state = juce::ValueTree(LumatoneEditSelectionProperty::AssignKeyEdits);
        parentStateIn.addChild(state, -1, nullptr);
    }

    state.addListener(this);
}

LumatoneEditSelectionState::LumatoneEditSelectionState(juce::String nameIn,  juce::ValueTree parentStateIn, const LumatoneEditSelectionState &stateToCopy)
    : LumatoneStateBase(nameIn)
{
    juce::ValueTree stateData = stateToCopy.state.getChildWithName(LumatoneEditSelectionProperty::AssignKeyEdits);
    state = loadStateProperties(stateData);
    state.addListener(this);

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
    // DBG(name + " edit selection state changed");
    // DBG(state.toXmlString());

    if (state.isValid() && stateIn != state)
        return;

    bool setProperty = stateIn.hasProperty(property);
    juce::var value = stateIn.getProperty(property);

    if (property == LumatoneEditSelectionProperty::AssignKeyColour)
    {
        if (setProperty && value.isString())
        {
            data.useColour = true;
            data.colour = juce::Colour::fromString(value.toString());
        }
        else
        {
            data.useColour = false;
        }
    }
    else if (property == LumatoneEditSelectionProperty::AssignKeyType)
    {
        if (setProperty && value.isInt())
        {
            data.useType = true;
            data.type = LumatoneKeyType((int)value);
        }
        else
        {
            data.useType = false;
        }
    }
    else if (property == LumatoneEditSelectionProperty::AssignKeyNote)
    {
        if (setProperty && value.isInt())
        {
            data.useNote = true;
            data.note = (int)value;
        }
        else
        {
            data.useNote = false;
        }
    }
    else if (property == LumatoneEditSelectionProperty::AssignKeyChannel)
    {
        if (setProperty && value.isInt())
        {
            data.useChannel = true;
            data.channel = (int)value;
        }
        else
        {
            data.useChannel = false;
        }
    }
    else if (property == LumatoneEditSelectionProperty::AssignKeyCCFader)
    {
        if (setProperty && value.isBool())
        {
            data.useCCFaderDefault = true;
            data.ccFaderDefault = (bool)value;
        }
        else
        {
            data.useCCFaderDefault = false;
        }
    }
}

void LumatoneEditSelectionState::setKeyColour(bool set, juce::Colour colourIn)
{
    data.useColour = set;
    data.colour = colourIn;

    if (set)
    {
        state.setPropertyExcludingListener(this, LumatoneEditSelectionProperty::AssignKeyColour, colourIn.toString(), nullptr);
    }
    else
    {
        state.removeProperty(LumatoneEditSelectionProperty::AssignKeyColour, nullptr);
    }
}

void LumatoneEditSelectionState::setKeyType(bool set, LumatoneKeyType typeIn)
{
    data.useType = set;
    data.type = typeIn;

    if (set)
    {
        state.setPropertyExcludingListener(this, LumatoneEditSelectionProperty::AssignKeyType, juce::var((int)typeIn), nullptr);
    }
    else
    {
        state.removeProperty(LumatoneEditSelectionProperty::AssignKeyType, nullptr);
    }
}

void LumatoneEditSelectionState::setKeyNote(bool set, int noteIn)
{
    data.useNote = set;
    data.note = noteIn;

    if (set)
    {
        state.setPropertyExcludingListener(this, LumatoneEditSelectionProperty::AssignKeyNote, juce::var(noteIn), nullptr);
    }
    else
    {
        state.removeProperty(LumatoneEditSelectionProperty::AssignKeyNote, nullptr);
    }
}

void LumatoneEditSelectionState::setKeyChannel(bool set, int channelIn)
{
    data.useChannel = set;
    data.channel = channelIn;

    if (set)
    {
        state.setPropertyExcludingListener(this, LumatoneEditSelectionProperty::AssignKeyChannel, juce::var(channelIn), nullptr);
    }
    else
    {
        state.removeProperty(LumatoneEditSelectionProperty::AssignKeyChannel, nullptr);
    }
}

void LumatoneEditSelectionState::setCCFader(bool set, bool ccFaderDefaultIn)
{
    data.useCCFaderDefault = set;
    data.ccFaderDefault = ccFaderDefaultIn;

    if (set)
    {
        state.setPropertyExcludingListener(this, LumatoneEditSelectionProperty::AssignKeyCCFader, juce::var(ccFaderDefaultIn), nullptr);
    }
    else
    {
        state.removeProperty(LumatoneEditSelectionProperty::AssignKeyCCFader, nullptr);
    }
}

void LumatoneEditSelectionState::clear()
{
    data = LumatoneKeyPropertyData();

    state.removeProperty(LumatoneEditSelectionProperty::AssignKeyColour, nullptr);
    state.removeProperty(LumatoneEditSelectionProperty::AssignKeyType, nullptr);
    state.removeProperty(LumatoneEditSelectionProperty::AssignKeyNote, nullptr);
    state.removeProperty(LumatoneEditSelectionProperty::AssignKeyChannel, nullptr);
    state.removeProperty(LumatoneEditSelectionProperty::AssignKeyCCFader, nullptr);
}

LumatoneKeySelectionData LumatoneEditSelectionState::findSelectionProperties(const juce::Array<MappedLumatoneKey> &selection)
{
    LumatoneKeySelectionData selectionProperties;

    for (int i = 0; i < selection.size(); i++)
    {
        const MappedLumatoneKey& key = selection.getReference(i);
        selectionProperties.colours.addIfNotAlreadyThere(key.getColour());
        selectionProperties.types.addIfNotAlreadyThere(key.getType());
        selectionProperties.notes.addIfNotAlreadyThere(key.getMidiNumber());
        selectionProperties.channels.addIfNotAlreadyThere(key.getMidiChannel());
    }

    return selectionProperties;
}

LumatoneKeyPropertyData LumatoneEditSelectionState::findSharedSelectionProperties(const juce::Array<MappedLumatoneKey> &selection)
{
    LumatoneKeyPropertyData data;

    bool updated = false;

    for (int i = 0; i < selection.size(); i++)
    {
        const MappedLumatoneKey& key = selection.getReference(i);

        if (i == 0)
        {
            data.useColour = true;
            data.colour = key.getColour();
        }
        else if (data.colour != key.getColour())
        {
            data.useColour = false;
        }

        if (i == 0)
        {
            data.useType = true;
            data.type = key.getType();
        }
        else if (data.type != key.getType())
        {
            data.useType = false;
        }

        if (i == 0)
        {
            data.useNote = true;
            data.note = key.getMidiNumber();
        }
        else if (data.note != key.getMidiNumber())
        {
            data.useNote = false;
        }

        if (i == 0)
        {
            data.useChannel = true;
            data.channel = key.getMidiChannel();
        }
        else if (data.channel != key.getMidiChannel())
        {
            data.useChannel = false;
        }

        if (i == 0)
        {
            data.useCCFaderDefault = true;
            data.ccFaderDefault = key.isCCFaderDefault();
        }
        else if (data.ccFaderDefault != key.isCCFaderDefault())
        {
            data.ccFaderDefault = false;
        }

    }

    return data;
}

LumatoneKeyPropertyData LumatoneEditSelectionState::findSharedSelectionProperties(const LumatoneKeySelectionData &properties)
{
    LumatoneKeyPropertyData data;

    if (properties.colours.size() == 1)
    {
        data.useColour = true;
        data.colour = properties.colours[0];
    }
    if (properties.types.size() == 1)
    {
        data.useType = true;
        data.type = properties.types[0];
    }
    if (properties.notes.size() == 1)
    {
        data.useNote = true;
        data.note = properties.notes[0];
    }
    if (properties.channels.size() == 1)
    {
        data.useChannel = true;
        data.channel = properties.channels[0];
    }

    return data;
}
