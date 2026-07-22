#ifndef COCALSCREENVIEW_HPP
#define COCALSCREENVIEW_HPP

#include <gui_generated/cocalscreen_screen/COCalScreenViewBase.hpp>
#include <gui/cocalscreen_screen/COCalScreenPresenter.hpp>

class COCalScreenView : public COCalScreenViewBase
{
public:
    COCalScreenView();
    virtual ~COCalScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();


protected:



};

#endif // COCALSCREENVIEW_HPP
