/*
  ==============================================================================
    DebugCalibrationDragView.h
    Zoomed interactive view of the Lumatone graphic. Renders key shapes at float
    positions, and overlays three draggable anchor handles (oct1Key1, oct1Key56,
    oct5Key7) that live-update LumatoneRender's calibration when dragged.
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "LumatoneController.h"
#include "backport/lumatone_render.h"
#include "backport/lumatone_assets.h"

//==============================================================================
class DebugCalibrationDragView : public juce::Component
{
public:
    DebugCalibrationDragView(LumatoneRender*     renderIn,
                        LumatoneController* controllerIn,
                        std::function<void()> onChangedIn)
        : lumatoneRender(renderIn)
        , controller(controllerIn)
        , onChanged(std::move(onChangedIn))
    {
        jassert(lumatoneRender  != nullptr);
        jassert(controller      != nullptr);
    }

    ~DebugCalibrationDragView() override = default;

    /** Called by the container when external nudge changes calibration. */
    void refresh()
    {
        keyCentres = lumatoneRender->getKeyCentres();
        repaint();
    }

    float getKeyScaleFactor() const { return lumatoneRender->keyScaleFactor; }

    void setKeyScaleFactor(float f)
    {
        lumatoneRender->keyScaleFactor = juce::jlimit(0.5f, 3.0f, f);
        resized();   // rebuilds cached key graphics at new size
        repaint();
    }

    void resized() override
    {
        updateBounds();

        const int imgW = (int)lumatoneBounds.getWidth();
        const int imgH = (int)lumatoneBounds.getHeight();
        const int kW   = juce::roundToInt(imgW * lumatoneRender->getKeyW() * lumatoneRender->keyScaleFactor);
        const int kH   = juce::roundToInt(imgH * lumatoneRender->getKeyH() * lumatoneRender->keyScaleFactor);

        backgroundImage  = lumatoneRender->getResizedAsset(LumatoneAssets::ID::LumatoneGraphic, imgW, imgH);
        keyShapeGraphic  = lumatoneRender->getResizedAsset(LumatoneAssets::ID::KeyShape,  kW, kH);
        keyShadowGraphic = lumatoneRender->getResizedAsset(LumatoneAssets::ID::KeyShadow, kW, kH, true);

        keyCentres = lumatoneRender->getKeyCentres();
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0xff1a1a1a));

        if (backgroundImage.isValid())
            g.drawImage(backgroundImage, lumatoneBounds);

        if (keyCentres.isEmpty())
            return;

        const float bX = lumatoneBounds.getX();
        const float bY = lumatoneBounds.getY();
        const float bW = lumatoneBounds.getWidth();
        const float bH = lumatoneBounds.getHeight();
        const float kW = bW * lumatoneRender->getKeyW() * lumatoneRender->keyScaleFactor;
        const float kH = bH * lumatoneRender->getKeyH() * lumatoneRender->keyScaleFactor;

        // --- Key graphics ---
        if (keyShapeGraphic.isValid() && keyShadowGraphic.isValid())
        {
            const int numBoards   = controller->getNumBoards();
            const int octaveSize  = (numBoards > 0) ? keyCentres.size() / numBoards : 0;

            const int shapeW  = keyShapeGraphic.getWidth();
            const int shapeH  = keyShapeGraphic.getHeight();
            const int shadowW = keyShadowGraphic.getWidth();
            const int shadowH = keyShadowGraphic.getHeight();

            for (int i = 0; i < keyCentres.size(); ++i)
            {
                const float kx = keyCentres[i].x * bW + bX - kW * 0.5f;
                const float ky = keyCentres[i].y * bH + bY - kH * 0.5f;

                const int board = (octaveSize > 0) ? i / octaveSize : 0;
                const int key   = (octaveSize > 0) ? i % octaveSize : i;
                const juce::Colour keyColour = controller->getKey(board, key).getColour();

                if (!keyColour.isTransparent())
                {
                    g.setColour(keyColour);
                    g.drawImage(keyShapeGraphic, kx, ky, kW, kH, 0, 0, shapeW, shapeH, true);
                }

                g.drawImage(keyShadowGraphic, kx, ky, kW, kH, 0, 0, shadowW, shadowH);
            }
        }

        // --- Overlay: dots, anchor triangle, drag handles ---

        // White dot at every key centre
        const float dotR = 2.0f;
        g.setColour(juce::Colours::white.withAlpha(0.75f));
        for (auto& kc : keyCentres)
            g.fillEllipse(kc.x * bW + bX - dotR, kc.y * bH + bY - dotR, dotR * 2.f, dotR * 2.f);

        auto& c = lumatoneRender->calibration;
        const float ax1 = c.oct1Key1X  * bW + bX,  ay1 = c.oct1Key1Y  * bH + bY;
        const float ax2 = c.oct1Key56X * bW + bX,  ay2 = c.oct1Key56Y * bH + bY;
        const float ax3 = c.oct5Key7X  * bW + bX,  ay3 = c.oct5Key7Y  * bH + bY;

        // Triangle
        g.setColour(juce::Colours::yellow.withAlpha(0.55f));
        g.drawLine(ax1, ay1, ax2, ay2, 1.5f);
        g.drawLine(ax2, ay2, ax3, ay3, 1.5f);
        g.drawLine(ax3, ay3, ax1, ay1, 1.5f);

        // Draggable handles
        drawHandle(g, ax1, ay1, juce::Colours::red,       draggedAnchor == 0, hoveredAnchor == 0);
        drawHandle(g, ax2, ay2, juce::Colours::limegreen, draggedAnchor == 1, hoveredAnchor == 1);
        drawHandle(g, ax3, ay3, juce::Colours::cyan,      draggedAnchor == 2, hoveredAnchor == 2);

        // Labels
        g.setFont(11.0f);
        auto drawLabel = [&](float px, float py, juce::Colour col, const juce::String& text)
        {
            g.setColour(col);
            g.drawText(text, (int)(px + handleRadius + 3), (int)(py - 7), 80, 14,
                       juce::Justification::left, false);
        };
        drawLabel(ax1, ay1, juce::Colours::red,       "oct1 key1");
        drawLabel(ax2, ay2, juce::Colours::limegreen, "oct1 key56");
        drawLabel(ax3, ay3, juce::Colours::cyan,      "oct5 key7");
    }

    void mouseMove(const juce::MouseEvent& e) override
    {
        int h = anchorIndexAt(e.position);
        if (h != hoveredAnchor)
        {
            hoveredAnchor = h;
            setMouseCursor(h >= 0 ? juce::MouseCursor::DraggingHandCursor
                                  : juce::MouseCursor::NormalCursor);
            repaint();
        }
    }

    void mouseDown(const juce::MouseEvent& e) override
    {
        draggedAnchor = anchorIndexAt(e.position);
    }

    void mouseDrag(const juce::MouseEvent& e) override
    {
        if (draggedAnchor < 0) return;

        const float bW = lumatoneBounds.getWidth();
        const float bH = lumatoneBounds.getHeight();
        const float bX = lumatoneBounds.getX();
        const float bY = lumatoneBounds.getY();

        const float nx = juce::jlimit(0.0f, 1.0f, (e.position.x - bX) / bW);
        const float ny = juce::jlimit(0.0f, 1.0f, (e.position.y - bY) / bH);

        auto& c = lumatoneRender->calibration;
        switch (draggedAnchor)
        {
            case 0: c.oct1Key1X  = nx; c.oct1Key1Y  = ny; break;
            case 1: c.oct1Key56X = nx; c.oct1Key56Y = ny; break;
            case 2: c.oct5Key7X  = nx; c.oct5Key7Y  = ny; break;
            default: break;
        }

        lumatoneRender->recalibrate();
        keyCentres = lumatoneRender->getKeyCentres();
        if (onChanged) onChanged();
        repaint();
    }

    void mouseUp(const juce::MouseEvent&) override
    {
        draggedAnchor = -1;
    }

    void mouseExit(const juce::MouseEvent&) override
    {
        hoveredAnchor = -1;
        setMouseCursor(juce::MouseCursor::NormalCursor);
        repaint();
    }

private:
    static constexpr float handleRadius = 9.0f;
    static constexpr float imageAspect  = 530.0f / 212.0f;

    void updateBounds()
    {
        const float w = (float)getWidth();
        const float h = (float)getHeight();
        float imgW, imgH;
        if (w / h > imageAspect) { imgH = h; imgW = h * imageAspect; }
        else                     { imgW = w; imgH = w / imageAspect; }
        lumatoneBounds = { (w - imgW) * 0.5f, (h - imgH) * 0.5f, imgW, imgH };
    }

    int anchorIndexAt(juce::Point<float> pos) const
    {
        const float bX = lumatoneBounds.getX(), bY = lumatoneBounds.getY();
        const float bW = lumatoneBounds.getWidth(), bH = lumatoneBounds.getHeight();
        auto& c = lumatoneRender->calibration;

        const juce::Point<float> anchors[3] = {
            { c.oct1Key1X  * bW + bX, c.oct1Key1Y  * bH + bY },
            { c.oct1Key56X * bW + bX, c.oct1Key56Y * bH + bY },
            { c.oct5Key7X  * bW + bX, c.oct5Key7Y  * bH + bY }
        };

        for (int i = 0; i < 3; i++)
            if (pos.getDistanceFrom(anchors[i]) < handleRadius + 5.0f)
                return i;

        return -1;
    }

    void drawHandle(juce::Graphics& g, float px, float py,
                    juce::Colour colour, bool dragging, bool hovered) const
    {
        const float r = dragging ? handleRadius * 1.35f
                                 : (hovered ? handleRadius * 1.15f : handleRadius);

        g.setColour(colour.withAlpha(dragging ? 0.75f : 0.45f));
        g.fillEllipse(px - r, py - r, r * 2.f, r * 2.f);

        g.setColour(colour);
        g.drawEllipse(px - r, py - r, r * 2.f, r * 2.f, dragging ? 2.5f : 1.8f);

        const float arm = r + 7.0f;
        g.drawLine(px - arm, py, px + arm, py, 1.5f);
        g.drawLine(px, py - arm, px, py + arm, 1.5f);
    }

    LumatoneRender*     lumatoneRender;
    LumatoneController* controller;
    std::function<void()> onChanged;

    juce::Rectangle<float> lumatoneBounds;
    juce::Image backgroundImage;
    juce::Image keyShapeGraphic;
    juce::Image keyShadowGraphic;
    juce::Array<juce::Point<float>> keyCentres;

    int   draggedAnchor  = -1;
    int   hoveredAnchor  = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DebugCalibrationDragView)
};
