#include "model.h"
#include <queue>
#include <stack>
#include <iostream>
#include <time.h> 
using namespace std;


bool RFModel::loadModel(char *file)
{
	cout << file << endl;
	ifstream in(file);
	string buff;
	Trees.clear();
	RFNode * tmpTree = new RFNode();
	stack<RFNode* > parrent;
	bool isHead = false;
	while (getline(in, buff)) {
		cout << buff << endl;
		if (buff.rfind("<tree") != string::npos) {
			RFNode * Node = new RFNode();
			tmpTree = Node;
			Trees.push_back(Node);
			parrent.push(Node);
			
			continue;
		}
		if (buff.rfind("</tree") != string::npos) {
			parrent.pop();
			continue;
		}
		if (buff.rfind("feature") != string::npos) {
			
			int featureid = getValue(buff);
			tmpTree->featureId = featureid;
			continue;
		}
		if (buff.rfind("threshold") != string::npos) {
			float val = getValue(buff);
			tmpTree->threshold = val;
			continue;
		}
		if (buff.rfind("<left>") != string::npos) {
			RFNode * Node = new RFNode();
			tmpTree = Node;
			parrent.top()->left = Node;
			parrent.push(Node);
			continue;
		}
		if (buff.rfind("<right>") != string::npos) {
			RFNode * Node = new RFNode();
			tmpTree = Node;
			parrent.top()->right = Node;
			parrent.push(Node);
			continue;
		}
		if (buff.rfind("</left>") != string::npos) {
			parrent.pop();
			continue;
		}
		if (buff.rfind("</right>") != string::npos) {
			parrent.pop();
			continue;
		}
	}
	in.close();
	return true;
}

float RFModel::getValue(string s) 
{
	int start = s.find('>');
	string sub = s.substr(start + 2);
	int end = sub.find("<");
	string valstr = sub.substr(0, end - 1);
	float res = atof(valstr.c_str());
	return res;
}

bool RFModel::outputModel(char *file)
{
	ofstream out(file);
	out << "Trees number : " << Trees.size() << endl;
	out << endl;
	for (int i = 0;i < Trees.size();i ++){
		out << "<tree id = " << i << " >" << endl;
		outputOneTree(out, Trees[i], 1);
		out << "</tree>" << endl;
	}
	out.close();
	return true;
}

void RFModel::outputOneTree(ofstream& out, RFNode * Tree, int deep)
{

	insertTab(out, deep);
	out << "<feature> " << Tree->featureId << " </feature>" << endl;
	insertTab(out, deep);
	out << "<threshold> " << Tree->threshold << " </threshold>" << endl;
	if (Tree->featureId == -1) return;
	insertTab(out, deep);
	out << "<left> " << endl;
	outputOneTree(out, Tree->left, deep + 1);
	insertTab(out, deep);
	out << "</left> " << endl;
	
	insertTab(out, deep);
	out << "<right> " << endl;
	outputOneTree(out, Tree->right, deep + 1);
	insertTab(out, deep);
	out << "</right> " << endl;
}

void RFModel::insertTab(ofstream& out, int deep) {
	for (int i = 0; i < deep; i++) {
		out << "\t";
	}
}

void RFModel::initSimpleModel()
{
	RFNode * Tree = new RFNode();
	queue<RFNode*> addlist;
	addlist.push(Tree);
	for(int i = 0;i < 15;i ++){
		RFNode* left = new RFNode();
		RFNode* right = new RFNode();
		RFNode* node = addlist.front();
		node -> left = left;
		node -> right = right;
		node -> featureId = i;
		node -> threshold = i * 10;
		addlist.pop();
		addlist.push(left);
		addlist.push(right);
	}
	Trees.push_back(Tree);
	Trees.push_back(Tree);
}

void RFModel::setTrainPara(int treeNum, float sampleFeatureRate)
{
	this -> treeNum = treeNum;
	this -> sampleFeatureRate = treeNum;
}

void RFModel::trainModel(DataRF * data)
{
	Trees.clear();
	for(int i = 0;i < treeNum;i ++){
		RFNode* Tree = trainOneTree(data);
		Trees.push_back(Tree);
	}
}

RFNode* RFModel::trainOneTree(DataRF * data)
{
	vector<int> types;
	vector<vector<float>> values;
	srand((unsigned)time(NULL));
	for(int i = 0;i < data->ds.types.size();i ++){
		int rad = rand()%data->ds.types.size();
		types.push_back(data->ds.types[rad]);
		values.push_back(data->ds.values[rad]);
	}
	vector<int> feature;
	for(int i = 0;i < values[0].size();i ++){
		feature.push_back(i);
	}
	return trainChildTree(types, values, feature, -1);
}

RFNode* RFModel::trainChildTree(vector<int> &types, vector<vector<float> > &values, vector<int> feature, int lastfeature)
{
	vector<int> orifeature;
	for(int i = 0;i < feature.size();i ++){
		orifeature.push_back(feature[i]);
	}
	vector<int> usefeature;
	srand((unsigned)time(NULL));
	for (int i=0;i< feature.size() * (1 - sampleFeatureRate);i++){
		int rad = rand()%feature.size();
		orifeature[rad] = -1;
	}
	for(int i = 0;i < orifeature.size();i ++){
		if(orifeature[i] == -1) continue;
		usefeature.push_back(orifeature[i]);
	}
	int bestfeature = -1;
	float mindev = 10000000000;
	float bestval = 0;
	for(int i = 0;i < usefeature.size();i ++){
		int fea = usefeature[i];
		vector<vector<float> > tmpdata;
		vector<float > square;
		vector<float > avgval;
		for(int j = 0;j < types.size();j ++){
			vector<float> tmp;
			tmp.push_back(types[j]);
			tmp.push_back(values[j][fea]);
			tmpdata.push_back(tmp);
		}
		sort(tmpdata.begin(), tmpdata.end(), [](vector<float> &foo1, vector<float> &foo2){
			if(foo1[1] < foo2[1]){
				return true;
			}else{
				return false;
			}
		});
		float lastsqu = 0;
		float lastavg = 0;
		for(int k = 0;k < types.size();k ++){
			lastsqu += tmpdata[k][1] * tmpdata[k][1]; 
			lastavg += tmpdata[k][1];
			square.push_back(lastsqu);
			avgval.push_back(lastavg);
		}
		for(int k = minleaf;k < types.size() - minleaf - 1;k ++){
			float tmpdev = square[k] * (k + 1) - avgval[k] * avgval[k] + (lastsqu - square[k]) * (types.size() - k - 1) - (lastavg - avgval[k]) * (lastavg - avgval[k]);
			
			if (tmpdev < mindev){
				//cout << tmpdev << " tmpdev" << endl;
				mindev = tmpdev;
				bestfeature = fea;
				bestval = tmpdata[k][1];
			}
		}
		
	}

	cout << bestfeature << " " << mindev << " " << bestval << endl;
	
	RFNode* node = new RFNode();
	if(lastfeature == bestfeature || mindev < 0.1 || mindev == 10000000000 || bestfeature == -1){
		float avgtype = 0;
		for(int i = 0;i < types.size();i ++){
			avgtype += types[i];
		}
		avgtype /= types.size();
		node -> threshold = avgtype;
		return node;
	}
	node -> featureId = bestfeature;
	node -> threshold = bestval;
	//vector<int> &types, vector<std::vector<float> > &values, vector<int> feature, int lastfeature
	vector<int> lefttypes;
	vector<int> righttypes;
	vector<vector<float> > leftvalues;
	vector<vector<float> > rightvalues;
	for(int i = 0;i < types.size();i ++){
		if (values[i][bestfeature] <= bestval){
			lefttypes.push_back(types[i]);
			leftvalues.push_back(values[i]);
		}else{
			righttypes.push_back(types[i]);
			rightvalues.push_back(values[i]);
		}
	}
	
	node -> left = trainChildTree(lefttypes, leftvalues, feature, bestfeature);
	node -> right = trainChildTree(righttypes, rightvalues, feature, bestfeature);
	return node;
}







































