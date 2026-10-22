#ifndef COSCREENVIEW_HPP
#define COSCREENVIEW_HPP

#include <gui_generated/coscreen_screen/coScreenViewBase.hpp>
#include <gui/coscreen_screen/coScreenPresenter.hpp>

class coScreenView : public coScreenViewBase
{
public:
    coScreenView();
    virtual ~coScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();

protected:
};

#endif // COSCREENVIEW_HPP
