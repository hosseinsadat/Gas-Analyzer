#ifndef CO2CALSCREENVIEW_HPP
#define CO2CALSCREENVIEW_HPP

#include <gui_generated/co2calscreen_screen/CO2CalScreenViewBase.hpp>
#include <gui/co2calscreen_screen/CO2CalScreenPresenter.hpp>
#include <touchgfx/Callback.hpp>

class CO2CalScreenView : public CO2CalScreenViewBase
{
public:
    CO2CalScreenView();
    virtual ~CO2CalScreenView() {}
    virtual void setupScreen() override;
    virtual void tearDownScreen() override;

    /* Update view with latest CO2 raw and calibrated values */
    void updateCO2Values(uint16_t raw_co2, float cal_co2);

    /* Increase and decrease target span percentage */
    void increaseSpan();
    void decreaseSpan();

private:
    /* Custom button callback handler for calibration buttons */
    void buttonClickHandler(const touchgfx::AbstractButton& src);

    /* Helper function to update text display for target span */
    void updateSpanDisplay();

    /* Target CO2 span percentage */
    float targetCo2Span;

    /* Callback instances */
    touchgfx::Callback<CO2CalScreenView, const touchgfx::AbstractButton&> myButtonCallback;
};

#endif // CO2CALSCREENVIEW_HPP
