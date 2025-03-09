/*
  ==============================================================================

    SelectionTabBar.h
    Created: 24 Jan 2025 12:38:17pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum class SelectionTabs
{
    Single = 0,
    Multi
};

class SelectionTabBar : public juce::TabbedButtonBar
{

public:

    SelectionTabBar();
    ~SelectionTabBar() override;

};
