#include "model.h"
#include <fstream>

using namespace std;
bool RFNode::loadModel(char *file)
{

}

bool RFNode::outputModel(char *file)
{
	ofstream out(file);
	out << "Trees number : " << Trees.size() << endl;
	out << endl;
	for (int i = 0;i < Trees.size();i ++){
		
	}
	
}

void initSimpleModel()
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