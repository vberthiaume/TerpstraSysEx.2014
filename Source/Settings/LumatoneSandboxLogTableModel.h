#ifndef LUMATONE_SANDBOX_LOG_TABLE_MODEL_H
#define LUMATONE_SANDBOX_LOG_TABLE_MODEL_H

#include "./LumatoneSandboxLogger.h"

typedef enum
{
    Date = 1,
    Class,
    Status,
    Type,
    Method,
    Message,
    Info
} LumatoneSandboxLogTableColumn;

class LumatoneSandboxLogTableModel   : public juce::Logger
                                     , public juce::TableListBoxModel
                                     , public juce::ChangeBroadcaster
{
public:

    LumatoneSandboxLogTableModel() {}
    ~LumatoneSandboxLogTableModel() override;

    void logMessage(const juce::String& message) override;


    int getNumRows() override { return numLogs; }

    void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;

    void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    const LumatoneSandboxLog& getLog(int logNum) const;

private:

    static juce::Colour getRowColour(int rowNumber, LumatoneEditorLogger::LogStatus status);

private:

    int maxLogs = 1000;

    int numLogs = 0;

    juce::Array<LumatoneSandboxLog> logs;

    juce::Time errorTime;

    LumatoneSandboxLog defaultLog =
    {
        juce::Time(),
        "LumatoneSandboxLogTableModel",
        LumatoneEditorLogger::LogStatus::INFO,
        LumatoneEditorLogger::LogType::APP,
        "initialized",
        "No logs yet."
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LumatoneSandboxLogTableModel)
};

#endif // LUMATONE_SANDBOX_LOG_TABLE_MODEL_H
