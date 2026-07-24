#include <gui/co2calscreen_screen/CO2CalScreenView.hpp>

CO2CalScreenView::CO2CalScreenView()
    : myButtonCallback(this, &CO2CalScreenView::buttonClickHandler)
{
}

void CO2CalScreenView::setupScreen()
{
    CO2CalScreenViewBase::setupScreen();

    /* Set custom button handlers */
    btnN2.setAction(myButtonCallback);
    btnAir.setAction(myButtonCallback);
    btnSpanCO2.setAction(myButtonCallback);

    /* Initialize target span text from current slider value */
    Unicode::snprintfFloat(txtSpanCO2Buffer, TXTSPANCO2_SIZE, "%.1f", (float)sliderCo2Span.getValue() / 100.0f);
    txtSpanCO2.invalidate();
}

void CO2CalScreenView::tearDownScreen()
{
    CO2CalScreenViewBase::tearDownScreen();
}

void CO2CalScreenView::buttonClickHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &btnN2)
    {
        /* Perform CO2 zero calibration */
        presenter->co2ZeroCalibrate();
    }
    else if (&src == &btnAir)
    {
        /* Perform CO2 air calibration (400 ppm = 0.04%) */
        presenter->co2SpanCalibrate(0.04f);
    }
    else if (&src == &btnSpanCO2)
    {
        /* Retrieve target CO2 percentage from slider and perform span calibration */
        float target_pct = (float)sliderCo2Span.getValue() / 100.0f;
        presenter->co2SpanCalibrate(target_pct);
    }
}

void CO2CalScreenView::updateCO2Values(uint16_t raw_co2, float cal_co2)
{
    /* Update raw value widget */
    Unicode::snprintf(txtCo2rawValueBuffer, TXTCO2RAWVALUE_SIZE, "%d", raw_co2);
    txtCo2rawValue.invalidate();

    /* Update calibrated measured value widget */
    Unicode::snprintfFloat(txtCOMeasuredBuffer, TXTCOMEASURED_SIZE, "%.2f", cal_co2);
    txtCOMeasured.invalidate();
}
