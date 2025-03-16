/*
  ==============================================================================

    LumatoneEditorFontLibrary.h
    Created: 12 Feb 2021 10:52:11pm
    Author:  Vincenzo

  ==============================================================================
*/

#ifndef LUMATONE_EDITOR_FONT_LIBRARY_H
#define LUMATONE_EDITOR_FONT_LIBRARY_H

#include <JuceHeader.h>

typedef enum
{
    UniviaPro = 0,
    UniviaProBold,
    FranklinGothic,
    GothamNarrowMedium,
    GothamNarrowBold,
    GothamNarrowItalic,
    GothamNarrowLight

} LumatoneEditorFont;

struct LumatoneEditorFontLibrary
{
    LumatoneEditorFontLibrary()
    {
        // Setup default fallback fonts

        // In place of...
        // UniviaPro
        fonts.add(juce::Font("Tahoma", 12.0f, juce::Font::plain));

        // UniviaProBold
        fonts.add(juce::Font("Tahoma", 12.0f, juce::Font::bold));

        // FranklinGothic
        fonts.add(juce::Font("Arial", 12.0f, juce::Font::plain));

        // GothamNarrowMedium
        fonts.add(juce::Font("Trebuchet MS", 12.0f, juce::Font::plain));

        // GothamNarrowMediumBold
        fonts.add(juce::Font("Trebuchet MS", 12.0f, juce::Font::bold));

        // GothamNarrowMediumItalic
        fonts.add(juce::Font("Trebuchet MS", 12.0f, juce::Font::italic));

        // GothamNarrowMediumLight
        fonts.add(juce::Font("Trebuchet MS", 12.0f, juce::Font::plain));

#if USE_LICENSED_FONTS

        fonts.set(LumatoneEditorFont::UniviaPro,
              juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::UniviaProRegular_ttf, BinaryData::UniviaProRegular_ttfSize))
        );

        fonts.set(LumatoneEditorFont::UniviaProBold,
              juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::UniviaProBold_ttf, BinaryData::UniviaProBold_ttfSize))
        );

        fonts.set(LumatoneEditorFont::FranklinGothic,
              juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::framd_ttf, BinaryData::framd_ttfSize))
        );

        fonts.set(LumatoneEditorFont::GothamNarrowMedium,
              juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::GothamNarrowMedium_otf, BinaryData::GothamNarrowMedium_otfSize))
        );

        fonts.set(LumatoneEditorFont::GothamNarrowBold,
              juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::GothamNarrowBold_otf, BinaryData::GothamNarrowBold_otfSize))
        );

        fonts.set(LumatoneEditorFont::GothamNarrowItalic,
              juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::GothamNarrowLightItalic_otf, BinaryData::GothamNarrowLightItalic_otfSize))
        );

        fonts.set(LumatoneEditorFont::GothamNarrowLight,
              juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::GothamNarrowLight_otf, BinaryData::GothamNarrowLight_otfSize))
        );

#endif
    }

    juce::Font getFont(LumatoneEditorFont fontId, float heightIn = 12.0f) const
    {
        return fonts[fontId].withHeight(heightIn);
    }

private:
    typedef juce::Array<juce::Font> Library;
    LumatoneEditorFontLibrary::Library fonts;
};

#endif // LUMATONE_EDITOR_FONT_LIBRARY_H
