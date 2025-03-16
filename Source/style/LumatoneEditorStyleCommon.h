/*
  ==============================================================================

    LumatoneEditorStyleCommon.h
    Created: 13 Dec 2020 8:49:14pm
    Author:  Vincenzo

    Helper functions for various positioning and sizing tasks.

  ==============================================================================
*/

#pragma once
#include "../lumatone_editor_library/graphics/lumatone_assets.h"
#include "../lumatone_editor_library/common/math.h"

//==================================================================
//
// STATIC HELPERS
//
//==================================================================

#define PHI 1.618033989f // (1 + sqrt(5)) / 2
#define HEXRADIUSTOLATERAL 0.8660254f // sqrt(3) / 2

// JUCE methods' radians are rotated -90deg with respect to trigonometry functions
#define PATH_PI_2_CW   1.5707963 // pi / 2
#define PATH_PI_2_CCW -1.5707963

#if JUCE_MAC
    #define GLOBALFONTSCALAR 0.9f
    #define CONTROLBOXFONTHEIGHTSCALAR 0.7f
#elif JUCE_WINDOWS
    #define GLOBALFONTSCALAR 1.0f
    #define CONTROLBOXFONTHEIGHTSCALAR 0.62f
#elif JUCE_LINUX
    #define GLOBALFONTSCALAR 1.0f
    #define CONTROLBOXFONTHEIGHTSCALAR 0.73f
#endif

/// <summary>
/// Returns a path representing a rectangle shape, considering rounded corners and connected edges
/// </summary>
/// <param name="bounds"></param>
/// <param name="roundedCornerSize"></param>
/// <param name="connectedFlags"></param>
/// <returns></returns>
static juce::Path getConnectedRoundedRectPath(juce::Rectangle<float> bounds, float roundedCornerSize, int connectedFlags)
{
    juce::Path rect;
    juce::Point<float> origin = bounds.getPosition();
    juce::Point<float> endpoint = bounds.getBottomRight();

    // TODO: finish implementing 3 and 12
    if (connectedFlags == 0 || connectedFlags == 3 || connectedFlags == 12)
    {
        rect.addRoundedRectangle(0, 0, bounds.getWidth(), bounds.getHeight(), roundedCornerSize);
        return rect;
    }

    roundedCornerSize *= 2;

    int xTo, yTo;

    if (connectedFlags & juce::Button::ConnectedEdgeFlags::ConnectedOnTop || connectedFlags & juce::Button::ConnectedOnLeft)
    {
        rect.startNewSubPath(origin);
    }
    else
    {
        rect.addArc(origin.x, origin.y, roundedCornerSize, roundedCornerSize, PATH_PI_2_CCW, 0, true);
    }

    if (connectedFlags & juce::Button::ConnectedEdgeFlags::ConnectedOnTop || connectedFlags & juce::Button::ConnectedEdgeFlags::ConnectedOnRight)
    {
        rect.lineTo(endpoint.x, origin.y);
    }
    else
    {
        xTo = endpoint.x - roundedCornerSize;
        rect.lineTo(xTo, origin.y);
        rect.addArc(xTo, origin.y , roundedCornerSize, roundedCornerSize, 0, PATH_PI_2_CW);
    }

    if (connectedFlags & juce::Button::ConnectedEdgeFlags::ConnectedOnBottom || connectedFlags & juce::Button::ConnectedEdgeFlags::ConnectedOnRight)
    {
        rect.lineTo(endpoint);
    }
    else
    {
        yTo = endpoint.y - roundedCornerSize;
        rect.lineTo(endpoint.x, yTo);
        rect.addArc(endpoint.x - roundedCornerSize, yTo, roundedCornerSize, roundedCornerSize, PATH_PI_2_CW, juce::MathConstants<float>::pi);
    }

    if (connectedFlags & juce::Button::ConnectedEdgeFlags::ConnectedOnBottom || connectedFlags & juce::Button::ConnectedEdgeFlags::ConnectedOnLeft)
    {
        rect.lineTo(origin.x, endpoint.y);
    }
    else
    {
        rect.lineTo(roundedCornerSize, endpoint.y);
        rect.addArc(origin.x, endpoint.y - roundedCornerSize, roundedCornerSize, roundedCornerSize, -juce::MathConstants<float>::pi, PATH_PI_2_CCW);
    }

    rect.closeSubPath();
    return rect;
}

// TODO: Integrate this into the above function somehow
static juce::Path getDiagonalRoundedCornersPath(juce::Rectangle<float> bounds, float roundedCornerSize, bool roundedTopRightAndBottomLeft, bool hasPopup = false)
{
    juce::Path rect;
    roundedCornerSize *= 2;
    int xTo, yTo;

    if (roundedTopRightAndBottomLeft)
    {
        rect.startNewSubPath(bounds.getPosition());

        xTo = bounds.getRight() - roundedCornerSize;
        yTo = bounds.getY();
        rect.lineTo(xTo, yTo);
        rect.addArc(xTo, yTo, roundedCornerSize, roundedCornerSize, 0, PATH_PI_2_CW);

        xTo = bounds.getRight();
        yTo = bounds.getBottom();
        rect.lineTo(xTo, yTo);

        xTo = bounds.getX();
        if (!hasPopup)
        {
            xTo += roundedCornerSize;
            rect.lineTo(xTo, yTo);
            rect.addArc(xTo - roundedCornerSize, yTo - roundedCornerSize, roundedCornerSize, roundedCornerSize, -juce::MathConstants<float>::pi, PATH_PI_2_CCW);
        }
        else
        {
            rect.lineTo(xTo, yTo);
        }
    }
    else
    {
        rect.startNewSubPath(bounds.getTopRight());

        xTo = bounds.getRight();
        yTo = bounds.getBottom();

        if (!hasPopup)
        {
            yTo -= roundedCornerSize;
            rect.lineTo(xTo, yTo);
            rect.addArc(xTo - roundedCornerSize, yTo, roundedCornerSize, roundedCornerSize, PATH_PI_2_CW, juce::MathConstants<float>::pi);
        }
        else
        {
            rect.lineTo(xTo, yTo);
        }

        xTo = bounds.getX();
        yTo = bounds.getBottom();
        rect.lineTo(xTo, yTo);

        yTo = bounds.getY() + roundedCornerSize;
        rect.lineTo(xTo, yTo);

        rect.addArc(xTo, yTo - roundedCornerSize, roundedCornerSize, roundedCornerSize, PATH_PI_2_CCW, 0);
    }

    rect.closeSubPath();
    return rect;
}

/// <summary>
/// Adds an arc to a Path using a juce::Rectangle object for bounds.
/// </summary>
/// <param name="pathIn"></param>
/// <param name="ellipseBounds"></param>
/// <param name="fromRadians"></param>
/// <param name="toRadians"></param>
/// <param name="startAsNewSubPath"></param>
static void addArcToPath(juce::Path& pathIn, juce::Rectangle<float>& ellipseBounds, float fromRadians, float toRadians, bool startAsNewSubPath)
{
    pathIn.addArc(
        ellipseBounds.getX(),
        ellipseBounds.getY(),
        ellipseBounds.getWidth(),
        ellipseBounds.getHeight(),
        fromRadians,
        toRadians,
        startAsNewSubPath
    );
}

///// <summary>
///// Intended for use with components in FlexBoxes, this scales a component and sets the justifications within the FlexBox
///// </summary>
///// <param name="component"></param>
///// <param name="scaleFactorX"></param>
///// <param name="scaleFactorY"></param>
///// <param name="justification"></param>
//static void scaleAndJustifyComponent(Component& component, float scaleFactorX, float scaleFactorY, Justification justification)
//{
//    int newWidth = round(component.getWidth() * scaleFactorX);
//    int newHeight = round(component.getHeight() * scaleFactorY);
//
//    int xOff, yOff;
//
//    if (justification.getFlags() & Justification::left)
//    {
//        xOff = 0;
//    }
//    else
//    {
//        if (justification.getFlags() & Justification::right | Justification::centred)
//        {
//            xOff = component.getWidth() - newWidth;
//        }
//
//        if (justification.getFlags() & Justification::horizontallyCentred)
//        {
//            xOff = round(xOff * 0.5f);
//        }
//    }
//
//    if (justification.getFlags() & Justification::top)
//    {
//        yOff = 0;
//    }
//    else
//    {
//        if (justification.getFlags() & Justification::bottom | Justification::centred)
//        {
//            yOff = component.getHeight() - newHeight;
//        }
//
//        if (justification.getFlags() & Justification::verticallyCentred)
//        {
//            yOff = round(yOff * 0.5f);
//        }
//    }
//
//    component.setBounds(component.getX() + xOff, component.getY() + yOff, newWidth, newHeight);
//}
//
///// <summary>
///// Intended for use with components in FlexBoxes, this scales a component and sets the justifications within the FlexBox
///// </summary>
///// <param name="component"></param>
///// <param name="scaleFactor"></param>
///// <param name="justification"></param>
//static void scaleAndJustifyComponent(Component& component, float scaleFactor, Justification justification = Justification::centred)
//{
//    scaleAndJustifyComponent(component, scaleFactor, scaleFactor, justification);
//}

/// <summary>
/// Sets the width of a component while retaining the aspect ratio of a given image
/// </summary>
/// <param name="component"></param>
/// <param name="image"></param>
/// <param name="widthIn"></param>
//static void setWidthRetainingAspectRatio(Component* component, const Image& image, int widthIn)
//{
//    component->setSize(widthIn, round(image.getHeight() / (float) image.getWidth() * widthIn));
//}

/// <summary>
/// Sets the height of a component while retaining the aspect ratio of a given image
/// </summary>
/// <param name="component"></param>
/// <param name="image"></param>
/// <param name="widthIn"></param>
//static void setHeightRetainingAspectRatio(Component* component, const Image& image, int heightIn)
//{
//    component->setSize(round(image.getWidth() / (float) image.getHeight() * heightIn), heightIn);
//}

/// <summary>
/// Sets the width of an ImageComponent while retaining the aspect ratio of its image
/// </summary>
/// <param name="component"></param>
/// <param name="image"></param>
/// <param name="widthIn"></param>
//static void setWidthRetainingAspectRatio(ImageComponent* component, int widthIn)
//{
//    setWidthRetainingAspectRatio(component, component->getImage(), widthIn);
//}

/// <summary>
/// Sets the height of an ImageComponent while retaining the aspect ratio of its image
/// </summary>
/// <param name="component"></param>
/// <param name="image"></param>
/// <param name="widthIn"></param>
//static void setHeightRetainingAspectRatio(ImageComponent* component, int heightIn)
//{
//    setHeightRetainingAspectRatio(component, component->getImage(), heightIn);
//}

/// <summary>
/// Sets the width of an ImageButton while retaining the aspect ratio of its normal image
/// </summary>
/// <param name="component"></param>
/// <param name="image"></param>
/// <param name="widthIn"></param>
//static void setWidthRetainingAspectRatio(ImageButton* component, int widthIn)
//{
//    setWidthRetainingAspectRatio(component, component->getNormalImage(), widthIn);
//}

/// <summary>
/// Sets the height of an ImageButton while retaining the aspect ratio of its normal image
/// </summary>
/// <param name="component"></param>
/// <param name="image"></param>
/// <param name="widthIn"></param>
//static void setHeightRetainingAspectRatio(ImageButton* component, int heightIn)
//{
//    setHeightRetainingAspectRatio(component, component->getNormalImage(), heightIn);
//}

static void resizeLabelWithHeight(juce::Label* label, int height, float fontHeightScalar = 1.0f, juce::String textSuffix = "_")
{
    label->setFont(label->getFont().withHeight(height * fontHeightScalar));
    label->setSize(juce::roundToInt(label->getFont().getStringWidthFloat(label->getText() + textSuffix)), height);

    // float width = overrideWidth;
    // if (width <= 0)
    //     roundToInt(label->getFont().getStringWidthFloat(label->getText() + textSuffix));
    // label->setSize(width, height);
}

//static void positionLabelWithHeight(Label* label, int xPosition, int yPosition, int height, float fontHeightScalar = 1.0f, String textSuffix = "_")
//{
//    label->setFont(label->getFont().withHeight(height * fontHeightScalar));
//    label->setBounds(
//        xPosition, yPosition,
//        round(label->getFont().getStringWidthFloat(label->getText() + textSuffix)), height
//    );
//}

static void resizeLabelWithWidth(juce::Label* label, int width, float fontHeightScalar = 1.0f)
{
    float heightOverWidth = label->getFont().getHeight() / label->getFont().getStringWidthFloat(label->getText());
    label->setSize(width, juce::roundToInt(heightOverWidth * width));
    label->setFont(label->getFont().withHeight(label->getHeight() * fontHeightScalar * GLOBALFONTSCALAR));
}

// For scaling Labels down so they're not clipped or skewed
static float scalarToFitString(juce::String lineOfText, juce::Font font, float width)
{
    float scalar = 1.0f;
    float fullWidth = font.getStringWidthFloat(lineOfText);
    if (fullWidth > width)
    {
        scalar = width / fullWidth;
    }
    return scalar;
}

// For scaling Labels down so they're not clipped or skewed
static float scalarToFitString(juce::Label& labelIn)
{
    return scalarToFitString(labelIn.getText(), labelIn.getFont(), labelIn.getWidth());
}

//static void positionLabelWithWidth(Label* label, int xPosition, int yPosition, int width, float fontHeightScalar = 1.0f)
//{
//    float heightOverWidth = label->getFont().getHeight() / label->getFont().getStringWidthFloat(label->getText());
//    label->setBounds(xPosition, yPosition, width, round(heightOverWidth * width));
//    label->setFont(label->getFont().withHeight(label->getHeight() * fontHeightScalar * GLOBALFONTSCALAR));
//}

static void resizeToggleButtonWithHeight(juce::ToggleButton* btn, juce::Font font, int heightIn, juce::String textSuffix = "_")
{
    font.setHeight(font.getHeight() * GLOBALFONTSCALAR);
    btn->setSize(btn->getHeight() + juce::roundToInt(font.getStringWidth(btn->getButtonText() + textSuffix)), heightIn);
}

static void drawPathToFillBounds(juce::Graphics& g, const juce::Path& path, juce::Rectangle<float> boundsToFill)
{
    juce::AffineTransform transform = juce::RectanglePlacement().getTransformToFit(
        path.getBounds(),
        boundsToFill
    );

    g.strokePath(path, juce::PathStrokeType(0.8f), transform);
}

// Creates the Lumatone logomark in 1x1 floating bit path
static juce::Path createLogomark()
{
    juce::Path logo;
    juce::Point<float> center(0.5f, 0.5f);

    float phi2 = PHI * 2;
    float innerRad = 1.0f / phi2;
    float outerRad = phi2 * 0.125f;
    float ang = juce::MathConstants<float>::pi * 0.083333f;
    float angOff = ang * 0.5f;

    logo.addPolygon(center, 6, innerRad, ang - angOff);
    logo.addPolygon(center, 6, HEXRADIUSTOLATERAL * innerRad, -ang - angOff);
    logo.addPolygon(center, 6, outerRad, ang - angOff);
    logo.addPolygon(center, 6, outerRad, -ang - angOff);

    return logo;
}

static juce::Path getArrowPath(juce::Point<float> start, juce::Point<float> headPoint, float headWidth, float headHeight)
{
    headWidth *= 0.5f;
    auto left = juce::Point<float>(start.x - headWidth, headHeight);
    auto right = juce::Point<float>(start.x + headWidth, headHeight);

    juce::Path path;

    path.startNewSubPath(start);
    path.lineTo(headPoint);
    path.closeSubPath();
    path.startNewSubPath(headPoint);
    path.lineTo(left);
    path.closeSubPath();
    path.startNewSubPath(headPoint);
    path.lineTo(right);
    path.closeSubPath();

    return path;
}

static juce::Path getFolderIcon(float folderHeight, float tabBaseX, float tabBaseY)
{
    juce::Path path;

    float tabBaseSlopeX = tabBaseX - 0.1f;
    float folderTop = 1.0f - folderHeight;

    auto v0 = juce::Point<float>(0.0f, folderTop);
    auto v1 = juce::Point<float>(0.0f, folderHeight);
    auto v2 = juce::Point<float>(1.0f, folderHeight);
    auto v3 = juce::Point<float>(1.0f, tabBaseY);
    auto v4 = juce::Point<float>(tabBaseX, tabBaseY);
    auto v5 = juce::Point<float>(tabBaseSlopeX, folderTop);

    path.startNewSubPath(v0);
    path.lineTo(v1);
    path.lineTo(v2);
    path.lineTo(v3);
    path.lineTo(v4);
    path.lineTo(v5);
    path.closeSubPath();

    return path;
}

static void drawFolderIconAt(juce::Graphics& g, int x, int y, int width, int height, juce::Colour folderColour, juce::Colour arrowColour)
{
    float folderHeight = 1.0f;
    juce::Path folder = getFolderIcon(0.91f, 0.5f, 0.22f).createPathWithRoundedCorners(0.05f);

    auto transform = juce::AffineTransform::scale(width, height).followedBy(juce::AffineTransform::translation(x, y));
    folder.applyTransform(transform);

    g.setColour(folderColour);
    g.fillPath(folder);

    juce::Path arrowPath = getArrowPath(juce::Point<float>(0.5f, folderHeight - 0.267f), juce::Point<float>(0.5f, 0.35f), 0.2f, 0.46f);
    arrowPath.applyTransform(transform);

    juce::PathStrokeType stroke(1.25f);
    stroke.setEndStyle(juce::PathStrokeType::EndCapStyle::rounded);
    stroke.setJointStyle(juce::PathStrokeType::JointStyle::curved);

    g.setColour(arrowColour);
    g.strokePath(arrowPath, stroke);
}

static juce::Path getSaveIconPath()
{
    float boxHeight = 0.6f;
    float boxWidth = 1.0f;
    boxWidth *= 0.5f;

    float boxLeft = 0.5f - boxWidth;
    float boxRight = 0.5f + boxWidth;

    // Box
    auto v0 = juce::Point<float>(boxLeft, boxHeight);
    auto v1 = juce::Point<float>(boxLeft, 1.0f);
    auto v2 = juce::Point<float>(boxRight, 1.0f);
    auto v3 = juce::Point<float>(boxRight, boxHeight);

    juce::Path path;
    path.startNewSubPath(v0);
    path.lineTo(v1);
    path.closeSubPath();
    path.startNewSubPath(v1);
    path.lineTo(v2);
    path.closeSubPath();
    path.startNewSubPath(v2);
    path.lineTo(v3);
    path.closeSubPath();

    juce::Path rounded = path.createPathWithRoundedCorners(0.01f);

    juce::Path arrow = getArrowPath(juce::Point<float>(0.5f, 0.0f), juce::Point<float>(0.5f, 0.667f), 0.4f, 0.5f);
    rounded.addPath(arrow);

    return rounded;
}

static void getCCPolarityIconPath(bool inverted, juce::Path& arrowPath, juce::Path& faderPath)
{
    float glyphWidth = 0.25f;

    float y1 = 0.8f;
    float y2 = 1.0f - y1;

    float arrowX = 0.3f;
    float arrowHeadDif = abs(y1 - y2) * 0.333;

    float faderMax = 0.81f;
    float faderEdge = faderMax - glyphWidth;

    if (inverted)
    {
        arrowPath = getArrowPath(juce::Point<float>(arrowX, y1), juce::Point<float>(arrowX, y2), glyphWidth, y2 + arrowHeadDif);

        faderPath.startNewSubPath(faderEdge, y1);
        faderPath.lineTo(faderEdge, y2);
        faderPath.lineTo(faderMax, y2);
        faderPath.closeSubPath();
    }
    else
    {
        arrowPath = getArrowPath(juce::Point<float>(arrowX, y2), juce::Point<float>(arrowX, y1), glyphWidth, y1 - arrowHeadDif);

        faderPath.startNewSubPath(faderEdge, y1);
        faderPath.lineTo(faderMax, y1);
        faderPath.lineTo(faderEdge, y2);
        faderPath.closeSubPath();
    }
}

static juce::Path getCloneIconPath()
{
// side-by-side
//    float yMargin = 0.1f;
//    float height = 1.0f - yMargin * 2.0f;
//
//    float width = 5.0f/12.0f;
//
//    auto leftRect = Rectangle<float>(1.0/12.0f, yMargin, width, height);
//    auto rightRect = Rectangle<float>(width, yMargin, width, height);

    // bottom left overlapping top right

    float xMargin = 0.1f;
    float yMargin = 0.1f;
    float size = 0.5f;

    auto leftRect = juce::Rectangle<float>(xMargin, 1.0f - yMargin - size, size, size);
    auto rightRect = juce::Rectangle<float>(1.0f - xMargin - size, yMargin, size, size);

    auto path = juce::Path();
    path.addRoundedRectangle(leftRect, 0.1f, 0.1f);
    path.addRoundedRectangle(rightRect, 0.1f, 0.1f);

    return path;
}

// TODO: clean up / make a better routine with ImageCache usage
static juce::Image getCachedCloneImage()
{
    auto cloneImg = juce::ImageCache::getFromHashCode((juce::int64)LumatoneAssets::ID::CloneIcon);
    if (cloneImg.isValid())
        return cloneImg;

    // Create duplicate icon
    auto cloneIcon = getCloneIconPath();
    cloneIcon.scaleToFit(0, 0, 80, 80, true);

    cloneImg = juce::Image(juce::Image::PixelFormat::ARGB, 100, 100, true);
    juce::Graphics cloneG(cloneImg);
    cloneG.setColour(juce::Colours::white.darker(0.1f));
    cloneG.setOrigin(juce::Point<int>(10, 10));
    auto stroke = juce::PathStrokeType(8.0f, juce::PathStrokeType::JointStyle::curved);
    cloneG.strokePath(cloneIcon, stroke);
    juce::ImageCache::addImageToCache(cloneImg, (juce::int64)LumatoneAssets::ID::CloneIcon);
    return cloneImg;
}

static juce::Path getPickerIconPath()
{
    const unsigned char pathData[] = {
        110,109,106,188,149,65,199,75,13,65,98,47,221,143,65,226,122,26,65,126,106,144,65,33,176,46,65,0,0,151,65,250,126,58,65,108,
        4,86,140,65,29,90,82,65,108,90,100,33,65,42,92,207,64,108,106,188,54,65,22,174,159,64,98,84,227,67,65,102,59,183,64,196,32,88,
        65,245,253,180,64,84,227,99,65,143,151,154,64,108,207,247,141,65,6,0,128,63,98,70,182,146,65,92,139,172,62,174,71,153,65,0,
        0,64,53,60,223,159,65,0,0,64,53,98,52,51,173,65,0,0,64,53,1,0,184,65,248,210,173,63,1,0,184,65,122,190,63,64,98,1,0,184,65,28,4,
        110,64,255,212,181,65,194,202,141,64,157,196,177,65,51,8,160,64,108,108,188,149,65,200,75,13,65,99,109,255,255,39,65,0,0,112,
        65,108,82,227,249,64,0,0,112,65,108,192,202,59,65,64,53,42,65,108,157,239,35,65,72,225,20,65,108,218,36,22,64,192,202,144,65,
        98,248,212,136,63,14,45,156,65,48,51,11,64,71,225,162,65,69,55,153,63,63,53,177,65,98,151,239,135,63,245,40,179,65,242,255,127,
        63,73,12,181,65,242,255,127,63,192,202,182,65,98,242,255,127,63,122,20,188,65,178,30,197,63,0,0,192,65,34,6,9,64,0,0,192,65,
        98,173,114,24,64,0,0,192,65,16,131,40,64,53,94,191,65,38,92,55,64,182,243,189,65,98,208,34,147,64,198,75,179,65,185,73,176,64,
        225,122,188,65,62,53,218,64,168,198,176,65,108,30,133,107,65,72,225,84,65,108,251,169,83,65,80,141,63,65,108,255,255,39,65,0,
        0,112,65,99,101,0,0
        };

    juce::Path path;
    path.loadPathFromData (pathData, sizeof (pathData));
    return path;
}

enum LumatoneEditorIcon
{
    Checkmark   = 0x01,
    ArrowUp,
    ArrowDown,
    SaveIcon,
    LoadIcon,
    CCPolarityIcon,
    ColourPicker
};

enum LumatoneEditorColourIDs
{
    TitlePink,
    LabelPink,
    LabelBlue,
    DisconnectedRed,
    ConnectedGreen,
    HeaderBackground,
    MediumBackground,
    LightBackground,
    ControlAreaHeader,
    ControlAreaBackground,
    ControlBoxHighlighted,
    ColourPaletteBackground,
    ActiveText,
    InactiveText,
    DescriptionText,
    NumKeySelectedText,
    ControlBoxBackground,
    DefaultPresetButtonActive,
    DefaultPresetButtonInactive,
    OutlineColourId,
    CurveGradientMin,
    CurveGradientMax,
    CurveGridColour,
    DisabledOverlay,
    MenuBarBackground,

    ExprRotaryGradientMax,
    ExprRotaryGradientMin,
    BrightnessRotaryGradientMin,
    BrightnessRotaryGradientMax,
    HueRotaryGradientMin,
    HueRotaryGradientMid,
    HueRotaryGradientMax,
    TempRotaryGradientMin,
    TempRotaryGradientMid,
    TempRotaryGradientMax
};

enum LumatoneEditorColourGradients
{
    ExpressionSlider,
    BrightnessSlider,
    HueSlider,
    TemperatureSlider
};

namespace LumatoneEditorStyleIDs
{
    static juce::Identifier fontOverride = juce::Identifier("AppFontIndex");
    // TODO: Review with new Font access method
    static juce::Identifier fontOverrideTypefaceStyle = juce::Identifier("ComponentTypefaceStyle");

    // A component with text can have it scaled with this property
    static juce::Identifier fontHeightScalar = juce::Identifier("FontHeightScalar");

    // Override background colour for a juce::Label's juce::TextEditor when being edited
    static juce::Identifier labelTextEditorBackgroundColour = juce::Identifier("LabelTextEditorBackgroundColour");

    static juce::Identifier labelMaximumLineCount = juce::Identifier("LabelMaximumLineCount");

    static juce::Identifier tabbedButtonBarDepthScalar = juce::Identifier("TabbedButtonBarDepthScalar");

    // The LumatoneEditorIcon hash code for an icon that should be displayed on a juce::TextButton
    static juce::Identifier textButtonIconHashCode = juce::Identifier("TextButtonIconHashCode");

    // Choose where to put the icon on a juce::TextButton
    static juce::Identifier textButtonIconPlacement = juce::Identifier("TextButtonIconPlacement");
    enum TextButtonIconPlacement
    {
        LeftOfText = 0,
        RightOfText
    };

    // A juce::TextButton will have an invisible background and an underline if it's properties contains this
    static juce::Identifier textButtonHyperlinkFlag = juce::Identifier("TextButtonHyperlinkFlag");

    static juce::Identifier popupMenuMaxColumns = juce::Identifier("PopupMenuMaxColumns");
    // Uses the width of the target component of a menu instead of parameters
    static juce::Identifier popupMenuTargetWidth = juce::Identifier("PopupMenuTargetWidth");

    static juce::Identifier popupMenuBackgroundColour = juce::Identifier("PopupMenuBackgroundColour");

    static juce::Identifier connectedEdgeFlags = juce::Identifier("ConnectedEdgesFlags");

    // Odd values will have top right and bottom left rounded corners
    // Even values will have top left and bottom right rounded corners
    static juce::Identifier roundedDiagonalCorners = juce::Identifier("RoundedDiagonalCorners");

    // Flag to not draw a value for juce::Slider LookAndFeel methods
    static juce::Identifier sliderValueNull = juce::Identifier("SliderValueNull");

    // Apply set of characters to restrict juce::ComboBox input to
    static juce::Identifier comboBoxEditorRestrictedChars = juce::Identifier("ComboBoxEditorRestrictedChars");
    static juce::Identifier comboBoxEditorRestrictedLength = juce::Identifier("ComboBoxEditorRestrictedLength");

    // Check juce::ComboBox items for colour hex strings, render if they pass
    static juce::Identifier comboBoxRenderColourItems = juce::Identifier("ComboBoxRenderColourItems");

    // Override the width of the pop up items, to account for varied string lengths
    static juce::Identifier comboBoxPopupItemWidthOverride = juce::Identifier("ComboBoxPopupItemWidthOverride");

    // Specify LumatoneEditorColourGradient id for rotary juce::Slider
    static juce::Identifier sliderRotaryColourGradient = juce::Identifier("SliderRotaryColourGradient");
}

// LookAndFeel doesn't have Slider IncDec button access in drawIncDecButtonsBackground
class TextButtonMouseHighlight : public juce::TextButton
{

    juce::Colour highlightColour;

public:

    TextButtonMouseHighlight(juce::Colour highlightColourIn = juce::Colour())
        : highlightColour(highlightColourIn) {}

    ~TextButtonMouseHighlight() {}

    void paint(juce::Graphics& g) override
    {
        juce::TextButton::paint(g);

        if (isEnabled() && isMouseOver())
        {
            auto bounds = getLocalBounds().toFloat();

            juce::Colour c = (isMouseButtonDown()) ? highlightColour.overlaidWith(juce::Colours::white.withAlpha(0.1f))
                                                   : highlightColour;
            g.setColour(c);
            g.fillRoundedRectangle(bounds, getHeight() * 0.25f);
        }
    }
};

static void findIdealComboBoxNumColumns(juce::ComboBox* box, int numItems)
{
    double sqNumItems = sqrt((double)numItems);
    int maxColumns = (int)(sqNumItems);

    if (maxColumns != sqNumItems)
    {
        // get prime factors
        auto factors = getFactors(numItems);

        // find largest exponent
        int maxF = 1;
        int maxExp = 1;
        for (int i = 0; i < juce::jmin(factors.size(), 100); i++)
        {
            int iExp = factors[i];
            if (iExp >= maxExp)
            {
                maxExp = iExp;
                maxF = (int)pow(PRIMES[i], maxExp);
            }
        }

        if (maxExp > 4)
            maxColumns = 8;
        else if (maxExp > 2)
            maxColumns = 4;
        else if (maxExp > 1)
            maxColumns = 2;
        else
        {
            if (maxF > 8)
            {
                maxColumns = sqrt(maxF);
            }
            else
                maxColumns = 1;
        }
    }

    box->getProperties().set(LumatoneEditorStyleIDs::popupMenuMaxColumns, juce::var(maxColumns));
}
