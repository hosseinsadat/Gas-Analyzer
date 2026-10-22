#ifndef TIMESCREENVIEW_HPP
#define TIMESCREENVIEW_HPP

#include <gui_generated/timescreen_screen/timeScreenViewBase.hpp>
#include <gui/timescreen_screen/timeScreenPresenter.hpp>
#include <touchgfx/Callback.hpp>

class timeScreenView : public timeScreenViewBase
{
public:
    timeScreenView();
    virtual ~timeScreenView() {}
    virtual void setupScreen() override;
    virtual void tearDownScreen() override;

    void hourUpClicked();
    void hourDownClicked();
    void minuteUpClicked();
    void minuteDownClicked();
    void updateTime(uint8_t hour, uint8_t minute, uint8_t second);
    void SaveTimeToRTC();

private:
    /* Custom button callback handler for time adjustments */
    void buttonClickHandler(const touchgfx::AbstractButton& src);

    /* Callback instances */
    touchgfx::Callback<timeScreenView, const touchgfx::AbstractButton&> myButtonCallback;

protected:
    uint8_t hour_set;
    uint8_t minute_set;
    bool initial_set;
};

#endif // TIMESCREENVIEW_HPP
