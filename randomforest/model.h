//model struct api
#include "data.h"

struct RFNode{
	int featureId;
	float threshold;
	RFNode* left, right;
};

class RFModel{
public:
	void loadModel(char *file);
	void outputModel(char *file);
	void trainModel(DataRF * data);
	int predict(std::vector<float> inputData);
};
