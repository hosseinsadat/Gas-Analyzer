#include <gui/co2calscreen_screen/CO2CalScreenView.hpp>
#include <touchgfx/Color.hpp>

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
    if (&src == &btnAir)
    {
        /* Disable buttons while calibration is in progress to prevent duplicate requests */
        btnAir.setTouchable(false);
        btnN2.setTouchable(false);
        btnAir.invalidate();
        btnN2.invalidate();

        /* Visual feedback: Yellow indicates calibration in progress */
        txtSpan_1.setColor(touchgfx::Color::getColorFromRGB(255, 255, 0));
        txtSpan_1.invalidate();

        /* Perform Senseair K33 Modbus fresh-air ("Zero in Air") calibration */
        bool ok = presenter->co2AirCalibrate();

        /* Visual feedback: Green indicates success, Red indicates failure */
        if (ok)
        {
            txtSpan_1.setColor(touchgfx::Color::getColorFromRGB(0, 255, 0));
        }
        else
        {
            txtSpan_1.setColor(touchgfx::Color::getColorFromRGB(255, 0, 0));
        }
        txtSpan_1.invalidate();

        /* Re-enable buttons upon completion or failure */
        btnAir.setTouchable(true);
        btnN2.setTouchable(true);
        btnAir.invalidate();
        btnN2.invalidate();
    }
    else if (&src == &btnN2)
    {
        /* Disable buttons while calibration is in progress to prevent duplicate requests */
        btnAir.setTouchable(false);
        btnN2.setTouchable(false);
        btnAir.invalidate();
        btnN2.invalidate();

        /* Visual feedback: Yellow indicates calibration in progress */
        txtSpan_1.setColor(touchgfx::Color::getColorFromRGB(255, 255, 0));
        txtSpan_1.invalidate();

        /* Perform Senseair K33 Modbus nitrogen ("Zero in N2") calibration */
        bool ok = presenter->co2N2Calibrate();

        /* Visual feedback: Green indicates success, Red indicates failure */
        if (ok)
        {
            txtSpan_1.setColor(touchgfx::Color::getColorFromRGB(0, 255, 0));
        }
        else
        {
            txtSpan_1.setColor(touchgfx::Color::getColorFromRGB(255, 0, 0));
        }
        txtSpan_1.invalidate();

        /* Re-enable buttons upon completion or failure */
        btnAir.setTouchable(true);
        btnN2.setTouchable(true);
        btnAir.invalidate();
        btnN2.invalidate();
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
