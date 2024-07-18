/*
  ==============================================================================

    BatchColourActions.h
    Created: 16 June 2024
    Author:  Vito

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_BATCH_COLOUR_ACTIONS_H
#define LUMATONE_EDITOR_BATCH_COLOUR_ACTIONS_H

#include "../LumatoneEditorState.h"
#include "../lumatone_editor_library/actions/lumatone_action.h"
#include "../data/LumatoneEditorBatchColourState.h"

// Records action of setting control values in the editor (before assign action)
class SetBatchColourSettingsAction : public LumatoneEditorState
                                   , private LumatoneEditorState::Controller
                                   , public LumatoneAction
{
public:

    SetBatchColourSettingsAction(const LumatoneEditorState& stateIn
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

    static SetBatchColourSettingsAction* NewSetBrightnessValue(const LumatoneEditorState& stateIn, float value);
    static SetBatchColourSettingsAction* NewSetHueValue(const LumatoneEditorState& stateIn, float value);
    static SetBatchColourSettingsAction* NewSetTemperatureValue(const LumatoneEditorState& stateIn, float value);

private:

    LumatoneEditor::BatchColourEditData previousData;
    LumatoneEditor::BatchColourEditData newEditData;
};

class ApplyBatchColourAdjustments : public LumatoneEditorState
                                  , private LumatoneEditorState::Controller
                                  , public LumatoneAction
{
public:

    ApplyBatchColourAdjustments(const LumatoneEditorState& stateIn
                                    , std::shared_ptr<LumatoneLayout>& baseLayoutIn
                                    , LumatoneEditor::BatchColourEditData editData
                                    , const juce::Array<MappedLumatoneKey>& keySelectionIn);

    ~ApplyBatchColourAdjustments() override;

    bool perform() override;
    bool undo() override;

    int getSizeInUnits() override { return sizeof(ApplyBatchColourAdjustments); }

public:
    static void applyColourAdjustmentToKey(LumatoneKey& key, const LumatoneEditor::BatchColourEditData& editData);

private:
    std::shared_ptr<LumatoneLayout> baseLayout;
    LumatoneLayout updatedLayout;

    LumatoneEditor::BatchColourEditData previousData;
    LumatoneEditor::BatchColourEditData newData;

    juce::Array<MappedLumatoneKey> keySelection;
    bool fullLayout = false;
};

#endif // LUMATONE_EDITOR_BATCH_COLOUR_ACTIONS_H
