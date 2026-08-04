#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

class Model {
public:
	Model();
	void tick();

	bool getEEPROMStatus();
	void setGasValues(float co_ppm, float o2_percent);

	void bind(ModelListener *listener) {
		modelListener = listener;
	}

	// Calibration functions
	void calibrateCOZero();
	void calibrateCOSpan(float target_ppm);
	void calibrateO2Zero();
	void calibrateO2Span(float target_pct);
	void calibrateCO2Zero();
	void calibrateCO2Span(float target_pct);

	// Reset Calibration
	void resetCOCal();
	void resetO2Cal();
	void resetCO2Cal();

	// Active Calibration Parameter Getters
	float getCO2Offset();
	float getCO2Slope();
	float getCOOffset();
	float getCOSlope();
	float getO2Offset();
	float getO2Slope();

	// Factory Reset Trigger
	void resetFactoryCalib();

protected:
	ModelListener *modelListener;
	float lastCoPpm;
	float lastO2Percent;
	float lastCO2ppm;
	float lastProbTemp;

};

#endif // MODEL_HPP
