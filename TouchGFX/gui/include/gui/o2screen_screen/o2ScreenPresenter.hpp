#ifndef O2SCREENPRESENTER_HPP
#define O2SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class o2ScreenView;

class o2ScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    o2ScreenPresenter(o2ScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~o2ScreenPresenter() {}

private:
    o2ScreenPresenter();

    o2ScreenView& view;
};

#endif // O2SCREENPRESENTER_HPP
