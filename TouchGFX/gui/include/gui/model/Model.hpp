#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

class Model {
public:
	Model();
	void tick();

	void setGasValues(float co_ppm, float o2_percent);

	void bind(ModelListener *listener) {
		modelListener = listener;
	}

	//span

	void saveCOSpan(int raw, int target);

protected:
	ModelListener *modelListener;
	float lastCoPpm;
	float lastO2Percent;
	float lastCO2ppm;
	float lastProbTemp;

};

#endif // MODEL_HPP
