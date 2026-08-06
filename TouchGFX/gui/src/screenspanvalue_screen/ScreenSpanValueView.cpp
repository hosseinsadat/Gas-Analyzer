#include <gui/screenspanvalue_screen/ScreenSpanValueView.hpp>
#include <cstdio>

ScreenSpanValueView::ScreenSpanValueView()
{
}

void ScreenSpanValueView::setupScreen()
{
    ScreenSpanValueViewBase::setupScreen();

    // Dynamically re-assign wildcards to use our larger safe buffers (capacity 32 >= 24)
    txtCO2_offset.setWildcard(co2_buffer);
    txtCO_offset.setWildcard(co_buffer);
    txtO2_offset.setWildcard(o2_buffer);

    // Retrieve active calibration offsets and slopes from the Model via the Presenter
    float co2_offset = presenter->getCO2Offset();
    float co2_slope  = presenter->getCO2Slope();
    float co_offset  = presenter->getCOOffset();
    float co_slope   = presenter->getCOSlope();
    float o2_offset  = presenter->getO2Offset();
    float o2_slope   = presenter->getO2Slope();

    // Format active calibration parameters to the requested layout: OFS: [Offset] | SLP: [Slope]
    // OFS formatted with %+.3f (to explicitly print positive/negative sign), SLP formatted with %.4f
    char temp_buf[32];

    snprintf(temp_buf, sizeof(temp_buf), "OFS: %+.3f | SLP: %.4f", co2_offset, co2_slope);
    touchgfx::Unicode::fromUTF8((const unsigned char*)temp_buf, co2_buffer, 32);

    snprintf(temp_buf, sizeof(temp_buf), "OFS: %+.3f | SLP: %.4f", co_offset, co_slope);
    touchgfx::Unicode::fromUTF8((const unsigned char*)temp_buf, co_buffer, 32);

    snprintf(temp_buf, sizeof(temp_buf), "OFS: %+.3f | SLP: %.4f", o2_offset, o2_slope);
    touchgfx::Unicode::fromUTF8((const unsigned char*)temp_buf, o2_buffer, 32);

    // Force a visual repaint of the updated text widgets
    txtCO2_offset.invalidate();
    txtCO_offset.invalidate();
    txtO2_offset.invalidate();

    // Hide the separate slope text widgets to maintain a clean, combined single-row layout
    txtCO2_slope.setVisible(false);
    txtCO_slope.setVisible(false);
    txtO2_slope.setVisible(false);

    txtCO2_slope.invalidate();
    txtCO_slope.invalidate();
    txtO2_slope.invalidate();
}

void ScreenSpanValueView::tearDownScreen()
{
    ScreenSpanValueViewBase::tearDownScreen();
}
