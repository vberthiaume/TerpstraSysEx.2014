/*
  ==============================================================================

    NoteOnOffVelocityCurveDialog.h
    Created: 13 Nov 2020 10:20:07pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include "VelocityCurveDlgBase.h"

class LumatoneEditorState;

// Note on/on velocity curve dialog. Horizontal axis stands for ticks
class NoteOnOffVelocityCurveDialog : public VelocityCurveDlgBase {
public:
    NoteOnOffVelocityCurveDialog(const LumatoneEditorState& stateIn);
};

class FaderVelocityCurveDialog : public VelocityCurveDlgBase {
public:
    FaderVelocityCurveDialog(const LumatoneEditorState& stateIn) : VelocityCurveDlgBase(stateIn, LumatoneConfigTable::TableType::fader)
    {
    }
};

class AftertouchVelocityCurveDialog : public VelocityCurveDlgBase {
public:
    AftertouchVelocityCurveDialog(const LumatoneEditorState& stateIn) : VelocityCurveDlgBase(stateIn, LumatoneConfigTable::TableType::afterTouch)
    {
    }
};

class LumatouchVelocityCurveDialog : public VelocityCurveDlgBase {
public:
    LumatouchVelocityCurveDialog(const LumatoneEditorState& stateIn) : VelocityCurveDlgBase(stateIn, LumatoneConfigTable::TableType::lumaTouch)
    {
    }
};
