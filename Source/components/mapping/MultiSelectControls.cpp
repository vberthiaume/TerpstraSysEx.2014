/*
  ==============================================================================

    MultiSelectControls.h
    Created: 3 June 2024
    Author:  Vincenzo

  ==============================================================================
*/

#include "MultiSelectControls.h"
#include "../../controls/LumatoneEditorControl.h"
#include "../../controls/ColourDropdownSelector.h"
#include "../../style/LumatoneEditorLookAndFeel.h"

#include "../../lumatone_editor_library/palettes/colour_edit_textbox.h"
#include "../../actions/KeySelectionControlActions.h"


MultiSelectControls::MultiSelectControls(const LumatoneEditorState& stateIn)
        : LumatoneEditorState("MultiSelectControls", stateIn)
        , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
        , juce::Component("MultiSelectControls")
{
    lblMultiSelect = std::make_unique<juce::Label>("lblMultiSelect", "Multi-Select");
    lblMultiSelect->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LabelBlue));
    lblMultiSelect->setFont(getAppFonts().getFont(LumatoneEditorFont::UniviaProBold));
    addAndMakeVisible(lblMultiSelect.get());

    colourInput = std::make_unique<LumatoneEditorControl>("MultiSelectColourEdit", LumatoneEditorControl::Style::ColourDropdownInput, true);
    addAndMakeVisible(*colourInput);
    colourDropdown = colourInput->getColourSelector();
    // colourDropdown = std::make_unique<ColourDropdownSelector>("MultiSelectColourEdit");
    colourDropdown->setOnValueChangeCallback([&]()
    {
        LumatoneKeyPropertyData properties;
        properties.useColour = true;
        properties.colour = colourDropdown->getSelectedColour();

        auto matchingKeyCoords = getMappingData()->getKeysWithProperties(properties);
        performAction(SetKeySelectionAction::NewSetKeySelectionActionByCoords(*this, matchingKeyCoords));
    });

    // addColourSelectionListener(colourDropdown.get());
    colourDropdown->setColourPickerChangedCallback([&]()
    {
        addColourSelectionListener(colourDropdown);
    });

    // addAndMakeVisible(colourDropdown);
    colourDropdown->setShowPicker(false);

    keyTypeCombo = std::make_unique<LumatoneEditorControl>("keyTypeComboSelect", LumatoneEditorControl::Style::DropdownBox, true);
    keyTypeCombo->setValueChangedCallback([&]()
    {
        LumatoneKeyPropertyData properties;
        properties.useType = true;
        properties.type = LumatoneKeyType(keyTypeCombo->getValue());

        auto matchingKeyCoords = getMappingData()->getKeysWithProperties(properties);
        performAction(SetKeySelectionAction::NewSetKeySelectionActionByCoords(*this, matchingKeyCoords));
    });
    addAndMakeVisible(keyTypeCombo.get());

    noteInput = std::make_unique<LumatoneEditorControl>("noteInputSelect", LumatoneEditorControl::Style::DropdownBox, true);
    noteInput->setTooltip (juce::translate("MIDI note or MIDI controller no. (for key type \'continuous controller\')"));
    noteInput->setValueChangedCallback([&]()
    {
        LumatoneKeyPropertyData properties;
        properties.useNote = true;
        properties.note = noteInput->getValue();

        auto matchingKeyCoords = getMappingData()->getKeysWithProperties(properties);
        performAction(SetKeySelectionAction::NewSetKeySelectionActionByCoords(*this, matchingKeyCoords));
    });
    addAndMakeVisible(noteInput.get());

    // notesDropDown = std::make_unique<juce::ComboBox>("notesDropDown");
    // notesDropDown->setEditableText (true);
    // notesDropDown->setJustificationType (juce::Justification::centredLeft);
    // notesDropDown->setTextWhenNothingSelected (juce::String());
    // notesDropDown->setTextWhenNoChoicesAvailable (juce::translate("(none)"));
    // notesDropDown->getProperties().set(LumatoneEditorStyleIDs::popupMenuMaxColumns, (juce::var)8);
    // notesDropDown->onChange = [&]()
    // {
    //     LumatoneKeyPropertyData properties;
    //     properties.useNote = true;
    //     properties.note = notesDropDown->getText().getIntValue();

    //     auto matchingKeyCoords = getMappingData()->getKeysWithProperties(properties);
    //     performAction(SetKeySelectionAction::NewSetKeySelectionActionByCoords(*this, matchingKeyCoords));
    // };
    // addAndMakeVisible(notesDropDown.get());


    channelInput = std::make_unique<LumatoneEditorControl>("channelInputSelect", LumatoneEditorControl::Style::DropdownBox, true);
    channelInput->setValueChangedCallback([&]()
    {
        LumatoneKeyPropertyData properties;
        properties.useChannel = true;
        properties.channel = channelInput->getValue();

        auto matchingKeyCoords = getMappingData()->getKeysWithProperties(properties);
        performAction(SetKeySelectionAction::NewSetKeySelectionActionByCoords(*this, matchingKeyCoords));
    });
    addAndMakeVisible(channelInput.get());

    // channelsDropDown = std::make_unique<juce::ComboBox>("channelsDropDown");
    // channelsDropDown->setEditableText (true);
    // channelsDropDown->setJustificationType (juce::Justification::centredLeft);
    // channelsDropDown->setTextWhenNothingSelected (juce::String());
    // channelsDropDown->setTextWhenNoChoicesAvailable (juce::translate("(none)"));
    // channelsDropDown->getProperties().set(LumatoneEditorStyleIDs::popupMenuMaxColumns, (juce::var)4);
    // channelsDropDown->onChange = [&]()
    // {
    //     LumatoneKeyPropertyData properties;
    //     properties.useChannel = true;
    //     properties.channel = channelsDropDown->getText().getIntValue();

    //     auto matchingKeyCoords = getMappingData()->getKeysWithProperties(properties);
    //     performAction(SetKeySelectionAction::NewSetKeySelectionActionByCoords(*this, matchingKeyCoords));
    // };
    // addAndMakeVisible(channelsDropDown.get());


    lblColour = std::make_unique<juce::Label>("lblColour", "Colour");
    lblColour->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    lblColour->setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(lblColour.get());

    lblKeyType = std::make_unique<juce::Label>("lblKeyType", "Key Type");
    lblKeyType->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    lblKeyType->setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(lblKeyType.get());

    lblNote = std::make_unique<juce::Label>("lblNote", "Note #");
    lblNote->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    lblNote->setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(lblNote.get());

    lblChannel = std::make_unique<juce::Label>("lblChannel", "Channel #");
    lblChannel->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    lblChannel->setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(lblChannel.get());

    addEditorListener(this);
}

MultiSelectControls::~MultiSelectControls()
{
    removeEditorListener(this);

    // channelsDropDown = nullptr;
    // notesDropDown = nullptr;

    lblChannel = nullptr;
    lblNote = nullptr;
    lblKeyType = nullptr;
    lblColour = nullptr;

    channelInput = nullptr;
    noteInput = nullptr;
    keyTypeCombo = nullptr;

    colourInput = nullptr;
    colourDropdown = nullptr;
    // colourSubwindow = nullptr;
    // colourTextEditor = nullptr;
}

void MultiSelectControls::paint(juce::Graphics& g)
{
    g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlAreaBackground));
    g.fillPath(controlPath);

    g.setColour(getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlAreaHeader));
    g.fillPath(headerPath);
}

void MultiSelectControls::resized()
{
    float w = (float)getWidth();
    float h = (float)getHeight();

    auto roundedCornerSize = getRoundedRectCornerSize();
    labelMarginWidth = roundToInt(roundedCornerSize * 0.5f);

    float windowH = getWindowBounds().getHeight();
    headerHeight = roundToInt(windowH * contentLabelHeightWindowH);
    headerPath = getConnectedRoundedRectPath(getLocalBounds().withBottom(headerHeight + 1).toFloat(), roundedCornerSize, juce::Button::ConnectedEdgeFlags::ConnectedOnBottom);
    controlPath = getConnectedRoundedRectPath(getLocalBounds().withTop(headerHeight).toFloat(), roundedCornerSize, juce::Button::ConnectedEdgeFlags::ConnectedOnTop);

    contentMarginWidth = roundToInt(getParentWidth() * contentMarginWidthWindowH);
    contentMarginHeight = roundToInt(h * controlMarginH);

    labelHeight = roundToInt(headerHeight * contentLabelFontScalar);
    lblMultiSelect->setTopLeftPosition(contentMarginWidth, 0);
    resizeLabelWithHeight(lblMultiSelect.get(), headerHeight, contentLabelFontScalar);

    controlLabelHeight = roundToInt(h * controlLabelH);

    controlMarginHeight = roundToInt(h * controlMarginH);
    controlHeight = roundToInt(h * controlH);

    colourTypeColumnWidth = roundToInt(w * colourTypeColumnW);

    noteChannelColumnX = (contentMarginWidth*2) + colourTypeColumnWidth;
    noteChannelColumnWidth = getWidth() - noteChannelColumnX - contentMarginWidth;

    lblColour->setTopLeftPosition(contentMarginWidth + labelMarginWidth, headerHeight + contentMarginHeight);
    resizeLabelWithHeight(lblColour.get(), controlLabelHeight, controlLabelFontScalar);

    colourButtonMargin = lblColour->getFont().getStringWidth(" ");
    colourButtonWidth = roundToInt(getParentWidth() * colourButtonParentW) - colourButtonMargin;

    colourTextBoxWidth = colourTypeColumnWidth - colourButtonWidth - colourButtonMargin;
    // colourDropdown->setBounds(contentMarginWidth, lblColour->getBottom(), colourTextBoxWidth + colourButtonWidth, controlHeight);
    colourInput->setBounds(contentMarginWidth, lblColour->getBottom(), colourTextBoxWidth + colourButtonWidth, controlHeight);

    lblKeyType->setTopLeftPosition(contentMarginWidth + labelMarginWidth, lblColour->getBottom() + controlHeight + controlMarginHeight);
    resizeLabelWithHeight(lblKeyType.get(), controlLabelHeight, controlLabelFontScalar);

    keyTypeCombo->setBounds(contentMarginWidth, lblKeyType->getBottom(), colourTypeColumnWidth, controlHeight);
    auto keyTypeComboFont = getEditorLookAndFeel().getComboBoxFont(*keyTypeCombo->getComboBox());
    float maxKeyTypeWidth = keyTypeCombo->getWidth();
    for (int i = 0; i < keyTypeCombo->getLength(); i++)
    {
        float itemWidth = keyTypeComboFont.getStringWidth(keyTypeCombo->getOptionText(i) + "_");
        if (itemWidth > maxKeyTypeWidth)
            maxKeyTypeWidth = itemWidth;
    }
    keyTypeCombo->getProperties().set(LumatoneEditorStyleIDs::comboBoxPopupItemWidthOverride, maxKeyTypeWidth);

    lblNote->setTopLeftPosition(noteChannelColumnX + labelMarginWidth, headerHeight + contentMarginHeight);
    resizeLabelWithHeight(lblNote.get(), controlLabelHeight, controlLabelFontScalar);

    noteInput->setTextBoxStyle(juce::Slider::TextBoxLeft, false, roundToInt(noteInput->getWidth() * 0.75f), roundToInt(noteInput->getHeight() * 0.8f));
    noteInput->setBounds(noteChannelColumnX, lblNote->getBottom(), noteChannelColumnWidth, controlHeight);
    // notesDropDown->setBounds(noteChannelColumnX, lblNote->getBottom(), noteChannelColumnWidth, controlHeight);

    // lblChannel->setTopLeftPosition(noteChannelColumnX + labelMarginWidth, noteInput->getBottom() + controlMarginHeight);
    lblChannel->setTopLeftPosition(noteChannelColumnX + labelMarginWidth, noteInput->getBottom() + controlMarginHeight);
    resizeLabelWithHeight(lblChannel.get(), controlLabelHeight, controlLabelFontScalar);

    channelInput->setTextBoxStyle(juce::Slider::TextBoxLeft, false, roundToInt(channelInput->getWidth() * 0.6f), roundToInt(channelInput->getHeight() * 0.8f));
    channelInput->setBounds(noteChannelColumnX, lblChannel->getBottom(), noteChannelColumnWidth, controlHeight);
    // channelsDropDown->setBounds(noteChannelColumnX, lblChannel->getBottom(), noteChannelColumnWidth, controlHeight);
}

void MultiSelectControls::layoutChanged(const LumatoneLayout &mappingData)
{
    auto layoutColours = mappingData.getLayoutColours();
    updateColours(layoutColours, UpdateType::REPLACE);

    auto layoutTypes = mappingData.getLayoutKeyTypes();
    updateKeyTypes(layoutTypes, UpdateType::REPLACE);

    auto layoutNotes = mappingData.getLayoutKeyNotes();
    updateKeyNotes(layoutNotes, UpdateType::REPLACE);

    auto layoutChannels = mappingData.getLayoutKeyChannels();
    updateKeyChannels(layoutChannels, UpdateType::REPLACE);
}

void MultiSelectControls::boardChanged(const LumatoneBoard &boardData)
{
    // auto boardColours = boardData.getBoardColours();
    // auto colourOptions = colourDropdown->getColourOptions();
    // for (auto colour : boardColours)
    // {
    //     if (colourOptions.contains(colour))
    //         continue;

    //     colourDropdown->addItem(colour.toDisplayString(false), colourDropdown->getNumItems() + 1);
    // }
    layoutChanged(*getMappingData());
}

void MultiSelectControls::keyChanged(int boardIndex, int keyIndex, const LumatoneKey &lumatoneKey)
{
    // auto colourOptions = colourDropdown->getColourOptions();
    // if (!colourOptions.contains(lumatoneKey.getColour()))
    //     colourDropdown->addItem(lumatoneKey.getColour().toDisplayString(false), colourDropdown->getNumItems() + 1);
    layoutChanged(*getMappingData());
}

void MultiSelectControls::keySetChanged(juce::Array<MappedLumatoneKey> selection)
{
    // auto colourOptions = colourDropdown->getColourOptions();
    // for (auto key : selection)
    // {
    //     if (colourOptions.contains(key.getColour()))
    //         continue;

    //     colourDropdown->addItem(key.getColour().toDisplayString(false), colourDropdown->getNumItems() + 1);
    // }
    layoutChanged(*getMappingData());
}

void MultiSelectControls::updateColours(const juce::Array<juce::Colour> &colours, UpdateType type)
{
    juce::Array<juce::Colour> newOptions = colours;
    if (type == UpdateType::MERGE)
    {

    }

    colourDropdown->setColourOptions(newOptions);
}

void MultiSelectControls::updateKeyTypes(const juce::Array<LumatoneKeyType> &types, UpdateType type)
{
    juce::Array<LumatoneKeyType> newOptions = types;
    if (type == UpdateType::MERGE)
    {

    }

    keyTypeCombo->clearOptions();
    for (auto type : newOptions)
    {
        switch (type)
        {
            case LumatoneKeyType::noteOnNoteOff:
                keyTypeCombo->addOption (juce::translate("Note on/Note off"), 1);
                break;
            case LumatoneKeyType::continuousController:
                keyTypeCombo->addOption (juce::translate("Continuous controller"), 2);
                break;
            case LumatoneKeyType::lumaTouch:
                keyTypeCombo->addOption (juce::translate("Lumatouch"), 3);
                break;
            default:
                keyTypeCombo->addOption (juce::translate("Disabled"), 4);
                break;
        }
    }
}

void MultiSelectControls::updateKeyNotes(const juce::Array<int> &notes, UpdateType type)
{
    juce::Array<int> newOptions = notes;
    if (type == UpdateType::MERGE)
    {

    }

    noteInput->clearOptions();
    for (int note : newOptions)
    {
        noteInput->addOption(juce::String(note), note+1);
    }
}

void MultiSelectControls::updateKeyChannels(const juce::Array<int> &channels, UpdateType type)
{
    juce::Array<int> newOptions = channels;
    if (type == UpdateType::MERGE)
    {

    }

    channelInput->clearOptions();
    for (int ch : newOptions)
    {
        channelInput->addOption(juce::String(ch), ch+1);
    }
}
