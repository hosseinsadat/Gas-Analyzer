#ifndef O2CALSCREENVIEW_HPP
#define O2CALSCREENVIEW_HPP

#include <gui_generated/o2calscreen_screen/O2CalScreenViewBase.hpp>
#include <gui/o2calscreen_screen/O2CalScreenPresenter.hpp>

class O2CalScreenView : public O2CalScreenViewBase
{
public:
    O2CalScreenView();
    virtual ~O2CalScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // O2CALSCREENVIEW_HPP
