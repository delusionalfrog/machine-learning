#include <iostream>
#include "model.h"
#include "stdio.h"

using namespace std;

int main(int argc, char **argv){
	//if(argc < 2)
	//	return 0;
	DataRF* drf = new DataRF();
	drf -> loadData((char *)argv[1]);
	//drf->loadData("D:/mphspace/machinelearning/machine-learning/randomforest/traindata");
	RFModel *rftest = new RFModel();
	rftest->trainModel(drf);
	//rftest->initSimpleModel();
	//rftest->loadModel("D:/mphspace/machinelearning/machine-learning/randomforest/test.model");
	//rftest->outputModel("D:/mphspace/machinelearning/machine-learning/randomforest/test.model.bak");
	rftest->outputModel((char *)argv[2]);
	return 0;
}
