/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>

#include "LumatoneEditorState.h"

#include "./lumatone_editor_library/mapping/lumatone_tiling.h"
#include "./lumatone_editor_library/palettes/colour_selection_broadcaster.h"
#include "./lumatone_editor_library/listeners/editor_listener.h"

class ColourViewComponent;
class ColourTextEditor;
class IsomorphicMassAssign;

class LumatoneKeyEdit;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class NoteEditArea  : public juce::Component,
                      public LumatoneEditorState,
                      public LumatoneEditorState::Controller,
                      public juce::ChangeListener,
                      public LumatoneEditor::EditorListener,
                      public ColourSelectionBroadcaster
{
public:
    //==============================================================================
    NoteEditArea(const LumatoneEditorState& stateIn);
    ~NoteEditArea() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	//void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);

	// Implementation of ChangeListener
	void changeListenerCallback(ChangeBroadcaster *source) override;

	juce::TabbedButtonBar* getOctaveBoardSelectorTab() { return octaveBoardSelectorTab.get(); }

    ColourViewComponent* getColourViewComponent();

    ColourTextEditor* getSingleNoteColourTextEditor();

    IsomorphicMassAssign* getIsomorphicMassAssignPanel();

	void changeSingleKeySelection(int newSelection);


    // ColourSelectionBroadcaster Implementation
    juce::Colour getSelectedColour() override;
    void deselectColour() override {}

    // Helper method for aligning the Octave Section TabbedButtonBar
    void setControlsTopLeftPosition(int controlsAreaX, int controlsAreaY);

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& e) override;


private:
    void refreshKeyFields();
    void resetOctaveSize(bool refreshAndResize = true);


    // Things to be done when a new mapping is loaded. E. g. fill the colour combo box with the colours appearing in the mapping.
    //void onSetData(LumatoneLayout& newData);

    // Fill key fields with values from a certain octaveboard subset
    void setKeyFieldValues(const LumatoneBoard& keySet);

    void updateShowIsomorphicAssign();

private:
    // LumatoneEditor::EditorListener 
    void completeMappingLoaded(const LumatoneLayout& mappingData) override;
    void boardChanged(const LumatoneBoard& boardData) override;
    void keyChanged(int boardIndex, int keyIndex, const LumatoneKey& lumatoneKey) override;
    void selectionChanged(juce::Array<MappedLumatoneKey> selection) override;

    // LumatoneEditorState implementaiton
    void handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier&  property) override;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    enum noteEditMode
    {
        SingleNoteAssignMode = 0,
        IsomorphicMassAssignMode = 1
    };

    int currentBoardSize = 0;
	bool showIsomorphicMassAssign = false;

	// Selector for octave boards
	std::unique_ptr<juce::TabbedButtonBar> octaveBoardSelectorTab;

	// Editing single keys (of the selected 56-key set)
	std::unique_ptr<LumatoneKeyEdit>	terpstraKeyFields[56];

    int	currentSingleKeySelection;

    // Key edit positioning
    LumatoneTiling lumatoneGeometry;

    //===========================================================================
    // Style Helpers
    juce::Colour backgroundColour = Colours::darkgrey;

    Rectangle<float> octaveTabsArea;
    Rectangle<float> contentBackground;
    Rectangle<float> assignControlsBounds;
    Rectangle<float> keyEditBounds;

    int roundedCornerLayout;

    //===========================================================================
    // Size & position constants relative to parent unless otherwise noted

    const float contentMarginY                  = 0.10101f;

    const float assignControlsWidth             = 0.3846f;
    const float assignControlsHeightInContent   = 0.9f;

    const float assignMarginX                   = 0.05f;
    const float assignMarginYInContent          = 0.0444f;
    const float assignTabDepthInContent         = 0.075f;

    const float assignLabelMarginX              = 0.01f;
    const float assignLabelTabDepthHeight       = 0.9f;

    const float keyEditWidth                    = 0.48f;
    const float keyEditMarginX                  = 0.48f;
    const float keyEditMarginYInContent         = 0.1f;

    //===========================================================================
    // Key fields relative to KeyEditBounds

    const float singleKeyFieldRimAbove = 0.075;// = 0.05106f;
    const float singleKeyFieldRimLeft           = 0.02105f;
    const float singleKeyFieldSize              = 0.11f;
    const float singleKeyFieldLineWidth         = 0.003f;
    const float singleKeyFieldLineWidthSelected = 0.005f;

    const float singleKeyMarginFromWidth = 0.0164f;

	// Geometry settings
	LumatoneGeometry	boardGeometry;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TabbedComponent> editFunctionsTab;
    std::unique_ptr<juce::Label> labelWindowTitle;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteEditArea)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
