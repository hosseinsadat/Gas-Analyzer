#include <gui/o2calscreen_screen/O2CalScreenView.hpp>

O2CalScreenView::O2CalScreenView()
    : targetO2Span(20.9f)
    , myButtonCallback(this, &O2CalScreenView::buttonClickHandler)
{
}

void O2CalScreenView::setupScreen()
{
    O2CalScreenViewBase::setupScreen();

    /* Set custom button handlers */
    btnN2.setAction(myButtonCallback);
    btnFreshAir.setAction(myButtonCallback);
    btnSpanO2.setAction(myButtonCallback);

    /* Connect increase and decrease span arrow buttons */
    btn_increase_o2_span.setAction(myButtonCallback);
    btn_decrease_o2_span.setAction(myButtonCallback);

    /* Initialize target span display */
    updateSpanDisplay();
}

void O2CalScreenView::tearDownScreen()
{
    O2CalScreenViewBase::tearDownScreen();
}

void O2CalScreenView::increaseSpan()
{
    targetO2Span += 0.1f;
    if (targetO2Span > 25.0f)
    {
        targetO2Span = 25.0f;
    }
    updateSpanDisplay();
}

void O2CalScreenView::decreaseSpan()
{
    targetO2Span -= 0.1f;
    if (targetO2Span < 0.1f)
    {
        targetO2Span = 0.1f;
    }
    updateSpanDisplay();
}

void O2CalScreenView::updateSpanDisplay()
{
    Unicode::snprintfFloat(txtSpanO2Buffer, TXTSPANO2_SIZE, "%.1f", targetO2Span);
    txtSpanO2.invalidate();
}

void O2CalScreenView::buttonClickHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &btn_increase_o2_span)
    {
        increaseSpan();
    }
    else if (&src == &btn_decrease_o2_span)
    {
        decreaseSpan();
    }
    else if (&src == &btnN2)
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
        /* Perform O2 span calibration with current target percentage */
        presenter->o2SpanCalibrate(targetO2Span);
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
