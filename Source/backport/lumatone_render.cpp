#include "lumatone_render.h"
#include "./colour_model.h"

#include "../LumatoneController.h"

LumatoneRender::LumatoneRender(LumatoneController* stateIn)
//    : LumatoneState("LumatoneRender", stateIn)
    : state(stateIn)
{
    imageProcessor.reset(new ImageProcessor());
}

LumatoneRender::~LumatoneRender()
{
    imageProcessor = nullptr;
}

void LumatoneRender::resetOctaveSize()
{
    lumatoneGeometry = LumatoneGeometry(GetLumatoneBoardSize(state->getOctaveBoardSize()));
    calibration = CalibrationPoints{};
    recalibrate();
}

void LumatoneRender::recalibrate()
{
    juce::Point<float> pt1Key1  (calibration.oct1Key1X,  calibration.oct1Key1Y);
    juce::Point<float> pt1Key56 (calibration.oct1Key56X, calibration.oct1Key56Y);
    juce::Point<float> pt5Key7  (calibration.oct5Key7X,  calibration.oct5Key7Y);

    tilingGeometry.fitSkewedTiling(pt1Key1, pt1Key56, 10, pt5Key7, 24, true);
    keyCentres = tilingGeometry.getHexagonCentresSkewed(lumatoneGeometry, 0, state->getNumBoards());
}

juce::Array<juce::Point<float>> LumatoneRender::getKeyCentres()
{
    return tilingGeometry.getHexagonCentresSkewed(lumatoneGeometry, 0, state->getNumBoards());
}

void LumatoneRender::render(LumatoneAssets::LumatoneGraphicRenderSize maxRenderSize, LumatoneColourModel* colourModel)
{
    int width = LumatoneAssets::LumatoneKeyboardRenderWidth(maxRenderSize);
    int height = LumatoneAssets::LumatoneKeyboardRenderHeight(maxRenderSize);

    int keyWidth = juce::roundToInt(width * keyW);
    int keyHeight = juce::roundToInt(height * keyH);

    juce::Image renderImage = juce::Image(juce::Image::PixelFormat::ARGB, width, height, true);
    juce::Graphics g(renderImage);

    g.drawImageAt(LumatoneAssets::getImage(LumatoneAssets::ID::LumatoneGraphic, height, width), 0, 0);

    auto shapeLayer = LumatoneAssets::getImage(LumatoneAssets::ID::KeyShape, keyHeight, keyWidth);
    auto shadowLayer = LumatoneAssets::getImage(LumatoneAssets::ID::KeyShadow, keyHeight, keyWidth);

    int keyNum = 0;
    for (int boardIndex = 0; boardIndex < state->getNumBoards(); boardIndex++)
    {
        for (int keyIndex = 0; keyIndex < state->getOctaveBoardSize(); keyIndex++)
        {
            juce::Colour keyColour = state->getKey(boardIndex, keyIndex).getColour();
            if (colourModel)
                keyColour = colourModel->getModelColour(keyColour);

            juce::Point<int> keyPos = juce::Point<int>(
                juce::roundToInt(keyCentres[keyNum].x * width - keyWidth * 0.5f),
                juce::roundToInt(keyCentres[keyNum].y * height - keyHeight * 0.5f)
            );

            if (!keyColour.isTransparent())
            {
                g.setColour(keyColour);
                g.drawImageAt(shapeLayer, keyPos.x, keyPos.y, true);
            }

            g.drawImageAt(shadowLayer, keyPos.x, keyPos.y);

            keyNum++;
        }
    }

    // Add rescaled renders to cache

    for (int renderSize = 0; renderSize <= (int)maxRenderSize; renderSize++)
    {
        auto size = (LumatoneAssets::LumatoneGraphicRenderSize)renderSize;
        int renderWidth = LumatoneAssets::LumatoneKeyboardRenderWidth(size);
        int renderHeight = LumatoneAssets::LumatoneKeyboardRenderHeight(size);

        juce::Image resized;
        if (renderHeight == height || renderWidth == width) // graceful handling
        {
            resized = renderImage;
        }
        else
        {
            resized = imageProcessor->resizeImage(renderImage, renderWidth, renderHeight);
        }

        renders.set((int)size, resized);
    }
}

juce::Image LumatoneRender::getResizedAsset(LumatoneAssets::ID assetId, int targetWidth, int targetHeight, bool useJuceResize)
{
    if (targetWidth == 0 || targetHeight == 0)
        return juce::Image();

    auto cachedImage = LumatoneAssets::getImage(assetId, targetHeight, targetWidth);

    if (cachedImage.isNull())
        return cachedImage;

    if (targetWidth == cachedImage.getWidth() && targetHeight == cachedImage.getHeight())
        return cachedImage;
    
    if (useJuceResize)
        return cachedImage.rescaled(targetWidth, targetHeight, juce::Graphics::ResamplingQuality::highResamplingQuality);

    return imageProcessor->resizeImage(cachedImage, targetWidth, targetHeight);
}

juce::Image LumatoneRender::getResizedRender(int targetWidth, int targetHeight)
{
    if (targetWidth == 0 || targetHeight == 0)
        return juce::Image();

    LumatoneAssets::LumatoneGraphicRenderSize size = LumatoneAssets::GetLumatoneRenderSize(targetWidth, targetHeight);

    juce::Image baseRender = renders[(int)size];

    if (baseRender.isNull())
        return baseRender;

    if (baseRender.getWidth() == targetWidth && baseRender.getHeight() == targetHeight)
        return baseRender;

    return imageProcessor->resizeImage(baseRender, targetWidth, targetHeight);
}
