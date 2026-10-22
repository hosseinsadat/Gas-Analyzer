#include <gui/timescreen_screen/timeScreenView.hpp>
#include <gui/timescreen_screen/timeScreenPresenter.hpp>

timeScreenPresenter::timeScreenPresenter(timeScreenView& v)
    : view(v)
{

}

void timeScreenPresenter::activate()
{

}

void timeScreenPresenter::deactivate()
{

}
#ifndef SIMULATOR
void timeScreenPresenter::updateTime(uint8_t hour,uint8_t minute,uint8_t second){
	view.updateTime(hour,minute,second);
}
#endif
