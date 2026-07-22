#ifndef O2CALSCREENPRESENTER_HPP
#define O2CALSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class O2CalScreenView;

class O2CalScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    O2CalScreenPresenter(O2CalScreenView& v);

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

    virtual ~O2CalScreenPresenter() {}

private:
    O2CalScreenPresenter();

    O2CalScreenView& view;
};

#endif // O2CALSCREENPRESENTER_HPP
