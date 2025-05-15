#ifndef LUMATONE_SANDBOX_DEBUG_WINDOW_H
#define LUMATONE_SANDBOX_DEBUG_WINDOW_H

#include <JuceHeader.h>
#include "../TerpstraMidiDriver.h"

class DebugLogModel;
class DebugComponent;
class KeyMonitorComponent;

class DebugWindow : public juce::DocumentWindow
{
public:

    DebugWindow(DebugLogModel* modelIn);
    ~DebugWindow() override {}

    void closeButtonPressed() override { setVisible(false); }

private:
    std::unique_ptr<DebugComponent> table;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DebugWindow)
};

class DebugComponent : public juce::Component, private juce::ChangeListener
{
public:

    DebugComponent(DebugLogModel* modelIn);
    ~DebugComponent();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
    TerpstraMidiDriver* midiDriver;

    DebugLogModel* logModel;
    std::unique_ptr<juce::TableListBox> logTable;
    juce::OwnedArray<juce::ToggleButton> columnToggle;

    std::unique_ptr<KeyMonitorComponent> keyMonitor;

    std::unique_ptr<juce::ToggleButton> monitorDeviceToggle;

    bool scrollToNewLogs = true;
};

#endif // LUMATONE_SANDBOX_DEBUG_WINDOW_H
