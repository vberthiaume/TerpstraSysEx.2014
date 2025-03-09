/*
  ==============================================================================

    SelectionTabBar.cpp
    Created: 24 Jan 2025 12:38:17pm
    Author:  Vincenzo

  ==============================================================================
*/

#include "SelectionTabBar.h"

SelectionTabBar::SelectionTabBar()
    :
    juce::TabbedButtonBar(juce::TabbedButtonBar::Orientation::TabsAtTop)
{
    setName("SelectionTabBar");

    addTab("Single", juce::Colour(), (int)SelectionTabs::Single);
    addTab("Multi", juce::Colour(), (int)SelectionTabs::Multi);

    setCurrentTabIndex(0, false);
}

SelectionTabBar::~SelectionTabBar()
{
    removeAllChangeListeners();
}
