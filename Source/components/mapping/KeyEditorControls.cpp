/*
  ==============================================================================

    KeyEditorControls.h
    Created: 3 June 2024
    Author:  Vincenzo

  ==============================================================================
*/

#include "KeyEditorControls.h"
#include "../../style/LumatoneEditorLookAndFeel.h"

// #include "./SelectionTabBar.h"
// #include "./SingleSelectControls.h"
#include "./MultiSelectControls.h"

// #include "../../controls/colour_view_component.h"
#include "../../controls/ColourDropdownSelector.h"
#include "../../lumatone_editor_library/palettes/colour_edit_textbox.h"

#include "../palettes/ColourSelectorPanel.h"
// #include "../lumatone_editor_library/palettes/palette_selection_panel.h"
#include "../../lumatone_editor_library/palettes/colour_picker_panel.h"
#include "../../lumatone_editor_library/palettes/colour_selection_group.h"

#include "../../actions/EditorControlActions.h"

#include "../../controls/RangedControl.h"

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

    // colourTextEditor = std::make_unique<ColourTextEditor>("colourTextEditor", "");
    // addAndMakeVisible(colourTextEditor.get());

    // colourSubwindow = std::make_unique<ColourViewComponent>(juce::Colour(0xff5398b7));
    // colourSubwindow->setColourButtonMode(ColourViewComponent::ColourButtonMode::Dropper);
    // addAndMakeVisible(colourSubwindow.get());

    colourDropdown = std::make_unique<ColourDropdownSelector>("AssignColourDropdown");
    // colourDropdown->setOnValueChangeCallback([&]()
    // {
    //     performAction(SetKeySettingsAction::NewSetAssignColourAction(*this, colourDropdown->getSelectedColour()));
    //     if (colourDropdown->getSelectedColour() != juce::Colours::transparentBlack)
    //         performAction(new ApplyAssignmentsToSelectionAction(*this, getEditSelectionData(), *getSelectedKeys()));
    // });

    // addColourSelectionListener(colourDropdown.get());
    // colourDropdown->setColourPickerChangedCallback([&]()
    // {
    //     addColourSelectionListener(colourDropdown.get());
    // });

    addAndMakeVisible(colourDropdown.get());
    colourDropdown->setColourOptions(colourHistory);
    colourDropdown->setShowPicker(false);
    colourDropdown->setEditText(true);

    keyTypeCombo = std::make_unique<juce::ComboBox>("keyTypeCombo");
    keyTypeCombo->setEditableText (false);
    keyTypeCombo->setJustificationType (juce::Justification::centredLeft);
    keyTypeCombo->setTextWhenNothingSelected (juce::String());
    keyTypeCombo->setTextWhenNoChoicesAvailable (juce::translate("(none)"));
    keyTypeCombo->addItem (juce::translate("Note on/Note off"), (int)LumatoneKeyType::noteOnNoteOff);
    keyTypeCombo->addItem (juce::translate("Continuous Controller"), (int)LumatoneKeyType::continuousController);
    keyTypeCombo->addItem (juce::translate("Lumatouch"), (int)LumatoneKeyType::lumaTouch);
    keyTypeCombo->addItem (juce::translate("Disabled"), (int)LumatoneKeyType::disabled);
    keyTypeCombo->onChange = [&]()
    {
        performAction(SetKeySettingsAction::NewSetAssignKeyTypeAction(*this, LumatoneKeyType(keyTypeCombo->getSelectedId())));
        performAction(new ApplyAssignmentsToSelectionAction(*this, getEditSelectionData(), *getSelectedKeys()));
    };
    addAndMakeVisible(keyTypeCombo.get());

    noteInput = std::make_unique<RangedControl>("noteInput", 0, 127, RangedControl::Style::IncDecButtons);
    noteInput->setTooltip (juce::translate("MIDI note or MIDI controller no. (for key type \'continuous controller\')"));
    noteInput->setValueChangedCallback([&]()
    {
        performAction(SetKeySettingsAction::NewSetAssignKeyNoteAction(*this, (int)noteInput->getValue()));
        if (!noteInput->isValueNull())
            performAction(new ApplyAssignmentsToSelectionAction(*this, getEditSelectionData(), *getSelectedKeys()));
    });
    addAndMakeVisible(noteInput.get());

    channelInput = std::make_unique<RangedControl>("channelInput", 1, 16, RangedControl::Style::IncDecButtons);
    channelInput->setValueChangedCallback([&]()
    {
        performAction(SetKeySettingsAction::NewSetAssignKeyChannelAction(*this, (int)channelInput->getValue()));
        if (!channelInput->isValueNull())
            performAction(new ApplyAssignmentsToSelectionAction(*this, getEditSelectionData(), *getSelectedKeys()));
    });
    addAndMakeVisible(channelInput.get());

    noteAutoIncButton.reset (new juce::TextButton ("noteAutoIncrButton", juce::translate("Assign notes per-click and step up value")));
    noteAutoIncButton->setButtonText (juce::translate("Auto"));
    noteAutoIncButton->setToggleable(true);
    addAndMakeVisible(noteAutoIncButton.get());

    noteAutoIncrInput.reset (new juce::Slider ("noteAutoIncrInput"));
    noteAutoIncrInput->setTooltip (juce::translate(" - step notes by: "));
    noteAutoIncrInput->setRange (0, 127, 1);
    noteAutoIncrInput->setSliderStyle (juce::Slider::IncDecButtons);
    noteAutoIncrInput->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 56, 20);
    addAndMakeVisible(noteAutoIncrInput.get());

    channelAutoIncrNoteInput.reset (new juce::Slider ("channelAutoIncrNoteInput"));
    channelAutoIncrNoteInput->setTooltip (juce::translate("After reaching this note, the channel is incremented and the note is reset to 0."));
    channelAutoIncrNoteInput->setRange (0, 127, 1);
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

    lblAutoIncNotes = std::make_unique<juce::Label>("lblAutoIncNotes", "-step");
    lblAutoIncNotes->setFont(getAppFonts().getFont(LumatoneEditorFont::GothamNarrowMedium));
    // lblAutoIncNotes->getProperties().set(LumatoneEditorStyleIDs::fontHeightScalar, controlBoxFontHeightScalar);
    lblAutoIncNotes->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    lblAutoIncNotes->setJustificationType(juce::Justification::centredLeft);
    lblAutoIncNotes->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    addAndMakeVisible(lblAutoIncNotes.get());

    lblAutoIncChannels = std::make_unique<juce::Label>("lblAutoIncChannels", juce::translate("ChannelsAfterNote"));
    lblAutoIncChannels->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    lblAutoIncChannels->setJustificationType(juce::Justification::centredLeft);
    lblAutoIncChannels->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    addAndMakeVisible(lblAutoIncChannels.get());

    colourPalettePanel = std::make_unique<ColourSelectorPanel>(stateIn);
    colourPalettePanel->setBackgroundColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ColourPaletteBackground));
    // colourPalettePanel->setColourSelectionGroup(group);
    addAndMakeVisible(colourPalettePanel.get());
    // colourPalettePanel->addColourSelectorToGroup(colourSelectionGroup.get());

    // addColourSelectionBroadcaster(colourPalettePanel.get());
    LumatoneEditorState::addColourSelectionBroadcaster(colourDropdown.get());
    LumatoneEditorState::addColourSelectionListener(colourDropdown.get());
    LumatoneEditorState::addColourSelectionListener(this);
    LumatoneEditorState::addColourSelectionBroadcaster(this);
    // ColourSelectionGroup* group = new ColourSelectionGroup("KeyEditorControlColour");
    // colourSelectionGroup = colourPalettePanel->getColourSelectionGroup();
    // colourSelectionGroup->addSelector(colourDropdown.get());
    // colourSelectionGroup->addColourSelectionListener(colourDropdown.get());
    // colourSelectionGroup->addColourSelectionListener(this);

    // colourSelectionGroup->addSelector(colourTextEditor.get())
    // colourSelectionGroup->addColourSelectionListener(colourTextEditor.get());
    // group->addSelector(colourTextEditor.get());
    // group->addColourSelectionListener(colourSubwindow.get());
    // group->addColourSelectionListener(colourTextEditor.get());

    // singleSelectControls = std::make_unique<SingleSelectControls>(stateIn);
    // addChildComponent(*singleSelectControls);

    // multiSelectControls = std::make_unique<MultiSelectControls>(stateIn);
    // addChildComponent(*multiSelectControls);

    // selectionTabBar = std::make_unique<SelectionTabBar>();
    // addAndMakeVisible(*selectionTabBar);
    // selectionTabBar->addChangeListener(this);
    // setSelectionTab(SelectionTabs::Single);

    addEditorListener(this);
}

KeyEditorControls::~KeyEditorControls()
{
    LumatoneEditorState::removeColourSelectionBroadcaster(this);
    LumatoneEditorState::removeColourSelectionBroadcaster(this);
    LumatoneEditorState::removeColourSelectionListener(colourDropdown.get());
    LumatoneEditorState::removeColourSelectionBroadcaster(colourDropdown.get());

    // colourSelectionGroup = nullptr;
    colourPalettePanel = nullptr;

    lblChannel = nullptr;
    lblNote = nullptr;
    lblKeyType = nullptr;
    lblColour = nullptr;

    channelInput = nullptr;
    noteInput = nullptr;
    keyTypeCombo = nullptr;

    // colourSubwindow = nullptr;
    colourDropdown = nullptr;
    // colourTextEditor = nullptr;

    lblKeySettings = nullptr;
}

void KeyEditorControls::paint(juce::Graphics& g)
{
    g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlAreaBackground));
    g.fillPath(controlPath);

    g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlAreaHeader));
    g.fillPath(headerPath);
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

    // colourTextEditor->setBounds(lblColour->getRight(), lblColour->getY(), keyControlColumnRight - lblColour->getRight() - colourButtonWidth, keyControlHeight);
    // colourSubwindow->setBounds(colourTextEditor->getRight() + colourButtonMargin, colourTextEditor->getY(), colourButtonWidth, keyControlHeight);
    colourDropdown->setBounds(lblColour->getRight(), lblColour->getY(), keyControlColumnRight - lblColour->getRight(), keyControlHeight);

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

    // second column
    juce::Font buttonFont = getEditorLookAndFeel().getTextButtonFont(*noteAutoIncButton, keyControlHeight);
    int noteInputWidth = buttonFont.getStringWidth(noteInputWidthRef);
    int autoNoteButtonWidth = buttonFont.getStringWidth(juce::translate("Auto"));
    noteAutoIncButton->setBounds(noteInput->getRight() + contentMarginHeight, noteInput->getY(), autoNoteButtonWidth, keyControlHeight);
    resizeLabelWithHeight(lblAutoIncNotes.get(), keyControlHeight, controlLabelFontScalar, "_" /*, getWidth() - keyControlColumnRight*/);
    lblAutoIncNotes->setTopLeftPosition(noteAutoIncButton->getRight(), noteAutoIncButton->getY());
    noteAutoIncrInput->setBounds(lblAutoIncNotes->getRight(), lblAutoIncNotes->getY(), noteInputWidth, keyControlHeight);

    int autoChnlLabelWidth = getEditorLookAndFeel().getLabelFont(*lblAutoIncChannels).getStringWidth(lblAutoIncChannels->getText());
    resizeLabelWithHeight(lblAutoIncChannels.get(), keyControlHeight, controlLabelFontScalar, "_");
    lblAutoIncChannels->setTopLeftPosition(keyControlColumnRight + contentMarginWidth, channelInput->getY());
    channelAutoIncrNoteInput->setTopLeftPosition(channelInput->getRight() + contentMarginHeight, channelInput->getY());

    channelAutoIncrNoteInput->setBounds(lblAutoIncChannels->getRight(), lblAutoIncChannels->getY(), noteInputWidth, keyControlHeight);

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
        lastSelectedColour =  newData.colour;
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
        keyTypeCombo->setSelectedId((int)newData.type, juce::NotificationType::dontSendNotification);
    else
        keyTypeCombo->setSelectedId(0, juce::NotificationType::dontSendNotification);

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

void KeyEditorControls::updateColourHistory(const juce::Colour &newColour)
{
    int indexOfColour = colourHistory.indexOf(newColour);
    if (indexOfColour >= 0)
        colourHistory.move(indexOfColour, 0);
    else
        colourHistory.insert(0, newColour);

    colourDropdown->setColourOptions(colourHistory);
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
        updateColourHistory(newColour);
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
