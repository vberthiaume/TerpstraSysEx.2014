/*
  ==============================================================================

    MultiSelectControls.h
    Created: 3 June 2024
    Author:  Vincenzo

  ==============================================================================
*/

#include "MultiSelectControls.h"
#include "../LumatoneEditorLookAndFeel.h"

#include "../colour_view_component.h"
#include "../actions/KeySelectionControlActions.h"
#include "../components/RangedControl.h"
#include "../lumatone_editor_library/palettes/colour_edit_textbox.h"

#include "../components/ColourDropdownSelector.h"

MultiSelectControls::MultiSelectControls(const LumatoneEditorState& stateIn)
        : LumatoneEditorState("MultiSelectControls", stateIn)
        , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
        , juce::Component("MultiSelectControls")
{
    lblMultiSelect = std::make_unique<juce::Label>("lblMultiSelect", "Multi-Select");
    lblMultiSelect->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LabelBlue));
    lblMultiSelect->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    addAndMakeVisible(lblMultiSelect.get());

    colourTextEditor = std::make_unique<ColourTextEditor>("colourSelectEditor", "000000");
    // addAndMakeVisible(colourTextEditor.get());

    colourSubwindow = std::make_unique<ColourViewComponent>(juce::Colour(0xff5398b7));
    colourSubwindow->setColourButtonMode(ColourViewComponent::ColourButtonMode::Dropper);
    // addAndMakeVisible(colourSubwindow.get());

    colourDropdown = std::make_unique<ColourDropdownSelector>("MultiSelectColourEdit");
    colourDropdown->setOnValueChangeCallback([&]()
    {
        LumatoneKeyPropertyData properties;
        properties.useColour = true;
        properties.colour = colourDropdown->getSelectedColour();

        auto matchingKeyCoords = getMappingData()->getKeysWithProperties(properties);
        performAction(SetKeySelectionAction::NewSetKeySelectionActionByCoords(*this, matchingKeyCoords));
    });
    addAndMakeVisible(colourDropdown.get());
    colourDropdown->setShowPicker(true);

    keyTypeCombo = std::make_unique<juce::ComboBox>("keyTypeComboSelect");
    keyTypeCombo->setEditableText (false);
    keyTypeCombo->setJustificationType (juce::Justification::centredLeft);
    keyTypeCombo->setTextWhenNothingSelected (juce::String());
    keyTypeCombo->setTextWhenNoChoicesAvailable (juce::translate("(no choices)"));
    keyTypeCombo->addItem (juce::translate("Note on/Note off"), 1);
    keyTypeCombo->addItem (juce::translate("Continuous controller"), 2);
    keyTypeCombo->addItem (juce::translate("Lumatouch"), 3);
    keyTypeCombo->addItem (juce::translate("Disabled"), 4);
    keyTypeCombo->onChange = [&]()
    {
        LumatoneKeyPropertyData properties;
        properties.useType = true;
        properties.type = LumatoneKeyType(keyTypeCombo->getSelectedId());

        auto matchingKeyCoords = getMappingData()->getKeysWithProperties(properties);
        performAction(SetKeySelectionAction::NewSetKeySelectionActionByCoords(*this, matchingKeyCoords));
    };
    addAndMakeVisible(keyTypeCombo.get());

    noteInput = std::make_unique<RangedControl>("noteInputSelect", 0, 127, RangedControl::Style::DropdownBox);
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

    channelInput = std::make_unique<RangedControl>("channelInputSelect", 1, 16, RangedControl::Style::DropdownBox);
    channelInput->setValueChangedCallback([&]()
    {
        LumatoneKeyPropertyData properties;
        properties.useChannel = true;
        properties.channel = channelInput->getValue();

        auto matchingKeyCoords = getMappingData()->getKeysWithProperties(properties);
        performAction(SetKeySelectionAction::NewSetKeySelectionActionByCoords(*this, matchingKeyCoords));
    });
    addAndMakeVisible(channelInput.get());

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

    lblChannel = nullptr;
    lblNote = nullptr;
    lblKeyType = nullptr;
    lblColour = nullptr;

    channelInput = nullptr;
    noteInput = nullptr;
    keyTypeCombo = nullptr;

    colourDropdown = nullptr;
    colourSubwindow = nullptr;
    colourTextEditor = nullptr;
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

    headerHeight = roundToInt(h * headerH);
    headerPath = getConnectedRoundedRectPath(getLocalBounds().withBottom(headerHeight + 1).toFloat(), roundedCornerSize, juce::Button::ConnectedEdgeFlags::ConnectedOnBottom);
    controlPath = getConnectedRoundedRectPath(getLocalBounds().withTop(headerHeight).toFloat(), roundedCornerSize, juce::Button::ConnectedEdgeFlags::ConnectedOnTop);

    contentMarginWidth = roundToInt(getParentWidth() * contentMarginParentW);
    contentMarginHeight = roundToInt(h * controlMarginH);

    labelHeight = roundToInt(headerHeight * labelToHeaderH);
    lblMultiSelect->setTopLeftPosition(contentMarginWidth, 0);
    resizeLabelWithHeight(lblMultiSelect.get(), headerHeight, labelToHeaderH);

    controlLabelHeight = roundToInt(h * controlLabelH);

    controlMarginHeight = roundToInt(h * controlMarginH);
    controlHeight = roundToInt(h * controlH);

    colourTypeColumnWidth = roundToInt(w * colourTypeColumnW);

    noteChannelColumnWidth = roundToInt(w * noteChannelColumnW);
    noteChannelColumnX = (int)w - noteChannelColumnWidth - roundToInt(contentMarginWidth * 0.5f);

    lblColour->setTopLeftPosition(contentMarginWidth + labelMarginWidth, headerHeight + contentMarginHeight);
    resizeLabelWithHeight(lblColour.get(), labelHeight, controlLabelFontScalar);

    colourButtonMargin = lblColour->getFont().getStringWidth(" ");
    colourButtonWidth = roundToInt(getParentWidth() * colourButtonParentW) - colourButtonMargin;

    colourTextBoxWidth = colourTypeColumnWidth - colourButtonWidth - colourButtonMargin;
    colourTextEditor->setBounds(contentMarginWidth, lblColour->getBottom(), colourTextBoxWidth, controlHeight);
    colourSubwindow->setBounds(colourTextEditor->getRight() + colourButtonMargin, colourTextEditor->getY(), colourButtonWidth, controlHeight);

    colourDropdown->setBounds(contentMarginWidth, lblColour->getBottom(), colourTextBoxWidth + colourButtonWidth, controlHeight);

    lblKeyType->setTopLeftPosition(contentMarginWidth + labelMarginWidth, colourTextEditor->getBottom() + controlMarginHeight);
    resizeLabelWithHeight(lblKeyType.get(), labelHeight, controlLabelFontScalar);

    keyTypeCombo->setBounds(contentMarginWidth, lblKeyType->getBottom(), colourTypeColumnWidth, controlHeight);
    auto keyTypeComboFont = getEditorLookAndFeel().getComboBoxFont(*keyTypeCombo);
    float maxKeyTypeWidth = keyTypeCombo->getWidth();
    for (int i = 0; i < keyTypeCombo->getNumItems(); i++)
    {
        float itemWidth = keyTypeComboFont.getStringWidth(keyTypeCombo->getItemText(i) + "_");
        if (itemWidth > maxKeyTypeWidth)
            maxKeyTypeWidth = itemWidth;
    }
    keyTypeCombo->getProperties().set(LumatoneEditorStyleIDs::comboBoxPopupItemWidthOverride, maxKeyTypeWidth);

    lblNote->setTopLeftPosition(noteChannelColumnX + labelMarginWidth, headerHeight + contentMarginHeight);
    resizeLabelWithHeight(lblNote.get(), labelHeight, controlLabelFontScalar);

    noteInput->setTextBoxStyle(juce::Slider::TextBoxLeft, false, roundToInt(noteInput->getWidth() * 0.75f), roundToInt(noteInput->getHeight() * 0.8f));
    noteInput->setBounds(noteChannelColumnX, lblNote->getBottom(), noteChannelColumnWidth, controlHeight);

    lblChannel->setTopLeftPosition(noteChannelColumnX + labelMarginWidth, noteInput->getBottom() + controlMarginHeight);
    resizeLabelWithHeight(lblChannel.get(), labelHeight, controlLabelFontScalar);

    channelInput->setTextBoxStyle(juce::Slider::TextBoxLeft, false, roundToInt(channelInput->getWidth() * 0.6f), roundToInt(channelInput->getHeight() * 0.8f));
    channelInput->setBounds(noteChannelColumnX, lblChannel->getBottom(), noteChannelColumnWidth, controlHeight);
}

void MultiSelectControls::layoutChanged(const LumatoneLayout &mappingData)
{
    auto layoutColours = mappingData.getLayoutColours();
    colourDropdown->setColourOptions(layoutColours);
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
