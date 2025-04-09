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
    colourInput->setValueChangedCallback([&]()
    {
        LumatoneKeyPropertyData properties;
        properties.useColour = true;
        properties.colour = colourDropdown->getSelectedColour();

        auto matchingKeyCoords = getMappingData()->getKeysWithProperties(properties);
        performAction(SetKeySelectionAction::NewSetKeySelectionActionByCoords(*this, matchingKeyCoords));
    });
    // colourInput->setColour(LumatoneEditorControl::ColourIds::outline, juce::Colours::white);

    colourDropdown = colourInput->getColourSelector();
    // addColourSelectionListener(colourDropdown.get());
    colourDropdown->setColourPickerChangedCallback([&]()
    {
        addColourSelectionListener(colourDropdown);
    });
    colourDropdown->setShowPicker(false);

    keyTypeCombo = std::make_unique<LumatoneEditorControl>(*this, "keyTypeComboSelect", LumatoneEditorControl::Style::DropdownBox, true);
    keyTypeCombo->setLabelOptions(juce::translate("Type"), LumatoneEditorControl::LabelLocation::Top);
    keyTypeCombo->setValueChangedCallback([&]()
    {
        LumatoneKeyPropertyData properties;
        properties.useType = true;
        properties.type = LumatoneKeyType(keyTypeCombo->getValue());

        auto matchingKeyCoords = getMappingData()->getKeysWithProperties(properties);
        performAction(SetKeySelectionAction::NewSetKeySelectionActionByCoords(*this, matchingKeyCoords));
    });
    addAndMakeVisible(keyTypeCombo.get());
    // keyTypeCombo->setColour(LumatoneEditorControl::ColourIds::outline, juce::Colours::white);

    noteInput = std::make_unique<LumatoneEditorControl>(*this, "noteInputSelect", LumatoneEditorControl::Style::DropdownBox, true);
    noteInput->setLabelOptions(juce::translate("Note #") + juce::String(":"), LumatoneEditorControl::LabelLocation::Top);
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

    channelInput = std::make_unique<LumatoneEditorControl>(*this, "channelInputSelect", LumatoneEditorControl::Style::DropdownBox, true);
    channelInput->setLabelOptions(juce::translate("Channel #") + juce::String(":"), LumatoneEditorControl::LabelLocation::Top);
    channelInput->setValueChangedCallback([&]()
    {
        LumatoneKeyPropertyData properties;
        properties.useChannel = true;
        properties.channel = channelInput->getValue();

        auto matchingKeyCoords = getMappingData()->getKeysWithProperties(properties);
        performAction(SetKeySelectionAction::NewSetKeySelectionActionByCoords(*this, matchingKeyCoords));
    });
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

    noteChannelColumnX = (contentMarginWidth*2) + colourTypeColumnWidth;
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

void MultiSelectControls::updateKeyNotes(const juce::Array<int> &notes, UpdateType type)
{
    juce::Array<int> newOptions = notes;
    if (type == UpdateType::MERGE)
    {

    }

    noteInput->clearOptions();
    for (int note : newOptions)
    {
        noteInput->addOption(juce::String(note), note+1, note == newOptions.getLast());
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
        channelInput->addOption(juce::String(ch), ch+1, ch == newOptions.getLast());
    }
}
