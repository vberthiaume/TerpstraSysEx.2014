#ifndef LUMATONE_EDITOR_EDITOR_LISTENER_H
#define LUMATONE_EDITOR_EDITOR_LISTENER_H

#include <JuceHeader.h>
#include "../data/lumatone_context.h"

namespace LumatoneEditor
{

class EditorListener
{
public:

    virtual ~EditorListener() {}

    // App Actions
    virtual void layoutChanged(const LumatoneLayout& mappingData) {}
    virtual void boardChanged(const LumatoneBoard& boardData) {}
    virtual void keyChanged(int boardIndex, int keyIndex, const LumatoneKey& lumatoneKey) {}

    virtual void keySetChanged(juce::Array<MappedLumatoneKey> selection) {}

    // virtual void newFileLoaded(juce::File file) {}

    //virtual void tableChanged(LumatoneConfigTable::TableType type, const juce::uint8* table, int tableSize) {}

    virtual void contextChanged(LumatoneContext* context) {}

    virtual void configTableChanged(LumatoneConfigTable::TableType type) {}

    virtual void selectionChanged() {}

    virtual void firmwareVersionChanged() {}

    // Mapping Options
    virtual void lightOnKeyStrokesChanged(bool lightOn) {}
    virtual void aftertouchToggled(bool enabled) {}
    virtual void expressionPedalSensitivityChanged(unsigned char value) {}
    virtual void invertFootControllerChanged(bool inverted) {}
    virtual void invertSustainToggled(bool inverted) {}

    virtual void velocityConfigChanged() {}
    virtual void calibrateAftertouchToggled(bool active) {}
    virtual void aftertouchConfigChanged() {}
    virtual void lumatouchConfigChanged() {}

    // Global Options
    virtual void macroButtonActiveColourChanged(juce::Colour colour) {}
    virtual void macroButtonInactiveColourChanged(juce::Colour colour) {}

    virtual void calibrateKeysRequested() {}
    virtual void calibratePitchModWheelToggled(bool active) {}
    virtual void peripheralChannelsChanged(int pitchWheelChannel, int modWheelChannel, int expressionChannel, int sustainChannel) {}

    // Firmware Actions
    // virtual void keyConfigChanged(int boardIndex, int keyIndex, const LumatoneKey& keyData) {}
    // virtual void keyColourChanged(int boardIndex, int keyIndex, juce::Colour keyColour) {}

    // virtual void pingSent(juce::uint8 pingId) {}


};

class EditorEmitter
{
protected:

    juce::ListenerList<LumatoneEditor::EditorListener> editorListeners;

public:
    EditorEmitter() { }

    virtual ~EditorEmitter() { editorListeners.clear(); }

    virtual void addEditorListener(LumatoneEditor::EditorListener* listenerIn) { editorListeners.add(listenerIn); }

    virtual void removeEditorListener(LumatoneEditor::EditorListener* listenerIn) { editorListeners.remove(listenerIn); }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorEmitter)
};


class LayoutChangeListener : public EditorListener
{
public:

    virtual ~LayoutChangeListener() override {}

    virtual void layoutWasUpdated() = 0;

protected:

    virtual void layoutChanged(const LumatoneLayout& mappingData)
    {
        layoutWasUpdated();
    }

    virtual void boardChanged(const LumatoneBoard& boardData)
    {
        layoutWasUpdated();
    }

    virtual void keyChanged(int boardIndex, int keyIndex, const LumatoneKey& lumatoneKey)
    {
        layoutWasUpdated();
    }

    virtual void keySetChanged(juce::Array<MappedLumatoneKey> selection)
    {
        layoutWasUpdated();
    }

    // virtual void newFileLoaded(juce::File file)
    // {
    //     layoutWasUpdated();
    // }

};

}

#endif
