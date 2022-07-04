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
#include "../JuceLibraryCode/JuceHeader.h"

#include "ViewComponents.h"
#include "KeyboardDataStructure.h"
#include "ColourEditComponent.h"

#include "HexagonTilingGeometry.h"
#include "LumatoneEditorStyleCommon.h"

#include "BoardGeometry.h"

#include "SingleNoteAssign.h"
#include "IsomorphicMassAssign.h"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class NoteEditArea  : public Component,
                      public ChangeListener,
                      public LumatoneKeyEditListener,
                      public ColourSelectionBroadcaster
{

public:
    //==============================================================================
    NoteEditArea ();
    ~NoteEditArea() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void restoreStateFromPropertiesFile(PropertiesFile* propertiesFile);
	void saveStateToPropertiesFile(PropertiesFile* propertiesFile);

	// Implementation of ChangeListener
	void changeListenerCallback(ChangeBroadcaster *source) override;

	// Things to be done when a new mapping is loaded. E. g. fill the colour combo box with the colours appearing in the mapping.
	void onSetData(TerpstraKeyMapping& newData);

	// Fill key fields with values from a certain octaveboard subset
	//void setKeyFieldValues(const LumatoneKeySelection& keySelection);
    void setKeyFieldValues(const TerpstraKeys& keys, int octaveIndex);
    void setKeyFieldValues(const TerpstraKeys (&allOctaveKeys)[5]);

	TabbedButtonBar* getOctaveBoardSelectorTab() { return octaveBoardSelectorTab.get(); }

    ColourEditComponent* getColourEditComponent();

    ColourTextEditor* getSingleNoteColourTextEditor();

    IsomorphicMassAssign* getIsomorphicMassAssignPanel() { return dynamic_cast<IsomorphicMassAssign*>(editFunctionsTab->getTabContentComponent(1)); }

	void changeKeySelection(LumatoneKeySelection& selection);

    void resetKeySelection();

    void setEditAllOctaves(bool editAllOctaves);
    void hideInactiveOctaves();

	void refreshKeyFields();

    void lookAndFeelChanged() override;

    // Helper method for aligning the Octave Section TabbedButtonBar
    void setControlsTopLeftPosition(int controlsAreaX, int controlsAreaY);

    void resetOctaveSize(bool refreshAndResize=true);

    // ColourSelectionBroadcaster Implementation
    Colour getSelectedColour() override;
    void deselectColour() override {};

    void keyClickedCallback(const juce::MouseEvent& e, int boardIndex, int keyIndex, TerpstraKey value) override;

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent& e) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    enum noteEditMode
    {
        SingleNoteAssignMode = 0,
        IsomorphicMassAssignMode = 1
    };

    int currentBoardSize = 0;
	bool showIsomorphicMassAssign = false;

    // TODO support arbitrary octave sets?
    int octaveIndexBeingEdited = 0; // Less than 0 means all octaves are being edited

	// Selector for octave boards
	std::unique_ptr<TabbedButtonBar> octaveBoardSelectorTab;

	// Editing single keys (of the selected 56-key set)
	juce::OwnedArray<LumatoneBoardSet> octaveKeySets;

    //int currentSingleKeySelection;
    LumatoneKeySelection currentKeySelection;

    // Key edit positioning
    HexagonTilingGeometry tilingGeometry;

    //===========================================================================
    // Style Helpers
    Colour backgroundColour = Colours::darkgrey;

    Rectangle<float> octaveTabsArea;
    Rectangle<float> contentBackground;
    Rectangle<float> assignControlsBounds;
    Rectangle<float> keyEditBounds;

    int roundedCornerLayout;

    //===========================================================================
    // Size & position constants relative to parent unless otherwise noted

    const float contentMarginY                  = 0.10101f;

    const float assignControlsWidthHRatio       = 0.545f;
    const float assignControlsHeightInContent   = 0.9f;

    const float assignControlsMarginXHRatio     = 0.07f;
    const float assignMarginYInContent          = 0.0444f;
    const float assignTabDepthInContent         = 0.075f;
    const float tabBarLengthDepthRatio          = 15.0f;

    const float assignLabelMarginXHRatio        = 0.0155f;
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
	TerpstraBoardGeometry	boardGeometry;

    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::TabbedComponent> editFunctionsTab;
    std::unique_ptr<juce::Label> labelWindowTitle;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteEditArea)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

