#include <gui/resetscreen_screen/ResetScreenView.hpp>
#include <touchgfx/Color.hpp>


ResetScreenView::ResetScreenView()
{
}

void ResetScreenView::setupScreen()
{
    ResetScreenViewBase::setupScreen();
}

void ResetScreenView::tearDownScreen()
{
    ResetScreenViewBase::tearDownScreen();
}

void ResetScreenView::resetFactory()
{
    // Trigger the calibration reset to defaults on the Model via the Presenter
    presenter->triggerFactoryReset();

    // After reset is complete, transition back to the ScreenSpanValue screen
    // so the UI immediately refreshes and shows the restored default values!
    application().gotoScreenSpanValueScreenNoTransition();
}
