#ifndef RESETSCREENVIEW_HPP
#define RESETSCREENVIEW_HPP

#include <gui_generated/resetscreen_screen/ResetScreenViewBase.hpp>
#include <gui/resetscreen_screen/ResetScreenPresenter.hpp>

class ResetScreenView : public ResetScreenViewBase
{
public:
    ResetScreenView();
    virtual ~ResetScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void resetFactory();
protected:
};

#endif // RESETSCREENVIEW_HPP
