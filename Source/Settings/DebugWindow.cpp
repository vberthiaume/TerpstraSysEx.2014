#include "DebugWindow.h"
#include "DebugLogModel.h"
#include "KeyMonitorComponent.h"

#include "../Main.h"

DebugWindow::DebugWindow(DebugLogModel *logTableModelIn)
    : juce::DocumentWindow("LumatoneSandboxDebugWindow",
                           juce::Colours::darkslategrey,
                           juce::DocumentWindow::TitleBarButtons::minimiseButton | juce::DocumentWindow::TitleBarButtons::closeButton,
                           true)

        // midiDriver(midiDriverIn)
{
    DebugComponent* component = new DebugComponent(logTableModelIn);
    component->setSize(1024, 768);
    component->setVisible(true);
    setContentOwned(component, true);
    setResizable(true, true);
}

DebugComponent::DebugComponent( DebugLogModel* modelIn)
{
    logModel = modelIn;

    DebugLogModel::TableColumn columns[] = {
        DebugLogModel::TableColumn::Date,
        DebugLogModel::TableColumn::Class,
        DebugLogModel::TableColumn::Status,
        DebugLogModel::TableColumn::Type,
        DebugLogModel::TableColumn::Method,
        DebugLogModel::TableColumn::Message
        };
    const int numColumns = 6;


    logTable = std::make_unique<juce::TableListBox>("LumatoneSandboxLogTable", static_cast<juce::TableListBoxModel*>(logModel));
    logTable->setHeader(std::make_unique<juce::TableHeaderComponent>());
    logTable->getHeader().addColumn(DebugLogModel::TableColumnName(DebugLogModel::TableColumn::Date),     DebugLogModel::TableColumn::Date,    72);
    logTable->getHeader().addColumn(DebugLogModel::TableColumnName(DebugLogModel::TableColumn::Class),    DebugLogModel::TableColumn::Class,   72);
    logTable->getHeader().addColumn(DebugLogModel::TableColumnName(DebugLogModel::TableColumn::Status),   DebugLogModel::TableColumn::Status,  50);
    logTable->getHeader().addColumn(DebugLogModel::TableColumnName(DebugLogModel::TableColumn::Type),     DebugLogModel::TableColumn::Type,    64);
    logTable->getHeader().addColumn("Command",   DebugLogModel::TableColumn::Method,  180);
    // logTable->getHeader().addColumn(DebugLogModel::TableColumnName(DebugLogModel::TableColumn::Method),   DebugLogModel::TableColumn::Method,  160);
    logTable->getHeader().addColumn(DebugLogModel::TableColumnName(DebugLogModel::TableColumn::Message),  DebugLogModel::TableColumn::Message, 512);
    addAndMakeVisible(logTable.get());

    for (int i = 0; i < numColumns; i++)
    {
        DebugLogModel::TableColumn column = columns[i];
        juce::String columnName = DebugLogModel::TableColumnName(column);
        juce::ToggleButton* btn = columnToggle.add(std::make_unique<juce::ToggleButton>(columnName));
        btn->setButtonText(columnName);
        btn->setToggleState(true, juce::NotificationType::dontSendNotification);
        btn->onStateChange = [this, i, btn]() { logTable->getHeader().setColumnVisible(i + 1, btn->getToggleState()); };
        addAndMakeVisible(btn);
    }

    logModel->addChangeListener(this);

    keyMonitor = std::make_unique<KeyMonitorComponent>();
    addAndMakeVisible(keyMonitor.get());

    monitorDeviceToggle = std::make_unique<juce::ToggleButton>("Monitor Device online status");
    monitorDeviceToggle->onStateChange = [this]()
    {
        TerpstraSysExApplication::getApp().getLumatoneController()->checkConnectionWhenInactive(monitorDeviceToggle->getToggleState());
    };

    addAndMakeVisible(monitorDeviceToggle.get());

    // Set default settings
    logTable->getHeader().setColumnVisible(DebugLogModel::TableColumn::Class, false);
    logTable->getHeader().setColumnVisible(DebugLogModel::TableColumn::Type, false);
    // logTable->getHeader().setColumnVisible(DebugLogModel::TableColumn::Method, false);

    bool monitorDevice = TerpstraSysExApplication::getApp().getPropertiesFile()->getBoolValue("CheckConnectionIfInactive");
    monitorDeviceToggle->setToggleState(monitorDevice, juce::NotificationType::dontSendNotification);
}

DebugComponent::~DebugComponent()
{
    logModel->removeChangeListener(this);
}

void DebugComponent::paint(juce::Graphics& g)
{

}

void DebugComponent::resized()
{
    int monitorMarginX = 12;
    int monitorMarginY = 18;
    int monitorWidth = (getWidth() / 2) - monitorMarginX;
    int monitorHeight = getHeight() - monitorMarginY * 2;
    keyMonitor->setBounds(monitorMarginX, monitorMarginY, monitorWidth, monitorHeight);

    // int tableX = getWidth() * 0.5f;
    // int tableY = 0;
    // int tableWidth = getWidth();
    // int tableHeight = getHeight();
    // int toggleRowHeight = 24;
    // logTable->setBounds(0, toggleRowHeight, tableWidth, tableHeight - toggleRowHeight);
    int tableMargin = 10;
    logTable->setBounds(getLocalBounds().withTrimmedLeft(getWidth() * 0.2).reduced(tableMargin));

    // add other controls
    // int col_id = (int)DebugLogModel::TableColumn::Date;
    // juce::ToggleButton* btn = columnToggle.getUnchecked(col_id - 1);
    // int col_x = 0;
    // int col_w = logTable->getHeader().getColumnWidth(col_id);
    // btn->setBounds(col_x, tableY, col_w, toggleRowHeight);
    // col_x += col_w;

    // col_id = (int)DebugLogModel::TableColumn::Class;
    // btn = columnToggle.getUnchecked(col_id - 1);
    // col_w = logTable->getHeader().getColumnWidth(col_id);
    // btn->setBounds(col_x, tableY, col_w, toggleRowHeight);
    // col_x += col_w;

    // col_id = (int)DebugLogModel::TableColumn::Status;
    // btn = columnToggle.getUnchecked(col_id - 1);
    // col_w = logTable->getHeader().getColumnWidth(col_id);
    // btn->setBounds(col_x, tableY, col_w, toggleRowHeight);
    // col_x += col_w;

    // col_id = (int)DebugLogModel::TableColumn::Type;
    // btn = columnToggle.getUnchecked(col_id - 1);
    // col_w = logTable->getHeader().getColumnWidth(col_id);
    // btn->setBounds(col_x, tableY, col_w, toggleRowHeight);
    // col_x += col_w;

    // col_id = (int)DebugLogModel::TableColumn::Method;
    // btn = columnToggle.getUnchecked(col_id - 1);
    // col_w = logTable->getHeader().getColumnWidth(col_id);
    // btn->setBounds(col_x, tableY, col_w, toggleRowHeight);
    // col_x += col_w;

    // col_id = (int)DebugLogModel::TableColumn::Message;
    // btn = columnToggle.getUnchecked(col_id - 1);
    // col_w = logTable->getHeader().getColumnWidth(col_id);
    // btn->setBounds(col_x, tableY, col_w, toggleRowHeight);
    // col_x += col_w;

    // col_id = DebugLogModel::TableColumn::Info;
    // btn = columnToggle.getUnchecked(col_id);
    // col_w = logTable->getHeader().getColumnWidth(col_id);
    // btn->setBounds(col_x, 0, col_w, toggleRowHeight);
    // col_x += col_w;

    // Settings
    int toggleHeight = 24;
    int toggleWidth = logTable->getX();
    monitorDeviceToggle->setBounds(0, getHeight() - toggleHeight, toggleWidth, toggleHeight);
}

void DebugComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == logModel)
    {
        logTable->resized();

        if (scrollToNewLogs)
        {
            logTable->scrollToEnsureRowIsOnscreen(logModel->getNumRows());
        }
    }
}
