#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


int main(){
	ifstream inputFile;
	string line;
	vector<vector<float> > system;
	vector<vector<int> > boundaryconditions;
	vector<float> systemp;
	vector<int> boundtemp;
	int x;
	int y;

	
	inputFile.open("inputsystem.txt");
	
	while(inputFile >> line){
		for (int i = 0; i < line.length(); i++){
			if (line[i] == '.'){
				(systemp).push_back(0);
				(boundtemp).push_back(0);
				x++;
			}
			else if (line[i] == '/'){
				system.push_back(systemp);
				boundaryconditions.push_back(boundtemp);
				systemp.clear();
				boundtemp.clear();
				y++;
				x = 0;
				
			}
			else if (line[i] == '|'){
				system.push_back(systemp);
				boundaryconditions.push_back(boundtemp);
				systemp.clear();
				boundtemp.clear();
			}
			else {
				(systemp).push_back(line[i]-48);
				(boundtemp).push_back(1);
				x++;
			}
		}
	}
	
	inputFile.close();

	for(vector<vector<float> >::iterator it = system.begin(); it != system.end(); ++it)
	{
		//it is now a pointer to a vector<int>
		for(vector<float>::iterator jt = it->begin(); jt != it->end(); ++jt)
		{
			// jt is now a pointer to an integer.
			cout << *jt;
		}
		cout << endl;
	}
	cout << endl;
	for(vector<vector<int> >::iterator it = boundaryconditions.begin(); it != boundaryconditions.end(); ++it)
	{
		//it is now a pointer to a vector<int>
		for(vector<int>::iterator jt = it->begin(); jt != it->end(); ++jt)
		{
			// jt is now a pointer to an integer.
			cout << *jt;
		}
		cout << endl;
	}
	return 0;
}
