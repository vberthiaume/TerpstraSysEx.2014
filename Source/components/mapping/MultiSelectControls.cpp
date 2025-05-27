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

    colourInput = std::make_unique<LumatoneEditorControl>(*this, "MultiSelectColourEdit", LumatoneEditorControl::Style::ColourDropdownInput, true);
    colourInput->setLabelOptions(juce::translate("Colour") + juce::String(":"), LumatoneEditorControl::LabelLocation::Top);
    addAndMakeVisible(*colourInput);
    colourInput->setValueChangedCallback([&](){ colourInputCallback(colourInput->getColourValue(), UpdateType::REPLACE); });
    // colourInput->setColour(LumatoneEditorControl::ColourIds::outline, juce::Colours::white);

    colourDropdown = colourInput->getColourSelector();
    // addColourSelectionListener(colourDropdown.get());
    colourDropdown->setColourPickerChangedCallback([&]() { addColourSelectionListener(colourDropdown); });
    colourDropdown->setShowPicker(false);

    keyTypeCombo = std::make_unique<LumatoneEditorControl>(*this, "keyTypeComboSelect", LumatoneEditorControl::Style::DropdownBox, true);
    keyTypeCombo->setLabelOptions(juce::translate("Type"), LumatoneEditorControl::LabelLocation::Top);
    keyTypeCombo->setValueChangedCallback([&]() { typeInputCallback(keyTypeCombo->getValue(), UpdateType::REPLACE); });
    addAndMakeVisible(keyTypeCombo.get());
    // keyTypeCombo->setColour(LumatoneEditorControl::ColourIds::outline, juce::Colours::white);

    noteInput = std::make_unique<LumatoneEditorControl>(*this, "noteInputSelect", LumatoneEditorControl::Style::DropdownBox, true);
    noteInput->setLabelOptions(juce::translate("Note #") + juce::String(":"), LumatoneEditorControl::LabelLocation::Top);
    noteInput->setTooltip (juce::translate("MIDI note or MIDI controller no. (for key type \'continuous controller\')"));
    noteInput->setValueChangedCallback([&]() { noteInputCallback(noteInput->getValue(), UpdateType::REPLACE); });
    addAndMakeVisible(noteInput.get());

    channelInput = std::make_unique<LumatoneEditorControl>(*this, "channelInputSelect", LumatoneEditorControl::Style::DropdownBox, true);
    channelInput->setLabelOptions(juce::translate("Channel #") + juce::String(":"), LumatoneEditorControl::LabelLocation::Top);
    channelInput->setValueChangedCallback([&]() { channelInputCallback(channelInput->getValue(), UpdateType::REPLACE); });
    addAndMakeVisible(channelInput.get());

    addEditorListener(this);
}

MultiSelectControls::~MultiSelectControls()
{
    removeEditorListener(this);

    channelInput = nullptr;
    noteInput = nullptr;
    keyTypeCombo = nullptr;

    colourInput = nullptr;
    colourDropdown = nullptr;
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

    int controlAndLabelHeight = controlLabelHeight + controlHeight;

    colourTypeColumnWidth = roundToInt(w * colourTypeColumnW);

    noteChannelColumnX = juce::roundToInt(contentMarginWidth * 1.3f) + colourTypeColumnWidth;
    noteChannelColumnWidth = getWidth() - noteChannelColumnX - contentMarginWidth;

    // lblColour->setTopLeftPosition(contentMarginWidth + labelMarginWidth, headerHeight + contentMarginHeight);
    // resizeLabelWithHeight(lblColour.get(), controlLabelHeight, controlLabelFontScalar);

    colourButtonMargin = getEditorLookAndFeel().getLabelFont(controlLabelHeight * controlLabelFontScalar).getStringWidth(" ");
    colourButtonWidth = roundToInt(getParentWidth() * colourButtonParentW) - colourButtonMargin;

    colourTextBoxWidth = colourTypeColumnWidth - colourButtonWidth - colourButtonMargin;
    // colourDropdown->setBounds(contentMarginWidth, lblColour->getBottom(), colourTextBoxWidth + colourButtonWidth, controlHeight);
    colourInput->setBounds(contentMarginWidth, headerHeight + contentMarginHeight, colourTextBoxWidth + colourButtonWidth, controlAndLabelHeight);

    // lblKeyType->setTopLeftPosition(contentMarginWidth + labelMarginWidth, lblColour->getBottom() + controlHeight + controlMarginHeight);
    // resizeLabelWithHeight(lblKeyType.get(), controlLabelHeight, controlLabelFontScalar);

    keyTypeCombo->setBounds(contentMarginWidth, colourInput->getBottom() + controlMarginHeight, colourTypeColumnWidth, controlAndLabelHeight);
    auto keyTypeComboFont = getEditorLookAndFeel().getComboBoxFont(*keyTypeCombo->getComboBox());
    float maxKeyTypeWidth = keyTypeCombo->getWidth();
    for (int i = 0; i < keyTypeCombo->getLength(); i++)
    {
        float itemWidth = keyTypeComboFont.getStringWidth(keyTypeCombo->getOptionText(i) + "_");
        if (itemWidth > maxKeyTypeWidth)
            maxKeyTypeWidth = itemWidth;
    }
    keyTypeCombo->getProperties().set(LumatoneEditorStyleIDs::comboBoxPopupItemWidthOverride, maxKeyTypeWidth);

    // lblNote->setTopLeftPosition(noteChannelColumnX + labelMarginWidth, headerHeight + contentMarginHeight);
    // resizeLabelWithHeight(lblNote.get(), controlLabelHeight, controlLabelFontScalar);

    noteInput->setTextBoxStyle(juce::Slider::TextBoxLeft, false, roundToInt(noteInput->getWidth() * 0.75f), roundToInt(noteInput->getHeight() * 0.8f));
    noteInput->setBounds(noteChannelColumnX, headerHeight + contentMarginHeight, noteChannelColumnWidth, controlAndLabelHeight);
    // notesDropDown->setBounds(noteChannelColumnX, lblNote->getBottom(), noteChannelColumnWidth, controlHeight);

    // lblChannel->setTopLeftPosition(noteChannelColumnX + labelMarginWidth, noteInput->getBottom() + controlMarginHeight);
    // lblChannel->setTopLeftPosition(noteChannelColumnX + labelMarginWidth, noteInput->getBottom() + controlMarginHeight);
    // resizeLabelWithHeight(lblChannel.get(), controlLabelHeight, controlLabelFontScalar);

    channelInput->setTextBoxStyle(juce::Slider::TextBoxLeft, false, roundToInt(channelInput->getWidth() * 0.6f), roundToInt(channelInput->getHeight() * 0.8f));
    channelInput->setBounds(noteChannelColumnX, noteInput->getBottom() + controlMarginHeight, noteChannelColumnWidth, controlAndLabelHeight);
    // channelsDropDown->setBounds(noteChannelColumnX, lblChannel->getBottom(), noteChannelColumnWidth, controlHeight);
}

void MultiSelectControls::setSelection(const MultiSelection &selectionIn, bool sendUpdate)
{
    coloursSelected.clear();
    typesSelected.clear();
    notesSelected.clear();
    channelsSelected.clear();

    coloursSelected.addArray(selectionIn.coloursSelected);
    typesSelected.addArray(selectionIn.typesSelected);
    notesSelected.addArray(selectionIn.notesSelected);
    channelsSelected.addArray(selectionIn.channelsSelected);

    if (coloursSelected.size() > 0)
        colourInput->setColourValue(coloursSelected[0], juce::NotificationType::dontSendNotification);
    else
        colourInput->clearValue(juce::NotificationType::dontSendNotification);

    if (typesSelected.size() > 0)
    {
        int id = (int)typesSelected[0] - 1;
        keyTypeCombo->setValue(id, juce::NotificationType::dontSendNotification);
    }
    else
        keyTypeCombo->clearValue(juce::NotificationType::dontSendNotification);

    if (notesSelected.size() > 0)
        noteInput->setValue(notesSelected[0], juce::NotificationType::dontSendNotification);
    else
        noteInput->clearValue(juce::NotificationType::dontSendNotification);

    if (channelsSelected.size() > 0)
    {
        int id = channelsSelected[0] - 1;
        channelInput->setValue(id, juce::NotificationType::dontSendNotification);
    }
    else
        channelInput->clearValue(juce::NotificationType::dontSendNotification);
}

void MultiSelectControls::setSelection(juce::Colour colour, bool sendUpdate)
{
    MultiSelection selection;
    selection.coloursSelected.add(colour);
    setSelection(selection, sendUpdate);

    if (sendUpdate)
    {
        LumatoneKeyPropertyData properties;
        properties.useColour = true;
        properties.colour = colour;
        auto matchingKeyCoords = getMappingData()->getKeysWithProperties(properties);
        performAction(SetKeySelectionAction::NewSetKeySelectionActionByCoords(*this, matchingKeyCoords));
    }
}

void MultiSelectControls::setSelection(LumatoneKeyType type, bool sendUpdate)
{
    MultiSelection selection;

    if (type > LumatoneKeyType::disabledDefault && type < LumatoneKeyType::disabled)
        selection.typesSelected.add(type);

    setSelection(selection, sendUpdate);

    if (sendUpdate)
    {
        LumatoneKeyPropertyData properties;
        properties.useType = true;
        properties.type = type;
        auto matchingKeyCoords = getMappingData()->getKeysWithProperties(properties);
        performAction(SetKeySelectionAction::NewSetKeySelectionActionByCoords(*this, matchingKeyCoords));
    }
}

void MultiSelectControls::setSelection(int note, int channel, bool sendUpdate)
{
    MultiSelection selection;
    bool skippedNote = false;
    if (note >= 0 && note < 128)
    {
        selection.notesSelected.add(note);
    }
    else
    {
        skippedNote = true;
    }

    bool skippedChannel = false;
    if (channel > 0 && channel <= 16)
    {
        selection.channelsSelected.add(channel);
    }
    else
    {
        skippedChannel = true;
    }

    setSelection(selection, sendUpdate);

    if (sendUpdate)
    {
        juce::Array<LumatoneKeyCoord> newSelection;

        if (!skippedNote || !skippedChannel)
        {
            LumatoneKeyPropertyData properties;
            properties.useNote = !skippedNote;
            properties.useChannel = !skippedChannel;
            properties.note = note;
            properties.channel = channel;

            newSelection = getMappingData()->getKeysWithProperties(properties);
        }

        performAction(SetKeySelectionAction::NewSetKeySelectionActionByCoords(*this, newSelection));
    }
}

void MultiSelectControls::addToSelection(juce::Colour colour, bool sendUpdate)
{
    coloursSelected.add(colour);

    if (sendUpdate)
    {

    }
}

void MultiSelectControls::addToSelection(LumatoneKeyType colour, bool sendUpdate)
{

}

void MultiSelectControls::addToSelection(int note, int channel, bool sendUpdate)
{
}

void MultiSelectControls::removeFromSelection(juce::Colour colour, bool sendUpdate)
{
}

void MultiSelectControls::removeFromSelection(LumatoneKeyType colour, bool sendUpdate)
{
}

void MultiSelectControls::removeFromSelection(int note, int channel, bool sendUpdate)
{
}

void MultiSelectControls::clearSelection(bool sendUpdate)
{
    MultiSelection empty;
    setSelection(empty);

    if (sendUpdate)
    {
        juce::Array<MappedLumatoneKey> noKeys;
        performAction(new SetKeySelectionAction(*this, noKeys));
    }
}

void MultiSelectControls::colourInputCallback(juce::Colour colour, UpdateType update)
{
    switch (update)
    {
    default:
        setSelection(colour, true);
        break;
    }
}

void MultiSelectControls::typeInputCallback(int type, UpdateType update)
{
    switch (update)
    {
    default:
        setSelection(LumatoneKeyType(type + 1), true);
        break;
    }
}

void MultiSelectControls::noteInputCallback(int note, UpdateType update)
{
    switch (update)
    {
    default:
        setSelection(note, -1, true);
        break;
    }
}

void MultiSelectControls::channelInputCallback(int channel, UpdateType update)
{
    switch (update)
    {
    default:
        setSelection(-1, channel, true);
        break;
    }
}

void MultiSelectControls::layoutChanged(const LumatoneLayout &mappingData)
{
    auto layoutColours = mappingData.getLayoutColours();
    updateColoursOptions(layoutColours);

    auto layoutTypes = mappingData.getLayoutKeyTypes();
    updateKeyTypesOptions(layoutTypes);

    auto layoutNotes = mappingData.getLayoutKeyNotes();
    updateKeyNotesOptions(layoutNotes);

    auto layoutChannels = mappingData.getLayoutKeyChannels();
    updateKeyChannelsOptions(layoutChannels);

    clearSelection(false);
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

void MultiSelectControls::selectionChanged()
{
    // bool matchingColour     = true;
    // bool matchingType       = true;
    // bool matchingNote       = true;
    // bool matchingChannel    = true;

    // const juce::Array<MappedLumatoneKey>* selection = getSelectedKeys();
    // for (const MappedLumatoneKey& key : *selection)
    // {
    //     if (matchingColour)
    //     {
    //         matchingColour = coloursSelected.contains(key.getColour());
    //     }
    //     if (matchingType)
    //     {
    //         matchingType = typesSelected.contains(key.getType());
    //     }
    //     if (matchingNote)
    //     {
    //         matchingNote = notesSelected.contains(key.getMidiNumber());
    //     }
    //     if (matchingChannel)
    //     {
    //         matchingChannel = channelsSelected.contains(key.getMidiChannel());
    //     }
    // }

    // MultiSelection newSelection;
    // if (matchingColour)
    //     newSelection.coloursSelected.addArray(coloursSelected);
    // if (matchingType)
    //     newSelection.typesSelected.addArray(typesSelected);
    // if (matchingNote)
    //     newSelection.notesSelected.addArray(notesSelected);
    // if (matchingChannel)
    //     newSelection.channelsSelected.addArray(channelsSelected);

    // setSelection(newSelection, false);
}

void MultiSelectControls::updateColoursOptions(const juce::Array<juce::Colour> &colours)
{
    juce::Array<juce::Colour> newOptions = colours;
    colourDropdown->setColourOptions(newOptions);
}

void MultiSelectControls::updateKeyTypesOptions(const juce::Array<LumatoneKeyType> &types)
{
    juce::Array<LumatoneKeyType> newOptions;
    for (LumatoneKeyType type : types)
    {
        if (type == LumatoneKeyType::disabledDefault)
            type = LumatoneKeyType::disabled;
        newOptions.addIfNotAlreadyThere(type);
    }

    keyTypeCombo->clearOptions();
    for (auto type : newOptions)
    {
        switch (type)
        {
            case LumatoneKeyType::noteOnNoteOff:
                keyTypeCombo->addOption (juce::translate("Note on/Note off"), 1, false);
                break;
            case LumatoneKeyType::continuousController:
                keyTypeCombo->addOption (juce::translate("Continuous controller"), 2, false);
                break;
            case LumatoneKeyType::lumaTouch:
                keyTypeCombo->addOption (juce::translate("Lumatouch"), 3, false);
                break;
            default:
                keyTypeCombo->addOption (juce::translate("Disabled"), 4, false);
                break;
        }
    }
}

void MultiSelectControls::updateKeyNotesOptions(const juce::Array<int> &notes)
{
    juce::Array<int> newOptions = notes;

    noteInput->clearOptions();
    for (int note : newOptions)
    {
        noteInput->addOption(juce::String(note), note+1, note == newOptions.getLast());
    }
}

void MultiSelectControls::updateKeyChannelsOptions(const juce::Array<int> &channels)
{
    juce::Array<int> newOptions = channels;

    channelInput->clearOptions();
    for (int ch : newOptions)
    {
        channelInput->addOption(juce::String(ch), ch+1, ch == newOptions.getLast());
    }
}
