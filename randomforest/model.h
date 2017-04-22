//model struct api
#include "data.h"
#include <fstream>

struct RFNode{
	int featureId;
	float threshold;
	RFNode* left, *right;
	RFNode(): featureId(-1), threshold(0), left(NULL), right(NULL) {}
};

class RFModel{
private:
	std::vector<RFNode* > Trees;
	void outputOneTree(std::ofstream& out, RFNode * Tree, int deep);
	void insertTab(std::ofstream& out, int deep);
	float getValue(std::string s);
	RFNode* trainOneTree(DataRF * data);
	RFNode* trainChildTree(std::vector<int> &types, std::vector<std::vector<float> > &values, std::vector<int> usefeature, int lastfeature);
	
	int treeNum = 10;
	float sampleFeatureRate = 0.3;
	float minleaf = 10;
	
public:
	bool loadModel(char *file);
	bool outputModel(char *file);
	void trainModel(DataRF * data);
	void initSimpleModel();
	int predict(std::vector<float> inputData);
	void setTrainPara(int treeNum, float sampleFeatureRate);
};
