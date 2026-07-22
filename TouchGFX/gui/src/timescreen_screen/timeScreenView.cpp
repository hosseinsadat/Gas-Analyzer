#include <gui/timescreen_screen/timeScreenView.hpp>

#ifndef SIMULATOR
#include "stm32h7xx_hal.h"

extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef sTime ;
extern RTC_DateTypeDef sDate ;
#endif

timeScreenView::timeScreenView():hour_set(0),minute_set(0)
{

}

void timeScreenView::setupScreen()
{
    timeScreenViewBase::setupScreen();
}

void timeScreenView::tearDownScreen()
{
    timeScreenViewBase::tearDownScreen();
}

#ifndef SIMULATOR


void timeScreenView::hourUpClicked()
{
	//hour++;
	hour_set = (hour_set +1 )%24 ;

	Unicode::snprintf(txtHourBuffer, TXTHOUR_SIZE, "%02d",hour_set);
	txtHour.invalidate();
}
void timeScreenView::hourDownClicked()
{
	//hour--;
	hour_set = (hour_set + 24 -1 )% 24 ;
	Unicode::snprintf(txtHourBuffer, TXTHOUR_SIZE, "%02d",hour_set);
			txtHour.invalidate();
}

void timeScreenView::minuteUpClicked()
{
	//minute++;
	minute_set = ( minute_set + 1 )% 60 ;
	Unicode::snprintf(txtMinuteBuffer, TXTMINUTE_SIZE, "%02d",minute_set);
		txtMinute.invalidate();
}
void timeScreenView::minuteDownClicked()
{
	//minute--;
	minute_set = ( minute_set + 60 - 1 )% 60 ;
		Unicode::snprintf(txtMinuteBuffer, TXTMINUTE_SIZE, "%02d",minute_set);
			txtMinute.invalidate();
}
/*
void ClockView::handleTickEvent()
{
	Unicode::snprintf(hour_textAreaBuffer, HOUR_TEXTAREA_SIZE, "%d",hour);
				hour_textArea.invalidate();
	Unicode::snprintf(minute_textAreaBuffer, MINUTE_TEXTAREA_SIZE, "%d",minute);
				minute_textArea.invalidate();
}
*/
void timeScreenView::updateTime(uint8_t hour,uint8_t minute,uint8_t second)
{
digitalClock1.setTime24Hour(hour, minute, second);
digitalClock1.invalidate();
}
void timeScreenView::SaveTimeToRTC()

{
	sTime.Hours = hour_set;
	sTime.Minutes = minute_set;
	sTime.Seconds = 0;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sDate.WeekDay = RTC_WEEKDAY_THURSDAY; //
	sDate.Month = RTC_MONTH_AUGUST; //
	sDate.Date = 0x9; // date
	sDate.Year = 0x18; // year
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);



}
#endif
