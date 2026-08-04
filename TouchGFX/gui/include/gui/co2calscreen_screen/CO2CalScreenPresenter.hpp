#ifndef CO2CALSCREENPRESENTER_HPP
#define CO2CALSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class CO2CalScreenView;

class CO2CalScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    CO2CalScreenPresenter(CO2CalScreenView& v);

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

    virtual ~CO2CalScreenPresenter() {}

    virtual void onCO2CalDataUpdated(uint16_t raw_co2, float cal_co2) override;

    void co2ZeroCalibrate();
    void co2SpanCalibrate(float target_pct);
    void co2ResetCalibrate();

private:
    CO2CalScreenPresenter();

    CO2CalScreenView& view;
};

#endif // CO2CALSCREENPRESENTER_HPP
