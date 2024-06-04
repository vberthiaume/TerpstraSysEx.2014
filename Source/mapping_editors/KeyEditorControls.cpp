/*
  ==============================================================================

    KeyEditorControls.h
    Created: 3 June 2024
    Author:  Vincenzo

  ==============================================================================
*/

#include "KeyEditorControls.h"
#include "../LumatoneEditorLookAndFeel.h"

#include "../colour_view_component.h"
#include "../lumatone_editor_library/palettes/colour_edit_textbox.h"

KeyEditorControls::KeyEditorControls(const LumatoneEditorState& stateIn)
        : LumatoneEditorState(stateIn)
        , juce::Component("KeyEditorControls")
{
    lblKeySettings = std::make_unique<juce::Label>("lblKeySettings", "Key Settings");
    lblKeySettings->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LabelBlue));
    lblKeySettings->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    addAndMakeVisible(lblKeySettings.get());

    colourControlTabs = std::make_unique<juce::TabbedComponent>(juce::TabbedButtonBar::Orientation::TabsAtTop);
    addAndMakeVisible(colourControlTabs.get());

    colourTextEditor = std::make_unique<ColourTextEditor>("colourTextEditor", "000000");
    addAndMakeVisible(colourTextEditor.get());

    colourSubwindow = std::make_unique<ColourViewComponent>();
    addAndMakeVisible(colourSubwindow.get());

    colourPickerToggle = std::make_unique<juce::TextButton>("Pick");
    addAndMakeVisible(colourPickerToggle.get());

    keyTypeCombo = std::make_unique<juce::ComboBox>();
    keyTypeCombo->setEditableText (false);
    keyTypeCombo->setJustificationType (juce::Justification::centredLeft);
    keyTypeCombo->setTextWhenNothingSelected (juce::String());
    keyTypeCombo->setTextWhenNoChoicesAvailable (juce::translate("(no choices)"));
    keyTypeCombo->addItem (juce::translate("Note on/Note off"), 1);
    keyTypeCombo->addItem (juce::translate("Continuous controller"), 2);
    keyTypeCombo->addItem (juce::translate("Lumatouch"), 3);
    keyTypeCombo->addItem (juce::translate("Disabled"), 4);
    addAndMakeVisible(keyTypeCombo.get());

    noteInput = std::make_unique<juce::Slider>();
    noteInput->setTooltip (juce::translate("MIDI note or MIDI controller no. (for key type \'continuous controller\')"));
    noteInput->setRange (0, 127, 1);
    noteInput->setSliderStyle (juce::Slider::IncDecButtons);
    noteInput->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 56, 20);
    // noteInput->addListener (this);
    addAndMakeVisible(noteInput.get());

    channelInput = std::make_unique<juce::Slider>();
    channelInput->setRange (1, 16, 1);
    channelInput->setSliderStyle (juce::Slider::IncDecButtons);
    channelInput->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 56, 20);
    addAndMakeVisible(channelInput.get());

}

KeyEditorControls::~KeyEditorControls()
{
    channelInput = nullptr;
    noteInput = nullptr;
    keyTypeCombo = nullptr;
    colourPickerToggle = nullptr;
    colourSubwindow = nullptr;
    colourTextEditor = nullptr;
    colourControlTabs = nullptr;
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

    headerHeight = roundToInt(h * headerH);
    headerPath = getConnectedRoundedRectPath(getLocalBounds().withBottom(headerHeight + 1).toFloat(), getRoundedRectCornerSize(), juce::Button::ConnectedEdgeFlags::ConnectedOnBottom);
    controlPath = getConnectedRoundedRectPath(getLocalBounds().withTop(headerHeight).toFloat(), getRoundedRectCornerSize(), juce::Button::ConnectedEdgeFlags::ConnectedOnTop);

    contentMarginWidth = roundToInt(w * contentMarginW);
    contentMarginHeight = roundToInt(h * controlMarginH);

    labelHeight = roundToInt(headerHeight * labelToHeaderH);
    lblKeySettings->setTopLeftPosition(contentMarginWidth, 0);
    resizeLabelWithHeight(lblKeySettings.get(), headerHeight, labelToHeaderH);

    keyControlColumnWidth = roundToInt(w * keyControlColumnW);
    keyControlHeight = roundToInt(h * keyControlH);
    keyControlMarginHeight = roundToInt(h * keyControlMarginH);

    colourTextEditor->setBounds(contentMarginWidth, headerHeight + contentMarginHeight, keyControlColumnWidth, keyControlHeight);
    keyTypeCombo->setBounds(contentMarginWidth, colourTextEditor->getBottom() + keyControlMarginHeight, keyControlColumnWidth, keyControlHeight);
    noteInput->setBounds(contentMarginWidth, keyTypeCombo->getBottom() + keyControlMarginHeight, keyControlColumnWidth, keyControlHeight);
    channelInput->setBounds(contentMarginWidth, noteInput->getBottom() + keyControlMarginHeight, keyControlColumnWidth, keyControlHeight);

    colourColumnX = contentMarginWidth + keyControlColumnWidth + roundToInt(w * columnMarginW);
    colourColumnWidth = w - colourColumnX - contentMarginWidth;
    colourColumnHeight = roundToInt(h * colourColumnH);

    colourControlTabs->setBounds(colourColumnX, headerHeight + contentMarginHeight, colourColumnWidth, colourColumnHeight);
}
