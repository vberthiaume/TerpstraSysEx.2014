# Lumatone Editor - Project Memory

## Branch Structure
- `develop-1.5.0` — current/future branch; uses newer architecture (LumatoneController, LumatoneAction, LumatoneLayout)
- `develop-1.2.0` — stable branch; uses TerpstraKey/TerpstraKeyMapping, UndoableAction directly

## develop-1.2.0 Architecture
- **Undo**: `TerpstraSysExApplication::getApp().performUndoableAction(action)` — actions subclass `juce::UndoableAction` directly (no intermediate base)
- **Mapping data**: `mainComponent->getMappingInEdit()` returns `TerpstraKeyMapping&` (5 boards × 56 keys, `TerpstraKey` has `.colour` field)
- **Device send**: `controller->sendAllParamsOfBoard(boardIndex+1, boardData)` for full board; `sendKeyParam(board+1, key, keyData)` for single key
- **New UndoableActions**: follow pattern in `EditActions.h/.cpp` — store previous data in constructor, restore in `undo()`
- **Batch colour state**: `controller->getBatchColourEditState().isInEdit()` — SingleNoteAssignAction handles this; new ColourReplaceAction intentionally ignores it for simplicity

## Tool Selector
- `ToolSelectorComponent` is a `TabbedComponent` (tabs at top) in MainComponent's bottom-right area
- Adding a tab: `addTab(name, tabColour, component, true)` in constructor; component is owned by the tabbed component
- Tab colour comes from `lf.findColour(LumatoneEditorColourIDs::LightBackground).brighter(0.02f)`

## Colour UI Patterns
- `ColourSelectionBroadcaster` / `ColourSelectionListener` — the colour event system
- `ColourEditComponent` — Button subclass that paints current colour, used as a swatch display
- `CustomPickerPanel` — wraps `juce::ColourSelector`, used in `ColourPaletteWindow`
- `ColourSwatchButton` (new, in ColourReplacePanel.h) — self-contained swatch+picker using `juce::SafePointer<ColourSelector>` + `CallOutBox::launchAsynchronously`

## Project Files
- `.jucer` file must be updated to register new `.h`/`.cpp` files (id is a 6-char alphanumeric, compile="1" for .cpp, "0" for .h)
- Source files in root `Source/` — no subdirectory needed for most UI components
- `Source/backport/` — contains newer-architecture classes backported into 1.2.0 (BatchToolsColourControls, BatchColourActions, etc.)

## Colour Find & Replace (added to develop-1.2.0)
- `Lumatone::ColourReplaceAction` in EditActions.h/.cpp — stores affected key list in constructor, replaces/restores per affected board
- `ColourReplacePanel` + `ColourSwatchButton` in Source/ColourReplacePanel.h/.cpp
- Wired as "Colour Replace" tab in ToolSelectorComponent
- `refreshColourReplace()` on ToolSelectorComponent refreshes match count after external mapping changes
