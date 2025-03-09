/*
  ==============================================================================

    ColourSelectionPanels.h
    Created: 21 Dec 2020 9:48:08pm
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_PALETTE_SELECTION_PANEL_H
#define LUMATONE_PALETTE_SELECTION_PANEL_H

#include "../../data/LumatoneEditorState.h"
#include "../../lumatone_editor_library/palettes/colour_selection_group.h"
#include "../../lumatone_editor_library/palettes/colour_palette_file.h"

class ColourPaletteComponent;
class PaletteControlGroup;

//==============================================================================
/*
*   Panel that displays the palettes and allows users to select a swatch
*/
class ColorPalettesControls : public juce::Component
                            , public LumatoneEditorState
                            , public ColourSelectionListener
{
public:

    ColorPalettesControls(const LumatoneEditorState& stateIn);
    ~ColorPalettesControls();

    void setParams(int itemHeight, bool autoSize=true, int maxColumns = -1);

    void updateGrid(bool resize = false);

    // juce::Component implementation
    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseMove(const juce::MouseEvent& mouse) override;

    //====================================================================

    int getNumCols() const { return numCols; }
    int getNumRows() const { return numRows; }

    ColourPaletteComponent* getPaletteComponent(int paletteIndex);

    int getLastSelectedPaletteIndex() const { return lastSelectedPalette; }

private:
    void updateGridBounds();

    void setControlsVisibleForPalette(int paletteIndex, bool areVisible);

public:

    void colourChangedCallback(ColourSelectionBroadcaster* source, juce::Colour newColour) override;

private:

    std::unique_ptr<ColourPaletteComponent> newPalette;
    std::unique_ptr<juce::TextButton> newPaletteBtn;
    juce::OwnedArray<PaletteControlGroup> controlGroups;
    juce::Array<ColourPaletteComponent*> allPalettes;
    juce::OwnedArray<juce::Label> paletteLabels;

    juce::Array<juce::FlexItem> dbgItems;

    // Params
    int maxNumColumns = 3;
    int itemHeight = 100;
    bool autoSize = true;

    // Data
    int numCols = 3;
    int numRows = 1;
    int itemWidth = 110;
    int controlHeight = 30;
    juce::FlexItem::Margin  paletteItemMargins;

    int newPaletteMarginWidth = 8;
    float newBtnWidth = 48;


    int viewableWidth = 0;
    int viewableHeight = 0;
    bool needsResize = false;

    juce::Array<juce::Rectangle<int>> controlGroupHitBoxes;
    int lastPaletteMouseOver = -1;

    int lastSelectedPalette = -1;

    const float itemWidthScalar         = 0.265f;
    const float itemHeightScalar        = 0.24f;
    const float itemAspect              = 1.1f;

    const float controlHeightAspect     = 0.333f;

    const float topMarginScalar         = 0.1f;
    const float horizontalMarginScalar  = 0.0367f;
    const float bottomMarginScalar      = 0.06f;
    const float btmMarginCtrlScalar     = 0.04f;

    const float buttonWidthScalar       = 0.333333f;
    const float buttonHeightScalar      = 0.166667f;

    const float panelLeftMarginWidth    = 0.020833f;

    const float newPaletteMarginW = 0.3f;


    //==============================================================================

public:

    class Listener
    {
    public:
        virtual ~Listener() {}

        virtual void editPaletteRequested(int paletteIndex, int selectedSwatchIndex) = 0;
        virtual void clonePaletteRequested(int paletteIndex) = 0;
        virtual void deletePaletteRequested(int paletteIndex) = 0;
        virtual void newPaletteRequested() = 0;

        virtual void selectionChanged(int paletteIndex, int selectedSwatchIndex) = 0;
    };

    void addListener(Listener* listenerIn) { listeners.add(listenerIn); }
    void removeListener(Listener* listenerIn) { listeners.remove(listenerIn); }

protected:

    juce::ListenerList<ColorPalettesControls::Listener> listeners;
};

#endif // LUMATONE_PALETTE_SELECTION_PANEL_H
