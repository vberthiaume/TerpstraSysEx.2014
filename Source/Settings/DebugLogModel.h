#ifndef LUMATONE_SANDBOX_LOG_TABLE_MODEL_H
#define LUMATONE_SANDBOX_LOG_TABLE_MODEL_H

#include "DebugLogger.h"

class DebugLogModel   : public juce::Logger
                                     , public juce::TableListBoxModel
                                     , public juce::ChangeBroadcaster
{
public:
    struct Filter
    {
        int columnFlag = 0;
    };

public:
    typedef enum
    {
        Date = 1,
        Class,
        Status,
        Type,
        Method,
        Message,
        Info
    } TableColumn;

    static juce::String TableColumnName(TableColumn column)
    {
        switch (column)
        {
            default:
                jassertfalse;
                return juce::String("");
            case TableColumn::Date:
                return juce::String("Date");
            case TableColumn::Class:
                return juce::String("Class");
            case TableColumn::Status:
                return juce::String("Status");
            case TableColumn::Type:
                return juce::String("Type");
            case TableColumn::Method:
                return juce::String("Method");
            case TableColumn::Message:
                return juce::String("Message");
            case TableColumn::Info:
                return juce::String("Info");
        }
    }

public:

    DebugLogModel();
    ~DebugLogModel() override;

    void logMessage(const juce::String& message) override;

    int getNumRows() override { return numLogs; }

    void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;

    void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    const DebugLogEntry& getLog(int logNum) const;

private:

    static juce::Colour getRowColour(int rowNumber, LumatoneEditorLogger::LogStatus status);

private:

    int maxLogs = 1000;

    int numLogs = 0;

    Filter filter;

    juce::Array<DebugLogEntry> logs;

    juce::Time errorTime;

    DebugLogEntry defaultLog =
    {
        juce::Time(),
        "DebugLogModel",
        LumatoneEditorLogger::LogStatus::INFO,
        LumatoneEditorLogger::LogType::APP,
        "initialized",
        "No logs yet."
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DebugLogModel)
};

#endif // LUMATONE_SANDBOX_LOG_TABLE_MODEL_H
