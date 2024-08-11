/*
  ==============================================================================

    KeyEditorControls.h
    Created: 3 June 2024
    Author:  Vincenzo

  ==============================================================================
*/

#include "KeyEditorControls.h"
#include "../../style/LumatoneEditorLookAndFeel.h"

// #include "../../controls/colour_view_component.h"
#include "../../controls/ColourDropdownSelector.h"
#include "../../lumatone_editor_library/palettes/colour_edit_textbox.h"

#include "../ColourPaletteWindow.h"
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
    lblKeySettings = std::make_unique<juce::Label>("lblKeySettings", "Key Settings");
    lblKeySettings->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LabelBlue));
    lblKeySettings->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    addAndMakeVisible(lblKeySettings.get());

    // colourTextEditor = std::make_unique<ColourTextEditor>("colourTextEditor", "");
    // addAndMakeVisible(colourTextEditor.get());

    // colourSubwindow = std::make_unique<ColourViewComponent>(juce::Colour(0xff5398b7));
    // colourSubwindow->setColourButtonMode(ColourViewComponent::ColourButtonMode::Dropper);
    // addAndMakeVisible(colourSubwindow.get());

    colourDropdown = std::make_unique<ColourDropdownSelector>("AssignColourDropdown");
    colourDropdown->setOnValueChangeCallback([&]()
    {
        performAction(SetKeySettingsAction::NewSetAssignColourAction(*this, colourDropdown->getSelectedColour()));
        if (colourDropdown->getSelectedColour() != juce::Colours::transparentBlack)
            performAction(new ApplyAssignmentsToSelectionAction(*this, getEditSelectionData(), *getSelectedKeys()));
    });

    // addColourSelectionListener(colourDropdown.get());
    // colourDropdown->setColourPickerChangedCallback([&]()
    // {
    //     addColourSelectionListener(colourDropdown.get());
    // });

    addAndMakeVisible(colourDropdown.get());
    colourDropdown->setShowPicker(true);

    keyTypeCombo = std::make_unique<juce::ComboBox>("keyTypeCombo");
    keyTypeCombo->setEditableText (false);
    keyTypeCombo->setJustificationType (juce::Justification::centredLeft);
    keyTypeCombo->setTextWhenNothingSelected (juce::String());
    keyTypeCombo->setTextWhenNoChoicesAvailable (juce::translate("(no choices)"));
    keyTypeCombo->addItem (juce::translate("Note on/Note off"), (int)LumatoneKeyType::noteOnNoteOff);
    keyTypeCombo->addItem (juce::translate("Continuous controller"), (int)LumatoneKeyType::continuousController);
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

    lblColour = std::make_unique<juce::Label>("lblColour", "Key Colour:");
    lblColour->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    lblColour->setJustificationType(juce::Justification::centredLeft);
    lblColour->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    addAndMakeVisible(lblColour.get());

    lblKeyType = std::make_unique<juce::Label>("lblKeyType", "Key Type: ");
    lblKeyType->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    lblKeyType->setJustificationType(juce::Justification::centredLeft);
    lblKeyType->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    addAndMakeVisible(lblKeyType.get());

    lblNote = std::make_unique<juce::Label>("lblNote", "MIDI Note: ");
    lblNote->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    lblNote->setJustificationType(juce::Justification::centredLeft);
    lblNote->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    addAndMakeVisible(lblNote.get());

    lblChannel = std::make_unique<juce::Label>("lblChannel", "MIDI Channel: ");
    lblChannel->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    lblChannel->setJustificationType(juce::Justification::centredLeft);
    lblChannel->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::DescriptionText));
    addAndMakeVisible(lblChannel.get());

    colourPalettePanel = std::make_unique<ColourPaletteWindow>(stateIn);
    colourPalettePanel->setBackgroundColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ColourPaletteBackground));
    // colourPalettePanel->setColourSelectionGroup(group);
    addAndMakeVisible(colourPalettePanel.get());
    // colourPalettePanel->addColourSelectorToGroup(colourSelectionGroup.get());

    // ColourSelectionGroup* group = new ColourSelectionGroup("KeyEditorControlColour");
    colourSelectionGroup = colourPalettePanel->getColourSelectionGroup();
    colourSelectionGroup->addSelector(colourDropdown.get());
    colourSelectionGroup->addColourSelectionListener(colourDropdown.get());
    colourSelectionGroup->addColourSelectionListener(this);
    // colourSelectionGroup = std::make_unique<ColourSelectionGroup>("KeyEditorControlColour");
    // colourSelectionGroup->addSelector(colourTextEditor.get())
    // colourSelectionGroup->addColourSelectionListener(colourTextEditor.get());
    // group->addSelector(colourTextEditor.get());
    // group->addColourSelectionListener(colourSubwindow.get());
    // group->addColourSelectionListener(colourTextEditor.get());

    addEditorListener(this);
}

KeyEditorControls::~KeyEditorControls()
{
    colourSelectionGroup = nullptr;
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

    float windowH = getWindowBounds().getHeight();

    auto roundedCornerSize = getRoundedRectCornerSize();

    headerHeight = roundToInt(windowH * contentLabelHeightWindowH);
    headerPath = getConnectedRoundedRectPath(getLocalBounds().withBottom(headerHeight + 1).toFloat(), roundedCornerSize, juce::Button::ConnectedEdgeFlags::ConnectedOnBottom);
    controlPath = getConnectedRoundedRectPath(getLocalBounds().withTop(headerHeight).toFloat(), roundedCornerSize, juce::Button::ConnectedEdgeFlags::ConnectedOnTop);

    contentMarginWidth = roundToInt(windowH * contentMarginWidthWindowH);
    contentMarginHeight = roundToInt(h * controlMarginH);

    labelHeight = roundToInt(headerHeight * contentLabelFontScalar);
    lblKeySettings->setTopLeftPosition(contentMarginWidth, 0);
    resizeLabelWithHeight(lblKeySettings.get(), headerHeight, contentLabelFontScalar);

    keyControlColumnWidth = roundToInt(w * keyControlColumnW);
    keyControlColumnRight = contentMarginWidth + keyControlColumnWidth;

    keyControlHeight = roundToInt(h * keyControlH);
    keyControlMarginHeight = roundToInt(h * keyControlMarginH);

    lblColour->setTopLeftPosition(contentMarginWidth, headerHeight + contentMarginHeight);
    resizeLabelWithHeight(lblColour.get(), keyControlHeight, contentLabelFontScalar);

    colourButtonMargin = lblColour->getFont().getStringWidth(" ");
    colourButtonWidth = roundToInt(getParentWidth() * colourButtonParentW) - colourButtonMargin;

    // colourTextEditor->setBounds(lblColour->getRight(), lblColour->getY(), keyControlColumnRight - lblColour->getRight() - colourButtonWidth, keyControlHeight);
    // colourSubwindow->setBounds(colourTextEditor->getRight() + colourButtonMargin, colourTextEditor->getY(), colourButtonWidth, keyControlHeight);
    colourDropdown->setBounds(lblColour->getRight(), lblColour->getY(), keyControlColumnRight - lblColour->getRight(), keyControlHeight);

    lblKeyType->setTopLeftPosition(contentMarginWidth, lblColour->getBottom() + keyControlMarginHeight);
    resizeLabelWithHeight(lblKeyType.get(), keyControlHeight, controlLabelFontScalar);
    keyTypeCombo->setBounds(lblKeyType->getRight(), lblKeyType->getY(), keyControlColumnRight - lblKeyType->getRight(), keyControlHeight);

    lblNote->setTopLeftPosition(contentMarginWidth, keyTypeCombo->getBottom() + keyControlMarginHeight);
    resizeLabelWithHeight(lblNote.get(), keyControlHeight, controlLabelFontScalar);
    noteInput->setBounds(lblNote->getRight(), lblNote->getY(), keyControlColumnRight - lblNote->getRight(), keyControlHeight);

    lblChannel->setTopLeftPosition(contentMarginWidth, noteInput->getBottom() + keyControlMarginHeight);
    resizeLabelWithHeight(lblChannel.get(), keyControlHeight, controlLabelFontScalar);
    channelInput->setBounds(lblChannel->getRight(), lblChannel->getY(), keyControlColumnRight - lblChannel->getRight(), keyControlHeight);

    colourColumnX = contentMarginWidth + keyControlColumnWidth + roundToInt(w * columnMarginW);
    colourColumnWidth = w - colourColumnX - contentMarginWidth;
    colourColumnHeight = roundToInt(h * colourColumnH);

    colourPalettePanel->setBounds(colourColumnX, 0, colourColumnWidth, h);
    colourPalettePanel->setIndentSize(contentMarginHeight, false);
    colourPalettePanel->setTabBarDepth(headerHeight, true);
}

void KeyEditorControls::selectionChanged()
{
    // Make more efficient? (each time goes through loop)
    auto newData = LumatoneEditSelectionState::findSharedSelectionProperties(*getSelectedKeys());

    if (newData.useColour)
    {
        setAssignKeyColour(newData.useColour, newData.colour);
        // performAction(SetKeySettingsAction::NewSetAssignColourAction(*this, newData.colour), true, false);
        // colourSubwindow->setColour(newData.colour.toString(), false);
        colourDropdown->setSelectedColour(newData.colour);

        // if (newData.colour.isTransparent())
        //     colourTextEditor->setText("", juce::NotificationType::dontSendNotification);
        // else
        //     colourTextEditor->setText(newData.colour.toDisplayString(false), juce::NotificationType::dontSendNotification);
    }
    else
    {
        // colourSubwindow->setColour("", false);
        colourDropdown->deselectColour();
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

void KeyEditorControls::handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier &property)
{
    LumatoneEditorState::handleStatePropertyChange(stateIn, property);

    juce::var value = stateIn.getProperty(property);

    if (property == LumatoneEditSelectionProperty::AssignKeyColour)
    {
        auto colourString = stateIn[property].toString();
        // colourTextEditor->setText(colourString, juce::NotificationType::dontSendNotification);
        // colourSubwindow->setColour(colourString, false);
        colourDropdown->setSelectedColour(juce::Colour::fromString(colourString));
    }
    else if (property == LumatoneEditSelectionProperty::AssignKeyType)
    {
        keyTypeCombo->setSelectedId(((int)value) + 1, juce::NotificationType::dontSendNotification);
    }
    else if (property == LumatoneEditSelectionProperty::AssignKeyNote)
    {
        noteInput->setValue((int)value, juce::NotificationType::dontSendNotification);
    }
    else if (property == LumatoneEditSelectionProperty::AssignKeyChannel)
    {
        channelInput->setValue((int)value, juce::NotificationType::dontSendNotification);
    }
    else if (property == LumatoneEditSelectionProperty::AssignKeyCCFader)
    {

    }
}

void KeyEditorControls::colourChangedCallback(ColourSelectionBroadcaster *source, juce::Colour newColour)
{
    performAction(SetKeySettingsAction::NewSetAssignColourAction(*this, newColour));
    performAction(new ApplyAssignmentsToSelectionAction(*this, getEditSelectionData(), *getSelectedKeys()), true, false);
}
