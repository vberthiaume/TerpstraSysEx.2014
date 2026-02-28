/*
  ==============================================================================

    EditActions.h
    Created: 9 May 2021 9:49:13pm
    Author:  hsstraub

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "KeyboardDataStructure.h"

namespace Lumatone {

	class SingleNoteAssignAction : public UndoableAction
	{
	public:
		SingleNoteAssignAction(
			int setSelection,
			int keySelection,
			bool setKeyType,
			bool setChannel, 
			bool setNote,
			bool setColour,
            bool ccFaderDefault,
			LumatoneKeyType newKeyType = LumatoneKeyType::noteOnNoteOff,
			int newChannelNumber = 0, 
			int newNoteNumber = 0,
			TerpstraKey::COLOURTYPE newColour = juce::Colour(),
            bool newCCFaderDefault = true);

		SingleNoteAssignAction(const SingleNoteAssignAction& second)
			: setSelection(second.setSelection)
			, keySelection(second.keySelection)
			, setKeyType(second.setKeyType)
			, setChannel(second.setChannel)
			, setNote(second.setNote)
			, setColour(second.setColour)
            , setCCFaderPolarity(second.setCCFaderPolarity)
			, batchEditActiveAtCreation(second.batchEditActiveAtCreation)
			, previousData(second.previousData)
			, previousVisibleColour(second.previousVisibleColour)
			, newData(second.newData)
		{}

		bool isValid() const;

		virtual bool perform() override;
		virtual bool undo() override;
		
		int getSizeInUnits() override { return sizeof(SingleNoteAssignAction); }

	private:
		int setSelection = - 1;
		int keySelection = -1;

		bool setKeyType = false;
		bool setChannel = false;
		bool setNote = false;
		bool setColour = false;
        bool setCCFaderPolarity = false;

		bool batchEditActiveAtCreation = false;
		TerpstraKey previousData;
		juce::Colour previousVisibleColour;
		TerpstraKey newData;
	};

	class SectionEditAction : public UndoableAction
	{
	public:
		SectionEditAction(int setSelection, TerpstraKeys& newSectionValue);

		SectionEditAction(const SectionEditAction& second)
			: setSelection(second.setSelection)
			, batchEditActiveAtCreation(second.batchEditActiveAtCreation)
			, previousData(second.previousData)
			, newData(second.newData)
		{}

		bool isValid() const;

		virtual bool perform() override;
		virtual bool undo() override;
		int getSizeInUnits() override { return sizeof(SectionEditAction); }

	private:
		int setSelection = -1;

		bool batchEditActiveAtCreation = false;
		TerpstraKeys previousData;
		TerpstraKeys newData;
	};

	class InvertFootControllerEditAction : public UndoableAction
	{
	public:
		InvertFootControllerEditAction(bool newValue);

		InvertFootControllerEditAction(const InvertFootControllerEditAction& second)
			: previousData(second.previousData), newData(second.newData)
		{}

		virtual bool perform() override;
		virtual bool undo() override;
		int getSizeInUnits() override { return sizeof(InvertFootControllerEditAction); }

	private:
		bool previousData;
		bool newData;
	};

	class ExprPedalSensivityEditAction : public UndoableAction
	{
	public:
		ExprPedalSensivityEditAction(int newValue);

		ExprPedalSensivityEditAction(const ExprPedalSensivityEditAction& second)
			: previousData(second.previousData), newData(second.newData)
		{}

		virtual bool perform() override;
		virtual bool undo() override;
		int getSizeInUnits() override { return sizeof(ExprPedalSensivityEditAction); }

	private:
		int previousData;
		int newData;;
	};

    class InvertSustainEditAction : public UndoableAction
    {
    public:
        InvertSustainEditAction(bool newValue);

        InvertSustainEditAction(const InvertSustainEditAction& second)
            : previousData(second.previousData), newData(second.newData)
        {}

        virtual bool perform() override;
        virtual bool undo() override;
        int getSizeInUnits() override { return sizeof(InvertSustainEditAction); }

    private:
        int previousData;
        int newData;;
    };

    // Finds all keys across the entire mapping that match searchColour and
    // replaces them with replaceColour, with full undo support.
    class ColourReplaceAction : public UndoableAction
    {
    public:
        ColourReplaceAction(juce::Colour searchColour, juce::Colour replaceColour);

        virtual bool perform() override;
        virtual bool undo() override;
        int getSizeInUnits() override { return sizeof(ColourReplaceAction); }

        int getMatchCount() const { return affectedKeys.size(); }

    private:
        juce::Colour searchColour;
        juce::Colour replaceColour;

        struct AffectedKey { int boardIndex; int keyIndex; };
        juce::Array<AffectedKey> affectedKeys;
    };

}
