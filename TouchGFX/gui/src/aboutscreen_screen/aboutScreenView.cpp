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
    txtDebugCO.setTypedText(touchgfx::TypedText(T___SINGLEUSE_DU4R));
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
    if (evt.getType() == touchgfx::ClickEvent::PRESSED)
    {
        if (debugModeActive)
        {
            /* If we are already in debug mode, a click anywhere else returns to the main screen */
            application().gotomainScreenScreenNoTransition();
            return;
        }

        /* Check if the click falls inside the logo boundaries */
        touchgfx::Rect logoRect = scalableImage1.getAbsoluteRect();
        if (logoRect.rectWithin(evt.getX(), evt.getY()))
        {
            /* Enable debug mode and hide logo/about texts */
            debugModeActive = true;
            scalableImage1.setVisible(false);
            textArea1.setVisible(false);
            textArea2.setVisible(false);
            textArea2_1.setVisible(false);

            /* Load current slope and offset values into wildcard buffers */
#ifndef SIMULATOR
            touchgfx::Unicode::snprintfFloat(debugCOBuffer, DEBUG_CO_SIZE, "CO S:%.4f O:%.1f", g_CalibData.co.slope, g_CalibData.co.offset);
            touchgfx::Unicode::snprintfFloat(debugO2Buffer, DEBUG_O2_SIZE, "O2 S:%.4f O:%.1f", g_CalibData.o2.slope, g_CalibData.o2.offset);
            touchgfx::Unicode::snprintfFloat(debugCO2Buffer, DEBUG_CO2_SIZE, "CO2 S:%.4f O:%.1f", g_CalibData.co2.slope, g_CalibData.co2.offset);
#else
            touchgfx::Unicode::snprintfFloat(debugCOBuffer, DEBUG_CO_SIZE, "CO S:1.0234 O:-15.3");
            touchgfx::Unicode::snprintfFloat(debugO2Buffer, DEBUG_O2_SIZE, "O2 S:-0.0233 O:258.3");
            touchgfx::Unicode::snprintfFloat(debugCO2Buffer, DEBUG_CO2_SIZE, "CO2 S:1.0000 O:0.0");
#endif

            /* Display debug widgets and invalidate */
            txtDebugCO.setVisible(true);
            txtDebugO2.setVisible(true);
            txtDebugCO2.setVisible(true);

            txtDebugCO.invalidate();
            txtDebugO2.invalidate();
            txtDebugCO2.invalidate();
            invalidate();
        }
    }
}
