#ifndef CO2CALSCREENVIEW_HPP
#define CO2CALSCREENVIEW_HPP

#include <gui_generated/co2calscreen_screen/CO2CalScreenViewBase.hpp>
#include <gui/co2calscreen_screen/CO2CalScreenPresenter.hpp>

class CO2CalScreenView : public CO2CalScreenViewBase
{
public:
    CO2CalScreenView();
    virtual ~CO2CalScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // CO2CALSCREENVIEW_HPP
