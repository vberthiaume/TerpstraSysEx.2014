#include "colour_picker_panel.h"

void CustomPickerPanel::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    selectorListeners.call(&ColourSelectionListener::colourChangedCallback, this, colourPicker->getCurrentColour());
}

void CustomPickerPanel::mouseDown(const juce::MouseEvent &e)
{
    if (!mouseIsDown)
        mouseIsDown = true;
    else
        mouseIsDragging = true;
}

void CustomPickerPanel::mouseUp(const juce::MouseEvent &e)
{
    if (mouseIsDown)
        mouseIsDown = false;

    if (mouseIsDragging)
        mouseIsDragging = false;
}

void CustomPickerPanel::mouseDrag(const juce::MouseEvent &e)
{
    if (!mouseIsDown)
        mouseIsDown = true;
    if (!mouseIsDragging)
        mouseIsDragging = true;
}

void CustomPickerPanel::mouseExit(const juce::MouseEvent &e)
{
    // ?
}
