
#include "LumatoneSandboxLogger.h"

using namespace LumatoneEditorLogger;

LumatoneSandboxLogger::LumatoneSandboxLogger(juce::String classNameIn)
    : className(classNameIn)
{

}

void LumatoneSandboxLogger::Log(LumatoneSandboxLog info)
{
    juce::String logMessage = info.toFullString();
    juce::Logger::writeToLog(logMessage);

    // juce::String simpleMsg = info.toShortString();
    // juce::Logger::outputDebugString(simpleMsg);
}

void LumatoneSandboxLogger::logMessage(const juce::String &message)
{
    juce::Logger::writeToLog(message);
}

void LumatoneSandboxLogger::log(LogStatus status, LogType type, juce::String method, juce::String message) const
{
    if (logLevel == LogLevel::NONE)
        return;

    LogLevel level = StatusToLogLevel(status);
    if (level > logLevel)
        return;

    LumatoneSandboxLog info = createLog(status, type, method, message);
    LumatoneSandboxLogger::Log(info);
}

void LumatoneSandboxLogger::logInfo(LogType type, juce::String method, juce::String message) const
{
    log(LogStatus::INFO, type, method, message);
}

void LumatoneSandboxLogger::logWarning(LogType type, juce::String method, juce::String message) const
{
    log(LogStatus::WARNING, type, method, message);
}

void LumatoneSandboxLogger::logError(LogType type, juce::String method, juce::String message) const
{
    log(LogStatus::ERROR, type, method, message);
}

LumatoneSandboxLog LumatoneSandboxLogger::createLog(LogStatus status, LogType type, juce::String method, juce::String message) const
{
    juce::Time now = juce::Time::getCurrentTime();

    LumatoneSandboxLog info =
    {
        now,
        className,
        status,
        type,
        method,
        message
    };

    return info;
}

juce::String LumatoneSandboxLog::toFullString() const
{
    juce::StringArray tokens;

    auto dateStamp = time.toISO8601(true);
    tokens.add(dateStamp);

    tokens.add(className);

    switch (status)
    {
    case LogStatus::ERROR:
        tokens.add("Error");
        break;
    case LogStatus::INFO:
        tokens.add("Info");
        break;
    case LogStatus::WARNING:
        tokens.add("Warning");
        break;
    default:
        jassertfalse;
    }

    tokens.add(method + "()");
    tokens.add(message);

    if (info.size() > 0)
    {
        auto infoKeys = info.getAllKeys();
        auto infoValues = info.getAllValues();
        auto infoPacked = infoKeys.joinIntoString(",") + juce::String(":") + infoValues.joinIntoString(",");
        tokens.add(infoPacked);
    }

    juce::String logMsg = tokens.size() == 1 ? tokens[0] : tokens.joinIntoString("|");
    return logMsg;
}

juce::String LumatoneSandboxLog::toShortString() const
{
    juce::StringArray tokens;

    tokens.add(className);

    juce::String statusString = getStatusString();
    tokens.add(statusString);

    tokens.add(method + "()");
    tokens.add(message);

    juce::String logMsg = tokens.joinIntoString(": ");
    return logMsg;
}

juce::String LumatoneSandboxLog::getStatusString() const
{
    switch (status)
    {
    default:
        jassertfalse;
        break;
    case LogStatus::ERROR:
        return "Error";
    case LogStatus::INFO:
        return "Info";
    case LogStatus::WARNING:
        return "Warning";
    }

    return juce::String();
}

juce::String LumatoneSandboxLog::getTypeString() const
{
    switch (type)
    {
        default:
            jassertfalse;
            break;
        case LogType::APP:
            return "App";
        case LogType::MIDI:
            return "MIDI";
        case LogType::SYSEX:
            return "Sysex";
        case LogType::DEVICE:
            return "Device";
    }
    return juce::String();
}

LumatoneSandboxLog LumatoneSandboxLog::FromString(juce::String logString)
{
    LumatoneSandboxLog logInfo;

    juce::String properties;

    juce::juce_wchar delim('|');
    const int lastIndex = logString.length() - 1;
    int wordStart = 0;
    int wordNum = 0;

    for (int i = 1; i < logString.length(); i++)
    {
        if (logString[i] == delim || i == lastIndex)
        {
            if (i == lastIndex)
                i++;

            auto word = logString.substring(wordStart, i);
            switch (wordNum)
            {
            case 0:
                logInfo.time = juce::Time::fromISO8601(word);
                break;
            case 1:
                logInfo.className = word;
                break;
            case 2:
                logInfo.status = LumatoneSandboxLog::LogStringToStatus(word);
                break;
            case 3:
                logInfo.method = word;
                break;
            case 4:
                logInfo.message = word;
                break;
            case 5:
                properties = word;
                break;
            default:
                jassertfalse;
                break;
            }

        wordStart = i + 1;
        i += 2;
        wordNum++;
        }
    }

    if (properties.length() > 0)
    {
        // auto propStrings = juce::StringArray::fromTokens(properties, "|");
        // auto keys = juce::StringArray::fromTokens(propStrings[0], ",");
        // auto values = juce::StringArray::fromTokens(propStrings[1], ",");

        // juce::StringPairArray props;
        // for (int i = 0; i < keys.size(); i++)
        // {
        //     props.set(keys[i], values[i]);
        // }

        // logInfo.info = props;
    }

    return logInfo;
}

LogStatus LumatoneSandboxLog::CodeToStatus(int statusCode)
{
    if (statusCode == 0)
        return LogStatus::INFO;
    if (statusCode > 0)
        return LogStatus::WARNING;
    return LogStatus::ERROR;
}

LogStatus LumatoneSandboxLog::LogStringToStatus(juce::StringRef statusString)
{
    if (juce::String(statusString).startsWith("Info"))
        return LogStatus::INFO;
    if (juce::String(statusString).startsWith("Warning"))
        return LogStatus::WARNING;

    return LogStatus::ERROR;
}
