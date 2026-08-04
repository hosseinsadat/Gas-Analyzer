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

    /* Initialize custom debugging widgets (hidden initially) */
/*    txtDebugCO.setTypedText(touchgfx::TypedText(T___SINGLEUSE_DU4R));
    txtDebugCO.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    txtDebugCO.setPosition(20, 50, 440, 40);
    txtDebugCO.setWildcard(debugCOBuffer);
    txtDebugCO.setVisible(false);
    add(txtDebugCO);

    txtDebugO2.setTypedText(touchgfx::TypedText(T___SINGLEUSE_DU4R));
    txtDebugO2.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    txtDebugO2.setPosition(20, 110, 440, 40);
    txtDebugO2.setWildcard(debugO2Buffer);
    txtDebugO2.setVisible(false);
    add(txtDebugO2);

    txtDebugCO2.setTypedText(touchgfx::TypedText(T___SINGLEUSE_DU4R));
    txtDebugCO2.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    txtDebugCO2.setPosition(20, 170, 440, 40);
    txtDebugCO2.setWildcard(debugCO2Buffer);
    txtDebugCO2.setVisible(false);
    add(txtDebugCO2);
    */
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

