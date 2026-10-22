#include <gui/o2screen_screen/o2ScreenView.hpp>

o2ScreenView::o2ScreenView()
{

}

void o2ScreenView::setupScreen()
{
    o2ScreenViewBase::setupScreen();
}

void o2ScreenView::tearDownScreen()
{
    o2ScreenViewBase::tearDownScreen();
}
void o2ScreenView:: handleTickEvent(){
#ifndef SIMULATOR
	//TODO extern global value
	extern volatile int16_t g_o2_x10;
	float o2_pct = (float)g_o2_x10 / 10.0f;
	Unicode::snprintfFloat(txtGraphO2Buffer, TXTGRAPHO2_SIZE, "%.1f", o2_pct);
	txtGraphO2.setWildcard(txtGraphO2Buffer);
	txtGraphO2.invalidate();

	dGraph_O2.addDataPoint(o2_pct);
	dGraph_O2.invalidate();
#endif
}
