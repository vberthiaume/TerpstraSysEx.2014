#include "lumatone_action.h"
// #include "./application_state.h"
#include "../LumatoneController.h"

LumatoneAction::LumatoneAction(LumatoneController* stateIn, juce::String nameIn)
    : state(stateIn)
    , name(nameIn)
{
    if (state)
    {
        octaveBoardSize = state->getOctaveSize();
        numOctaveBoards = state->getNumBoards();
    }
}
