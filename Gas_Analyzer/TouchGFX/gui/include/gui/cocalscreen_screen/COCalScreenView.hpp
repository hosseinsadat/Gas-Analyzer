#ifndef COCALSCREENVIEW_HPP
#define COCALSCREENVIEW_HPP

#include <gui_generated/cocalscreen_screen/COCalScreenViewBase.hpp>
#include <gui/cocalscreen_screen/COCalScreenPresenter.hpp>
#include <touchgfx/Callback.hpp>

class COCalScreenView : public COCalScreenViewBase
{
public:
    COCalScreenView();
    virtual ~COCalScreenView() {}
    virtual void setupScreen() override;
    virtual void tearDownScreen() override;

    /* Update view with latest CO raw and calibrated values */
    void updateCOValues(int16_t raw_co, float cal_co);

    /* Increase and decrease target span PPM */
    void increaseSpan();
    void decreaseSpan();

private:
    /* Custom button callback handler for calibration buttons */
    void buttonClickHandler(const touchgfx::AbstractButton& src);

    /* Helper function to update text display for target span */
    void updateSpanDisplay();

    /* Target CO span PPM */
    float targetCoSpan;

    /* Callback instances */
    touchgfx::Callback<COCalScreenView, const touchgfx::AbstractButton&> myButtonCallback;
};

#endif // COCALSCREENVIEW_HPP
