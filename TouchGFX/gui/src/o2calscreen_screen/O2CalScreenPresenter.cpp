#include <gui/o2calscreen_screen/O2CalScreenView.hpp>
#include <gui/o2calscreen_screen/O2CalScreenPresenter.hpp>

O2CalScreenPresenter::O2CalScreenPresenter(O2CalScreenView& v)
    : view(v)
{

}

void O2CalScreenPresenter::onO2CalDataUpdated(int16_t raw_o2, float cal_o2)
{
    view.updateO2Values(raw_o2, cal_o2);
}

void O2CalScreenPresenter::o2ZeroCalibrate()
{
    model->calibrateO2Zero();
}

void O2CalScreenPresenter::o2SpanCalibrate(float target_pct)
{
    model->calibrateO2Span(target_pct);
}

void O2CalScreenPresenter::o2ResetCalibrate()
{
    model->resetO2Cal();
}

void O2CalScreenPresenter::activate()
{

}

void O2CalScreenPresenter::deactivate()
{

}
