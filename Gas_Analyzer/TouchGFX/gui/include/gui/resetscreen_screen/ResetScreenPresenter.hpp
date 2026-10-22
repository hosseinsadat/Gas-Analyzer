#ifndef RESETSCREENPRESENTER_HPP
#define RESETSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ResetScreenView;

class ResetScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ResetScreenPresenter(ResetScreenView& v);

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

    virtual ~ResetScreenPresenter() {}

    /* Expose Factory Reset logic to the View */
    void triggerFactoryReset()
    {
        model->resetFactoryCalib();
    }

private:
    ResetScreenPresenter();

    ResetScreenView& view;
};

#endif // RESETSCREENPRESENTER_HPP
