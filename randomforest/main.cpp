#include <iostream>
#include "data.h"
#include "model.h"
using namespace std;

int main(int argc, char **argv){
	if(argc < 2)
		return 0;
	DataRF* drf = new DataRF();
	drf -> loadData((char *)argv[1]);
	RFModel 
	return 0;
}
