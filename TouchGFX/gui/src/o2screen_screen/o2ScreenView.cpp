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
	Unicode::snprintf(txtGraphO2Buffer, TXTGRAPHO2_SIZE, "%d",g_o2_x10/10);
	txtGraphO2.setWildcard(txtGraphO2Buffer);
	txtGraphO2.invalidate();


	dGraph_O2.addDataPoint(g_o2_x10/10 );
	dGraph_O2.invalidate();
#endif
}
