#include "data.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void splitstring(string &s, const char* delim, vector<string> & v){
	char * dup = _strdup(s.c_str());
	char * token = strtok(dup, delim);
	while(token != NULL){
		v.push_back(string(token));
		token = strtok(NULL, delim);
	}
	free(dup);
}

int DataRF::loadData(char *file){
	this -> ds.featureMap.clear();
	this -> ds.types.clear();
	this -> ds.values.clear();
	ifstream in(file);
	if(!in){
		cout << "open file error " << file << endl;
	}
	string line;
	while(getline(in, line)){
		vector<string> ls;
		splitstring(line, " :", ls);
		this -> ds.types.push_back(int(stoi(ls[0])));
		vector<float> tmp;
		for(int i = 0;i < ls.size() / 2;i ++){
			tmp.push_back(stof(ls[(i + 1) * 2]));
		}
		this -> ds.values.push_back(tmp);
	}
	cout << "load data has " << this -> ds.types.size() << " lines" << endl;
	return 1;
}
