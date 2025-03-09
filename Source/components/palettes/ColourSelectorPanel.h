/*
  ==============================================================================

    colour_palette_window.h
    Created: 14 Dec 2020 11:32:03pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include "./../../lumatone_editor_library/palettes/colour_selection_group.h"
#include "./../../lumatone_editor_library/palettes/colour_palette_file.h"

#include "./ColorPalettesControls.h"

#include "../../data/LumatoneEditorState.h"

class ColourSelectionGroup;
class ColourPaletteComponent;

class CustomPickerPanel;
class PaletteEditPanel;

//==============================================================================
/*
*/
class ColourSelectorPanel   : public juce::Component
                            , public LumatoneEditorState
                            , private LumatoneEditorState::Controller
                            , public juce::ChangeListener
                            , public ColorPalettesControls::Listener
{
public:
    ColourSelectorPanel(const LumatoneEditorState& stateIn);
    ~ColourSelectorPanel() override;

    void paint(juce::Graphics& g) override;

    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void setIndentSize(int size, bool resize=true);
    void setTabBarDepth(int height, bool resize=true);

    void setBackgroundColour(juce::Colour backgroundIn);

    //====================================================================

    // /// <summary>
    // /// Registers a listener to receive the colour the user's input resolves to
    // /// </summary>
    // /// <param name="listenerIn"></param>
    // void listenToColourSelection(ColourSelectionListener* listenerIn) { colourSelectorGroup->addColourSelectionListener(listenerIn); }

    // /// <summary>
    // /// Adds a colour selector component to the colour selection group
    // /// </summary>
    // /// <param name="broadcaster"></param>
    // void addColourSelectorToGroup(ColourSelectionBroadcaster* broadcaster) { colourSelectorGroup->addSelector(broadcaster); }

    // /// <summary>
    // /// Force a colour selector (added to the group) to be selected
    // /// </summary>
    // /// <param name="newSelector"></param>
    // void setCurrentColourSelector(ColourSelectionBroadcaster* newSelector)
    // {
    //     // This statement is kind of a kludge - vsicurella
    //     if (colourSelectorGroup->getIndexOfSelector(newSelector) < 0)
    //         colourSelectorGroup->addSelector(newSelector);

    //     colourSelectorGroup->setCurrentSelector(newSelector);
    // }

    // ColourSelectionGroup* getColourSelectionGroup() { return colourSelectorGroup.get(); }

private:

    /// <summary>
    /// Launches the Palette Edit panel and listens for user interaction.
    /// </summary>
    /// <param name="paletteIn"></param>
    void startEditingPalette(int paletteIndexIn, int selectedSwatchIndex = -1);

    /// <summary>
    /// Creates a new palette with the same name and swatches
    /// </summary>
    /// <param name="paletteIndexIn"></param>
    void duplicatePalette(int paletteIndexIn);

    /// <summary>
    /// Removes a palette and associated buttons
    /// </summary>
    /// <param name="paletteIndexToRemove"></param>
    void removePalette(int paletteIndexToRemove);

protected:

    void customPickerRequested();

    void selectPaletteRequested();

    void editPaletteRequested(int paletteIndex, int selectedSwatchIndex) override;

    void clonePaletteRequested(int paletteIndex) override;

    void deletePaletteRequested(int paletteIndex) override;

    void newPaletteRequested() override;

    void selectionChanged(int paletteIndex, int selectedSwatchIndex) override;


    void colourToolTabChanged(int index, bool writeToProperties=true);

private:

    // std::unique_ptr<ColourSelectionGroup> colourSelectorGroup;

    std::unique_ptr<ColorPalettesControls> palettePanel;
    std::unique_ptr<juce::Viewport> palettePanelViewport;

    std::unique_ptr<juce::TextButton> newPaletteBtn;
    std::unique_ptr<juce::TextButton> editPaletteBtn;
    std::unique_ptr<juce::TextButton> deletePaletteBtn;

    std::unique_ptr<CustomPickerPanel> customPickerPanel;

    std::unique_ptr<juce::TabbedComponent> colourToolTabs;

    std::unique_ptr<PaletteEditPanel> paletteEditPanel;

    int paletteIndexEditing = -1;
    bool paletteEditingIsNew = false;

    int indent = 48;
    int tabBarDepth = 48;

    const float viewportIndentScalar = 0.04f;
    const float viewportScrollbarAreaScalar = 0.03f;
    const float paletteControlWindowH = 0.07f;

    juce::Rectangle<int> controlArea;

    int paletteHeightFix = 0;

    juce::Colour backgroundColour;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColourSelectorPanel)
};
