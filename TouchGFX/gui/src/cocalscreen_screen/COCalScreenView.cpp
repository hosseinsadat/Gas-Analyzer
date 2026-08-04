#include <gui/cocalscreen_screen/COCalScreenView.hpp>

COCalScreenView::COCalScreenView()
    : myButtonCallback(this, &COCalScreenView::buttonClickHandler)
{
}

void COCalScreenView::setupScreen()
{
    COCalScreenViewBase::setupScreen();

    /* Set custom button handlers */
    btnN2.setAction(myButtonCallback);
    btnSpanCO.setAction(myButtonCallback);

    /* Initialize target span text from current slider value */
    Unicode::snprintf(txtSpanCOBuffer, TXTSPANCO_SIZE, "%d", sliderCOSpan.getValue());
    txtSpanCO.invalidate();
}

void COCalScreenView::tearDownScreen()
{
    COCalScreenViewBase::tearDownScreen();
}

void COCalScreenView::buttonClickHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &btnN2)
    {
        /* Perform CO zero calibration */
        presenter->coZeroCalibrate();
    }
    else if (&src == &btnSpanCO)
    {
        /* Retrieve target PPM from slider value and perform span calibration */
        float target_ppm = (float)sliderCOSpan.getValue();
        presenter->coSpanCalibrate(target_ppm);
    }
}

void COCalScreenView::updateCOValues(int16_t raw_co, float cal_co)
{
    /* Update raw value widget */
    Unicode::snprintf(txtCOrawValueBuffer, TXTCORAWVALUE_SIZE, "%d", raw_co);
    txtCOrawValue.invalidate();

    /* Update calibrated measured value widget */
    Unicode::snprintfFloat(txtCOMeasuredBuffer, TXTCOMEASURED_SIZE, "%.1f", cal_co);
    txtCOMeasured.invalidate();
}
