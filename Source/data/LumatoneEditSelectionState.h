/*
  ==============================================================================

    LumatoneEditSelectionState.h
    Created: 6 June 2024
    Author:  Vito

    Holds the key property values that are active in a key assignment controller

  ==============================================================================
*/


#ifndef LUMATONE_EDIT_SELECTION_STATE_H
#define LUMATONE_EDIT_SELECTION_STATE_H

#include "../lumatone_editor_library/data/state_base.h"
#include "../lumatone_editor_library/data/lumatone_key.h"

class LumatoneEditSelectionState : public LumatoneStateBase
{
public:

    LumatoneEditSelectionState(juce::String nameIn, juce::ValueTree parentStateIn);
    LumatoneEditSelectionState(juce::String nameIn,  juce::ValueTree parentStateIn, const LumatoneEditSelectionState& stateToCopy);

    virtual ~LumatoneEditSelectionState() override { };

    LumatoneKeyPropertyData getData() const { return data; }

    void setKeyColour(bool set, juce::Colour colourIn);
    void setKeyType(bool set, LumatoneKeyType typeIn);
    void setKeyNote(bool set, int noteIn);
    void setKeyChannel(bool set, int channelIn);
    void setCCFader(bool set, bool ccFaderDefaultIn);

public:

    static LumatoneKeyPropertyData findSharedSelectionProperties(const juce::Array<MappedLumatoneKey>& selection);

protected:

    juce::ValueTree loadStateProperties(juce::ValueTree stateIn) override;
    void handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier& property) override;

    void loadPropertiesFile(juce::PropertiesFile*) override { };

private:

    LumatoneKeyPropertyData data;
};

namespace LumatoneEditSelectionProperty
{
    static const juce::Identifier AssignKeyEdits = juce::Identifier("AssignKeyEdits");
    static const juce::Identifier AssignKeyColour = juce::Identifier("AssignKeyColour");
    static const juce::Identifier AssignKeyType = juce::Identifier("AssignKeyType");
    static const juce::Identifier AssignKeyNote = juce::Identifier("AssignKeyNote");
    static const juce::Identifier AssignKeyChannel = juce::Identifier("AssignKeyChannel");
    static const juce::Identifier AssignKeyCCFader = juce::Identifier("AssignKeyCCFader");
}


#endif // LUMATONE_EDIT_SELECTION_STATE_H
