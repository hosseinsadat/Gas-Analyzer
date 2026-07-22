#ifndef CO2SCREENVIEW_HPP
#define CO2SCREENVIEW_HPP

#include <gui_generated/co2screen_screen/co2ScreenViewBase.hpp>
#include <gui/co2screen_screen/co2ScreenPresenter.hpp>

class co2ScreenView : public co2ScreenViewBase
{
public:
    co2ScreenView();
    virtual ~co2ScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
protected:
};

#endif // CO2SCREENVIEW_HPP
