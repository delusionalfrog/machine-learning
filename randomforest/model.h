//model struct api
#include "data.h"

struct RFNode{
	int featureId;
	float threshold;
	RFNode* left, right;
	RFNode(): featureId(-1), threshold(0), left(NULL), right(NULL) {}
};

class RFModel{
private:
	vector<RFModel* > Trees;
public:
	bool loadModel(char *file);
	bool outputModel(char *file);
	void trainModel(DataRF * data);
	void initSimpleModel();
	int predict(std::vector<float> inputData);
};
