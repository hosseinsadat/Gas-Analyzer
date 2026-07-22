#include <gui/warmupscreen_screen/warmupScreenView.hpp>

warmupScreenView::warmupScreenView()
{

}

void warmupScreenView::setupScreen()
{
    warmupScreenViewBase::setupScreen();

}

void warmupScreenView::tearDownScreen()
{
    warmupScreenViewBase::tearDownScreen();
}

/*
void warmupScreenView::handleTickEvent()
{
	extern volatile int16_t g_o2_x10;
	uint16_t barValue =
    if (newVal != currentProgress)
    {
        currentProgress = newVal;
        warmupScreenView.setValue(currentProgress);
        warmupScreenView.invalidate();
    }

    // انتقال به صفحه اصلی وقتی warm-up تمام شد
    if (currentProgress >= 100)
    {
        application().gotomainScreenScreenNoTransition();
    }
}

*/
