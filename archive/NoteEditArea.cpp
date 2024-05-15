/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 6.0.8

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
// #include "IsomorphicMassAssign.h"
#include "SingleNoteAssign.h"
#include "LumatoneEditorLookAndFeel.h"
#include "KeyEditComponent.h"

#include "./lumatone_editor_library/device/lumatone_controller.h"
#include "./lumatone_editor_library/graphics/view_constants.h"

#include "MainComponent.h"
#include "./colour_view_component.h"
//[/Headers]

#include "NoteEditArea.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

//[/MiscUserDefs]

//==============================================================================
NoteEditArea::NoteEditArea (const LumatoneEditorState& stateIn)
    : LumatoneEditorState("NoteEditArea", stateIn)
	, LumatoneEditorState::Controller(static_cast<LumatoneEditorState&>(*this))
	, currentSingleKeySelection(-1)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("NoteEditArea");
    editFunctionsTab.reset (new juce::TabbedComponent (juce::TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (editFunctionsTab.get());
    editFunctionsTab->setTabBarDepth (30);
    editFunctionsTab->addTab (juce::translate("Manual Assign"), juce::Colours::lightgrey, new SingleNoteAssign(stateIn), true);
    editFunctionsTab->setCurrentTabIndex (0);

    labelWindowTitle.reset (new juce::Label ("labelWindowTitle",
                                             juce::translate("Assign Keys")));
    addAndMakeVisible (labelWindowTitle.get());
	labelWindowTitle->setFont(getAppFonts().getFont(LumatoneEditorFont::UniviaProBold));
    labelWindowTitle->setJustificationType (juce::Justification::centredLeft);
    labelWindowTitle->setEditable (false, false, false);
	labelWindowTitle->setColour(Label::ColourIds::textColourId, getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::LabelBlue));
    labelWindowTitle->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));


    //[UserPreSize]
	editFunctionsTab->setIndent(0);
	editFunctionsTab->setOutline(0);

	editFunctionsTab->setColour(TabbedComponent::ColourIds::outlineColourId, Colour());
	editFunctionsTab->setColour(TabbedComponent::ColourIds::backgroundColourId, Colour());


	// Selector for octave boards
	octaveBoardSelectorTab.reset(new TabbedButtonBar(TabbedButtonBar::Orientation::TabsAtTop));
	addAndMakeVisible(octaveBoardSelectorTab.get());

	for (int i = 0; i < MAXNUMBOARDS; i++)
	{
		octaveBoardSelectorTab->addTab(translate("Section") + " " + String(i + 1), juce::Colours::lightgrey, i + 1);
	}

	octaveBoardSelectorTab->addChangeListener(this);

	// Single Key fields
	resetOctaveSize(false);

	/* Don't want to resize now
    /*
    //[/UserPreSize]

    setSize (760, 470);


    //[Constructor] You can add your own custom stuff here..
	*/

	getEditorLookAndFeel().setColour(LumatoneKeyEdit::backgroundColourId, findColour(juce::ResizableWindow::backgroundColourId));
	getEditorLookAndFeel().setColour(LumatoneKeyEdit::outlineColourId, Colour(0xffd7d9da));
	getEditorLookAndFeel().setColour(LumatoneKeyEdit::selectedKeyOutlineId, Colour(0xfff7990d));

	backgroundColour = getEditorLookAndFeel().findColour(LumatoneEditorColourIDs::ControlAreaBackground);

	// First octaveboard selection, selection on first key: see MainComponent (Has to be done after change listener has been established)

	auto singleNoteAssign = dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(0));
	if (singleNoteAssign != nullptr)
	{
		addColourSelectionListener(singleNoteAssign);
	}

	addEditorListener(this);

	showIsomorphicMassAssign = getProperty(LumatoneEditorProperty::IsomorphicMassAssign).getIntValue();
	updateShowIsomorphicAssign();

	refreshKeyFields();
    //[/Constructor]
}

NoteEditArea::~NoteEditArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

	removeEditorListener(this);

    editFunctionsTab = nullptr;
    labelWindowTitle = nullptr;


    //[Destructor]. You can add your own custom destruction code here..

	octaveBoardSelectorTab = nullptr;

	for (int i = 0; i < getOctaveBoardSize(); i++)
	{
		terpstraKeyFields[i] = nullptr;
	}

    //[/Destructor]
}

//==============================================================================
void NoteEditArea::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..

	/*
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xffbad0de));

    //[UserPaint] Add your own custom painting code here..
	*/

	g.setColour(backgroundColour);
	g.fillRoundedRectangle(contentBackground, roundedCornerLayout);

	//g.setColour(Colours::pink);
	//g.drawRect(keyEditBounds);
    //[/UserPaint]
}

void NoteEditArea::resized()
{
    //[UserPreResize] Add your own custom resize code here..
	int contentY = round(getHeight() * contentMarginY);
	contentBackground.setBounds(0, contentY, getWidth(), getHeight() - contentY);

	assignControlsBounds.setBounds(
		getWidth() * assignMarginX, contentBackground.getHeight() * assignMarginYInContent + contentBackground.getY(),
		getWidth() * assignControlsWidth, contentBackground.getHeight() * assignControlsHeightInContent
	);

	roundedCornerLayout = round(getParentHeight() * ROUNDEDCORNERTOAPPHEIGHT);

	octaveTabsArea.setBounds(0, 0, getWidth(), round(getHeight() * 0.069f));

	editFunctionsTab->setBounds(assignControlsBounds.toNearestInt());
	editFunctionsTab->setTabBarDepth(assignTabDepthInContent * contentBackground.getHeight());

	resizeLabelWithHeight(labelWindowTitle.get(), roundToInt(octaveBoardSelectorTab->getHeight() * assignLabelTabDepthHeight));
	labelWindowTitle->setTopLeftPosition(roundToInt(getWidth() * assignLabelMarginX), roundToInt((octaveTabsArea.getHeight() - labelWindowTitle->getHeight()) * 0.5f));

	keyEditBounds.setBounds(
		getWidth() * keyEditMarginX, contentBackground.getHeight() * assignMarginYInContent + contentBackground.getY(),
		getWidth() * keyEditWidth, contentBackground.getHeight() * assignControlsHeightInContent
	);

    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

	octaveBoardSelectorTab->setBounds(labelWindowTitle->getRight(), 0, getWidth() - labelWindowTitle->getRight(), octaveTabsArea.getHeight());

	// Single Key fields

	keyEditBounds = contentBackground.withLeft(assignControlsBounds.getRight() + assignControlsBounds.getX() * 0.5f);

	lumatoneGeometry.fitTilingTo(
		keyEditBounds,
		boardGeometry.getMaxHorizontalLineSize(),
		boardGeometry.horizontalLineCount(),
		round(keyEditBounds.getWidth() * singleKeyMarginFromWidth),
		TERPSTRASINGLEKEYROTATIONANGLE, true
	);

	Array<Point<float>> keyCentres = lumatoneGeometry.getHexagonCentres(boardGeometry);
	jassert(keyCentres.size() == getOctaveBoardSize());

	float keySize = lumatoneGeometry.getKeySize();

	int keyIndex = 0;
	for (keyIndex = 0; keyIndex < keyCentres.size(); keyIndex++)
	{
		Point<float> centre = keyCentres[keyIndex];

		terpstraKeyFields[keyIndex]->setKeySize(keySize);
		terpstraKeyFields[keyIndex]->setCentrePosition(centre.roundToInt());
	}

	jassert(getOctaveBoardSize() == keyIndex);

    //[/UserResized]
}

void NoteEditArea::mouseDown (const juce::MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
	// Selection of single key fields
	for (int keyIndex = 0; keyIndex < getOctaveBoardSize(); keyIndex++)
	{
		if (e.eventComponent == terpstraKeyFields[keyIndex].get() || e.eventComponent->getParentComponent() == terpstraKeyFields[keyIndex].get())
		{
			// Select field
			changeSingleKeySelection(keyIndex);

			// Grab key colour - may be replaced with eyedropper tool
			if (e.mods.isAltDown())
			{
				auto colour = terpstraKeyFields[keyIndex]->getValue().getColour();
				selectorListeners.call(&ColourSelectionListener::colourChangedCallback, this, colour);
			}
			// Standard assign action
			else
			{
				// Perform the edit, according to edit mode. Including sending to device
				auto setSelection = octaveBoardSelectorTab->getCurrentTabIndex();
				jassert(setSelection >= 0 && setSelection < MAXNUMBOARDS&& keyIndex >= 0 && keyIndex < getOctaveBoardSize());

				int editMode = editFunctionsTab->getCurrentTabIndex();
				switch (editMode)
				{
				case noteEditMode::SingleNoteAssignMode:
				{
					auto editAction = dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->createEditAction(setSelection, keyIndex);
					performAction(editAction);
					break;
				}
				case noteEditMode::IsomorphicMassAssignMode:
				{
					bool mappingChanged = dynamic_cast<IsomorphicMassAssign*>(editFunctionsTab->getTabContentComponent(editMode))->performMouseDown(setSelection, keyIndex);
					if (mappingChanged)
					{
						// TerpstraSysExApplication::getApp().setHasChangesToSave(true);

						// TODO
						// Refresh key fields (all may be affected)
						//((MainContentComponent*)getParentComponent())->refreshKeyDataFields();
					}
					break;
				}
				default:
					break;
				}
			}
			break;
		}
	}

    //[/UserCode_mouseDown]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void NoteEditArea::setControlsTopLeftPosition(int controlsAreaX, int controlsAreaY)
{
	setTopLeftPosition(controlsAreaX, controlsAreaY - octaveTabsArea.getHeight());
}

//void NoteEditArea::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
//{
//	dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->restoreStateFromPropertiesFile(propertiesFile);
//
//	if (showIsomorphicMassAssign)
//		dynamic_cast<IsomorphicMassAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::IsomorphicMassAssignMode))->restoreStateFromPropertiesFile(propertiesFile);
//
//    resized();
//}

void NoteEditArea::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	// dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->saveStateToPropertiesFile(propertiesFile);

	if (showIsomorphicMassAssign)
		dynamic_cast<IsomorphicMassAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::IsomorphicMassAssignMode))->saveStateToPropertiesFile(propertiesFile);
}

void NoteEditArea::changeListenerCallback(ChangeBroadcaster *source)
{
	if (source == octaveBoardSelectorTab.get())
	{
		auto setSelection = octaveBoardSelectorTab->getCurrentTabIndex();
		jassert(setSelection >= 0 && setSelection < MAXNUMBOARDS);

		setKeyFieldValues(getBoard(setSelection));
	}
}
//
//void NoteEditArea::onSetData(LumatoneLayout& newData)
//{
//	// Add colours of the mapping to the colour combo box
//	//return dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->onSetData(newData);
//}

void NoteEditArea::setKeyFieldValues(const LumatoneBoard& keySet)
{
	for (int i = 0; i < getOctaveBoardSize(); i++)
		terpstraKeyFields[i]->setValue(keySet.getKey(i));
}

ColourViewComponent* NoteEditArea::getColourViewComponent()
{
	return dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->getColourViewComponent();
}

ColourTextEditor* NoteEditArea::getSingleNoteColourTextEditor()
{
	return dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->getColourTextEditor();
}

IsomorphicMassAssign *NoteEditArea::getIsomorphicMassAssignPanel()
{
	return dynamic_cast<IsomorphicMassAssign*>(editFunctionsTab->getTabContentComponent(1));
}

void NoteEditArea::changeSingleKeySelection(int newSelection)
{
	// Unselect previous key
	if (currentSingleKeySelection >= 0 && currentSingleKeySelection < getOctaveBoardSize())
		terpstraKeyFields[currentSingleKeySelection]->setIsSelected(false);

	// Select new key
	currentSingleKeySelection = newSelection;
	if (currentSingleKeySelection >= 0 && currentSingleKeySelection < getOctaveBoardSize())
		terpstraKeyFields[currentSingleKeySelection]->setIsSelected(true);
}

void NoteEditArea::refreshKeyFields()
{
	auto setSelection = octaveBoardSelectorTab->getCurrentTabIndex();
	jassert(setSelection >= 0 && setSelection < MAXNUMBOARDS);
	setKeyFieldValues(getBoard(setSelection));
}

void NoteEditArea::resetOctaveSize(bool refreshAndResize)
{
	int boardSize = getOctaveBoardSize();

	jassert(boardSize == 55 || boardSize == 56);

	int boardIndex = octaveBoardSelectorTab->getCurrentTabIndex();

	if (currentBoardSize != boardSize)
	{
		boardGeometry = LumatoneGeometry();

		for (int i = 0; i < 56; i++)
			terpstraKeyFields[i] = nullptr;

		for (int i = 0; i < boardSize; i++)
		{
			terpstraKeyFields[i].reset(new LumatoneKeyEdit(*this, boardIndex, i));
			addAndMakeVisible(terpstraKeyFields[i].get());
			terpstraKeyFields[i]->addMouseListener(this, true);
		}

		currentBoardSize = boardSize;

		if (refreshAndResize)
		{
			refreshKeyFields();
			resized();
		}
	}
}

Colour NoteEditArea::getSelectedColour()
{
	if (currentSingleKeySelection >= 0 && currentSingleKeySelection < getOctaveBoardSize())
	{
		return terpstraKeyFields[currentSingleKeySelection]->getValue().getColour();
	}

	auto singleNoteAssign = dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(SingleNoteAssignMode));
	if (singleNoteAssign != nullptr)
	{
		return singleNoteAssign->getColourViewComponent()->getColourAsObject();
	}

	return Colour();
}

void NoteEditArea::completeMappingLoaded(const LumatoneLayout& mappingData)
{
	refreshKeyFields();
}

void NoteEditArea::boardChanged(const LumatoneBoard& boardData)
{
	refreshKeyFields();
}

void NoteEditArea::keyChanged(int boardIndex, int keyIndex, const LumatoneKey& lumatoneKey)
{
	refreshKeyFields();
}

void NoteEditArea::selectionChanged(juce::Array<MappedLumatoneKey> selection)
{
	refreshKeyFields();
}

void NoteEditArea::updateShowIsomorphicAssign()
{
	bool showMode = showIsomorphicMassAssign || getInDeveloperMode();

	if (showMode && editFunctionsTab->getNumTabs() == 1)
	{
		editFunctionsTab->addTab(juce::translate("Isomorphic Assign"), juce::Colours::lightgrey, new IsomorphicMassAssign(*this), true);
	}
	else if (!showMode && editFunctionsTab->getNumTabs() == 2)
	{
		editFunctionsTab->removeTab(1);
	}
}

void NoteEditArea::handleStatePropertyChange(juce::ValueTree stateIn, const juce::Identifier& property)
{
	LumatoneEditorState::handleStatePropertyChange(stateIn, property);

	if (property == LumatoneEditorProperty::DeveloperModeOn)
	{
		updateShowIsomorphicAssign();
	}
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="NoteEditArea" componentName="NoteEditArea"
                 parentClasses="public Component, public ChangeListener, public ColourSelectionBroadcaster"
                 constructorParams="" variableInitialisers="currentSingleKeySelection(-1)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="760" initialHeight="470">
  <METHODS>
    <METHOD name="mouseDown (const juce::MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffbad0de"/>
  <TABBEDCOMPONENT name="editFunctionsTab" id="9eb88c4dce6dede9" memberName="editFunctionsTab"
                   virtualName="" explicitFocusOrder="0" pos="8 48 320 422" orientation="top"
                   tabBarDepth="30" initialTab="0">
    <TAB name="Manual Assign" colour="ffd3d3d3" useJucerComp="0" contentClassName="SingleNoteAssign"
         constructorParams="" jucerComponentFile=""/>
  </TABBEDCOMPONENT>
  <LABEL name="labelWindowTitle" id="afc0b85c8e03b3d6" memberName="labelWindowTitle"
         virtualName="" explicitFocusOrder="0" pos="8 8 104 24" textCol="ff61acc8"
         edTextCol="ff000000" edBkgCol="0" labelText="Assign Keys" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="18.0" kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
