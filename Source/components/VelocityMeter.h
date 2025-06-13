/**
 * @class VelocityMeter
 * @brief A JUCE component that visually displays MIDI velocity values with a decaying meter effect.
 *
 * The VelocityMeter class provides a simple vertical meter for displaying MIDI velocity values (0-127).
 * When a new velocity is set via setVelocity(), the meter fills to the corresponding level and then decays
 * smoothly back to zero. The decay rate and update frequency are configurable for smooth GUI performance.
 *
 * Usage:
 *   - Call setVelocity(int newVelocity) to update the meter with a new velocity value.
 *   - Add the component to your parent component and set its bounds as needed.
 *
 * Inherits from juce::Component and uses a private juce::Timer for decay animation.
 */
class VelocityMeter : public juce::Component, private juce::Timer
{
public:
    VelocityMeter()
    {
        setVelocity(0);
    }

    void setVelocity(int newVelocity)
    {
        velocity = juce::jlimit(0, 127, newVelocity);
        decayValue = static_cast<float>(velocity);
        repaint();

        startTimerHz(30); // ~33ms interval, smooth enough for GUI
    }

    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();

        // Draw background
        g.setColour(juce::Colours::darkgrey);
        g.fillRoundedRectangle(bounds, 4.0f);

        // Draw filled area
        if (decayValue > 0.0f)
        {
            const float fillRatio = decayValue / 127.0f;
            const float fillHeight = bounds.getHeight() * fillRatio;
            auto fillArea = bounds.withTop(bounds.getBottom() - fillHeight);

            g.setColour(juce::Colours::limegreen);
            g.fillRoundedRectangle(fillArea, 4.0f);
        }

        // Optional outline
        g.setColour(juce::Colours::black);
        g.drawRoundedRectangle(bounds, 4.0f, 1.0f);
    }

    void resized() override {}

private:
    int velocity = 0;
    float decayValue = 0.0f;

    void timerCallback() override
    {
        // Decay rate per frame (tweak for slower/faster decay)
        constexpr float decayRate = 2.0f;

        decayValue = std::max(0.0f, decayValue - decayRate);

        if (decayValue <= 0.0f)
            stopTimer();

        repaint();
    }
};
