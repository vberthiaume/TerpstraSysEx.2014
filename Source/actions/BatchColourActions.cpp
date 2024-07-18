#include "BatchColourActions.h"
#include "../lumatone_editor_library/color/adjust_layout_colour.h"

SetBatchColourSettingsAction::SetBatchColourSettingsAction(const LumatoneEditorState &stateIn, bool setBrightnessIn, bool setHueIn, bool setTemperatureIn, float brightnessIn, float hueIn, float tempIn)
    : LumatoneEditorState("SetBatchColourSettingsAction", stateIn)
    , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
    , LumatoneAction(this, "SetBatchColourSettingsAction")
{
    previousData = getBatchColourEditData();

    newEditData.useBrightness = setBrightnessIn;
    newEditData.useHueShift = setHueIn;
    newEditData.useTemperatureShift = setTemperatureIn;

    newEditData.brightnessMultiplier = brightnessIn;
    newEditData.hueShiftAmount = hueIn;
    newEditData.temperatureShiftValue = tempIn;
}

bool SetBatchColourSettingsAction::perform()
{
    // apply new data

    if (newEditData.useHueShift)
    {
        setBatchColourHueShift(newEditData.hueShiftAmount);
    }

    if (newEditData.useTemperatureShift)
    {
        setBatchColourTempShift(newEditData.temperatureShiftValue);
    }

    if (newEditData.useBrightness)
    {
        setBatchColourBrightness(newEditData.brightnessMultiplier);
    }

    return true;
}

bool SetBatchColourSettingsAction::undo()
{
    // apply old data

    if (newEditData.useHueShift)
    {
        setBatchColourHueShift(previousData.hueShiftAmount);
    }

    if (newEditData.useTemperatureShift)
    {
        setBatchColourTempShift(previousData.temperatureShiftValue);
    }

    if (newEditData.useBrightness)
    {
        setBatchColourBrightness(previousData.brightnessMultiplier);
    }

    return true;
}

SetBatchColourSettingsAction *SetBatchColourSettingsAction::NewSetBrightnessValue(const LumatoneEditorState &stateIn, float value)
{
    return new SetBatchColourSettingsAction(stateIn, true, false, false, value);
}

SetBatchColourSettingsAction *SetBatchColourSettingsAction::NewSetHueValue(const LumatoneEditorState &stateIn, float value)
{
    return new SetBatchColourSettingsAction(stateIn, false, true, false, 0, value);
}

SetBatchColourSettingsAction *SetBatchColourSettingsAction::NewSetTemperatureValue(const LumatoneEditorState &stateIn, float value)
{
    return new SetBatchColourSettingsAction(stateIn, false, false, true, 0, 0, value);
}

ApplyBatchColourAdjustments::ApplyBatchColourAdjustments(const LumatoneEditorState &stateIn, std::shared_ptr<LumatoneLayout>& baseLayoutIn, LumatoneEditor::BatchColourEditData editData, const juce::Array<MappedLumatoneKey> &keySelectionIn)
    : LumatoneEditorState("ApplyBatchColourAdjustments", stateIn)
    , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
    , LumatoneAction(this, "ApplyBatchColourAdjustments")
    , baseLayout(baseLayoutIn)
{
    newData = editData;
    previousData = getBatchColourEditData();

    if (keySelectionIn.size() > 0 && keySelection.size() < (MAXNUMBOARDS * MAXBOARDSIZE)) // todo get actual number from each board
    {
        for (const MappedLumatoneKey& key : keySelectionIn)
        {
            MappedLumatoneKey baseKey = baseLayout->getMappedKey(key.boardIndex, key.keyIndex);
            keySelection.add(baseKey);
        }
    }
    else
    {
        keySelection.addArray(baseLayout->getAllKeysMapped());
        fullLayout = true;
    }
}

bool ApplyBatchColourAdjustments::perform()
{
    if (fullLayout)
    {
        for (const MappedLumatoneKey& key : keySelection)
        {
            MappedLumatoneKey updatedKey = key;
            applyColourAdjustmentToKey(updatedKey, newData);
            updatedLayout.setKey(updatedKey, key.boardIndex, key.keyIndex);
        }

        setLayout(updatedLayout);
    }
    else
    {
        juce::Array<MappedLumatoneKey> updatedKeys;
        for (const MappedLumatoneKey& key : keySelection)
        {
            MappedLumatoneKey updatedKey = key;
            applyColourAdjustmentToKey(updatedKey, newData);
            updatedKeys.add(updatedKey);
        }

        sendSelectionColours(updatedKeys);
    }

    return true;
}

bool ApplyBatchColourAdjustments::undo()
{
    if (fullLayout)
    {
        for (const MappedLumatoneKey& key : keySelection)
        {
            MappedLumatoneKey baseKey = baseLayout->getMappedKey(key.boardIndex, key.keyIndex);
            applyColourAdjustmentToKey(baseKey, previousData);
            updatedLayout.setKey(baseKey);
        }

        setLayout(updatedLayout);
    }
    else
    {
        for (MappedLumatoneKey& key : keySelection)
        {
            juce::Colour baseColour = baseLayout->getKey(key.boardIndex, key.keyIndex)
                                                 .getColour();
            key.setColour(baseColour);
            applyColourAdjustmentToKey(key, previousData);
        }
        sendSelectionParam(keySelection, true);
    }

    return true;
}

void ApplyBatchColourAdjustments::applyColourAdjustmentToKey(LumatoneKey &key, const LumatoneEditor::BatchColourEditData& editData)
{
    if (editData.useHueShift)
    {
        float hueValue = LumatoneEditor::BatchColourEditData::HueShiftMapFromNorm(editData.hueShiftAmount);
        AdjustLayoutColour::rotateHue(hueValue, key);
    }

    if (editData.useTemperatureShift)
    {
        int tempValue = LumatoneEditor::BatchColourEditData::TempShiftMapFromNorm(editData.temperatureShiftValue);
        AdjustLayoutColour::adjustWhiteBalance(tempValue, key);
    }

    if (editData.useBrightness)
    {
        float brightnessValue = LumatoneEditor::BatchColourEditData::BrightnessMapFromNorm(editData.brightnessMultiplier);
        AdjustLayoutColour::multiplyBrightness(brightnessValue, key);
    }
}
