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
    /* Custom, larger wildcard text buffers (minimum capacity of 24 characters as requested) */
    touchgfx::Unicode::UnicodeChar co2_buffer[32];
    touchgfx::Unicode::UnicodeChar co_buffer[32];
    touchgfx::Unicode::UnicodeChar o2_buffer[32];
};

#endif // SCREENSPANVALUEVIEW_HPP
