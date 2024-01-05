/*
  ==============================================================================

    MainWindow.cpp
    Created: 2 Apr 2022 3:24:02pm

  ==============================================================================
*/

#include "MainWindow.h"
#include "MainComponent.h"

#include "LumatoneEditorLookAndFeel.h"
#include "./lumatone_editor_library/palettes/colour_palette_file.h"

#include "./lumatone_editor_library/graphics/view_constants.h"

MainWindow::MainWindow(const LumatoneEditorState& stateIn, juce::ApplicationCommandManager* cmdMgr)
    : juce::DocumentWindow("Lumatone Editor", juce::Colours::black, juce::DocumentWindow::minimiseButton + juce::DocumentWindow::closeButton)
    , LumatoneEditorState("MainWindow", stateIn)
    , commandManager(cmdMgr)
{
    // setContentOwned(new MainContentComponent(*TerpstraSysExApplication::getApp().getMappingData()), true);
    setContentOwned(new MainContentComponent(stateIn), true);
    setResizable(true, true);
#if JUCE_ANDROID
    setFullScreen(true);
#else

    setLookAndFeel(&getEditorLookAndFeel());

    menuModel = std::make_unique<Lumatone::Menu::MainMenuModel>(*this, commandManager);

#if JUCE_MAC
    MenuBarModel::setMacMainMenu(menuModel.get());
#else
    setMenuBar(menuModel.get());
    getMenuBarComponent()->getProperties().set(
        LumatoneEditorStyleIDs::popupMenuBackgroundColour,
        getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::MenuBarBackground).toString()
    );
#endif

    // Window aspect ratio
    constrainer.reset(new juce::ComponentBoundsConstrainer());
    
    constrainer->setFixedAspectRatio(DEFAULTMAINWINDOWASPECT);
    constrainer->setMinimumSize(800, juce::roundToInt(800 / DEFAULTMAINWINDOWASPECT));
    constrainer->setMaximumHeight(1000);
    setConstrainer(constrainer.get());

    addKeyListener(commandManager->getKeyMappings());
    updateBounds();

    startTimer(2000);
#endif

    // setLookAndFeel(stateIn.getEditorLookAndFeel());
    // setLookAndFeel(&TerpstraSysExApplication::getApp().getLookAndFeel());
}

MainWindow::~MainWindow()
{
    removeKeyListener(commandManager->getKeyMappings());

#if JUCE_MAC
    MenuBarModel::setMacMainMenu(nullptr);
#else
    setMenuBarComponent(nullptr);
#endif

    menuModel = nullptr;

    stopTimer();
    setConstrainer(nullptr);
}

void MainWindow::closeButtonPressed()
{
    // This is called when the user tries to close this window. Here, we'll just
    // ask the app to quit when this happens, but you can change this to do
    // whatever you need.
    JUCEApplication::getInstance()->systemRequestedQuit();
}

BorderSize<int> MainWindow::getBorderThickness()
{
    return BorderSize <int>(1);
}

bool MainWindow::isLargerThanCurrentScreen() const
{
    if (maxWindowHeight > 0)
        return getHeight() > maxWindowHeight;

    return false; // shouldn't happen
}

bool MainWindow::isOutOfVerticalBounds() const
{
    return getScreenY() < 0
        || getScreenY() >= (maxWindowHeight - verticalBoundsThreshold);
}

bool MainWindow::isOutOfHorizontalBounds() const
{
    return getScreenBounds().getRight() < horizontalBoundsThreshold
        || getScreenX() >= (maxWindowWidth - horizontalBoundsThreshold);
}

void MainWindow::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
    // Save state of main window
    propertiesFile->setValue(LumatoneEditorProperty::MainWindowState, getWindowStateAsString());
    ((MainContentComponent*)(getContentComponent()))->saveStateToPropertiesFile(propertiesFile);
}

void MainWindow::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
    bool useSavedState = restoreWindowStateFromString(getProperty(LumatoneEditorProperty::MainWindowState));

    fixWindowPositionAndSize(!useSavedState);

    setVisible(true);

    ((MainContentComponent*)(getContentComponent()))->restoreStateFromPropertiesFile(propertiesFile);
}

void MainWindow::updateBounds()
{
    auto thisDisplay = Desktop::getInstance().getDisplays().getDisplayForRect(getScreenBounds());

    if (thisDisplay != nullptr)
    {
        int screenWidth = thisDisplay->userArea.getWidth();
        int screenHeight = thisDisplay->userArea.getHeight();
        if (screenWidth != maxWindowWidth || screenHeight != maxWindowHeight)
        {
            maxWindowWidth = screenWidth;
            maxWindowHeight = screenHeight;
            constrainer->setMaximumHeight(maxWindowHeight);

            fixWindowPositionAndSize();
            return;
        }
    }

    if (isOutOfVerticalBounds())
        fixWindowPositionAndSize();
}

void MainWindow::fixWindowPositionAndSize(bool setToDefault)
{
    if (setToDefault || isLargerThanCurrentScreen())
    {
        // Default window state
        setSize(DEFAULTMAINWINDOWWIDTH, DEFAULTMAINWINDOWHEIGHT);
        return;
    }

    if (isOutOfVerticalBounds())
    {
        int correctedY = (getScreenY() < 0) ? 0
                                            : maxWindowHeight - verticalBoundsThreshold;
        setTopLeftPosition(getScreenX(), correctedY);
    }

    if (isOutOfHorizontalBounds())
    {
        auto bounds = getScreenBounds();
        int correctedX = (bounds.getX() < 0) ? horizontalBoundsThreshold - maxWindowWidth
                                             : maxWindowWidth - horizontalBoundsThreshold;
        setTopLeftPosition(correctedX, getScreenY());
    }
}

void MainWindow::timerCallback()
{
    // Set threshold to be a quarter of the window handle height
    verticalBoundsThreshold = round(getTitleBarHeight() * 0.25f);

    updateBounds();
}
