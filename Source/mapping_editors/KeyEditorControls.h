/*
  ==============================================================================

    KeyEditorControls.h
    Created: 3 June 2024
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_KEY_EDITOR_CONTROLS_H
#define LUMATONE_EDITOR_KEY_EDITOR_CONTROLS_H

#include "../LumatoneEditorState.h"

class ColourViewComponent;
class ColourTextEditor;
class ColourPaletteWindow;
class CustomPickerPanel;
class ColourPalettesPanel;

class KeyEditorControls : public LumatoneEditorState
                        , public juce::Component
{
public:
    KeyEditorControls(const LumatoneEditorState& stateIn);

    virtual ~KeyEditorControls() override;


    void paint(juce::Graphics&) override;
    void resized() override;


private:

    std::unique_ptr<juce::Label>            lblKeySettings;

    std::unique_ptr<juce::TabbedComponent>  colourControlTabs;

    std::unique_ptr<ColourTextEditor>       colourTextEditor;
    std::unique_ptr<ColourViewComponent>    colourSubwindow;
    // std::unique_ptr<juce::TextButton>       colourPickerToggle;

    std::unique_ptr<juce::ComboBox>         keyTypeCombo;
    std::unique_ptr<juce::Slider>           noteInput;
    std::unique_ptr<juce::Slider>           channelInput;

    std::unique_ptr<juce::Label>            lblColour;
    std::unique_ptr<juce::Label>            lblKeyType;
    std::unique_ptr<juce::Label>            lblNote;
    std::unique_ptr<juce::Label>            lblChannel;

    std::unique_ptr<ColourPaletteWindow>    colourPalettePanel;
    // std::unique_ptr<>    colourPalettePanel;
    std::unique_ptr<CustomPickerPanel>      colourPickerPanel;


    const float headerH         = 0.19f;
    int headerHeight;
    juce::Path headerPath;
    juce::Path controlPath;

    int contentMarginWidth;
    const float contentMarginParentW  = 0.024f;

    int contentMarginHeight;
    const float controlMarginH        = 0.08f;

    int labelHeight;
    // int labelMarginWidth;
    const float labelToHeaderH         = 0.5f;
    const float controlLabelFontScalar = 0.67f;

    int keyControlColumnWidth;
    int keyControlColumnRight;
    const float keyControlColumnW     = 0.3f;

    int keyControlHeight;
    const float keyControlH           = 0.128f;

    int keyControlMarginHeight;
    const float keyControlMarginH     = 0.059f;

    int colourColumnX;
    int colourColumnWidth;
    const float columnMarginW         = 0.055f;

    int colourButtonMargin;
    int colourButtonWidth;
    const float colourButtonParentW   = 0.031f;

    int colourColumnHeight;
    const float colourColumnH         = 0.6f;

};

#endif // LUMATONE_EDITOR_KEY_EDITOR_CONTROLS_H
