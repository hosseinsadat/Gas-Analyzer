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
	extern volatile int16_t g_co2_x10;

	Unicode::snprintf(txtCO2Buffer, TXTCO2_SIZE, "%d",g_co2_x10/(10*1000));
	txtCO2.setWildcard(txtCO2Buffer);
	txtCO2.invalidate();
	dGraph_CO2.addDataPoint(g_co2_x10/(10*1000) );
	dGraph_CO2.invalidate();
#endif
}
