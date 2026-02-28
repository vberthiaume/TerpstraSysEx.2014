/*
  ==============================================================================

    ColourReplacePanel.h
    Created: 2024
    Author:  Vincenzo

    Shows all unique colours currently used in the mapping as editable swatches.
    Clicking a swatch opens a colour picker; when the picker closes the colour
    is replaced across the entire mapping via an undoable action.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LumatoneEditorStyleCommon.h"
#include "EditActions.h"

//==============================================================================
/*
    A square swatch button representing one colour in the mapping.

    - Displays originalColour (the colour it stands for in the mapping).
    - Clicking opens a ColourSelector CallOutBox for live preview.
    - When the CallOutBox closes, onColourChanged(original, picked) fires once
      via callAsync so it is safe to rebuild the swatch list inside the callback.
*/
class ColourSwatchButton : public juce::TextButton,
                           private juce::ChangeListener,
                           private juce::ComponentListener
{
public:

    // Called once when the picker is dismissed, only if the colour changed.
    // Signature: (originalColour, newColour)
    std::function<void(juce::Colour, juce::Colour)> onColourChanged;

    explicit ColourSwatchButton(juce::Colour colour)
        : juce::TextButton(colour.toDisplayString(false))
        , originalColour(colour.withAlpha((uint8) 255))
        , displayColour(colour.withAlpha((uint8) 255))
    {
        setTooltip(colour.toDisplayString(false));
        onClick = [this]() { showPicker(); };
    }

    ~ColourSwatchButton() override
    {
        if (activePicker != nullptr)
        {
            activePicker->removeChangeListener(this);
            activePicker->removeComponentListener(this);
        }
    }

    void paintButton(juce::Graphics& g, bool highlighted, bool down) override
    {
        auto bounds = getLocalBounds().toFloat().reduced(1.0f);
        const float corner = bounds.getHeight() * 0.15f;

        g.setColour(displayColour);
        g.fillRoundedRectangle(bounds, corner);

        if (highlighted || down)
        {
            g.setColour(juce::Colours::white.withAlpha(down ? 0.2f : 0.1f));
            g.fillRoundedRectangle(bounds, corner);
        }

        g.setColour(findColour(LumatoneEditorColourIDs::OutlineColourId));
        g.drawRoundedRectangle(bounds, corner, 1.0f);
    }

private:

    void showPicker()
    {
        auto picker = std::make_unique<juce::ColourSelector>(
            juce::ColourSelector::showColourspace
            | juce::ColourSelector::editableColour
            | juce::ColourSelector::showColourAtTop
        );
        picker->setCurrentColour(displayColour);
        picker->setSize(280, 300);
        picker->addChangeListener(this);
        picker->addComponentListener(this);
        activePicker = picker.get();
        juce::CallOutBox::launchAsynchronously(std::move(picker), getScreenBounds(), nullptr);
    }

    // Live preview: update display as the user drags in the picker.
    void changeListenerCallback(juce::ChangeBroadcaster* source) override
    {
        if (source == activePicker)
        {
            displayColour = activePicker->getCurrentColour().withAlpha((uint8) 255);
            repaint();
        }
    }

    // Fired once when the ColourSelector (and its CallOutBox) is destroyed.
    void componentBeingDeleted(juce::Component& comp) override
    {
        if (&comp == activePicker)
        {
            const juce::Colour picked = displayColour;
            activePicker = nullptr;

            if (picked != originalColour && onColourChanged)
            {
                // Defer so the CallOutBox tear-down finishes before we
                // rebuild the swatch list (which would destroy this button).
                auto cb   = onColourChanged;
                auto orig = originalColour;
                juce::MessageManager::callAsync([cb, orig, picked]()
                {
                    cb(orig, picked);
                });
            }
        }
    }

    const juce::Colour originalColour;
    juce::Colour       displayColour;
    juce::ColourSelector* activePicker = nullptr;
};

//==============================================================================
/*
    Colour map panel. Displays all unique colours in the current mapping as
    a scrollable grid of ColourSwatchButtons. Editing a swatch replaces that
    colour everywhere in the mapping.
*/
class ColourReplacePanel : public juce::Component
{
public:

    ColourReplacePanel();
    ~ColourReplacePanel() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // Rebuild the swatch grid from the current mapping data.
    // Call whenever the mapping changes externally.
    void refreshMatchCount();

private:

    void rebuildSwatches(const juce::Array<juce::Colour>& uniqueColours);
    void layoutSwatches();
    void onSwatchColourChanged(juce::Colour original, juce::Colour newColour);

private:

    std::unique_ptr<juce::Viewport>   viewport;
    std::unique_ptr<juce::Component>  swatchContainer;
    juce::OwnedArray<ColourSwatchButton> swatches;

    std::unique_ptr<juce::Label> emptyLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColourReplacePanel)
};
