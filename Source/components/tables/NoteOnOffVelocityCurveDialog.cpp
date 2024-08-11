/*
  ==============================================================================

    NoteOnOffVelocityCurveDialog.cpp
    Created: 13 Nov 2020 10:20:07pm
    Author:  hsstraub

  ==============================================================================
*/

#include "NoteOnOffVelocityCurveDialog.h"

//==============================================================================
NoteOnOffVelocityCurveDialog::NoteOnOffVelocityCurveDialog(const LumatoneEditorState& stateIn)
    : VelocityCurveDlgBase(stateIn, LumatoneConfigTable::TableType::velocityInterval)
{
}
