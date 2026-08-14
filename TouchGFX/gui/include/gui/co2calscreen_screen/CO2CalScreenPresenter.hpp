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

    virtual void activate();
    virtual void deactivate();

    virtual ~CO2CalScreenPresenter() {}

    virtual void onCO2CalDataUpdated(uint16_t raw_co2, float cal_co2);

    void co2ZeroCalibrate();
    void co2SpanCalibrate(float target_pct);
    void co2ResetCalibrate();

    bool co2AirCalibrate();
    bool co2N2Calibrate();

private:
    CO2CalScreenPresenter();

    CO2CalScreenView& view;
};

#endif // CO2CALSCREENPRESENTER_HPP
