/*
  ==============================================================================

    DeveloperDlg.cpp
    Created: 6 May 2021 7:17:34pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "DeveloperDlg.h"
#include "../Main.h"

DeveloperDlg::DeveloperDlg()
    : Component("DeveloperDialog")
{
    btnMidiLog.reset(new TextButton("MidiLogButton", translate("Show a log of MIDI messages sent and received")));
    btnMidiLog->setButtonText(translate("MIDI Log"));
    addAndMakeVisible(btnMidiLog.get());
    btnMidiLog->addListener(this);

    btnShowCommandLine.reset(new TextButton("ShowCommandLineButton", translate("Open window with a command line to send and review messages")));
    btnShowCommandLine->setButtonText(translate("Command Line"));
    addAndMakeVisible(btnShowCommandLine.get());
    btnShowCommandLine->addListener(this);

    btnPropertyViewer.reset(new TextButton("PropertyViewerButton", translate("View various settings in current Lumatone layout")));
    btnPropertyViewer->setButtonText(translate("Properties Viewer"));
    addAndMakeVisible(btnPropertyViewer.get());
    btnPropertyViewer->addListener(this);

    toggleDemoMode.reset(new ToggleButton("Toggle Demo Mode"));
    addAndMakeVisible(toggleDemoMode.get());
    toggleDemoMode->addListener(this);

    cbTableSelector.reset(new ComboBox("CurveTableSelector"));
    cbTableSelector->addItem("Velocity", ResetMenuId::Velocity);
    cbTableSelector->addItem("Fader", ResetMenuId::Fader);
    cbTableSelector->addItem("Aftertouch", ResetMenuId::Aftertouch);
    cbTableSelector->addItem("Lumatouch", ResetMenuId::Lumatouch);
    cbTableSelector->addItem("Board Thresholds", ResetMenuId::BoardThresholds);
    cbTableSelector->addItem("Pitch & Mod Wheel Thresholds", ResetMenuId::WheelThresholds);
    cbTableSelector->addItem("Expression Pedal Bounds", ResetMenuId::ExprPedalBounds);
    addAndMakeVisible(cbTableSelector.get());

    btnTableReset.reset(new TextButton("TableResetButton", translate("Reset the selected table to factory settings")));
    btnTableReset->setButtonText(translate("Reset Table"));
    addAndMakeVisible(btnMidiLog.get());
    btnTableReset->addListener(this);

    flexbox.flexDirection = FlexBox::Direction::column;
    flexbox.alignContent = FlexBox::AlignContent::spaceAround;
    flexbox.justifyContent = FlexBox::JustifyContent::center;
    flexbox.flexWrap = FlexBox::Wrap::wrap;
}

DeveloperDlg::~DeveloperDlg()
{

}

void DeveloperDlg::paint(Graphics& g)
{

}

void DeveloperDlg::resized()
{
    int w = getWidth();
    int h = getHeight();

    int controlWidth = round(w * 0.333f);
    int controlHeight = round(h * 0.12f);
    int marginW = round((w - (controlWidth * 2)));
    int marginH = round((h - (controlHeight * 2)) * 0.25);

    flexbox.items.clear();

    for (auto child : getChildren())
    {
        flexbox.items.add(FlexItem(*child).withWidth(controlWidth).withHeight(controlHeight).withMargin(FlexItem::Margin(5, 0, 5, 0)));
    }

    // Remove table reset button
    flexbox.items.removeLast(1);

    flexbox.performLayout(getLocalBounds().reduced(marginW, 0));

    cbTableSelector->setSize(cbTableSelector->getWidth() * 0.6f, cbTableSelector->getHeight());
    btnTableReset->setBounds(cbTableSelector->getRight(), cbTableSelector->getY(), cbTableSelector->getWidth() * 0.4f, cbTableSelector->getHeight());

    toggleDemoMode->setBounds(toggleDemoMode->getBounds().reduced(0, toggleDemoMode->getHeight() * 0.25f));
    resizeToggleButtonWithHeight(toggleDemoMode.get(), TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::GothamNarrowMedium), toggleDemoMode->getHeight());
}

void DeveloperDlg::buttonClicked(Button* btn)
{
    if (btn == btnMidiLog.get())
    {
        // TODO
    }
    else if (btn == btnShowCommandLine.get())
    {
        launchCommandLineDialog();
    }
    else if (btn == btnPropertyViewer.get())
    {
        // TODO
    }
    else if (btn == btnTableReset.get())
    {
        sendResetRequest((ResetMenuId)cbTableSelector->getSelectedId());
    }
    else if (btn == toggleDemoMode.get())
    {
        TerpstraSysExApplication::getApp().getLumatoneController()->setDemoModeState(toggleDemoMode->getToggleState());
    }
}

void DeveloperDlg::lookAndFeelChanged()
{
    // auto lookAndFeel = dynamic_cast<LumatoneEditorLookAndFeel*>(&getLookAndFeel());
    // if (lookAndFeel)
    // {
    //     lookAndFeel->setupTextButton(*btnMidiLog);
    //     lookAndFeel->setupTextButton(*btnPropertyViewer);
    //     lookAndFeel->setupTextButton(*btnShowCommandLine);
    //     lookAndFeel->setupToggleButton(*toggleDemoMode);
    //     lookAndFeel->setupComboBox(*cbTableSelector);
    //     lookAndFeel->setupTextButton(*btnTableReset);
    // }
}

void DeveloperDlg::sendResetRequest(ResetMenuId resetPropertyId)
{
    auto lumatoneController = TerpstraSysExApplication::getApp().getLumatoneController();

    switch (resetPropertyId)
    {
    case ResetMenuId::Velocity:
        lumatoneController->resetVelocityConfig();
        break;
    case ResetMenuId::Aftertouch:
        lumatoneController->resetAftertouchConfig();
        break;
    case ResetMenuId::Fader:
        lumatoneController->resetFaderConfig();
        break;
    case ResetMenuId::Lumatouch:
        lumatoneController->resetLumatouchConfig();
        break;
    case ResetMenuId::BoardThresholds:
        //TODO
        break;
    case ResetMenuId::WheelThresholds:
        // TODO
        break;
    case ResetMenuId::ExprPedalBounds:
        // TODO
        break;

    default:
        // Do nothing
        break;
    }
}

void DeveloperDlg::launchCommandLineDialog()
{
    auto commandWindow = new CommandLineComponent();
    //commandWindow->setLookAndFeel(&getLookAndFeel());

    DialogWindow::LaunchOptions launchOptions;
    launchOptions.content.setOwned(commandWindow);
    launchOptions.content->setSize(480, 240);

    launchOptions.dialogTitle = "Lumatone Command Line";
    launchOptions.escapeKeyTriggersCloseButton = true;
    launchOptions.useNativeTitleBar = false;
    launchOptions.resizable = false;

    launchOptions.dialogBackgroundColour = Colour();

    auto dw = launchOptions.launchAsync();
    dw->centreWithSize(548, 240);
    dw->setLookAndFeel(&TerpstraSysExApplication::getApp().getLookAndFeel().compactWindowStyle);
}
