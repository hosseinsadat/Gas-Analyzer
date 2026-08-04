#ifndef CO2SCREENPRESENTER_HPP
#define CO2SCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class co2ScreenView;

class co2ScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    co2ScreenPresenter(co2ScreenView& v);

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

    virtual ~co2ScreenPresenter() {}



private:
    co2ScreenPresenter();

    co2ScreenView& view;
};

#endif // CO2SCREENPRESENTER_HPP
