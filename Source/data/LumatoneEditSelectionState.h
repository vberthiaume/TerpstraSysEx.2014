/*
  ==============================================================================

    LumatoneEditSelectionState.h
    Created: 6 June 2024
    Author:  Vito

  ==============================================================================
*/


#ifndef LUMATONE_EDIT_SELECTION_STATE_H
#define LUMATONE_EDIT_SELECTION_STATE_H

#include "../lumatone_editor_library/data/state_base.h"
#include "../lumatone_editor_library/data/lumatone_key.h"

class LumatoneEditSelectionState : public LumatoneStateBase
{
public:
    struct Data
    {
        bool setColour = false;
        bool setType = false;
        bool setNote = false;
        bool setChannel = false;
        bool setCCFaderDefault = false;

        juce::Colour colour = juce::Colour();
        LumatoneKeyType type = LumatoneKeyType::disabledDefault;
        int note = -1;
        int channel = -1;
        bool ccFaderDefault = false;
    };

public:

    LumatoneEditSelectionState(juce::String nameIn);
    LumatoneEditSelectionState(juce::String nameIn, juce::ValueTree parentStateIn);
    LumatoneEditSelectionState(juce::String nameIn,  juce::ValueTree parentStateIn, const LumatoneEditSelectionState& stateToCopy);

    virtual ~LumatoneEditSelectionState() override { };

    LumatoneEditSelectionState::Data getData() const { return data; }

    void setKeyColour(bool set, juce::Colour colourIn);
    void setKeyType(bool set, LumatoneKeyType typeIn);
    void setKeyNote(bool set, int noteIn);
    void setKeyChannel(bool set, int channelIn);
    void setCCFader(bool set, bool ccFaderDefaultIn);

protected:

    juce::ValueTree loadStateProperties(juce::ValueTree stateIn) override;
    void handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier& property) override;

    void loadPropertiesFile(juce::PropertiesFile* properties) override { };

private:

    LumatoneEditSelectionState::Data data;
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
