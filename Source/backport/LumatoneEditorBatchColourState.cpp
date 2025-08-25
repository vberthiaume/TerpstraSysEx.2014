#include "LumatoneEditorBatchColourState.h"
#include "../LumatoneController.h"

static juce::Array<juce::Identifier> getBatchColourEditProperties()
{
    juce::Array<juce::Identifier> properties;
    properties.add(LumatoneEditorBatchColourProperty::UseBrightness);
    properties.add(LumatoneEditorBatchColourProperty::UseHueShift);
    properties.add(LumatoneEditorBatchColourProperty::UseTempShift);
    properties.add(LumatoneEditorBatchColourProperty::BrightnessMultiplier);
    properties.add(LumatoneEditorBatchColourProperty::HueShiftAmount);
    properties.add(LumatoneEditorBatchColourProperty::TemperatureShiftValue);
    return properties;
}

LumatoneEditorBatchColourState::LumatoneEditorBatchColourState(LumatoneController* stateIn)
    // : LumatoneStateBase(nameIn)
    : state(*stateIn)
{
    // state = parentStateIn.getChildWithName(LumatoneEditorBatchColourProperty::State);

    // if (!state.isValid())
    // {
    //     state = juce::ValueTree(LumatoneEditorBatchColourProperty::State);
    //     parentStateIn.addChild(state, -1, nullptr);
    // }

    // state.addListener(this);
}

// LumatoneEditorBatchColourState::LumatoneEditorBatchColourState(juce::String nameIn, juce::ValueTree parentStateIn, const LumatoneEditorBatchColourState &stateToCopy)
//     : LumatoneStateBase(nameIn)
// {
//     juce::ValueTree stateData = stateToCopy.state.getChildWithName(LumatoneEditorBatchColourProperty::State);
//     state = loadStateProperties(stateData);
//     state.addListener(this);

//     parentStateIn.addChild(state, -1, nullptr);
// }

void LumatoneEditorBatchColourState::setBrightnessMultiplier(bool set, float value)
{
    data.useBrightness = set;
    data.brightnessMultiplier = value;

    // if (set)
    // {
    //     state.setPropertyExcludingListener(this, LumatoneEditorBatchColourProperty::BrightnessMultiplier, value, nullptr);
    // }
    // else
    // {
    //     state.removeProperty(LumatoneEditorBatchColourProperty::BrightnessMultiplier, nullptr);
    // }
}

void LumatoneEditorBatchColourState::setHueShiftAmount(bool set, float value)
{
    data.useHueShift = set;
    data.hueShiftAmount = value;

    // if (set)
    // {
    //     state.setPropertyExcludingListener(this, LumatoneEditorBatchColourProperty::HueShiftAmount, value, nullptr);
    // }
    // else
    // {
    //     state.removeProperty(LumatoneEditorBatchColourProperty::HueShiftAmount, nullptr);
    // }
}

void LumatoneEditorBatchColourState::setTempShiftAmount(bool set, float value)
{
    data.useTemperatureShift = set;
    data.temperatureShiftValue = value;

    // if (set)
    // {
    //     state.setPropertyExcludingListener(this, LumatoneEditorBatchColourProperty::TemperatureShiftValue, value, nullptr);
    // }
    // else
    // {
    //     state.removeProperty(LumatoneEditorBatchColourProperty::TemperatureShiftValue, nullptr);
    // }
}

// juce::ValueTree LumatoneEditorBatchColourState::loadStateProperties(juce::ValueTree stateIn)
// {
//     juce::ValueTree newState = (stateIn.hasType(LumatoneEditorBatchColourProperty::State))
//                              ? stateIn
//                              : juce::ValueTree(LumatoneEditorBatchColourProperty::State);

//     for (auto property : getBatchColourEditProperties())
//     {
//         handleStatePropertyChange(newState, property);
//     }

//     return newState;
// }

// void LumatoneEditorBatchColourState::handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier &property)
// {
//     if (stateIn != state)
//         return;

//     bool setProperty = stateIn.hasProperty(property);
//     juce::var value = stateIn.getProperty(property);

//     if (property == LumatoneEditorBatchColourProperty::BrightnessMultiplier)
//     {
//         if (setProperty)
//         {
//             data.useBrightness = true;
//             data.brightnessMultiplier = (float)value;
//         }
//         else
//         {
//             data.useBrightness = false;
//         }
//     }
//     else if (property == LumatoneEditorBatchColourProperty::HueShiftAmount)
//     {
//         if (setProperty)
//         {
//             data.useHueShift = true;
//             data.hueShiftAmount = (float)value;
//         }
//         else
//         {
//             data.useHueShift = false;
//         }
//     }
//     else if (property == LumatoneEditorBatchColourProperty::TemperatureShiftValue)
//     {
//         if (setProperty)
//         {
//             data.useTemperatureShift = true;
//             data.temperatureShiftValue = (float)value;
//         }
//         else
//         {
//             data.useTemperatureShift = false;
//         }
//     }
// }
