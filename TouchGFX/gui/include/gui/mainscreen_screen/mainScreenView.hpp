#ifndef MAINSCREENVIEW_HPP
#define MAINSCREENVIEW_HPP

#include <gui_generated/mainscreen_screen/mainScreenViewBase.hpp>
#include <gui/mainscreen_screen/mainScreenPresenter.hpp>






class mainScreenView: public mainScreenViewBase {
public:
	mainScreenView();
	virtual ~mainScreenView() {
	}
	virtual void setupScreen();
	virtual void tearDownScreen();
	void setGasHumidity(float hum);
	void setGasTemperature(float temp);
	void setGasValues(float co_ppm, float o2_percent);
	void setCO2Value(float co2_ppm);
	void setProbeValue(float probe_temp);


	void updateTime(uint8_t hour,uint8_t minute,uint8_t second);

    void handleTickEvent();
    void start_Transmitte();
    void startLogger();

    void setEEPROMStatus(bool ok);


protected:

	//temperature & humidity buffer
	Unicode::UnicodeChar gasTempBuffer[10];  // matches wildcard size
	Unicode::UnicodeChar gasHumBuffer[10];
	//co & o2 buffer
	Unicode::UnicodeChar gasCoBuffer[10];
	Unicode::UnicodeChar gasO2Buffer[10];
	//co2 buffer
	Unicode::UnicodeChar gasCO2Buffer[10];
	//probe temperature buffer
	Unicode::UnicodeChar probTemperatureBuffer[10];


};

#endif // MAINSCREENVIEW_HPP
