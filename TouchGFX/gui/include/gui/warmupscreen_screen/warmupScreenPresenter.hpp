#ifndef WARMUPSCREENPRESENTER_HPP
#define WARMUPSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class warmupScreenView;

class warmupScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    warmupScreenPresenter(warmupScreenView& v);

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

    virtual ~warmupScreenPresenter() {}

private:
    warmupScreenPresenter();

    warmupScreenView& view;
};

#endif // WARMUPSCREENPRESENTER_HPP
