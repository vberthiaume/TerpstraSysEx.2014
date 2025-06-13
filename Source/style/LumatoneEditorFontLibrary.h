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
    GothamNarrowLight,
    CourierNew

} LumatoneEditorFont;

struct LumatoneEditorFontLibrary
{
    LumatoneEditorFontLibrary()
    {
        // Setup default fallback fonts

        // In place of...
        // UniviaPro
        fonts.add(Font("Tahoma", 12.0f, Font::plain));

        // UniviaProBold
        fonts.add(Font("Tahoma", 12.0f, Font::bold));

        // FranklinGothic
        fonts.add(Font("Arial", 12.0f, Font::plain));

        // GothamNarrowMedium
        fonts.add(Font("Trebuchet MS", 12.0f, Font::plain));

        // GothamNarrowMediumBold
        fonts.add(Font("Trebuchet MS", 12.0f, Font::bold));

        // GothamNarrowMediumItalic
        fonts.add(Font("Trebuchet MS", 12.0f, Font::italic));

        // GothamNarrowMediumLight
        fonts.add(Font("Trebuchet MS", 12.0f, Font::plain));

        // CourierNew
        fonts.add (Font ("Courier New", 12.0f, Font::plain));

#if USE_LICENSED_FONTS

        fonts.set(LumatoneEditorFont::UniviaPro,
              Font(Typeface::createSystemTypefaceFor(BinaryData::UniviaProRegular_ttf, BinaryData::UniviaProRegular_ttfSize))
        );

        fonts.set(LumatoneEditorFont::UniviaProBold,
              Font(Typeface::createSystemTypefaceFor(BinaryData::UniviaProBold_ttf, BinaryData::UniviaProBold_ttfSize))
        );

        fonts.set(LumatoneEditorFont::FranklinGothic,
              Font(Typeface::createSystemTypefaceFor(BinaryData::framd_ttf, BinaryData::framd_ttfSize))
        );

        fonts.set(LumatoneEditorFont::GothamNarrowMedium,
              Font(Typeface::createSystemTypefaceFor(BinaryData::GothamNarrowMedium_otf, BinaryData::GothamNarrowMedium_otfSize))
        );

        fonts.set(LumatoneEditorFont::GothamNarrowBold,
              Font(Typeface::createSystemTypefaceFor(BinaryData::GothamNarrowBold_otf, BinaryData::GothamNarrowBold_otfSize))
        );

        fonts.set(LumatoneEditorFont::GothamNarrowItalic,
              Font(Typeface::createSystemTypefaceFor(BinaryData::GothamNarrowLightItalic_otf, BinaryData::GothamNarrowLightItalic_otfSize))
        );

        fonts.set(LumatoneEditorFont::GothamNarrowLight,
              Font(Typeface::createSystemTypefaceFor(BinaryData::GothamNarrowLight_otf, BinaryData::GothamNarrowLight_otfSize))
        );

        //TODO: this is untested
        // CourierNew
        fonts.add (Font ("Courier New", 12.0f, Font::plain));
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
