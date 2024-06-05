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

#include "../ColourPaletteWindow.h"
// #include "../lumatone_editor_library/palettes/palette_selection_panel.h"
#include "../lumatone_editor_library/palettes/colour_picker_panel.h"

KeyEditorControls::KeyEditorControls(const LumatoneEditorState& stateIn)
        : LumatoneEditorState(stateIn)
        , juce::Component("KeyEditorControls")
{
    lblKeySettings = std::make_unique<juce::Label>("lblKeySettings", "Key Settings");
    lblKeySettings->setColour(juce::Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LabelBlue));
    lblKeySettings->setFont(getAppFonts().getFont(LumatoneEditorFont::FranklinGothic));
    addAndMakeVisible(lblKeySettings.get());

    colourTextEditor = std::make_unique<ColourTextEditor>("colourTextEditor", "000000");
    addAndMakeVisible(colourTextEditor.get());

    colourSubwindow = std::make_unique<ColourViewComponent>(juce::Colour(0xff5398b7));
    colourSubwindow->setColourButtonMode(ColourViewComponent::ColourButtonMode::Dropper);
    addAndMakeVisible(colourSubwindow.get());

    // colourPickerToggle = std::make_unique<juce::TextButton>("Pick");
    // addAndMakeVisible(colourPickerToggle.get());

    keyTypeCombo = std::make_unique<juce::ComboBox>("keyTypeCombo");
    keyTypeCombo->setEditableText (false);
    keyTypeCombo->setJustificationType (juce::Justification::centredLeft);
    keyTypeCombo->setTextWhenNothingSelected (juce::String());
    keyTypeCombo->setTextWhenNoChoicesAvailable (juce::translate("(no choices)"));
    keyTypeCombo->addItem (juce::translate("Note on/Note off"), 1);
    keyTypeCombo->addItem (juce::translate("Continuous controller"), 2);
    keyTypeCombo->addItem (juce::translate("Lumatouch"), 3);
    keyTypeCombo->addItem (juce::translate("Disabled"), 4);
    addAndMakeVisible(keyTypeCombo.get());

    noteInput = std::make_unique<juce::Slider>("noteInput");
    noteInput->setTooltip (juce::translate("MIDI note or MIDI controller no. (for key type \'continuous controller\')"));
    noteInput->setRange (0, 127, 1);
    noteInput->setSliderStyle (juce::Slider::IncDecButtons);
    noteInput->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 56, 20);
    // noteInput->addListener (this);
    addAndMakeVisible(noteInput.get());

    channelInput = std::make_unique<juce::Slider>("channelInput");
    channelInput->setRange (1, 16, 1);
    channelInput->setSliderStyle (juce::Slider::IncDecButtons);
    channelInput->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 56, 20);
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
    addAndMakeVisible(colourPalettePanel.get());
    // colourPickerPanel = std::make_unique<CustomPickerPanel>();

    // juce::Colour colourControlBackground = getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ColourPaletteBackground);
    // colourControlTabs = std::make_unique<juce::TabbedComponent>(juce::TabbedButtonBar::Orientation::TabsAtTop);
    // colourControlTabs->addTab("Colour Palettes", colourControlBackground, colourPalettePanel.get(), false);
    // colourControlTabs->addTab("Custom Picker", colourControlBackground, colourPickerPanel.get(), false);
    // addAndMakeVisible(colourControlTabs.get());

}

KeyEditorControls::~KeyEditorControls()
{
    colourControlTabs = nullptr;
    colourPickerPanel = nullptr;
    colourPalettePanel = nullptr;

    channelInput = nullptr;
    noteInput = nullptr;
    keyTypeCombo = nullptr;
    // colourPickerToggle = nullptr;
    colourSubwindow = nullptr;
    colourTextEditor = nullptr;
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

    auto roundedCornerSize = getRoundedRectCornerSize();
    // labelMarginWidth = roundToInt(roundedCornerSize * 0.25f);

    headerHeight = roundToInt(h * headerH);
    headerPath = getConnectedRoundedRectPath(getLocalBounds().withBottom(headerHeight + 1).toFloat(), roundedCornerSize, juce::Button::ConnectedEdgeFlags::ConnectedOnBottom);
    controlPath = getConnectedRoundedRectPath(getLocalBounds().withTop(headerHeight).toFloat(), roundedCornerSize, juce::Button::ConnectedEdgeFlags::ConnectedOnTop);

    contentMarginWidth = roundToInt(getParentWidth() * contentMarginParentW);
    contentMarginHeight = roundToInt(h * controlMarginH);

    labelHeight = roundToInt(headerHeight * labelToHeaderH);
    lblKeySettings->setTopLeftPosition(contentMarginWidth, 0);
    resizeLabelWithHeight(lblKeySettings.get(), headerHeight, labelToHeaderH);

    keyControlColumnWidth = roundToInt(w * keyControlColumnW);
    keyControlColumnRight = contentMarginWidth + keyControlColumnWidth;

    keyControlHeight = roundToInt(h * keyControlH);
    keyControlMarginHeight = roundToInt(h * keyControlMarginH);

    lblColour->setTopLeftPosition(contentMarginWidth, headerHeight + contentMarginHeight);
    resizeLabelWithHeight(lblColour.get(), keyControlHeight, controlLabelFontScalar);

    colourButtonMargin = lblColour->getFont().getStringWidth(" ");
    colourButtonWidth = roundToInt(getParentWidth() * colourButtonParentW) - colourButtonMargin;

    colourTextEditor->setBounds(lblColour->getRight(), lblColour->getY(), keyControlColumnRight - lblColour->getRight() - colourButtonWidth, keyControlHeight);
    colourSubwindow->setBounds(colourTextEditor->getRight() + colourButtonMargin, colourTextEditor->getY(), colourButtonWidth, keyControlHeight);

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

    colourPalettePanel->setBounds(colourColumnX, 0, colourColumnWidth, h - contentMarginHeight);
}
