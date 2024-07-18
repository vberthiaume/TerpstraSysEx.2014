/*
  ==============================================================================

    MainWindow.cpp
    Created: 2 Apr 2022 3:24:02pm

  ==============================================================================
*/

#include "MainWindow.h"
#include "MainComponent.h"

#include "LumatoneMenu.h"

#include "LumatoneEditorLookAndFeel.h"
#include "./lumatone_editor_library/palettes/colour_palette_file.h"

#include "./lumatone_editor_library/graphics/view_constants.h"

MainWindow::MainWindow(const LumatoneEditorState& stateIn, juce::ApplicationCommandManager* cmdManager)
    : juce::DocumentWindow("Lumatone Editor", juce::Colours::black, juce::DocumentWindow::minimiseButton + juce::DocumentWindow::closeButton)
    , LumatoneEditorState("MainWindow", stateIn)
    , LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
    , commandManager(cmdManager)
{
    // Window aspect ratio
    setResizeLimits(800, juce::roundToInt(800 / DEFAULTMAINWINDOWASPECT), juce::roundToInt(1000 * DEFAULTMAINWINDOWASPECT), 1000);
    getConstrainer()->setFixedAspectRatio(DEFAULTMAINWINDOWASPECT);
    getConstrainer()->setMinimumOnscreenAmounts(0xffffff, 0xffffff, 0xffffff, 0xffffff);

    restoreStateFromPropertiesFile(getPropertiesFile());

    auto mainComponent = new MainContentComponent(*this, commandManager);
    mainComponent->setSize(getWidth(), getHeight());
    setContentOwned(mainComponent, true);

#if JUCE_ANDROID
    setFullScreen(true);
#else
    setResizable(true, true);
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

    addKeyListener(commandManager->getKeyMappings());

    addEditorListener(this);

#endif
}

MainWindow::~MainWindow()
{
    removeEditorListener(this);
    removeKeyListener(commandManager->getKeyMappings());

#if JUCE_MAC
    MenuBarModel::setMacMainMenu(nullptr);
#else
    setMenuBarComponent(nullptr);
#endif

    menuModel = nullptr;
}

void MainWindow::closeButtonPressed()
{
    // This is called when the user tries to close this window. Here, we'll just
    // ask the app to quit when this happens, but you can change this to do
    // whatever you need.
    JUCEApplication::getInstance()->systemRequestedQuit();
}

//BorderSize<int> MainWindow::getBorderThickness() const
//{
//    return BorderSize <int>(1);
//}

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

void MainWindow::saveBounds()
{
    setWindowState(getBounds(), getWindowStateAsString());
}

void MainWindow::resized()
{
    juce::DocumentWindow::resized();
    saveBounds();
}

void MainWindow::moved()
{
    juce::DocumentWindow::moved();
    saveBounds();
}

void MainWindow::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
    // Save state of main window
    propertiesFile->setValue(LumatoneEditorProperty::MainWindowState, getWindowStateAsString());
    ((MainContentComponent*)(getContentComponent()))->saveStateToPropertiesFile(propertiesFile);
}

void MainWindow::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
    // auto restoredState = getProperty(LumatoneEditorProperty::MainWindowState);
    bool useSavedState = restoreWindowStateFromString(propertiesFile->getValue(LumatoneEditorProperty::MainWindowState));

    // fixWindowPositionAndSize(!useSavedState);
    // setWindowState(getBounds(), getWindowStateAsString());

    saveBounds();
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
            // constrainer->setMaximumHeight(maxWindowHeight);

            fixWindowPositionAndSize();
            return;
        }
    }

    if (isOutOfVerticalBounds())
        fixWindowPositionAndSize();

    setWindowState(getBounds(), getWindowStateAsString());
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

// void MainWindow::timerCallback()
// {
//     // Set threshold to be a quarter of the window handle height
//     // verticalBoundsThreshold = round(getTitleBarHeight() * 0.25f);

//     // updateBounds();

//     setWindowState(getBounds(), getWindowStateAsString());
// }

void MainWindow::updateTitle()
{
    juce::String windowTitle("Lumatone Editor");

	if (getCurrentFile().getFileName().isNotEmpty())
		windowTitle << " - " << getCurrentFile().getFileName();

	if (getHasChangesToSave())
		windowTitle << "*";

	setName(windowTitle);
}

// void MainWindow::layoutChanged(const LumatoneLayout &mappingData)
// {
//         updateTitle();
// }

// void MainWindow::newFileLoaded(juce::File file)
// {
//     updateTitle();
// }

void MainWindow::handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier &property)
{
    LumatoneEditorState::handleStatePropertyChange(stateIn, property);

    if (property == LumatoneEditorProperty::HasChangesToSave
        || property == LumatoneEditorProperty::CurrentFile
        )
    {
        updateTitle();
    }
}
