/*
  ==============================================================================

    KeyEditorControls.h
    Created: 3 June 2024
    Author:  Vincenzo

  ==============================================================================
*/

#include "KeyEditorControls.h"
#include "../../style/LumatoneEditorLookAndFeel.h"

#include "./MultiSelectControls.h"
#include "../palettes/ColourSelectorPanel.h"

#include "../../controls/ColourDropdownSelector.h"
#include "../../controls/LumatoneEditorControl.h"

#include "../../lumatone_editor_library/palettes/colour_picker_panel.h"
#include "../../lumatone_editor_library/palettes/colour_selection_group.h"

#include "../../actions/EditorControlActions.h"


KeyEditorControls::KeyEditorControls(const LumatoneEditorState& stateIn)
        : juce::Component("KeyEditorControls")
        , LumatoneEditorState("KeyEditorControls", stateIn)
        , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
{
    lblKeySettings = std::make_unique<juce::Label>("lblKeySettings", "Assign / Key Settings");
    lblKeySettings->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LabelBlue));
    lblKeySettings->setFont(getAppFonts().getFont(LumatoneEditorFont::UniviaProBold));
    addAndMakeVisible(lblKeySettings.get());

    colourInputBox = std::make_unique<LumatoneEditorControl>("colourInputBox", LumatoneEditorControl::Style::ColourDropdownInput, true);
    addAndMakeVisible(*colourInputBox);
    colourDropdown = colourInputBox->getColourSelector();

    keyTypeCombo = std::make_unique<LumatoneEditorControl>("keyTypeCombo", LumatoneEditorControl::Style::DropdownBox, true);
    keyTypeCombo->addOption(juce::translate("Note on/Note off"), (int)LumatoneKeyType::noteOnNoteOff);
    keyTypeCombo->addOption(juce::translate("Continuous Controller"), (int)LumatoneKeyType::continuousController);
    keyTypeCombo->addOption(juce::translate("Lumatouch"), (int)LumatoneKeyType::lumaTouch);
    keyTypeCombo->addOption(juce::translate("Disabled"), (int)LumatoneKeyType::disabled);
    keyTypeCombo->setValueChangedCallback([&]()
    {
        // performAction(SetKeySettingsAction::NewSetAssignKeyTypeAction(*this, LumatoneKeyType(keyTypeCombo->getSelectedId())));
        performAction(SetKeySettingsAction::NewSetAssignKeyTypeAction(*this, LumatoneKeyType(keyTypeCombo->getValue())));

        if (inAssignMode() && keyTypeCombo->getValue() > 0)
            performAction(new ApplyAssignmentsToSelectionAction(*this, getEditSelectionData(), *getSelectedKeys()));
    });
    addAndMakeVisible(keyTypeCombo.get());

    noteInput = std::make_unique<LumatoneEditorControl>("noteInput", 0, 127, LumatoneEditorControl::Style::IncDecButtons, true);
    noteInput->setTooltip (juce::translate("MIDI note or MIDI controller no. (for key type \'continuous controller\')"));
    noteInput->setValueChangedCallback([&]()
    {
        performAction(SetKeySettingsAction::NewSetAssignKeyNoteAction(*this, (int)noteInput->getValue()));
        if (inAssignMode() && !noteInput->isValueNull())
            performAction(new ApplyAssignmentsToSelectionAction(*this, getEditSelectionData(), *getSelectedKeys()));
    });
    addAndMakeVisible(noteInput.get());

    channelInput = std::make_unique<LumatoneEditorControl>("channelInput", 1, 16, LumatoneEditorControl::Style::IncDecButtons, true);
    channelInput->setValueChangedCallback([&]()
    {
        performAction(SetKeySettingsAction::NewSetAssignKeyChannelAction(*this, (int)channelInput->getValue()));
        if (inAssignMode() && !channelInput->isValueNull())
            performAction(new ApplyAssignmentsToSelectionAction(*this, getEditSelectionData(), *getSelectedKeys()));
    });
    addAndMakeVisible(channelInput.get());

    autoIncrementToggleButton.reset (new juce::ToggleButton ("autoIncrementToggleButton"));
    addAndMakeVisible (autoIncrementToggleButton.get());
    autoIncrementToggleButton->setButtonText (juce::translate("Auto-Increment"));
    autoIncrementToggleButton->onClick = [&] { autoIncrementToggleCallback(autoIncrementToggleButton->getToggleState()); };
    autoIncrementToggleButton->setAlwaysOnTop(true);

    noteAutoIncrInput.reset (new juce::Slider ("noteAutoIncrInput"));
    noteAutoIncrInput->setTooltip (juce::translate("Increment notes per-click by: "));
    noteAutoIncrInput->setRange (0, 127, 1);
    noteAutoIncrInput->setSliderStyle (juce::Slider::IncDecButtons);
    noteAutoIncrInput->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 56, 20);
    addAndMakeVisible(noteAutoIncrInput.get());

    channelAutoIncrNoteInput.reset (new juce::Slider ("channelAutoIncrNoteInput"));
    channelAutoIncrNoteInput->setTooltip (juce::translate("After reaching this note, the channel is incremented and the note is reset to 0."));
    channelAutoIncrNoteInput->setRange (1, 16, 1);
    channelAutoIncrNoteInput->setSliderStyle (juce::Slider::IncDecButtons);
    channelAutoIncrNoteInput->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 56, 20);
    addAndMakeVisible(channelAutoIncrNoteInput.get());

    lblColour = std::make_unique<juce::Label>("lblColour", juce::translate("Colour") + juce::String(":"));
    lblColour->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    lblColour->setJustificationType(juce::Justification::centredLeft);
    lblColour->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    addAndMakeVisible(lblColour.get());

    lblKeyType = std::make_unique<juce::Label>("lblKeyType", "Type:");
    lblKeyType->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    lblKeyType->setJustificationType(juce::Justification::centredLeft);
    lblKeyType->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    addAndMakeVisible(lblKeyType.get());

    lblNote = std::make_unique<juce::Label>("lblNote", "Note #:");
    lblNote->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    lblNote->setJustificationType(juce::Justification::centredLeft);
    lblNote->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    addAndMakeVisible(lblNote.get());

    lblChannel = std::make_unique<juce::Label>("lblChannel", "Channel #:");
    lblChannel->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    lblChannel->setJustificationType(juce::Justification::centredLeft);
    lblChannel->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    addAndMakeVisible(lblChannel.get());

    lblAutoIncNotes = std::make_unique<juce::Label>("lblAutoIncNotes", "Notes, per-click:");
    lblAutoIncNotes->setFont(getAppFonts().getFont(LumatoneEditorFont::GothamNarrowMedium));
    // lblAutoIncNotes->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, controlBoxFontHeightScalar);
    lblAutoIncNotes->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    lblAutoIncNotes->setJustificationType(juce::Justification::centredLeft);
    lblAutoIncNotes->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    addAndMakeVisible(lblAutoIncNotes.get());

    lblAutoIncChannels = std::make_unique<juce::Label>("lblAutoIncChannels", juce::translate("ChannelsAfterNote") + ":");
    lblAutoIncChannels->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    lblAutoIncChannels->setJustificationType(juce::Justification::centredLeft);
    lblAutoIncChannels->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    addAndMakeVisible(lblAutoIncChannels.get());

    colourPalettePanel = std::make_unique<ColourSelectorPanel>(stateIn);
    colourPalettePanel->setBackgroundColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ColourPaletteBackground));
    addAndMakeVisible(colourPalettePanel.get());

    LumatoneEditorState::addColourSelectionBroadcaster(colourDropdown);
    LumatoneEditorState::addColourSelectionListener(colourDropdown);
    LumatoneEditorState::addColourSelectionListener(this);
    LumatoneEditorState::addColourSelectionBroadcaster(this);

    // Todo - callback for changing select/assign mode
    autoIncrementToggleCallback(autoIncrementToggleButton->getToggleState());

    addEditorListener(this);
}

KeyEditorControls::~KeyEditorControls()
{
    LumatoneEditorState::removeColourSelectionBroadcaster(this);
    LumatoneEditorState::removeColourSelectionBroadcaster(this);
    LumatoneEditorState::removeColourSelectionListener(colourDropdown);
    LumatoneEditorState::removeColourSelectionBroadcaster(colourDropdown);

    colourPalettePanel = nullptr;

    lblChannelAutoIncr = nullptr;
    lblAutoIncChannels = nullptr;
    lblAutoIncNotes = nullptr;

    lblChannel = nullptr;
    lblNote = nullptr;
    lblKeyType = nullptr;
    lblColour = nullptr;

    channelAutoIncrNoteInput = nullptr;
    noteAutoIncrInput = nullptr;
    autoIncrementToggleButton = nullptr;

    channelInput = nullptr;
    noteInput = nullptr;
    keyTypeCombo = nullptr;

    colourInputBox = nullptr;

    lblKeySettings = nullptr;
}

void KeyEditorControls::paint(juce::Graphics& g)
{
    g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlAreaBackground));
    g.fillPath(controlPath);

    g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlAreaHeader));
    g.fillPath(headerPath);

    // Draw border around auto-increment area, block-out space for toggle button
    g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlBoxBackground));
    g.strokePath(autoIncrBorder, juce::PathStrokeType(2));

    g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlAreaBackground));
    g.fillRect(autoIncrementToggleButton->getBounds().withX(autoIncrementToggleButton->getX() - autoIncrementToggleButton->getHeight() * 2 / 5));
}

void KeyEditorControls::resized()
{
    float w = (float)getWidth();
    float h = (float)getHeight();

    // controlMarginH = JUCE_LIVE_CONSTANT(controlMarginH);
    // keyControlColumnW = JUCE_LIVE_CONSTANT(keyControlColumnW);
    // keyControlH = JUCE_LIVE_CONSTANT(keyControlH);
    // keyControlMarginH = JUCE_LIVE_CONSTANT(keyControlMarginH);
    // columnMarginW = JUCE_LIVE_CONSTANT(columnMarginW);
    // colourButtonParentW = JUCE_LIVE_CONSTANT(colourButtonParentW);
    // colourColumnH = JUCE_LIVE_CONSTANT(colourColumnH);

    float windowH = getWindowBounds().getHeight();

    auto roundedCornerSize = getRoundedRectCornerSize();

    // const float labelWidth = JUCE_LIVE_CONSTANT(0.12) * w;
    float labelWidth = 0.12 * w;

    headerHeight = roundToInt(windowH * contentLabelHeightWindowH);
    headerPath = getConnectedRoundedRectPath(getLocalBounds().withBottom(headerHeight + 1).toFloat(), roundedCornerSize, juce::Button::ConnectedEdgeFlags::ConnectedOnBottom);
    controlPath = getConnectedRoundedRectPath(getLocalBounds().withTop(headerHeight).toFloat(), roundedCornerSize, juce::Button::ConnectedEdgeFlags::ConnectedOnTop);

    contentMarginWidth = roundToInt(windowH * contentMarginWidthWindowH);
    contentMarginHeight = roundToInt(h * controlMarginH);

    // first control column

    labelHeight = roundToInt(headerHeight * contentLabelFontScalar);
    lblKeySettings->setTopLeftPosition(contentMarginWidth, 0);
    resizeLabelWithHeight(lblKeySettings.get(), headerHeight, contentLabelFontScalar, "_");

    keyControlColumnWidth = roundToInt(w * keyControlColumnW);
    keyControlColumnRight = contentMarginWidth + keyControlColumnWidth;

    int controlAreaHeight = (h - headerHeight);
    int numRows = 4;
    keyControlMarginHeight = juce::roundToInt(h * keyControlMarginH);
    controlRowHeight = juce::roundToInt((controlAreaHeight - ((keyControlMarginHeight + 1) * numRows)) / (float)numRows);

    int controlMarginHeight = juce::roundToInt(controlRowHeight * keyControlH);
    keyControlHeight = controlRowHeight - controlMarginHeight;

    lblColour->setTopLeftPosition(contentMarginWidth, headerHeight + contentMarginHeight + controlMarginHeight);
    resizeLabelWithHeight(lblColour.get(), keyControlHeight, contentLabelFontScalar, "_");

    colourButtonMargin = lblColour->getFont().getStringWidth(" ");
    colourButtonWidth = roundToInt(getParentWidth() * colourButtonParentW) - colourButtonMargin;

    colourInputBox->setBounds(lblColour->getRight(), lblColour->getY(), keyControlColumnRight - lblColour->getRight(), keyControlHeight);

    lblKeyType->setTopLeftPosition(contentMarginWidth, lblColour->getBottom() + keyControlMarginHeight);
    resizeLabelWithHeight(lblKeyType.get(), keyControlHeight, controlLabelFontScalar, "_");
    keyTypeCombo->setBounds(lblKeyType->getRight(), lblKeyType->getY(), keyControlColumnRight - lblKeyType->getRight(), keyControlHeight);


    // second column, colour area

    colourPalettePanel->setBounds(colourColumnX, 0, colourColumnWidth, keyTypeCombo->getBottom() + contentMarginHeight);
    // colourPalettePanel->setBounds(colourColumnX, 0, colourColumnWidth, keyTypeCombo->getBottom());
    // colourPalettePanel->setIndentSize(contentMarginHeight, false);
    colourPalettePanel->setTabBarDepth(headerHeight, true);

    colourColumnX = contentMarginWidth + keyControlColumnWidth + roundToInt(w * columnMarginW);
    colourColumnWidth = w - colourColumnX - contentMarginWidth;
    colourColumnHeight = roundToInt(h * colourColumnH);

    // bottom two rows

    lblNote->setTopLeftPosition(contentMarginWidth, keyTypeCombo->getBottom() + keyControlMarginHeight);
    resizeLabelWithHeight(lblNote.get(), keyControlHeight, controlLabelFontScalar, "_");
    noteInput->setBounds(lblNote->getRight(), lblNote->getY(), keyControlColumnRight - lblNote->getRight(), keyControlHeight);

    lblChannel->setTopLeftPosition(contentMarginWidth, noteInput->getBottom() + keyControlMarginHeight);
    resizeLabelWithHeight(lblChannel.get(), keyControlHeight, controlLabelFontScalar, "_");
    channelInput->setBounds(lblChannel->getRight(), lblChannel->getY(), keyControlColumnRight - lblChannel->getRight(), keyControlHeight);


    autoIncrementBounds = juce::Rectangle<int>(colourColumnX, colourPalettePanel->getBottom() + controlMarginH, getWidth() - colourColumnX - contentMarginWidth, getHeight() - colourPalettePanel->getBottom() - contentMarginHeight);
    autoIncrBorder = juce::Path();
    autoIncrBorder.addRoundedRectangle(autoIncrementBounds, roundedCornerSize);

    juce::String autoIncToggleWidthRef = autoIncrementToggleButton->getButtonText() + juce::String::repeatedString("__", 2);
    int autoIncToggleHeight = juce::roundToInt(keyControlHeight * 0.4f);
    int autoIncToggleWidth = getEditorLookAndFeel().getToggleButtonFont(*autoIncrementToggleButton, autoIncToggleHeight).getStringWidth(autoIncToggleWidthRef);
    // autoIncrementToggleButton->setBounds(autoIncrementBounds.withSize(autoIncToggleWidth, autoIncToggleHeight).translated(autoIncToggleHeight*2, - autoIncToggleHeight / 2));
    autoIncrementToggleButton->setBounds(autoIncrementBounds.getX() + autoIncToggleHeight*2, autoIncrementBounds.getY() - autoIncToggleHeight / 2, autoIncToggleWidth, autoIncToggleHeight);

    // second column
    int autoControlBoundsMargin = keyControlMarginHeight / 2;
    juce::Rectangle<int> autoControlBounds = autoIncrementBounds.reduced(autoControlBoundsMargin * 2, autoControlBoundsMargin);
    int autoControlMarginHeight = autoControlBounds.getHeight() * 0.25f;
    int autoControlHeight = (autoControlBounds.getHeight() - autoControlMarginHeight) / 2;

    // juce::Font buttonFont = getEditorLookAndFeel().getTextButtonFont(*noteAutoIncButton, keyControlHeight);
    juce::Font buttonFont = getEditorLookAndFeel().getSliderTextBoxFont(autoControlHeight);
    int noteInputWidth = buttonFont.getStringWidth("999");
    // int autoNoteButtonWidth = buttonFont.getStringWidth(juce::translate("Auto"));
    // noteAutoIncButton->setBounds(autoControlBounds.getX(), autoControlBounds.getY(), autoNoteButtonWidth, autoControlHeight);
    resizeLabelWithHeight(lblAutoIncNotes.get(), autoControlHeight, controlLabelFontScalar, "_" /*, getWidth() - keyControlColumnRight*/);
    lblAutoIncNotes->setTopLeftPosition(autoControlBounds.getX(), autoControlBounds.getY() + (autoControlBoundsMargin / 2));
    noteAutoIncrInput->setTextBoxStyle(juce::Slider::TextBoxLeft, false, noteInputWidth, autoControlHeight);
    noteAutoIncrInput->setBounds(lblAutoIncNotes->getRight(), lblAutoIncNotes->getY(), noteInputWidth + autoControlHeight*2, autoControlHeight);

    int autoChnlLabelWidth = getEditorLookAndFeel().getLabelFont(*lblAutoIncChannels).getStringWidth(lblAutoIncChannels->getText());
    resizeLabelWithHeight(lblAutoIncChannels.get(), autoControlHeight, controlLabelFontScalar, "_");
    lblAutoIncChannels->setTopLeftPosition(autoControlBounds.getX(), lblAutoIncNotes->getBottom() + autoControlMarginHeight/2);
    // channelAutoIncrNoteInput->setTopLeftPosition(autoControlBounds.getX(), autoControlBounds.getY() + autoControlHeight);
    channelAutoIncrNoteInput->setTextBoxStyle(juce::Slider::TextBoxLeft, false, noteInputWidth, autoControlHeight);
    channelAutoIncrNoteInput->setBounds(lblAutoIncChannels->getRight(), lblAutoIncChannels->getY(), noteInputWidth + autoControlHeight*2, autoControlHeight);


    // int selectionTabBarWidth = juce::roundToInt(w * 0.4f);
    // selectionTabBar->setBounds(w - selectionTabBarWidth, 0, selectionTabBarWidth, headerHeight);
    // singleSelectControls->setBounds(w - selectionTabBarWidth, headerHeight, selectionTabBarWidth, h - headerHeight);
    // multiSelectControls->setBounds(w - selectionTabBarWidth, headerHeight, selectionTabBarWidth, h - headerHeight);
}

void KeyEditorControls::selectionChanged()
{
    // Make more efficient? (each time goes through loop)
    auto newData = LumatoneEditSelectionState::findSharedSelectionProperties(*getSelectedKeys());

    if (newData.useColour)
    {
        lastSelectedColour = newData.colour;
        // setAssignKeyColour(true, lastSelectedColour);
        // colourSelectionGroup->setSelectedColour(newData.colour, nullptr);
        // colourSubwindow->setColour(newData.colour.toString(), false);

        // selectorListeners.call(&ColourSelectionListener::colourChangedCallback, (ColourSelectionBroadcaster*)this, lastSelectedColour);
        // performAction(SetKeySettingsAction::NewSetAssignColourAction(*this, newData.colour), true, false);

        // if (newData.colour.isTransparent())
        //     colourTextEditor->setText("", juce::NotificationType::dontSendNotification);
        // else
        //     colourTextEditor->setText(newData.colour.toDisplayString(false), juce::NotificationType::dontSendNotification);
    }
    else if (getSelectedKeys()->size() > 0)
    {
        // TODO use first key that matches use-properties
        // KeyProperties properties; // ...

        lastSelectedColour = getSelectedKeys()->getUnchecked(0).getColour();
        // setAssignKeyColour(true, lastSelectedColour);

        // selectorListeners.call(&ColourSelectionListener::colourChangedCallback, (ColourSelectionBroadcaster*)this, lastSelectedColour);

        // colourSubwindow->setColour("", false);
        // colourDropdown->clearColour(false);
        // colourTextEditor->setText("", juce::NotificationType::dontSendNotification);
    }

    if (newData.useType)
        keyTypeCombo->setValue((int)newData.type, juce::NotificationType::dontSendNotification);
    else
        keyTypeCombo->setValue(0, juce::NotificationType::dontSendNotification);
    // if (newData.useType)
    //     keyTypeCombo->setSelectedId((int)newData.type, juce::NotificationType::dontSendNotification);
    // else
    //     keyTypeCombo->setSelectedId(0, juce::NotificationType::dontSendNotification);

    if (newData.useNote)
        noteInput->setValue(newData.note, juce::NotificationType::dontSendNotification);
    else
        noteInput->setValue(-1, juce::NotificationType::dontSendNotification);

    if (newData.useChannel)
        channelInput->setValue(newData.channel, juce::NotificationType::dontSendNotification);
    else
        channelInput->setValue(-1, juce::NotificationType::dontSendNotification);
}

// void KeyEditorControls::setSelectionTab(SelectionTabs tabIndex)
// {
//     if (selectionControls != nullptr)
//         selectionControls->setVisible(false);

//     switch (tabIndex)
//     {
//     case SelectionTabs::Multi:
//         selectionControls = multiSelectControls.get();
//         break;
//     case SelectionTabs::Single:
//     default:
//         selectionControls = singleSelectControls.get();
//         break;
//     }

//     selectionControls->setVisible(true);
// }

void KeyEditorControls::autoIncrementToggleCallback(bool isToggled)
{
    if (isToggled)
    {
        noteAutoIncrInput->setEnabled(true);
        channelAutoIncrNoteInput->setEnabled(true);
        lblAutoIncNotes->setEnabled(true);
        lblAutoIncChannels->setEnabled(true);
    }
    else
    {
        noteAutoIncrInput->setEnabled(false);
        channelAutoIncrNoteInput->setEnabled(false);
        lblAutoIncNotes->setEnabled(false);
        lblAutoIncChannels->setEnabled(false);
    }
}

void KeyEditorControls::handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier &property)
{
    LumatoneEditorState::handleStatePropertyChange(stateIn, property);

    // juce::var value = stateIn.getProperty(property);

    // if (property == LumatoneEditSelectionProperty::AssignKeyColour)
    // {
    //     auto colourString = stateIn[property].toString();
    //     // colourTextEditor->setText(colourString, juce::NotificationType::dontSendNotification);
    //     // colourSubwindow->setColour(colourString, false);
    //     colourDropdown->setSelectedColour(juce::Colour::fromString(colourString), true,  false);
    // }
    // else if (property == LumatoneEditSelectionProperty::AssignKeyType)
    // {
    //     keyTypeCombo->setSelectedId(((int)value) + 1, juce::NotificationType::dontSendNotification);
    // }
    // else if (property == LumatoneEditSelectionProperty::AssignKeyNote)
    // {
    //     noteInput->setValue((int)value, juce::NotificationType::dontSendNotification);
    // }
    // else if (property == LumatoneEditSelectionProperty::AssignKeyChannel)
    // {
    //     channelInput->setValue((int)value, juce::NotificationType::dontSendNotification);
    // }
    // else if (property == LumatoneEditSelectionProperty::AssignKeyCCFader)
    // {

    // }
}

void KeyEditorControls::colourChangedCallback(ColourSelectionBroadcaster *source, juce::Colour newColour)
{
    if (this == source)
        return;

    performAction(SetKeySettingsAction::NewSetAssignColourAction(*this, newColour));

    // A bit kludgey maybe
    juce::Component* src = dynamic_cast<juce::Component*>(source);
    if (src != nullptr && isParentOf(src))
    {
        colourInputBox->setLastColour(newColour);
        if (inAssignMode())
            performAction(new ApplyAssignmentsToSelectionAction(*this, getEditSelectionData(), *getSelectedKeys()), true, false);
    }
}

juce::Colour KeyEditorControls::getSelectedColour()
{
    return lastSelectedColour;
}

void KeyEditorControls::deselectColour()
{
    return;
}

void KeyEditorControls::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    // if (source == selectionTabBar.get())
    // {
    //     // resizeEditSectionTabs();
    //     setSelectionTab(SelectionTabs(selectionTabBar->getCurrentTabIndex()));
    // }
}
