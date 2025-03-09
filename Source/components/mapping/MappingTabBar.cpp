/*
  ==============================================================================

    MappingTabBar.cpp
    Created: 24 Jan 2025 12:38:17pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "MappingTabBar.h"

MappingTabBar::MappingTabBar()
    :
    juce::TabbedButtonBar(juce::TabbedButtonBar::Orientation::TabsAtTop)
{
    setName("MappingTabBar");

    addTab("Key Editor", juce::Colour(), (int)MappingEditorTabs::KeyEditor);
    addTab("AutoGenerator", juce::Colour(), (int)MappingEditorTabs::AutoGenerator);
    addTab("Advanced", juce::Colour(), (int)MappingEditorTabs::Advanced);
    addTab("Mapping Settings", juce::Colour(), (int)MappingEditorTabs::MappingSettings);

    setCurrentTabIndex(0, false);
}

MappingTabBar::~MappingTabBar()
{
    removeAllChangeListeners();
}
