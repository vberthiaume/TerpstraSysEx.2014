
#include "DebugLogModel.h"
#include "../LumatoneFirmwareDefinitions.h"

using namespace LumatoneEditorLogger;

DebugLogModel::DebugLogModel()
{
}

DebugLogModel::~DebugLogModel()
{

}

void DebugLogModel::logMessage(const juce::String& message)
{
    if (numLogs == maxLogs)
        logs.remove(0);
    else
        numLogs++;

    DebugLogEntry logInfo = DebugLogEntry::FromString(message);
    if (logInfo.time == errorTime)
    {
        logInfo.className = "DebugLogModel";
        logInfo.method = "logMessage";
        logInfo.message = "Unable to parse message: " + message;
    }

    logs.add(logInfo);

    juce::MessageManager::callAsync([=]() { sendChangeMessage(); });
}

const DebugLogEntry& DebugLogModel::getLog(int logNum) const
{
    if (logNum < logs.size())
        return logs.getReference(logNum);

    return defaultLog;
}

juce::Colour DebugLogModel::getRowColour(int rowNumber, LogStatus status)
{
    juce::Colour c = rowNumber % 2 == 0 ? juce::Colours::lightslategrey : juce::Colours::lightgrey;

    if (status == LogStatus::WARNING)
        c.overlaidWith(juce::Colours::yellow);
    else if (status == LogStatus::ERROR)
        c.overlaidWith(juce::Colours::red);

    return c;
}
void DebugLogModel::paintRowBackground(juce::Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
{
    auto log = getLog(rowNumber);
    auto c = getRowColour(rowNumber, log.status);
    g.fillAll(c);
}

void DebugLogModel::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if (rowNumber < 0)
        return;

    const DebugLogEntry& log = getLog(rowNumber);
    juce::String value;

    switch (columnId)
    {
    case TableColumn::Date:
        // value = log.time.toString(false, true, true, true);
        value = log.time.formatted(" %H:%m:%S");
        break;
    case TableColumn::Class:
        value = log.className;
        break;
    case TableColumn::Status:
        value = log.getStatusString();
        break;
    case TableColumn::Type:
        value = log.getTypeString();
        break;
    case TableColumn::Method:
        value = log.method;
        if (log.type == LogType::SYSEX) // quick hack
        {
            FirmwareSupport support;
            int commandId = support.getCommandNumber(log.message);
            value = CommandCodeToName(commandId);
        }
        break;
    case TableColumn::Message:
        value = log.message;
        // if (log.type == LogType::SYSEX)
        // {
        //     FirmwareSupport support;
        //     int commandId = support.getCommandNumber(log.message);
        //     value = CommandCodeToName(commandId) + juce::String(": ") + value;
        // }
        break;
    default:
        break;
    }

    juce::Colour textColour = getRowColour(rowNumber, log.status).contrasting(0.95f);
    g.setColour(textColour);
    g.drawText(value, 0, 0, width, height, juce::Justification::centredLeft);
}
