#ifndef O2SCREENVIEW_HPP
#define O2SCREENVIEW_HPP

#include <gui_generated/o2screen_screen/o2ScreenViewBase.hpp>
#include <gui/o2screen_screen/o2ScreenPresenter.hpp>

class o2ScreenView : public o2ScreenViewBase
{
public:
    o2ScreenView();
    virtual ~o2ScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
protected:
};

#endif // O2SCREENVIEW_HPP
