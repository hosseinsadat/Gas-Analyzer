#include <gui/aboutscreen_screen/aboutScreenView.hpp>

#ifndef SIMULATOR
#include "slope_offset_calib.h"
#endif

aboutScreenView::aboutScreenView()
    : debugModeActive(false)
{
}

void aboutScreenView::setupScreen()
{
    aboutScreenViewBase::setupScreen();


}

void aboutScreenView::tearDownScreen()
{
    aboutScreenViewBase::tearDownScreen();
}

void aboutScreenView::handleTickEvent()
{
    if (debugModeActive)
    {
        /* Prevent automatic transition to mainScreen while debugging */
        return;
    }
    aboutScreenViewBase::handleTickEvent();
}

void aboutScreenView::handleClickEvent(const touchgfx::ClickEvent& evt)
{

    }

