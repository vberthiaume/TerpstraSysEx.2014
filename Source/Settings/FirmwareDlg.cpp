/*
  ==============================================================================

    FirmwareDlg.cpp
    Created: 5 Feb 2021 7:33:07pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "FirmwareDlg.h"

#include "../style/LumatoneEditorLookAndFeel.h"
#include "../style/LumatoneEditorFontLibrary.h"

#include "../lumatone_editor_library/device/lumatone_controller.h"

FirmwareDlg::FirmwareDlg(const LumatoneEditorState& stateIn)
    : LumatoneEditorState("FirmwareDlg", stateIn)
{
    //checkUpdateBtn.reset(new TextButton("checkUpdateBtn"));
    //checkUpdateBtn->setButtonText("Check For Updates");
    //checkUpdateBtn->addListener(this);
    //addAndMakeVisible(checkUpdateBtn.get());

    // auto properties = TerpstraSysExApplication::getApp().getPropertiesFile();
    // File lastFirmwareLocation = properties->getValue("LastFirmwareBinPath", properties->getValue("UserDocumentsLocation", File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getFullPathName()));

    String openFileType =
#if JUCE_DEBUG
        ""
#else
        "*.tgz"
#endif
        ;
    fileBrowser.reset(new PathBrowserComponent("Lumatone Firmware Update", openFileType));
    fileBrowser->getEditor()->setColour(TextEditor::ColourIds::backgroundColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlBoxBackground));
    fileBrowser->getEditor()->setColour(TextEditor::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    fileBrowser->getEditor()->getProperties().set(LumatoneEditorStyleIDs::connectedEdgeFlags, Button::ConnectedEdgeFlags::ConnectedOnRight);
    fileBrowser->addListener(this);
    addAndMakeVisible(fileBrowser.get());

    doUpdateBtn.reset(new TextButton("checkUpdateBtn"));
    doUpdateBtn->setButtonText(translate("Begin Update"));
    doUpdateBtn->addListener(this);
    addAndMakeVisible(doUpdateBtn.get());

    infoBox.reset(new TextEditor("FirmwareUpdateInfoBox"));
    infoBox->setMouseClickGrabsKeyboardFocus(false);
    infoBox->setReadOnly(true);
    infoBox->setMultiLine(true);
    infoBox->setColour(TextEditor::ColourIds::backgroundColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlBoxBackground));
    infoBox->setColour(TextEditor::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    infoBox->setColour(ScrollBar::ColourIds::thumbColourId, Colour(0xff2d3135));
    infoBox->getProperties().set(LumatoneEditorStyleIDs::connectedEdgeFlags, 15);
    addAndMakeVisible(infoBox.get());

    firmwareStatusLabel.reset(new Label("FirmwareVersionStatusLabel"));
    addAndMakeVisible(firmwareStatusLabel.get());
    updateFirmwareVersionLabel();

    addFirmwareListener(this);

    postMessage(translate("Select a firmware file and then click \"Begin Update\""));

    //if (!updateIsAvailable)
    //{
        //doUpdateBtn->setEnabled(false);
    //}
}

FirmwareDlg::~FirmwareDlg()
{
    removeFirmwareListener(this);
}

void FirmwareDlg::paint(Graphics& g)
{

}

void FirmwareDlg::resized()
{
    int buttonWidth = proportionOfWidth(buttonWidthScalar);

    //checkUpdateBtn->setBounds(margin, margin, buttonWidth, buttonHeight);

    doUpdateBtn->setBounds(getWidth() - margin - buttonWidth, margin, buttonWidth, buttonHeight);
    fileBrowser->setBounds(margin, margin, doUpdateBtn->getX() - doubleMargin, buttonHeight);

    firmwareStatusLabel->setBounds(margin, fileBrowser->getBottom() + margin, getWidth() - doubleMargin, buttonHeight / 2);

    infoBox->setBounds(getLocalBounds().withTop(firmwareStatusLabel->getBottom()).reduced(margin));
    infoBox->applyFontToAllText(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
}

void FirmwareDlg::buttonClicked(Button* btn)
{
    //if (btn == checkUpdateBtn.get())
    //{
    //    DBG("Version check requested");
    //}
    if (btn == doUpdateBtn.get())
    {
        if (getMidiInputIndex() < 0 || getMidiOutputIndex() < 0)
        {
            AlertWindow::showMessageBoxAsync(AlertWindow::AlertIconType::NoIcon, "Not connected", "Please connect the Lumatone via USB before performing a firmware update.", "Ok", this);
            return;
        }

        DBG("Update requested");

        if (firmwareFileSelected.existsAsFile())
        {
            // TerpstraSysExApplication::getApp().getPropertiesFile()->setValue("LastFirmwareBinPath", firmwareFileSelected.getParentDirectory().getFullPathName());
            // getLumatoneController()->requestFirmwareUpdate(firmwareFileSelected, this);
        }
        else
        {
            postMessage("Error: Not a valid firmware file...");
            //infoBox->setText();
        }
    }
}

// TODO: use LumatoneFirmwareVersion
void FirmwareDlg::updateFirmwareVersionLabel()
{
    // juce::String firmwareVersion = TerpstraSysExApplication::getApp().getFirmwareVersionStr();
    juce::String firmwareVersion = getFirmwareVersion().toString();
    if (firmwareVersion != "0.0.0")
    {
        firmwareStatusLabel->setText("Current firmware version is: " + firmwareVersion, dontSendNotification);
    }
    else
    {
        firmwareStatusLabel->setText("Unable to determine firmware version.", dontSendNotification);
    }
}

void FirmwareDlg::fileChanged(PathBrowserComponent* source, File newFile)
{
    firmwareFileSelected = newFile;
}

void FirmwareDlg::firmwareTransferUpdate(FirmwareTransfer::StatusCode statusCode, String msg)
{
    if ((int)statusCode <= 0)
    {
        firmwareUpdateInProgress = false;
    }

    if (msg != "")
    {
        postMessage(msg);
    }
}

void FirmwareDlg::timerCallback()
{
    if (infoNeedsUpdate)
    {
        infoBox->setText(msgLog);
        infoNeedsUpdate = false;
    }
}

void FirmwareDlg::firmwareRevisionReceived(LumatoneFirmware::Version version)
{
     updateFirmwareVersionLabel();
     postMessage("Firmware update complete! Lumatone is now running firmware version " + version.toDisplayString());
}

void FirmwareDlg::postMessage(String msg)
{
    msgLog += msg + '\n';
    infoNeedsUpdate = true;
    startTimer(infoUpdateTimeoutMs);
}
