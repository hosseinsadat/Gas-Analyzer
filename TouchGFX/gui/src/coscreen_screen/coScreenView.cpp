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

	Unicode::snprintf(txtCOBuffer, TXTCO_SIZE, "%d",g_co_x10/10);
		txtCO.setWildcard(txtCOBuffer);
		txtCO.invalidate();


	dGraph_CO.addDataPoint(g_co_x10/10 );
	dGraph_CO.invalidate();

#endif
}
