#include <gui/cocalscreen_screen/COCalScreenView.hpp>
#include <gui/cocalscreen_screen/COCalScreenPresenter.hpp>

COCalScreenPresenter::COCalScreenPresenter(COCalScreenView& v)
    : view(v)
{

}

void COCalScreenPresenter::onCOCalDataUpdated(int16_t raw_co, float cal_co)
{
    view.updateCOValues(raw_co, cal_co);
}

void COCalScreenPresenter::coZeroCalibrate()
{
    model->calibrateCOZero();
}

void COCalScreenPresenter::coSpanCalibrate(float target_ppm)
{
    model->calibrateCOSpan(target_ppm);
}

void COCalScreenPresenter::coResetCalibrate()
{
    model->resetCOCal();
}

void COCalScreenPresenter::activate()
{

}

void COCalScreenPresenter::deactivate()
{

}
