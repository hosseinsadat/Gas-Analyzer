#ifndef O2CALSCREENVIEW_HPP
#define O2CALSCREENVIEW_HPP

#include <gui_generated/o2calscreen_screen/O2CalScreenViewBase.hpp>
#include <gui/o2calscreen_screen/O2CalScreenPresenter.hpp>
#include <touchgfx/Callback.hpp>

class O2CalScreenView : public O2CalScreenViewBase
{
public:
    O2CalScreenView();
    virtual ~O2CalScreenView() {}
    virtual void setupScreen() override;
    virtual void tearDownScreen() override;

    /* Update view with latest O2 raw and calibrated values */
    void updateO2Values(int16_t raw_o2, float cal_o2);

    /* Increase and decrease target span percentage */
    void increaseSpan();
    void decreaseSpan();

private:
    /* Custom button callback handler for calibration buttons */
    void buttonClickHandler(const touchgfx::AbstractButton& src);

    /* Helper function to update text display for target span */
    void updateSpanDisplay();

    /* Target O2 span percentage */
    float targetO2Span;

    /* Callback instances */
    touchgfx::Callback<O2CalScreenView, const touchgfx::AbstractButton&> myButtonCallback;
};

#endif // O2CALSCREENVIEW_HPP
