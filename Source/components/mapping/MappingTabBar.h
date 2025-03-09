/*
  ==============================================================================

    MappingTabBar.h
    Created: 24 Jan 2025 12:38:17pm
    Author:  Vincenzo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum class MappingEditorTabs
{
    KeyEditor       = 0,
    AutoGenerator,
    Advanced,
    MappingSettings
};

class MappingTabBar : public juce::TabbedButtonBar
{

public:

    MappingTabBar();
    ~MappingTabBar() override;

};
