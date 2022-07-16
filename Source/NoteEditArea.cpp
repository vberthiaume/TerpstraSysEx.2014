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
#include "ViewConstants.h"
#include "Main.h"
//[/Headers]

#include "NoteEditArea.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

//[/MiscUserDefs]

//==============================================================================
NoteEditArea::NoteEditArea ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    showIsomorphicMassAssign = TerpstraSysExApplication::getApp().getPropertiesFile()->getBoolValue("IsomorphicMassAssign", false);
    //[/Constructor_pre]

    setName ("NoteEditArea");
    editFunctionsTab.reset (new juce::TabbedComponent (juce::TabbedButtonBar::TabsAtTop));
    addAndMakeVisible (editFunctionsTab.get());
    editFunctionsTab->setTabBarDepth (30);
    editFunctionsTab->addTab (TRANS("Manual Assign"), juce::Colours::lightgrey, new SingleNoteAssign(), true);
    editFunctionsTab->setCurrentTabIndex (0);

    editFunctionsTab->setBounds (8, 48, 320, 422);

    labelWindowTitle.reset (new juce::Label ("labelWindowTitle",
                                             TRANS("Assign Keys")));
    addAndMakeVisible (labelWindowTitle.get());
    labelWindowTitle->setFont (juce::Font (18.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    labelWindowTitle->setJustificationType (juce::Justification::centredLeft);
    labelWindowTitle->setEditable (false, false, false);
    labelWindowTitle->setColour (juce::Label::textColourId, juce::Colour (0xff61acc8));
    labelWindowTitle->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    labelWindowTitle->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    labelWindowTitle->setBounds (8, 8, 104, 24);


    //[UserPreSize]
	editFunctionsTab->setIndent(0);
	editFunctionsTab->setOutline(0);

	editFunctionsTab->setColour(TabbedComponent::ColourIds::outlineColourId, Colour());
	editFunctionsTab->setColour(TabbedComponent::ColourIds::backgroundColourId, Colour());

	labelWindowTitle->setFont(TerpstraSysExApplication::getApp().getAppFont(LumatoneEditorFont::UniviaProBold));

	if (showIsomorphicMassAssign)
		editFunctionsTab->addTab(TRANS("Isomorphic Assign"), juce::Colours::lightgrey, new IsomorphicMassAssign(), true);

	// Selector for octave boards
	octaveBoardSelectorTab.reset(new TabbedButtonBar(TabbedButtonBar::Orientation::TabsAtTop));
	addAndMakeVisible(octaveBoardSelectorTab.get());

	for (int i = 0; i < NUMBEROFBOARDS; i++)
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

	// First octaveboard selection, selection on first key: see MainComponent (Has to be done after change listener has been established)

	auto singleNoteAssign = dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(0));
	if (singleNoteAssign != nullptr)
	{
		addColourSelectionListener(singleNoteAssign);
	}

    //[/Constructor]
}

NoteEditArea::~NoteEditArea()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    editFunctionsTab = nullptr;
    labelWindowTitle = nullptr;


    //[Destructor]. You can add your own custom destruction code here..

	octaveBoardSelectorTab = nullptr;

	octaveKeySets.clear();
	//for (int i = 0; i < TerpstraSysExApplication::getApp().getOctaveBoardSize(); i++)
	//{
	//	terpstraKeyFields[i] = nullptr;
	//}

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
		getHeight() * assignControlsMarginXHRatio, contentBackground.getHeight() * assignMarginYInContent + contentBackground.getY(),
		getHeight() * assignControlsWidthHRatio, contentBackground.getHeight() * assignControlsHeightInContent
	);

	//DBG("------NoteEditAreaSizes - START ------");
	//DBG("Parent size: " + getBounds().toString());
	//DBG("contentBackground: " + contentBackground.toString());
	//DBG("assignControlsBounds: " + assignControlsBounds.toString());

	roundedCornerLayout = round(getParentHeight() * ROUNDEDCORNERTOAPPHEIGHT);

	octaveTabsArea.setBounds(0, 0, getWidth(), round(getHeight() * 0.069f));

	const float tabBarDepth = assignTabDepthInContent * contentBackground.getHeight();
	editFunctionsTab->setBounds(assignControlsBounds.toNearestInt());
	editFunctionsTab->setTabBarDepth(tabBarDepth);
	//DBG("TabBarDepth is " + String(tabBarDepth));

	resizeLabelWithHeight(labelWindowTitle.get(), roundToInt(octaveBoardSelectorTab->getHeight() * assignLabelTabDepthHeight));
	labelWindowTitle->setTopLeftPosition(roundToInt(assignLabelMarginXHRatio * getHeight()), roundToInt((octaveTabsArea.getHeight() - labelWindowTitle->getHeight()) * 0.5f));
	//DBG("LabelWindwoTitle bounds: " + labelWindowTitle->getBoundsInParent().toString());

	//keyEditBounds.setBounds(
	//	getWidth() * keyEditMarginX, contentBackground.getHeight() * assignMarginYInContent + contentBackground.getY(),
	//	getWidth() * keyEditWidth, contentBackground.getHeight() * assignControlsHeightInContent
	//);


	//[/UserPreResize]

	//[UserResized] Add your own custom resize handling here..

	octaveBoardSelectorTab->setBounds(labelWindowTitle->getRight(), 0, getWidth() - labelWindowTitle->getRight(), octaveTabsArea.getHeight());
	//DBG("First tab bounds: " + octaveBoardSelectorTab->getTabButton(0)->getBoundsInParent().toString());
	//DBG("Last tab bounds: " + octaveBoardSelectorTab->getTabButton(4)->getBoundsInParent().toString());

	// Single Key fields

	keyEditBounds = contentBackground.withLeft(assignControlsBounds.getRight() + assignControlsBounds.getX() * 0.5f);
	DBG("KeyEditBounds: " + keyEditBounds.toString());

	int numOctaves = 1;
	int widestRow = boardGeometry.getMaxHorizontalLineSize();
	int longestColumn = boardGeometry.horizontalLineCount();
	int expectedNumKeys = currentBoardSize;
	
	if (octaveIndexBeingEdited < 0)
	{
		numOctaves = 5;
		widestRow = 33;
		expectedNumKeys *= 5;
	}

	tilingGeometry.fitTilingTo(
		keyEditBounds,
		widestRow,
		longestColumn,
		round(keyEditBounds.getHeight() * singleKeyMarginFromHeight), // TODO recalculate this to be height-based
		TERPSTRASINGLEKEYROTATIONANGLE, true
	);

	DBG("key margin: " + juce::String(keyEditBounds.getHeight() * singleKeyMarginFromHeight));


	Array<Point<float>> keyCentres = tilingGeometry.getHexagonCentres(boardGeometry, 0, numOctaves);

	jassert(keyCentres.size() == expectedNumKeys);

	float keySize = tilingGeometry.getKeySize();

	DBG("key size: " + String(keySize));

	for (int oct = 0; oct < numOctaves; oct++)
	{
		auto octave = octaveKeySets[oct];
		int keyIndex;
		for (keyIndex = 0; keyIndex < currentBoardSize; keyIndex++)
		{
			//juce::String msg = juce::String("Resizing Oct ") + juce::String(oct) + " Key " + juce::String(keyIndex);
			//DBG(msg);
			int centreIndex = keyIndex + oct * currentBoardSize;
			Point<float> centre = keyCentres[centreIndex];

			auto key = octave->keys[keyIndex].get();
			key->setKeySize(keySize);
			key->setCentrePosition(centre.roundToInt());
			//key->setVisible(true);
			
		}

		//jassert(TerpstraSysExApplication::getApp().getOctaveBoardSize() == keyIndex);
	}

	//DBG("------NoteEditAreaSizes - END ------");
    //[/UserResized]
}

void NoteEditArea::mouseDown (const juce::MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
	// Selection of single key fields

	return;

	//for (int keyIndex = 0; keyIndex < TerpstraSysExApplication::getApp().getOctaveBoardSize(); keyIndex++)
	//{
	//	// TODO support click modifiers for multiselection

	//	if (e.eventComponent == terpstraKeyFields[keyIndex].get() || e.eventComponent->getParentComponent() == terpstraKeyFields[keyIndex].get())
	//	{
	//		// Select field
	//		changeKeySelection(keyIndex);

	//		// Grab key colour - may be replaced with eyedropper tool
	//		if (e.mods.isAltDown())
	//		{
	//			auto colour = terpstraKeyFields[keyIndex]->getValue().colour;
	//			selectorListeners.call(&ColourSelectionListener::colourChangedCallback, this, colour);
	//		}
	//		// Standard assign action
	//		else
	//		{
	//			// Perform the edit, according to edit mode. Including sending to device
	//			auto setSelection = octaveBoardSelectorTab->getCurrentTabIndex();
	//			jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS&& keyIndex >= 0 && keyIndex < TerpstraSysExApplication::getApp().getOctaveBoardSize());

	//			int editMode = editFunctionsTab->getCurrentTabIndex();
	//			switch (editMode)
	//			{
	//			case noteEditMode::SingleNoteAssignMode:
	//			{
	//				auto editAction = dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->createEditAction(setSelection, keyIndex);
	//				TerpstraSysExApplication::getApp().performUndoableAction(editAction);
	//				break;
	//			}
	//			case noteEditMode::IsomorphicMassAssignMode:
	//			{
	//				bool mappingChanged = dynamic_cast<IsomorphicMassAssign*>(editFunctionsTab->getTabContentComponent(editMode))->performMouseDown(setSelection, keyIndex);
	//				if (mappingChanged)
	//				{
	//					TerpstraSysExApplication::getApp().setHasChangesToSave(true);

	//					// Refresh key fields (all may be affected)
	//					((MainContentComponent*)getParentComponent())->refreshKeyDataFields();
	//				}
	//				break;
	//			}
	//			default:
	//				break;
	//			}
	//		}
	//		break;
	//	}
	//}

    //[/UserCode_mouseDown]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void NoteEditArea::keyClickedCallback(const juce::MouseEvent& e, int boardIndex, int keyIndex, TerpstraKey value)
{
	auto octave = octaveKeySets[boardIndex];

	// TODO key modifiers for multiselection
	auto selection = LumatoneKeySelection();
	selection.add(LumatoneKeyPtr(boardIndex, keyIndex));

	// Select field
	changeKeySelection(selection);

	// Grab key colour - may be replaced with eyedropper tool
	if (e.mods.isAltDown())
	{
		auto colour = value.colour;
		selectorListeners.call(&ColourSelectionListener::colourChangedCallback, this, colour);
	}
	// Standard assign action
	else
	{
		// Perform the edit, according to edit mode. Including sending to device
		auto setSelection = octaveBoardSelectorTab->getCurrentTabIndex();
		jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS&& keyIndex >= 0 && keyIndex < currentBoardSize);

		int editMode = editFunctionsTab->getCurrentTabIndex();
		switch (editMode)
		{
		case noteEditMode::SingleNoteAssignMode:
		{
			auto editAction = dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->createEditAction(setSelection, keyIndex);
			TerpstraSysExApplication::getApp().performUndoableAction(editAction);
			break;
		}
		case noteEditMode::IsomorphicMassAssignMode:
		{
			bool mappingChanged = dynamic_cast<IsomorphicMassAssign*>(editFunctionsTab->getTabContentComponent(editMode))->performMouseDown(setSelection, keyIndex);
			if (mappingChanged)
			{
				TerpstraSysExApplication::getApp().setHasChangesToSave(true);

				// Refresh key fields (all may be affected)
				((MainContentComponent*)getParentComponent())->refreshKeyDataFields();
			}
			break;
		}
		default:
			break;
		}
	}
}

void NoteEditArea::lookAndFeelChanged()
{
	LookAndFeel& lookAndFeel = getLookAndFeel();
	backgroundColour = lookAndFeel.findColour(LumatoneEditorColourIDs::ControlAreaBackground);
	labelWindowTitle->setColour(Label::ColourIds::textColourId, lookAndFeel.findColour(LumatoneEditorColourIDs::LabelBlue));
}

void NoteEditArea::setControlsTopLeftPosition(int controlsAreaX, int controlsAreaY)
{
	setTopLeftPosition(controlsAreaX, controlsAreaY - octaveTabsArea.getHeight());
}


void NoteEditArea::restoreStateFromPropertiesFile(PropertiesFile* propertiesFile)
{
	dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->restoreStateFromPropertiesFile(propertiesFile);

	if (showIsomorphicMassAssign)
		dynamic_cast<IsomorphicMassAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::IsomorphicMassAssignMode))->restoreStateFromPropertiesFile(propertiesFile);

    resized();
}

void NoteEditArea::saveStateToPropertiesFile(PropertiesFile* propertiesFile)
{
	dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->saveStateToPropertiesFile(propertiesFile);

	if (showIsomorphicMassAssign)
		dynamic_cast<IsomorphicMassAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::IsomorphicMassAssignMode))->saveStateToPropertiesFile(propertiesFile);
}

void NoteEditArea::changeListenerCallback(ChangeBroadcaster *source)
{
	if (source == octaveBoardSelectorTab.get())
	{
		auto setSelection = octaveBoardSelectorTab->getCurrentTabIndex();
		jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS);

		setKeyFieldValues(((MainContentComponent*)getParentComponent())->getMappingInEdit().sets[setSelection], setSelection);
	}
}


void NoteEditArea::onSetData(TerpstraKeyMapping& newData)
{
	// Add colours of the mapping to the colour combo box
	return dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->onSetData(newData);
}
//
//void NoteEditArea::setKeyFieldValues(const LumatoneKeySelection& keySelection)
//{
//	for (auto keyPtr : keySelection)
//	{
//		if (keyPtr.isValid() && !keyPtr.isNull())
//		{
//			auto octave = octaveKeySets[keyPtr.board_idx];
//			for (int i = 0; i < TerpstraSysExApplication::getApp().getOctaveBoardSize(); i++)
//				octave->keys[i]->setValue(*keyPtr.key);
//
//			continue;
//		}
//
//		DBG("Warning: invalid or null key passed to NoteEditArea::setKeyFieldValues");
//	}
//}

void NoteEditArea::setKeyFieldValues(const TerpstraKeys& keys, int octaveIndex)
{
	if (octaveIndex >= 0 && octaveIndex < NUMBEROFBOARDS)
	{
		for (int i = 0; i < currentBoardSize; i++)
		{
			octaveKeySets[0]->keys[i]->setValue(keys.theKeys[i]);
		}

		octaveIndexBeingEdited = octaveIndex;

		return;
	}

	DBG("Warning: invalid octaveIndex passed to NoteEditArea::setKeyFieldValues");
}

void NoteEditArea::setKeyFieldValues(const TerpstraKeys (&allOctaveKeys)[5])
{
	if (allOctaveKeys == nullptr)
	{
		DBG("Warning: nullprtr passed to NoteEditArea::setKeyFieldValues");
		return;
	}

	for (int oct = 0; oct < NUMBEROFBOARDS; oct++)
	{
		//auto keysIn = (allOctaveKeys[oct])->theKeys;
		auto octave = octaveKeySets[oct];

		for (int i = 0; i < currentBoardSize; i++)
		{
			auto keyEdit = octave->keys[i].get();
			auto keyValue = &(allOctaveKeys[oct].theKeys[i]);
			keyEdit->setValue(*keyValue);
			keyEdit->setVisible(true);
			DBG("Oct " + String(oct) + ", Key " + String(i) + ": " + keyEdit->toString());
		}
	}

	octaveIndexBeingEdited = -1;
}

ColourEditComponent* NoteEditArea::getColourEditComponent()
{
	return dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->getColourEditComponent();
}

ColourTextEditor* NoteEditArea::getSingleNoteColourTextEditor()
{
	return dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(noteEditMode::SingleNoteAssignMode))->getColourTextEditor();
}

void NoteEditArea::changeKeySelection(LumatoneKeySelection& selection)
{
	// Unselect previous key
	for (auto key : currentKeySelection)
	{
		if (key.isValid())
		{
			auto octave = octaveKeySets[key.board_idx];			
			octave->keys[key.key_idx]->setIsSelected(false);
		}
	}

	// Select new key
	currentKeySelection = selection;

	for (auto key : currentKeySelection)
	{
		if (key.isValid())
		{
			auto octave = octaveKeySets[key.board_idx];
			octave->keys[key.key_idx]->setIsSelected(true);
		}
	}
}

void NoteEditArea::resetKeySelection()
{
	auto selection = LumatoneKeySelection();
	changeKeySelection(selection);
}

void NoteEditArea::setEditAllOctaves(bool editAllOctaves)
{
	octaveBoardSelectorTab->setVisible(!editAllOctaves);

	if (editAllOctaves)
	{
		//auto sets = &((MainContentComponent*)getParentComponent())->getMappingInEdit().sets;
		setKeyFieldValues(((MainContentComponent*)getParentComponent())->getMappingInEdit().sets);

		octaveIndexBeingEdited = -1;

		return;
	}

	hideInactiveOctaves();
}

void NoteEditArea::hideInactiveOctaves()
{
	for (int oct = 1; oct < NUMBEROFBOARDS; oct++)
	{
		auto octave = octaveKeySets[oct];
		for (int i = 0; i < currentBoardSize; i++)
		{
			octave->keys[i]->setVisible(false);
		}
	}

	octaveIndexBeingEdited = octaveBoardSelectorTab->getCurrentTabIndex();
	refreshKeyFields();
}

void NoteEditArea::refreshKeyFields()
{
	if (octaveIndexBeingEdited >= 0)
	{
		//auto setSelection = octaveBoardSelectorTab->getCurrentTabIndex();
		//jassert(setSelection >= 0 && setSelection < NUMBEROFBOARDS);
		jassert(octaveIndexBeingEdited >= 0 && octaveIndexBeingEdited < NUMBEROFBOARDS);

		setKeyFieldValues(((MainContentComponent*)getParentComponent())->getMappingInEdit().sets[octaveIndexBeingEdited], octaveIndexBeingEdited);
		return;
	}
	else
	{
		// All octaves mode
		setEditAllOctaves(true);
	}
}

void NoteEditArea::resetOctaveSize(bool refreshAndResize)
{
	int boardSize = TerpstraSysExApplication::getApp().getOctaveBoardSize();

	jassert(boardSize == 55 || boardSize == 56);

	if (currentBoardSize != boardSize)
	{
		boardGeometry = TerpstraBoardGeometry();

		octaveKeySets.clear();

		for (int oct = 0; oct < NUMBEROFBOARDS; oct++)
		{
			auto octave = octaveKeySets.add(new LumatoneBoardSet(oct));

			//for (int i = 0; i < 56; i++)
			//{
			//	octave->keys[i] = nullptr;
			//}

			for (int i = 0; i < boardSize; i++)
			{
				auto keyEdit = octave->keys[i].get();
				//octave->keys[i].reset(new TerpstraKeyEdit(oct, i));
				addAndMakeVisible(keyEdit);
				keyEdit->addKeyEditListener(this);
			}
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
	// TODO support multiselection

	auto firstKeySelection = currentKeySelection[0];

	if (firstKeySelection.isValid())
	{
		return firstKeySelection.key->colour;
	}

	auto singleNoteAssign = dynamic_cast<SingleNoteAssign*>(editFunctionsTab->getTabContentComponent(SingleNoteAssignMode));
	if (singleNoteAssign != nullptr)
	{
		return singleNoteAssign->getColourEditComponent()->getColourAsObject();
	}

	return Colour();
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

