/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.7

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>

#include "VelocityCurveComponents.h"
#include "VelocityCurveEditStrategy.h"

#include "LumatoneEditorState.h"

class LumatoneLayout;
class LumatoneConfigTable;

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
An auto-generated component, created by the Projucer.

Describe your class and how it works here!
                                                                    //[/Comments]
*/
class VelocityCurveDlgBase  : public Component,
                              public LumatoneEditorState,
                              public juce::ComboBox::Listener
{
public:
    //==============================================================================
    VelocityCurveDlgBase (const LumatoneEditorState& stateIn, LumatoneConfigTable::TableType typeValue);
    ~VelocityCurveDlgBase() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

	// New mapping is loaded. Display data.
	void loadFromMapping();

	virtual void sendVelocityTableToController();

	void mouseMove(const juce::MouseEvent &event);
	void mouseDown(const juce::MouseEvent &event);
	void mouseDrag(const juce::MouseEvent &event);
	void mouseUp(const juce::MouseEvent &event);

protected:
	virtual float beamWidth(int xPos) { return getWidth() / 128.0f; }

	// LumatoneLayout*	getMappingInEdit();
	const LumatoneConfigTable* getConfigInEdit() const;
	VelocityCurveEditStrategyBase* getCurrentDrawingStrategy();

public:
    void paintOverChildren(juce::Graphics& g) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void comboBoxChanged (juce::ComboBox* comboBoxThatHasChanged) override;
    void lookAndFeelChanged() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	LumatoneConfigTable::TableType velocityCurveType;
	//Path beamTableFrame;
    juce::Path beamTableContour;
    juce::Path beamTableGrid;
	std::unique_ptr<VelocityCurveBeam> velocityBeamTable[128];

	VelocityCurveFreeDrawingStrategy freeDrawingStrategy;
	VelocityCurveLinearDrawingStrategy linearDrawingStrategy;
	VelocityCurveQuadraticDrawingStrategy quadraticDrawingStrategy;

	std::map<LumatoneConfigTable::DrawMode, VelocityCurveEditStrategyBase*> drawingStrategies;

    juce::ColourGradient beamColourGradient;
    juce::Colour backgroundColour;
    juce::Colour gridColour;

protected:
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::ComboBox> cbEditMode;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VelocityCurveDlgBase)
};

//[EndFile] You can add extra defines here...

//[/EndFile]
