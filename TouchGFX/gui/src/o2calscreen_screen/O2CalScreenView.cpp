#include <gui/o2calscreen_screen/O2CalScreenView.hpp>

O2CalScreenView::O2CalScreenView()
    : myButtonCallback(this, &O2CalScreenView::buttonClickHandler)
{
}

void O2CalScreenView::setupScreen()
{
    O2CalScreenViewBase::setupScreen();

    /* Set custom button handlers */
    btnN2.setAction(myButtonCallback);
    btnFreshAir.setAction(myButtonCallback);
    btnSpanO2.setAction(myButtonCallback);

    /* Initialize target span text from current slider value */
    Unicode::snprintfFloat(txtSpanO2Buffer, TXTSPANO2_SIZE, "%.1f", (float)sliderO2Span.getValue() / 100.0f);
    txtSpanO2.invalidate();
}

void O2CalScreenView::tearDownScreen()
{
    O2CalScreenViewBase::tearDownScreen();
}

void O2CalScreenView::buttonClickHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &btnN2)
    {
        /* Perform O2 zero calibration */
        presenter->o2ZeroCalibrate();
    }
    else if (&src == &btnFreshAir)
    {
        /* Perform O2 fresh air (20.9%) calibration */
        presenter->o2SpanCalibrate(20.9f);
    }
    else if (&src == &btnSpanO2)
    {
        /* Retrieve target O2 percentage from slider and perform span calibration */
        float target_pct = (float)sliderO2Span.getValue() / 100.0f;
        presenter->o2SpanCalibrate(target_pct);
    }
}

void O2CalScreenView::updateO2Values(int16_t raw_o2, float cal_o2)
{
    /* Update raw value widget */
    Unicode::snprintf(txtO2rawValueBuffer, TXTO2RAWVALUE_SIZE, "%d", raw_o2);
    txtO2rawValue.invalidate();

    /* Update calibrated measured value widget */
    Unicode::snprintfFloat(txtO2MeasuredBuffer, TXTO2MEASURED_SIZE, "%.1f", cal_o2);
    txtO2Measured.invalidate();
}
