/*
  ==============================================================================

    FirmwareDlg.h
    Created: 5 Feb 2021 7:33:07pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "../LumatoneEditorState.h"
#include "../FileBrowserComponent.h"
#include "../FirmwareTransfer.h"

#include "../lumatone_editor_library/listeners/firmware_listener.h"

class FirmwareDlg : public Component
                  , public LumatoneEditorState
                  , protected Button::Listener
                  , protected PathBrowserComponent::Listener
                  , protected FirmwareTransfer::ProcessListener
                  , protected LumatoneEditor::FirmwareListener
                  , private Timer
{
public:

    FirmwareDlg(const LumatoneEditorState& stateIn);
    ~FirmwareDlg();

    void paint(juce::Graphics& g) override;

    void resized() override;

    void buttonClicked(juce::Button* btn) override;

    void updateFirmwareVersionLabel();

    void postMessage(juce::String msgForLog);

    //=========================================================================
    // PathBrowserComponent::Listener Implementation
    void fileChanged(PathBrowserComponent* source, juce::File) override;

    //=========================================================================
    // FirmwareTransfer::Listener Implementation
    void firmwareTransferUpdate(FirmwareTransfer::StatusCode statusCode, juce::String msg) override;

    //=========================================================================
    // LumatoneEditor::FirmwareListener implementation

    void firmwareRevisionReceived(LumatoneFirmware::Version version) override;

    //=========================================================================
    // juce::Timer Implementation
    void timerCallback() override;

private:

    bool updateIsAvailable = false;
    bool firmwareUpdateInProgress = false;

    juce::File firmwareFileSelected;

    //std::unique_ptr<TextButton> checkUpdateBtn;
    std::unique_ptr<PathBrowserComponent> fileBrowser;
    std::unique_ptr<juce::TextButton> doUpdateBtn;
    std::unique_ptr<juce::TextEditor> infoBox;

    std::unique_ptr<juce::Label> firmwareStatusLabel;

    juce::String msgLog;
    bool infoNeedsUpdate = false;
    const int infoUpdateTimeoutMs = 100;

    // Style helpers
    int margin = 12;
    int doubleMargin = margin * 2;
    float buttonWidthScalar = 0.3f;
    int buttonHeight = 30;
};
