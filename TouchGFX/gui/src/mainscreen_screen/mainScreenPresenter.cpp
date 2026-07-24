#include <gui/mainscreen_screen/mainScreenView.hpp>
#include <gui/mainscreen_screen/mainScreenPresenter.hpp>

mainScreenPresenter::mainScreenPresenter(mainScreenView &v) :
		view(v) {

}

void mainScreenPresenter::activate() {

}

void mainScreenPresenter::deactivate() {

}

void mainScreenPresenter::tick() {

}
void mainScreenPresenter::onSht22Updated(float t, float h) {
	view.setGasTemperature(t);
	view.setGasHumidity(h);
}
void mainScreenPresenter::onGasUpdated(float co_ppm, float o2_percent) {
	view.setGasValues(co_ppm, o2_percent);
}
void mainScreenPresenter::onCO2Updated(float co2_ppm) {
	view.setCO2Value(co2_ppm);
}
void mainScreenPresenter::onProbeUpdated(float probe_temp){
	view.setProbeValue(probe_temp);
}

void mainScreenPresenter::updateTime(uint8_t hour,uint8_t minute,uint8_t second){
	view.updateTime(hour,minute,second);
}
void mainScreenPresenter::updateEEPROMStatus()
{
   // view.setEEPROMStatus(model->getEEPROMStatus());
}
