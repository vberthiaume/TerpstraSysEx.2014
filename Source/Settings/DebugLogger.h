#ifndef LUMATONE_SANDBOX_LOGGER_H
#define LUMATONE_SANDBOX_LOGGER_H

#include <JuceHeader.h>

namespace LumatoneEditorLogger
{
    enum class LogStatus
    {
        ERROR   = -1,
        INFO    = 0,
        WARNING = 1
    };

    typedef enum
    {
        APP,
        MIDI,
        SYSEX,
        DEVICE
    } LogType;
}
struct DebugLogEntry
{
    juce::Time time;
    juce::String className;

    LumatoneEditorLogger::LogStatus status = LumatoneEditorLogger::LogStatus::ERROR;
    LumatoneEditorLogger::LogType type = LumatoneEditorLogger::LogType::APP;

    juce::String method = "undefined";
    juce::String message = "";
    juce::StringPairArray info = juce::StringPairArray();

    juce::String toFullString() const;
    juce::String toShortString() const;

    juce::String getStatusString() const;
    juce::String getTypeString() const;

    static DebugLogEntry FromString(juce::String logString);

    static LumatoneEditorLogger::LogStatus CodeToStatus(int statusCode);
    static LumatoneEditorLogger::LogStatus LogStringToStatus(juce::StringRef statusString);
    static LumatoneEditorLogger::LogType StringToLogType(juce::StringRef typeString);

};

class LumatoneSandboxLogger : public juce::Logger
{
public:
    typedef enum
    {
        NONE = 0,
        ERROR,
        WARNING,
        INFO,
        VERBOSE
    } LogLevel;

    static LogLevel StatusToLogLevel(LumatoneEditorLogger::LogStatus status)
    {
        int code = static_cast<int>(status);
        if (code == 0)
            return LogLevel::INFO;
        if (code > 0)
            return LogLevel::WARNING;
        return LogLevel::ERROR;
    }

public:

    LumatoneSandboxLogger(juce::String className);
    virtual ~LumatoneSandboxLogger() override { }

    // Basic log message with status parameter
    void log(LumatoneEditorLogger::LogStatus status, LumatoneEditorLogger::LogType type, juce::String method, juce::String message) const;

    // Status-based helper methods
    void logInfo(LumatoneEditorLogger::LogType type, juce::String method, juce::String message) const;
    void logWarning(LumatoneEditorLogger::LogType type, juce::String method, juce::String message) const;
    void logError(LumatoneEditorLogger::LogType type, juce::String method, juce::String message) const;

    // Helper to build struct, mainly to add more info
    DebugLogEntry createLog(LumatoneEditorLogger::LogStatus status, LumatoneEditorLogger::LogType type, juce::String method, juce::String message) const;

public:

    // Send logs to juce::Logger registered with applicatoin
    static void Log(DebugLogEntry logInfo);

private:

    virtual void logMessage(const juce::String& message) override;

protected:

    juce::String className = "undefined";

    LogLevel logLevel = LogLevel::INFO;

    JUCE_DECLARE_NON_COPYABLE(LumatoneSandboxLogger)
};

#endif // LUMATONE_SANDBOX_LOGGER_H
