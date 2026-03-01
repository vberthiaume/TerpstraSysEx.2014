#ifndef LUMATONE_GRAPHICS_RENDER_H
#define LUMATONE_GRAPHICS_RENDER_H

#include "./lumatone_assets.h"

// #include "./lumatone_state.h"
#include "./lumatone_tiling.h"

#include "./resampler/image_resampler.h"

class LumatoneColourModel;
class LumatoneController;

class LumatoneRender// : private LumatoneState
{
public:

    struct CalibrationPoints
    {
        float oct1Key1X  = 0.0849709f;
        float oct1Key1Y  = 0.3394341f;
        float oct1Key56X = 0.2743560f;
        float oct1Key56Y = 0.8333349f;
        float oct5Key7X  = 0.8802155f;
        float oct5Key7Y  = 0.3574272f;
    };

    LumatoneRender(LumatoneController* stateIn);
    ~LumatoneRender();

    LumatoneGeometry& getLumatoneGeometry() { return lumatoneGeometry; }
    LumatoneTiling& getLumatoneTiling() { return tilingGeometry; }

    void resetOctaveSize();
    void recalibrate();

    CalibrationPoints calibration;
    bool  showCalibrationOverlay = false;
    float keyScaleFactor         = 1.0716f;

    juce::Array<juce::Point<float>> getKeyCentres();

    float getKeyW() const { return keyW; }
    float getKeyH() const { return keyH; }

    void render(LumatoneAssets::LumatoneGraphicRenderSize maxRenderSize=LumatoneAssets::LumatoneGraphicRenderSize::_4x, LumatoneColourModel* colourModel=nullptr);
    
    juce::Image getResizedRender(int targetWidth, int targetHeight);
    juce::Image getResizedAsset(LumatoneAssets::ID assetId, int targetWidth, int targetHeight, bool useJuceResize=false);

private:

    LumatoneController* state;

    LumatoneGeometry    lumatoneGeometry;
    LumatoneTiling      tilingGeometry;

    juce::Array<juce::Point<float>> keyCentres;

    std::unique_ptr<ImageProcessor> imageProcessor;

    juce::Image lumatoneGraphic;
    juce::Image keyShapeGraphic;
    juce::Image keyShadowGraphic;

    juce::Array<juce::Image> renders;

    const float keybedX = 0.06908748f;
    const float keyW = 0.027352f;
    const float keyH = 0.07307f;
};

#endif LUMATONE_GRAPHICS_RENDER_H
