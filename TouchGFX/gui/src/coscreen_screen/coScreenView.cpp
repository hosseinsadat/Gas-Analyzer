#include <gui/coscreen_screen/coScreenView.hpp>

coScreenView::coScreenView()
{

}

void coScreenView::setupScreen()
{
    coScreenViewBase::setupScreen();
}

void coScreenView::tearDownScreen()
{
    coScreenViewBase::tearDownScreen();
}

void coScreenView:: handleTickEvent(){
#ifndef SIMULATOR
	//TODO extern global value
	extern volatile int16_t g_co_x10;

	float co_ppm = (float)g_co_x10 / 10.0f;
	Unicode::snprintfFloat(txtCOBuffer, TXTCO_SIZE, "%.0f", co_ppm);
	txtCO.setWildcard(txtCOBuffer);
	txtCO.invalidate();

	dGraph_CO.addDataPoint(co_ppm);
	dGraph_CO.invalidate();
#endif
}
