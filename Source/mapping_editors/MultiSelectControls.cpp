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
#include "../lumatone_editor_library/palettes/colour_edit_textbox.h"

MultiSelectControls::MultiSelectControls(const LumatoneEditorState& stateIn)
        : LumatoneEditorState("MultiSelectControls", stateIn)
        , juce::Component("MultiSelectControls")
{
    lblMultiSelect = std::make_unique<juce::Label>("lblMultiSelect", "Multi-Select");
    lblMultiSelect->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LabelBlue));
    lblMultiSelect->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    addAndMakeVisible(lblMultiSelect.get());

    colourTextEditor = std::make_unique<ColourTextEditor>("colourSelectEditor", "000000");
    addAndMakeVisible(colourTextEditor.get());

    colourSubwindow = std::make_unique<ColourViewComponent>(juce::Colour(0xff5398b7));
    colourSubwindow->setColourButtonMode(ColourViewComponent::ColourButtonMode::Dropper);
    addAndMakeVisible(colourSubwindow.get());

    keyTypeCombo = std::make_unique<juce::ComboBox>("keyTypeComboSelect");
    keyTypeCombo->setEditableText (false);
    keyTypeCombo->setJustificationType (juce::Justification::centredLeft);
    keyTypeCombo->setTextWhenNothingSelected (juce::String());
    keyTypeCombo->setTextWhenNoChoicesAvailable (juce::translate("(no choices)"));
    keyTypeCombo->addItem (juce::translate("Note on/Note off"), 1);
    keyTypeCombo->addItem (juce::translate("Continuous controller"), 2);
    keyTypeCombo->addItem (juce::translate("Lumatouch"), 3);
    keyTypeCombo->addItem (juce::translate("Disabled"), 4);
    addAndMakeVisible(keyTypeCombo.get());

    noteInput = std::make_unique<juce::Slider>("noteInputSelect");
    noteInput->setTooltip (juce::translate("MIDI note or MIDI controller no. (for key type \'continuous controller\')"));
    noteInput->setRange (0, 127, 1);
    noteInput->setSliderStyle (juce::Slider::IncDecButtons);
    // noteInput->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 56, 20);
    // noteInput->addListener (this);
    addAndMakeVisible(noteInput.get());

    channelInput = std::make_unique<juce::Slider>("channelInputSelect");
    channelInput->setRange (1, 16, 1);
    channelInput->setSliderStyle (juce::Slider::IncDecButtons);
    // channelInput->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 56, 20);
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

}

MultiSelectControls::~MultiSelectControls()
{
    lblChannel = nullptr;
    lblNote = nullptr;
    lblKeyType = nullptr;
    lblColour = nullptr;

    channelInput = nullptr;
    noteInput = nullptr;
    keyTypeCombo = nullptr;

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

    lblKeyType->setTopLeftPosition(contentMarginWidth + labelMarginWidth, colourTextEditor->getBottom() + controlMarginHeight);
    resizeLabelWithHeight(lblKeyType.get(), labelHeight, controlLabelFontScalar);

    keyTypeCombo->setBounds(contentMarginWidth, lblKeyType->getBottom(), colourTypeColumnWidth, controlHeight);


    lblNote->setTopLeftPosition(noteChannelColumnX + labelMarginWidth, headerHeight + contentMarginHeight);
    resizeLabelWithHeight(lblNote.get(), labelHeight, controlLabelFontScalar);

    noteInput->setBounds(noteChannelColumnX, lblNote->getBottom(), noteChannelColumnWidth, controlHeight);
    noteInput->setTextBoxStyle(juce::Slider::TextBoxLeft, false, roundToInt(noteInput->getWidth() * 0.75f), roundToInt(noteInput->getHeight() * 0.8f));

    lblChannel->setTopLeftPosition(noteChannelColumnX + labelMarginWidth, noteInput->getBottom() + controlMarginHeight);
    resizeLabelWithHeight(lblChannel.get(), labelHeight, controlLabelFontScalar);

    channelInput->setBounds(noteChannelColumnX, lblChannel->getBottom(), noteChannelColumnWidth, controlHeight);
    channelInput->setTextBoxStyle(juce::Slider::TextBoxLeft, false, roundToInt(channelInput->getWidth() * 0.6f), roundToInt(channelInput->getHeight() * 0.8f));
}
