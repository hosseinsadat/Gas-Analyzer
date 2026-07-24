#include <gui/mainscreen_screen/mainScreenView.hpp>
#include <touchgfx/hal/Types.hpp>


mainScreenView::mainScreenView()
{

}

void mainScreenView::setupScreen()
{
    mainScreenViewBase::setupScreen();
}

void mainScreenView::tearDownScreen()
{
    mainScreenViewBase::tearDownScreen();
}


void mainScreenView::setGasTemperature(float temp)
{
	Unicode::snprintfFloat(gasTempBuffer	,TXTGASTEMP_SIZE , "%.1f",temp);
	txtGasTemp.setWildcard(gasTempBuffer);
    txtGasTemp.invalidate();
}
void mainScreenView::setGasHumidity(float hum)
{
	Unicode::snprintfFloat(gasHumBuffer	,TXTGASHUM_SIZE , "%.1f",hum);
	txtGasHum.setWildcard(gasHumBuffer);
	txtGasHum.invalidate();
}
void mainScreenView::setGasValues(float co_ppm, float o2_percent)
{
    Unicode::snprintfFloat(gasCoBuffer, TXTCO_SIZE, "%.0f", co_ppm);
    txtCO.setWildcard(gasCoBuffer);
    txtCO.invalidate();

    Unicode::snprintfFloat(gasO2Buffer, TXTO2_SIZE, "%.1f", o2_percent);
    txtO2.setWildcard(gasO2Buffer);
    txtO2.invalidate();
}
void mainScreenView::setCO2Value(float co2_ppm)
{
	if(co2_ppm>0){
    Unicode::snprintfFloat(gasCO2Buffer, TXTCO2_SIZE, "%.0f", co2_ppm);
    txtCO2.setWildcard(gasCO2Buffer);
    txtCO2.invalidate();
    Unicode::snprintfFloat(txtCO2_percentBuffer, TXTCO2_PERCENT_SIZE, "%.1f", co2_ppm/10000.0f);
       txtCO2_percent.setWildcard(txtCO2_percentBuffer);
       txtCO2_percent.invalidate();
	}else{
		Unicode::snprintfFloat(gasCO2Buffer, TXTCO2_SIZE, "%.0f", 0.0f);
		    txtCO2.setWildcard(gasCO2Buffer);
		    txtCO2.invalidate();
		    Unicode::snprintfFloat(txtCO2_percentBuffer, TXTCO2_PERCENT_SIZE, "%.1f", 0.0f);
		       txtCO2_percent.setWildcard(txtCO2_percentBuffer);
		       txtCO2_percent.invalidate();
	}
}
void mainScreenView::setProbeValue(float probe_temp)
{
Unicode::snprintfFloat(probTemperatureBuffer,TXTPROBE_SIZE, "%.1f",probe_temp);
txtProbe.setWildcard(probTemperatureBuffer);
txtProbe.invalidate();

}
void mainScreenView::handleTickEvent(){
#ifndef SIMULATOR
	static uint16_t counter = 0;

	    counter++;

	    if(counter >= 60)
	    {
	        counter = 0;

	        presenter->updateEEPROMStatus();
	    }
#endif

}

void mainScreenView::updateTime(uint8_t hour,uint8_t minute,uint8_t second)
{
#ifndef SIMULATOR
	digitalClock.setTime24Hour(hour, minute, second);
	digitalClock.invalidate();
#endif
}


void mainScreenView::startLogger()
{
#ifndef SIMULATOR
	extern bool Logger_State;

	if(tgBtnLogger.getState())
	{
		Logger_State = true;

	}else{
		Logger_State = false;
	}
#endif
}
void mainScreenView::start_Transmitte()
{
#ifndef SIMULATOR
	extern bool NRF_State;

	if(tgBtnRadio.getState())
	{
		NRF_State = true;

	}else{
		NRF_State = false;
	}
#endif
}


