#include <gui/co2calscreen_screen/CO2CalScreenView.hpp>
#include <gui/co2calscreen_screen/CO2CalScreenPresenter.hpp>

CO2CalScreenPresenter::CO2CalScreenPresenter(CO2CalScreenView& v)
    : view(v)
{

}

void CO2CalScreenPresenter::onCO2CalDataUpdated(uint16_t raw_co2, float cal_co2)
{
    view.updateCO2Values(raw_co2, cal_co2);
}

void CO2CalScreenPresenter::co2ZeroCalibrate()
{
    model->calibrateCO2Zero();
}

void CO2CalScreenPresenter::co2SpanCalibrate(float target_pct)
{
    model->calibrateCO2Span(target_pct);
}

void CO2CalScreenPresenter::co2ResetCalibrate()
{
    model->resetCO2Cal();
}

bool CO2CalScreenPresenter::co2AirCalibrate()
{
    return model->calibrateK33ZeroAir();
}

bool CO2CalScreenPresenter::co2N2Calibrate()
{
    return model->calibrateK33ZeroN2();
}

void CO2CalScreenPresenter::activate()
{

}

void CO2CalScreenPresenter::deactivate()
{

}
