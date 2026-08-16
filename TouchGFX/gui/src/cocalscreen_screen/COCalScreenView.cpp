#include <gui/cocalscreen_screen/COCalScreenView.hpp>

COCalScreenView::COCalScreenView()
    : targetCoSpan(1000.0f)
    , myButtonCallback(this, &COCalScreenView::buttonClickHandler)
{
}

void COCalScreenView::setupScreen()
{
    COCalScreenViewBase::setupScreen();

    /* Set custom button handlers */
    btnN2.setAction(myButtonCallback);
    btnSpanCO.setAction(myButtonCallback);

    /* Initialize target span display */
    updateSpanDisplay();
}

void COCalScreenView::tearDownScreen()
{
    COCalScreenViewBase::tearDownScreen();
}

void COCalScreenView::increaseSpan()
{
    targetCoSpan += 50.0f;
    if (targetCoSpan > 2000.0f)
    {
        targetCoSpan = 2000.0f;
    }
    updateSpanDisplay();
}

void COCalScreenView::decreaseSpan()
{
    targetCoSpan -= 50.0f;
    if (targetCoSpan < 10.0f)
    {
        targetCoSpan = 10.0f;
    }
    updateSpanDisplay();
}

void COCalScreenView::updateSpanDisplay()
{
    Unicode::snprintf(txtSpanCOBuffer, TXTSPANCO_SIZE, "%.0f", targetCoSpan);
    txtSpanCO.invalidate();
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
        /* Perform CO span calibration with current target PPM */
        presenter->coSpanCalibrate(targetCoSpan);
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
