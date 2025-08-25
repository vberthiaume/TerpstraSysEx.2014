/*
  ==============================================================================

    BatchColourActions.h
    Created: 16 June 2024
    Author:  Vito

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_BATCH_COLOUR_ACTIONS_H
#define LUMATONE_EDITOR_BATCH_COLOUR_ACTIONS_H

// #include "./application_state.h"

#include "./lumatone_layout.h"
#include "./lumatone_action.h"
#include "./LumatoneEditorBatchColourState.h"

class LumatoneController;

// Records action of setting control values in the editor (before assign action)
class SetBatchColourSettingsAction : //public LumatoneApplicationState
                                //    , private LumatoneApplicationState::Controller
                                    public LumatoneAction
{
public:

    SetBatchColourSettingsAction(LumatoneController* stateIn
                               , bool setBrightnessIn
                               , bool setHueIn
                               , bool setTemperatureIn
                               , float brightnessIn = 0.5f
                               , float hueIn = 0.5f
                               , float tempIn = 0.5f);

    bool perform() override;
    bool undo() override;

    int getSizeInUnits() override { return sizeof(SetBatchColourSettingsAction); }

public:

    static SetBatchColourSettingsAction* NewSetBrightnessValue  (LumatoneController* stateIn, float value);
    static SetBatchColourSettingsAction* NewSetBrightnessValue  (LumatoneController* stateIn, double value);
    static SetBatchColourSettingsAction* NewSetHueValue         (LumatoneController* stateIn, float value);
    static SetBatchColourSettingsAction* NewSetHueValue         (LumatoneController* stateIn, double value);
    static SetBatchColourSettingsAction* NewSetTemperatureValue (LumatoneController* stateIn, float value);
    static SetBatchColourSettingsAction* NewSetTemperatureValue (LumatoneController* stateIn, double value);

private:

    BatchColourEditData previousData;
    BatchColourEditData newEditData;
};

class ApplyBatchColourAdjustments : //public LumatoneApplicationState
                                //   , private LumatoneApplicationState::Controller
                                   public LumatoneAction
{
public:

    ApplyBatchColourAdjustments(LumatoneController* stateIn
                                , BatchColourEditData oldData
                                );
                                // , const juce::Array<MappedLumatoneKey>& keySelectionIn);

    ~ApplyBatchColourAdjustments() override;

    bool perform() override;
    bool undo() override;

    int getSizeInUnits() override { return sizeof(ApplyBatchColourAdjustments); }

public:
    static void applyColourAdjustmentToKey(LumatoneKey& key, const BatchColourEditData& editData);

private:
    // const TerpstraKeyMapping& mapping;
    // TerpstraKeyMapping updatedLayout;
    LumatoneLayout baseLayout;
    LumatoneLayout updatedLayout;

    BatchColourEditData previousData;
    BatchColourEditData newData;

    // juce::Array<MappedLumatoneKey> keySelection;
    bool fullLayout = false;
};

#endif // LUMATONE_EDITOR_BATCH_COLOUR_ACTIONS_H
