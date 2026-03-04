#include "BatchColourActions.h"
#include "../LumatoneController.h"
#include "./adjust_layout_colour.h"

SetBatchColourSettingsAction::SetBatchColourSettingsAction(LumatoneController* stateIn, bool setBrightnessIn, bool setHueIn, bool setTemperatureIn, float brightnessIn, float hueIn, float tempIn)
    // : LumatoneApplicationState("SetBatchColourSettingsAction", stateIn)
    // , LumatoneApplicationState::Controller(static_cast<LumatoneApplicationState&>(*this))
    : LumatoneAction(stateIn, "SetBatchColourSettingsAction")
{
    previousData = state->getBatchColourEditState().getData();

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
        state->setBatchColourHueShift(newEditData.hueShiftAmount);
    }

    if (newEditData.useTemperatureShift)
    {
        state->setBatchColourTempShift(newEditData.temperatureShiftValue);
    }

    if (newEditData.useBrightness)
    {
        state->setBatchColourBrightness(newEditData.brightnessMultiplier);
    }

    return true;
}

bool SetBatchColourSettingsAction::undo()
{
    // Don't re-enter batch edit mode on undo after apply/reset.
    // The layout is restored by the paired ApplyBatchColourAdjustments undo.
    if (!state->getBatchColourEditState().isInEdit())
        return true;

    if (newEditData.useHueShift)
    {
        state->setBatchColourHueShift(previousData.hueShiftAmount);
    }

    if (newEditData.useTemperatureShift)
    {
        state->setBatchColourTempShift(previousData.temperatureShiftValue);
    }

    if (newEditData.useBrightness)
    {
        state->setBatchColourBrightness(previousData.brightnessMultiplier);
    }

    return true;
}

SetBatchColourSettingsAction *SetBatchColourSettingsAction::NewSetBrightnessValue(LumatoneController* stateIn, float value)
{
    return new SetBatchColourSettingsAction(stateIn, true, false, false, value);
}

SetBatchColourSettingsAction *SetBatchColourSettingsAction::NewSetBrightnessValue(LumatoneController* stateIn, double value)
{
    return new SetBatchColourSettingsAction(stateIn, true, false, false, (float)value);
}

SetBatchColourSettingsAction *SetBatchColourSettingsAction::NewSetHueValue(LumatoneController* stateIn, float value)
{
    return new SetBatchColourSettingsAction(stateIn, false, true, false, 0, value);
}

SetBatchColourSettingsAction *SetBatchColourSettingsAction::NewSetHueValue(LumatoneController* stateIn, double value)
{
    return new SetBatchColourSettingsAction(stateIn, false, true, false, 0, (float)value);
}

SetBatchColourSettingsAction *SetBatchColourSettingsAction::NewSetTemperatureValue(LumatoneController* stateIn, float value)
{
    return new SetBatchColourSettingsAction(stateIn, false, false, true, 0, 0, value);
}

SetBatchColourSettingsAction *SetBatchColourSettingsAction::NewSetTemperatureValue(LumatoneController* stateIn, double value)
{
    return new SetBatchColourSettingsAction(stateIn, false, false, true, 0, 0, (float)value);
}

ApplyBatchColourAdjustments::ApplyBatchColourAdjustments(LumatoneController* stateIn, BatchColourEditData oldData)//, const juce::Array<MappedLumatoneKey> &keySelectionIn)
    // : LumatoneApplicationState("ApplyBatchColourAdjustments", stateIn)
    // , LumatoneApplicationState::Controller(static_cast<LumatoneApplicationState&>(*this))
    : LumatoneAction(stateIn, "ApplyBatchColourAdjustments")
    // , mapping(stateIn->getBatchColourEditState().getMappingBeforeEdits())
    , newData(stateIn->getBatchColourEditState().getData())
    , previousData(oldData)
{
    // previousData = state->getBatchColourEditState().getData();

    // if (keySelectionIn.size() > 0 && keySelection.size() < (MAX_LUMATONE_BOARDS * MAX_LUMATONE_BOARD_KEYS)) // todo get actual number from each board
    // {
    //     for (const MappedLumatoneKey& key : keySelectionIn)
    //     {
    //         MappedLumatoneKey baseKey = baseLayout->getMappedKey(key.boardIndex, key.keyIndex);
    //         keySelection.add(baseKey);
    //     }
    // }
    // else
    // {
    //     keySelection.addArray(baseLayout->getAllKeysMapped());
        fullLayout = true;
        baseLayout = stateIn->getBatchColourEditState().getMappingBeforeEdits().getLumatoneLayout();
    // }
}

ApplyBatchColourAdjustments::~ApplyBatchColourAdjustments()
{

}

bool ApplyBatchColourAdjustments::perform()
{
    // if (fullLayout)
    // {

    for (int b = 0; b < state->getNumBoards(); b++)
    {
        auto board = &baseLayout.getBoard(b);
        for (int k = 0; k < state->getOctaveSize(); k++)
        {
            LumatoneKey updatedKey = board->getKey(k);
            applyColourAdjustmentToKey(updatedKey, newData);
            updatedLayout.setKey(updatedKey, b, k);
        }
    }

    state->setLayout(updatedLayout, false);
    // }
    // else
    // {
        // juce::Array<MappedLumatoneKey> updatedKeys;
        // for (const MappedLumatoneKey& key : keySelection)
        // {
        //     MappedLumatoneKey updatedKey = key;
        //     applyColourAdjustmentToKey(updatedKey, newData);
        //     updatedKeys.add(updatedKey);
        // }

        // bool useBuffer = keySelection.size() > 48;
        // sendSelectionColours(updatedKeys, true, useBuffer);
    // }

    return true;
}

bool ApplyBatchColourAdjustments::undo()
{
    // if (fullLayout)
    // {
    for (int b = 0; b < state->getNumBoards(); b++)
    {
        auto board = &baseLayout.getBoard(b);
        for (int k = 0; k < state->getOctaveSize(); k++)
        {
            LumatoneKey updatedKey = board->getKey(k);
            applyColourAdjustmentToKey(updatedKey, previousData);
            updatedLayout.setKey(updatedKey, b, k);
        }
    }

    state->setLayout(updatedLayout, false);
    // }
    // else
    // {
        // for (MappedLumatoneKey& key : keySelection)
        // {
        //     juce::Colour baseColour = baseLayout->getKey(key.boardIndex, key.keyIndex)
        //                                          .getColour();
        //     key.setColour(baseColour);
        //     // applyColourAdjustmentToKey(key, previousData);
        // }

        // bool useBuffer = keySelection.size() > 48;
        // sendSelectionColours(keySelection, true, useBuffer);
    // }

    return true;
}

void ApplyBatchColourAdjustments::applyColourAdjustmentToKey(LumatoneKey &key, const BatchColourEditData& editData)
{
    if (editData.useHueShift)
    {
        float hueValue = BatchColourEditData::HueShiftMapFromNorm(editData.hueShiftAmount);
        AdjustLayoutColour::rotateHue(hueValue, key);
    }

    if (editData.useTemperatureShift)
    {
        int tempValue = BatchColourEditData::TempShiftMapFromNorm(editData.temperatureShiftValue);
        AdjustLayoutColour::adjustWhiteBalance(tempValue, key);
    }

    if (editData.useBrightness)
    {
        float brightnessValue = BatchColourEditData::BrightnessMapFromNorm(editData.brightnessMultiplier);
        AdjustLayoutColour::multiplyBrightness(brightnessValue, key);
    }
}
