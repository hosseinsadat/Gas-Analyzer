#ifndef ABOUTSCREENVIEW_HPP
#define ABOUTSCREENVIEW_HPP

#include <gui_generated/aboutscreen_screen/aboutScreenViewBase.hpp>
#include <gui/aboutscreen_screen/aboutScreenPresenter.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

class aboutScreenView : public aboutScreenViewBase
{
public:
    aboutScreenView();
    virtual ~aboutScreenView() {}
    virtual void setupScreen() override;
    virtual void tearDownScreen() override;
    virtual void handleTickEvent() override;
    virtual void handleClickEvent(const touchgfx::ClickEvent& evt) override;

protected:
    bool debugModeActive;

    /* Custom widgets for showing calibration parameters */
    touchgfx::TextAreaWithOneWildcard txtDebugCO;
    touchgfx::TextAreaWithOneWildcard txtDebugO2;
    touchgfx::TextAreaWithOneWildcard txtDebugCO2;

    /* Wildcard buffers */
    static const uint16_t DEBUG_CO_SIZE = 40;
    touchgfx::Unicode::UnicodeChar debugCOBuffer[DEBUG_CO_SIZE];
    static const uint16_t DEBUG_O2_SIZE = 40;
    touchgfx::Unicode::UnicodeChar debugO2Buffer[DEBUG_O2_SIZE];
    static const uint16_t DEBUG_CO2_SIZE = 40;
    touchgfx::Unicode::UnicodeChar debugCO2Buffer[DEBUG_CO2_SIZE];
};

#endif // ABOUTSCREENVIEW_HPP
