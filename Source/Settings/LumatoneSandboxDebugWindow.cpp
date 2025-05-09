#include "LumatoneSandboxDebugWindow.h"

#include "LumatoneSandboxLogTableModel.h"

LumatoneSandboxDebugWindow::LumatoneSandboxDebugWindow(LumatoneSandboxLogTableModel *logTableModelIn)
    : juce::DocumentWindow("LumatoneSandboxDebugWindow",
                           juce::Colours::darkslategrey,
                           juce::DocumentWindow::TitleBarButtons::minimiseButton,
                           true)
{

    logModel = logTableModelIn;

    logTable = new juce::TableListBox("LumatoneSandboxLogTable", static_cast<juce::TableListBoxModel*>(logModel));
    logTable->setHeader(std::make_unique<juce::TableHeaderComponent>());
    logTable->getHeader().addColumn("Date",     LumatoneSandboxLogTableColumn::Date,    100);
    logTable->getHeader().addColumn("Class",    LumatoneSandboxLogTableColumn::Class,   72);
    logTable->getHeader().addColumn("Status",   LumatoneSandboxLogTableColumn::Status,  64);
    logTable->getHeader().addColumn("Type",     LumatoneSandboxLogTableColumn::Type,    64);
    logTable->getHeader().addColumn("Method",   LumatoneSandboxLogTableColumn::Method,  160);
    logTable->getHeader().addColumn("Message",  LumatoneSandboxLogTableColumn::Message, 1024);

    logModel->addChangeListener(this);

    logTable->setSize(1024, 1024);
    setContentOwned(logTable, true);
    setResizable(true, true);
}

void LumatoneSandboxDebugWindow::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == logModel)
    {
        getContentComponent()->resized();

        if (scrollToNewLogs)
        {
            logTable->scrollToEnsureRowIsOnscreen(logModel->getNumRows());
        }
    }
}
