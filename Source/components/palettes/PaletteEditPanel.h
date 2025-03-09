/*
  ==============================================================================

    ColourSelectionPanels.h
    Created: 21 Dec 2020 9:48:08pm
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_PALETTE_EDIT_PANEL_H
#define LUMATONE_PALETTE_EDIT_PANEL_H


#include "../../data/LumatoneEditorState.h"

class Palette;

//==============================================================================
/*
*   Colour palette edtior panel
*/
class PaletteEditPanel : public juce::Component
                       , public LumatoneEditorState
                       , public juce::Button::Listener
                       , public juce::Label::Listener
                       , public juce::ChangeBroadcaster
{
public:

    PaletteEditPanel(const LumatoneEditorState& stateIn, const LumatoneEditorColourPalette& paletteIn);
    ~PaletteEditPanel();

    void paint(juce::Graphics& g) override;
    void resized() override;

    //==============================================================================

    void buttonClicked(juce::Button* btn) override;

    //==============================================================================

    void labelTextChanged(juce::Label* label) override;

    void editorShown(juce::Label*, juce::TextEditor& editor) override;

    //==============================================================================

    juce::Array<juce::Colour> getCurrentPalette() const;

    void setSelectedSwatch(int selectedSwatchNumber);

    juce::String getPaletteName();

    bool wasSaveRequested() const;

private:

    LumatoneEditorColourPalette     colourPalette;

    std::unique_ptr<Palette>              paletteControl;
    std::unique_ptr<juce::ColourSelector> colourPicker;

    std::unique_ptr<juce::Label>          editPaletteLabel;
    std::unique_ptr<juce::Label>          paletteNameEditor;
    std::unique_ptr<juce::TextButton>     saveButton;
    std::unique_ptr<juce::TextButton>     cancelButton;

    bool saveRequested = false;

    bool paletteUnnamed = false;

    // Drawing constants

    const float leftColumnWidth     = 0.3677f;

    const float editPaletteLabelY   = 0.1212f;
    const float editPaletteHeight   = 0.0606f;

    const float paletteY            = 0.26f;
    const float paletteWidthScalar  = 0.25f;
    const float paletteHeightScalar = 0.25f;

    const float buttonY             = 0.6739f;
    const float buttonWidth         = 0.2208f;
    const float buttonHeight        = 0.0889f;

    const float pickerWidth         = 0.6f;
    const float pickerHeight        = 0.9f;
};

#endif // LUMATONE_PALETTE_EDIT_PANEL_H
