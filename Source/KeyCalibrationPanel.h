/*
  ==============================================================================
    KeyCalibrationPanel.h
    Interactive panel for nudging the three anchor reference points that drive
    the hexagonal key-centre tiling in LumatoneRender.
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "backport/lumatone_render.h"

//==============================================================================
class KeyCalibrationPanel : public juce::Component
{
public:
    KeyCalibrationPanel(LumatoneRender* renderIn, std::function<void()> onChangedIn)
        : lumatoneRender(renderIn)
        , onChanged(std::move(onChangedIn))
    {
        jassert(lumatoneRender != nullptr);

        // Default nudge step
        nudgeStep = 0.0001f;

        // Build rows for each of the 6 calibration floats
        paramNames = { "oct1Key1 X", "oct1Key1 Y", "oct1Key56 X", "oct1Key56 Y", "oct5Key7 X", "oct5Key7 Y" };

        for (int i = 0; i < 6; i++)
        {
            auto* lbl = new juce::Label({}, paramNames[i]);
            lbl->setJustificationType(juce::Justification::centredRight);
            addAndMakeVisible(labels.add(lbl));

            auto* val = new juce::Label({}, juce::String(getParamValue(i), 7));
            val->setJustificationType(juce::Justification::centred);
            val->setEditable(true);
            val->onTextChange = [this, i, val]()
            {
                float v = val->getText().getFloatValue();
                setParamValue(i, v);
                val->setText(juce::String(getParamValue(i), 7), juce::dontSendNotification);
                fireUpdate();
            };
            addAndMakeVisible(valueLabels.add(val));

            auto* btnMinus = new juce::TextButton("-");
            btnMinus->onClick = [this, i]() { nudge(i, -nudgeStep); };
            addAndMakeVisible(minusButtons.add(btnMinus));

            auto* btnPlus = new juce::TextButton("+");
            btnPlus->onClick = [this, i]() { nudge(i, +nudgeStep); };
            addAndMakeVisible(plusButtons.add(btnPlus));
        }

        // Nudge step row
        auto* stepLbl = new juce::Label({}, "Nudge step");
        stepLbl->setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(labels.add(stepLbl));

        nudgeStepLabel.reset(new juce::Label({}, juce::String(nudgeStep, 6)));
        nudgeStepLabel->setJustificationType(juce::Justification::centred);
        nudgeStepLabel->setEditable(true);
        nudgeStepLabel->onTextChange = [this]()
        {
            float v = nudgeStepLabel->getText().getFloatValue();
            if (v > 0.0f) nudgeStep = v;
            nudgeStepLabel->setText(juce::String(nudgeStep, 6), juce::dontSendNotification);
        };
        addAndMakeVisible(*nudgeStepLabel);

        // Placeholder spacer labels so column alignment is consistent
        for (int i = 0; i < 2; i++)
        {
            auto* sp = new juce::Label();
            addAndMakeVisible(stepSpacers.add(sp));
        }

        // Key size row
        keySizeLbl.reset(new juce::Label({}, "Key Size"));
        keySizeLbl->setJustificationType(juce::Justification::centredRight);
        addAndMakeVisible(*keySizeLbl);

        keySizeValueLabel.reset(new juce::Label({}, juce::String(1.06f, 4)));
        keySizeValueLabel->setJustificationType(juce::Justification::centred);
        keySizeValueLabel->setEditable(true);
        keySizeValueLabel->onTextChange = [this]()
        {
            float v = keySizeValueLabel->getText().getFloatValue();
            if (setKeyScaleFn && v > 0.0f)
            {
                setKeyScaleFn(v);
                if (getKeyScaleFn)
                    keySizeValueLabel->setText(juce::String(getKeyScaleFn(), 4), juce::dontSendNotification);
            }
        };
        addAndMakeVisible(*keySizeValueLabel);

        btnKeySizeMinus.reset(new juce::TextButton("-"));
        btnKeySizeMinus->onClick = [this]()
        {
            if (getKeyScaleFn && setKeyScaleFn)
            {
                setKeyScaleFn(getKeyScaleFn() - 0.005f);
                keySizeValueLabel->setText(juce::String(getKeyScaleFn(), 4), juce::dontSendNotification);
            }
        };
        addAndMakeVisible(*btnKeySizeMinus);

        btnKeySizePlus.reset(new juce::TextButton("+"));
        btnKeySizePlus->onClick = [this]()
        {
            if (getKeyScaleFn && setKeyScaleFn)
            {
                setKeyScaleFn(getKeyScaleFn() + 0.005f);
                keySizeValueLabel->setText(juce::String(getKeyScaleFn(), 4), juce::dontSendNotification);
            }
        };
        addAndMakeVisible(*btnKeySizePlus);

        // Reset button
        btnReset.reset(new juce::TextButton("Reset Defaults"));
        btnReset->onClick = [this]()
        {
            lumatoneRender->calibration = LumatoneRender::CalibrationPoints{};
            refreshValueLabels();
            fireUpdate();
        };
        addAndMakeVisible(*btnReset);

        // Overlay toggle
        btnOverlay.reset(new juce::TextButton("Show Overlay"));
        btnOverlay->setClickingTogglesState(true);
        btnOverlay->setToggleState(false, juce::dontSendNotification);
        btnOverlay->onClick = [this]()
        {
            lumatoneRender->showCalibrationOverlay = btnOverlay->getToggleState();
            if (onChanged) onChanged();
        };
        addAndMakeVisible(*btnOverlay);

        // Print / copy values button
        btnPrint.reset(new juce::TextButton("Copy Values"));
        btnPrint->onClick = [this]()
        {
            auto& c = lumatoneRender->calibration;
            juce::String text =
                "oct1Key1X  = " + juce::String(c.oct1Key1X,  7) + "f\n"
                "oct1Key1Y  = " + juce::String(c.oct1Key1Y,  7) + "f\n"
                "oct1Key56X = " + juce::String(c.oct1Key56X, 7) + "f\n"
                "oct1Key56Y = " + juce::String(c.oct1Key56Y, 7) + "f\n"
                "oct5Key7X  = " + juce::String(c.oct5Key7X,  7) + "f\n"
                "oct5Key7Y  = " + juce::String(c.oct5Key7Y,  7) + "f\n"
                "keySize    = " + juce::String(getKeyScaleFn ? getKeyScaleFn() : 1.06f, 4) + "f\n";
            juce::SystemClipboard::copyTextToClipboard(text);
            DBG(text);
        };
        addAndMakeVisible(*btnPrint);

        // Paste values button
        btnPaste.reset(new juce::TextButton("Paste Values"));
        btnPaste->onClick = [this]()
        {
            auto lines = juce::StringArray::fromLines(juce::SystemClipboard::getTextFromClipboard());
            auto& c = lumatoneRender->calibration;

            for (auto& line : lines)
            {
                int eqPos = line.indexOf("=");
                if (eqPos < 0) continue;

                juce::String key = line.substring(0, eqPos).trim();
                juce::String valStr = line.substring(eqPos + 1).trim();
                if (valStr.endsWithChar('f') || valStr.endsWithChar('F'))
                    valStr = valStr.dropLastCharacters(1);

                float v = valStr.getFloatValue();

                if      (key == "oct1Key1X")  c.oct1Key1X  = v;
                else if (key == "oct1Key1Y")  c.oct1Key1Y  = v;
                else if (key == "oct1Key56X") c.oct1Key56X = v;
                else if (key == "oct1Key56Y") c.oct1Key56Y = v;
                else if (key == "oct5Key7X")  c.oct5Key7X  = v;
                else if (key == "oct5Key7Y")  c.oct5Key7Y  = v;
                else if (key == "keySize" && setKeyScaleFn) setKeyScaleFn(v);
            }

            refreshValueLabels();
            if (getKeyScaleFn)
                keySizeValueLabel->setText(juce::String(getKeyScaleFn(), 4), juce::dontSendNotification);
            fireUpdate();
        };
        addAndMakeVisible(*btnPaste);
    }

    ~KeyCalibrationPanel() override = default;

    void refreshValueLabels()
    {
        for (int i = 0; i < 6; i++)
            valueLabels[i]->setText(juce::String(getParamValue(i), 7), juce::dontSendNotification);
    }

    void setKeyScaleCallbacks(std::function<float()> getF, std::function<void(float)> setF)
    {
        getKeyScaleFn = std::move(getF);
        setKeyScaleFn = std::move(setF);
        if (getKeyScaleFn)
            keySizeValueLabel->setText(juce::String(getKeyScaleFn(), 4), juce::dontSendNotification);
    }

    void resized() override
    {
        const int rowH  = 28;
        const int pad   = 4;
        const int labelW = 90;
        const int valW   = 90;
        const int btnW   = 28;
        const int totalW = labelW + pad + btnW + pad + valW + pad + btnW;

        int x = (getWidth() - totalW) / 2;
        int y = pad;

        // 6 parameter rows
        for (int i = 0; i < 6; i++)
        {
            labels[i]->setBounds(x, y, labelW, rowH);
            int cx = x + labelW + pad;
            minusButtons[i]->setBounds(cx, y, btnW, rowH);
            cx += btnW + pad;
            valueLabels[i]->setBounds(cx, y, valW, rowH);
            cx += valW + pad;
            plusButtons[i]->setBounds(cx, y, btnW, rowH);
            y += rowH + pad;
        }

        // Divider gap
        y += pad;

        // Nudge step row (index 6 in labels array)
        labels[6]->setBounds(x, y, labelW, rowH);
        int cx = x + labelW + pad;
        stepSpacers[0]->setBounds(cx, y, btnW, rowH);   // blank where - would be
        cx += btnW + pad;
        nudgeStepLabel->setBounds(cx, y, valW, rowH);
        cx += valW + pad;
        stepSpacers[1]->setBounds(cx, y, btnW, rowH);   // blank where + would be
        y += rowH + pad * 2;

        // Key size row
        keySizeLbl->setBounds(x, y, labelW, rowH);
        int kscx = x + labelW + pad;
        btnKeySizeMinus->setBounds(kscx, y, btnW, rowH);
        kscx += btnW + pad;
        keySizeValueLabel->setBounds(kscx, y, valW, rowH);
        kscx += valW + pad;
        btnKeySizePlus->setBounds(kscx, y, btnW, rowH);
        y += rowH + pad * 2;

        // Bottom buttons
        int bW = (totalW - pad) / 2;
        btnOverlay->setBounds(x, y, totalW, rowH);
        y += rowH + pad;
        btnReset->setBounds(x, y, bW, rowH);
        btnPrint->setBounds(x + bW + pad, y, bW, rowH);
        y += rowH + pad;
        btnPaste->setBounds(x, y, totalW, rowH);
    }

    static int preferredHeight()
    {
        const int rowH = 28;
        const int pad  = 4;
        return (6 + 1) * (rowH + pad) + pad * 4 + (rowH + pad * 2) + (rowH + pad) + (rowH + pad) + rowH; // 6 params + step row + key size row + overlay btn + reset/copy + paste
    }

    static int preferredWidth() { return 90 + 4 + 28 + 4 + 90 + 4 + 28 + 16; }  // ≈ 264

private:
    void nudge(int paramIndex, float delta)
    {
        setParamValue(paramIndex, getParamValue(paramIndex) + delta);
        valueLabels[paramIndex]->setText(juce::String(getParamValue(paramIndex), 7), juce::dontSendNotification);
        fireUpdate();
    }

    void fireUpdate()
    {
        lumatoneRender->recalibrate();
        if (onChanged) onChanged();
    }

    float getParamValue(int i) const
    {
        auto& c = lumatoneRender->calibration;
        switch (i)
        {
            case 0: return c.oct1Key1X;
            case 1: return c.oct1Key1Y;
            case 2: return c.oct1Key56X;
            case 3: return c.oct1Key56Y;
            case 4: return c.oct5Key7X;
            case 5: return c.oct5Key7Y;
            default: return 0.0f;
        }
    }

    void setParamValue(int i, float v)
    {
        auto& c = lumatoneRender->calibration;
        switch (i)
        {
            case 0: c.oct1Key1X  = v; break;
            case 1: c.oct1Key1Y  = v; break;
            case 2: c.oct1Key56X = v; break;
            case 3: c.oct1Key56Y = v; break;
            case 4: c.oct5Key7X  = v; break;
            case 5: c.oct5Key7Y  = v; break;
            default: break;
        }
    }

    LumatoneRender* lumatoneRender;
    std::function<void()> onChanged;

    float nudgeStep;

    juce::StringArray paramNames;

    juce::OwnedArray<juce::Label>      labels;       // 6 param labels + 1 step label
    juce::OwnedArray<juce::Label>      valueLabels;  // 6 value displays
    juce::OwnedArray<juce::TextButton> minusButtons;
    juce::OwnedArray<juce::TextButton> plusButtons;
    juce::OwnedArray<juce::Label>      stepSpacers;

    std::unique_ptr<juce::Label>      nudgeStepLabel;
    std::unique_ptr<juce::TextButton> btnOverlay;
    std::unique_ptr<juce::TextButton> btnReset;
    std::unique_ptr<juce::TextButton> btnPrint;

    std::unique_ptr<juce::Label>      keySizeLbl;
    std::unique_ptr<juce::Label>      keySizeValueLabel;
    std::unique_ptr<juce::TextButton> btnKeySizeMinus;
    std::unique_ptr<juce::TextButton> btnKeySizePlus;
    std::unique_ptr<juce::TextButton> btnPaste;

    std::function<float()>         getKeyScaleFn;
    std::function<void(float)>     setKeyScaleFn;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyCalibrationPanel)
};
