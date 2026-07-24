#include <gui/co2screen_screen/co2ScreenView.hpp>

co2ScreenView::co2ScreenView()
{

}

void co2ScreenView::setupScreen()
{
    co2ScreenViewBase::setupScreen();
}

void co2ScreenView::tearDownScreen()
{
    co2ScreenViewBase::tearDownScreen();
}
void co2ScreenView:: handleTickEvent(){
#ifndef SIMULATOR
	//TODO extern global value
	extern volatile uint32_t g_co2_x10;

	float co2_pct = (float)g_co2_x10 / 100000.0f;
	Unicode::snprintfFloat(txtCO2Buffer, TXTCO2_SIZE, "%.1f", co2_pct);
	txtCO2.setWildcard(txtCO2Buffer);
	txtCO2.invalidate();

	dGraph_CO2.addDataPoint(co2_pct);
	dGraph_CO2.invalidate();
#endif
}
