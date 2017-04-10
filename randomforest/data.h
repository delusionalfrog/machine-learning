//data struct api
#include <vector>
#include <map>
#include <string>

struct DataStruct{
	std::map<int, int> featureMap;
	std::vector<int> types;
	std::vector<std::vector<float> > values;
};

class DataRF{
public:
	DataStruct ds;
	int loadData(char* file);
	
};

void splitstring(std::string &s, const char* delim, std::vector<std::string> & v);
