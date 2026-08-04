#ifndef SCREENSPANVALUEPRESENTER_HPP
#define SCREENSPANVALUEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenSpanValueView;

class ScreenSpanValuePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenSpanValuePresenter(ScreenSpanValueView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~ScreenSpanValuePresenter() {}

    /* Active Calibration Parameter Getters */
    float getCO2Offset() { return model->getCO2Offset(); }
    float getCO2Slope() { return model->getCO2Slope(); }
    float getCOOffset() { return model->getCOOffset(); }
    float getCOSlope() { return model->getCOSlope(); }
    float getO2Offset() { return model->getO2Offset(); }
    float getO2Slope() { return model->getO2Slope(); }

private:
    ScreenSpanValuePresenter();

    ScreenSpanValueView& view;
};

#endif // SCREENSPANVALUEPRESENTER_HPP
