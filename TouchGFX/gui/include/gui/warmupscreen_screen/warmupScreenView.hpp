#ifndef WARMUPSCREENVIEW_HPP
#define WARMUPSCREENVIEW_HPP

#include <gui_generated/warmupscreen_screen/warmupScreenViewBase.hpp>
#include <gui/warmupscreen_screen/warmupScreenPresenter.hpp>

class warmupScreenView : public warmupScreenViewBase
{
public:
    warmupScreenView();
    virtual ~warmupScreenView() {}
    virtual void setupScreen() override;
    virtual void tearDownScreen() override;
    virtual void handleTickEvent() override;

protected:
};

#endif // WARMUPSCREENVIEW_HPP
