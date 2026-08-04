#ifndef SCREENSPANVALUEVIEW_HPP
#define SCREENSPANVALUEVIEW_HPP

#include <gui_generated/screenspanvalue_screen/ScreenSpanValueViewBase.hpp>
#include <gui/screenspanvalue_screen/ScreenSpanValuePresenter.hpp>

class ScreenSpanValueView : public ScreenSpanValueViewBase
{
public:
    ScreenSpanValueView();
    virtual ~ScreenSpanValueView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREENSPANVALUEVIEW_HPP
