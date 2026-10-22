#include <gui/timescreen_screen/timeScreenView.hpp>

#ifndef SIMULATOR
#include "stm32h7xx_hal.h"

extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef sTime;
extern RTC_DateTypeDef sDate;
#endif

timeScreenView::timeScreenView()
: myButtonCallback(this, &timeScreenView::buttonClickHandler),
      hour_set(12),
      minute_set(0),
      initial_set(false)
{
}

void timeScreenView::setupScreen()
{
    timeScreenViewBase::setupScreen();

    /* Set custom button handlers for time screen adjustment buttons */
    btnAddHour.setAction(myButtonCallback);
    btnMinusHour.setAction(myButtonCallback);
    btnAddMinute.setAction(myButtonCallback);
    btnMinusMinute.setAction(myButtonCallback);
    btnSaveTime.setAction(myButtonCallback);

    /* Set initial wildcard strings */
    Unicode::snprintf(txtHourBuffer, TXTHOUR_SIZE, "%02d", hour_set);
    txtHour.invalidate();
    Unicode::snprintf(txtMinuteBuffer, TXTMINUTE_SIZE, "%02d", minute_set);
    txtMinute.invalidate();
}

void timeScreenView::tearDownScreen()
{
    timeScreenViewBase::tearDownScreen();
}

void timeScreenView::buttonClickHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &btnAddHour)
    {
        hourUpClicked();
    }
    else if (&src == &btnMinusHour)
    {
        hourDownClicked();
    }
    else if (&src == &btnAddMinute)
    {
        minuteUpClicked();
    }
    else if (&src == &btnMinusMinute)
    {
        minuteDownClicked();
    }
    else if (&src == &btnSaveTime)
    {
        SaveTimeToRTC();
    }
}

void timeScreenView::hourUpClicked()
{
    hour_set = (hour_set + 1) % 24;
    Unicode::snprintf(txtHourBuffer, TXTHOUR_SIZE, "%02d", hour_set);
    txtHour.invalidate();
}

void timeScreenView::hourDownClicked()
{
    hour_set = (hour_set + 24 - 1) % 24;
    Unicode::snprintf(txtHourBuffer, TXTHOUR_SIZE, "%02d", hour_set);
    txtHour.invalidate();
}

void timeScreenView::minuteUpClicked()
{
    minute_set = (minute_set + 1) % 60;
    Unicode::snprintf(txtMinuteBuffer, TXTMINUTE_SIZE, "%02d", minute_set);
    txtMinute.invalidate();
}

void timeScreenView::minuteDownClicked()
{
    minute_set = (minute_set + 60 - 1) % 60;
    Unicode::snprintf(txtMinuteBuffer, TXTMINUTE_SIZE, "%02d", minute_set);
    txtMinute.invalidate();
}

void timeScreenView::updateTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    digitalClock1.setTime24Hour(hour, minute, second);
    digitalClock1.invalidate();

    /* Auto-initialize hour_set/minute_set from current RTC time on first update */
    if (!initial_set)
    {
        hour_set = hour;
        minute_set = minute;
        initial_set = true;

        Unicode::snprintf(txtHourBuffer, TXTHOUR_SIZE, "%02d", hour_set);
        txtHour.invalidate();
        Unicode::snprintf(txtMinuteBuffer, TXTMINUTE_SIZE, "%02d", minute_set);
        txtMinute.invalidate();
    }
}

void timeScreenView::SaveTimeToRTC()
{
#ifndef SIMULATOR
    sTime.Hours = hour_set;
    sTime.Minutes = minute_set;
    sTime.Seconds = 0;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;

    sDate.WeekDay = RTC_WEEKDAY_THURSDAY;
    sDate.Month = RTC_MONTH_AUGUST;
    sDate.Date = 0x9;
    sDate.Year = 0x18;

    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
#endif
}
