/*
  ==============================================================================

    LumatoneEditorBatchColourState.h
    Created: 16 July 2024
    Author:  Vito

    Holds the colour adjustment values that are controlled by the Batch Tools Colour section

  ==============================================================================
*/


#ifndef LUMATONE_EDITOR_BATCH_COLOUR_STATE_H
#define LUMATONE_EDITOR_BATCH_COLOUR_STATE_H

#include "../lumatone_editor_library/data/state_base.h"

namespace LumatoneEditor
{
struct BatchColourEditData
{
    bool useBrightness = false;
    bool useHueShift = false;
    bool useTemperatureShift = false;

    float brightnessMultiplier = 0.5f;
    float hueShiftAmount = 0.5f;
    float temperatureShiftValue = 0.5f;

    static float BrightnessMapToNorm(float multiplier)
    {
        return sqrtf(multiplier * 0.25f);
    }

    static float BrightnessMapFromNorm(float value)
    {
        return 4 * value * value;
    }

    static float HueShiftMapToNorm(float shiftAmt)
    {
        return (shiftAmt + 1.0f) * 0.5f;
    }

    static float HueShiftMapFromNorm(float value)
    {
        return value * 2 - 1.0f;
    }

    // piecewise
    // f(x) [0, 0.5] -> [1000, 6500]: 11000x + 1000
    // f(x) [0.5, 1.0] -> [6500, 40000]: 52000x^2 + 41000x

    static float TempShiftMapToNorm(int tempShift)
    {
        if (tempShift <= 6500)
        {
            return (float)(tempShift - 1000) / 11000.0f;
        }

        return sqrtf((tempShift + 1581.731f) / 52000.0f) + 0.105769f;
    }

    static int TempShiftMapFromNorm(double value)
    {
        // Linear
        if (value <= 0.5)
        {
            return juce::roundToInt(11000.0 * value + 1000.0);
        }

        return juce::roundToInt(52000.0 * (value - 0.5) * (value - 0.5) + 41000.0 * (value - 0.5) + 6500.0);
    }
};

}

class LumatoneEditorBatchColourState : public LumatoneStateBase
{
public:
    LumatoneEditorBatchColourState(juce::String nameIn, juce::ValueTree parentStateIn);
    LumatoneEditorBatchColourState(juce::String nameIn,  juce::ValueTree parentStateIn, const LumatoneEditorBatchColourState& stateToCopy);

    virtual ~LumatoneEditorBatchColourState() override { };

    LumatoneEditor::BatchColourEditData getData() const { return data; }

    void setBrightnessMultiplier(bool set, float value = 0.5f);
    void setHueShiftAmount(bool set, float value = 0.5f);
    void setTempShiftAmount(bool set, float value = 0.5f);

protected:

    juce::ValueTree loadStateProperties(juce::ValueTree stateIn) override;
    void handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier& property) override;

    void loadPropertiesFile(juce::PropertiesFile*) override { };

private:

    LumatoneEditor::BatchColourEditData data;

};

namespace LumatoneEditorBatchColourProperty
{
    static const juce::Identifier State                   = juce::Identifier("LumatoneEditorBatchColour");
    static const juce::Identifier UseBrightness           = juce::Identifier("UseBrightness");
    static const juce::Identifier UseHueShift             = juce::Identifier("UseHueShift");
    static const juce::Identifier UseTempShift            = juce::Identifier("UseTempShift");
    static const juce::Identifier BrightnessMultiplier    = juce::Identifier("BrightnessMultiplier");
    static const juce::Identifier HueShiftAmount          = juce::Identifier("HueShiftAmount");
    static const juce::Identifier TemperatureShiftValue   = juce::Identifier("TemperatureShiftValue");
}


#endif //LUMATONE_EDITOR_BATCH_COLOUR_STATE_H
