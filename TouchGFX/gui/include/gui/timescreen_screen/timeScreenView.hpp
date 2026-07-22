#ifndef TIMESCREENVIEW_HPP
#define TIMESCREENVIEW_HPP

#include <gui_generated/timescreen_screen/timeScreenViewBase.hpp>
#include <gui/timescreen_screen/timeScreenPresenter.hpp>

class timeScreenView : public timeScreenViewBase
{
public:
    timeScreenView();
    virtual ~timeScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

#ifndef SIMULATOR
    	virtual void hourUpClicked();
        virtual void hourDownClicked();
        virtual void minuteUpClicked();
        virtual void minuteDownClicked();
      //  void handleTickEvent();
        void updateTime(uint8_t hour,uint8_t minute,uint8_t second);
        void SaveTimeToRTC();
#endif
/*
 *
 *
 *

   	   	   virtual void SaveTimeToRTC()
          {
              // Override and implement this function in Clock
          }
          virtual void hourUpClicked()
          {
              // Override and implement this function in Clock
          }
          virtual void hourDownClicked()
          {
              // Override and implement this function in Clock
          }
          virtual void minuteUpClicked()
          {
              // Override and implement this function in Clock
          }
          virtual void minuteDownClicked()
          {
              // Override and implement this function in Clock
          }


 */
protected:

        uint8_t hour_set;
        uint8_t minute_set;



};

#endif // TIMESCREENVIEW_HPP
