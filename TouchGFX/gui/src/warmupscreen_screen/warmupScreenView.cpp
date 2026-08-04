#include <gui/warmupscreen_screen/warmupScreenView.hpp>

warmupScreenView::warmupScreenView()
{
}

void warmupScreenView::setupScreen()
{
    warmupScreenViewBase::setupScreen();
    warmup_progressbar.setValue(0);
    warmup_progressbar.invalidate();
}

void warmupScreenView::tearDownScreen()
{
    warmupScreenViewBase::tearDownScreen();
}

void warmupScreenView::handleTickEvent()
{
    static uint32_t ticks = 0;
    ticks++;

    /* Calculate progress percentage (0 to 100) linearly over 20 seconds (1200 ticks at 60 fps) */
    uint32_t progress = (ticks * 100) / 1200;
    if (progress > 100)
    {
        progress = 100;
    }

    warmup_progressbar.setValue(progress);
    warmup_progressbar.invalidate();

    if (ticks >= 1200)
    {
        ticks = 0; /* Reset for future power-ons */
        application().gotomainScreenScreenNoTransition();
    }
}
