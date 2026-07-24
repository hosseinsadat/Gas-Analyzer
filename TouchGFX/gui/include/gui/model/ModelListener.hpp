#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>
#include <touchgfx/hal/Types.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}


    virtual void onSht22Updated(float t, float h) {}
    virtual void onGasUpdated(float co_ppm, float o2_percent) {}
    virtual void onCO2Updated(float co2_ppm) {}
    virtual void onProbeUpdated(float probe_temp){}
   // virtual void onWarmupUpdate(float o2_pct);
    virtual void updateTime(uint8_t hour,uint8_t minute,uint8_t second){}

    virtual void onCOCalDataUpdated(int16_t raw_co, float cal_co) {}
    virtual void onO2CalDataUpdated(int16_t raw_o2, float cal_o2) {}
    virtual void onCO2CalDataUpdated(uint16_t raw_co2, float cal_co2) {}


    void bind(Model* m)
    {
        model = m;
    }
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
