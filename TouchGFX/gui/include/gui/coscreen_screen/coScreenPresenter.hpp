#ifndef COSCREENPRESENTER_HPP
#define COSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class coScreenView;

class coScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    coScreenPresenter(coScreenView& v);

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

    virtual ~coScreenPresenter() {}

private:
    coScreenPresenter();

    coScreenView& view;
};

#endif // COSCREENPRESENTER_HPP
