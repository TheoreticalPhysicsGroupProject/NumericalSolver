#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;


int main(int argc, char* argv[]){
	ifstream inputFile;
	string line;
	int negtest = 0;
	int fractest = 0;
	float number;
	vector<vector<float> > system;
	vector<vector<int> > boundaryconditions;
	vector<float> systemp;
	vector<int> boundtemp;
	vector<int> intigers;
	vector<int> fractions;
	int x;
	int y;

	
	inputFile.open(argv[1]);
	
	while(getline(inputFile, line)){
		for (int i = 0; i < line.length(); i++){
			if (line[i] == '_'){
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
			else if (line[i] == ' '){
				if(line[i-1] != '_' && line[i-1] != ' '){
					int intsize = intigers.size();
					for (int p = 0; p < intsize; p++){
						number += intigers[p]*pow(10, intsize-p-1);
					}
					if (fractest != 0){
						int fracsize = fractions.size();
						for (int p = 0; p < fracsize; p++){
						number += intigers[p]*pow(10, -p-1);
						}
						fractest = 0;
						fractions.clear();
					}
					if (negtest != 0){
						number = number * (-1);
						negtest = 0;
					}
					intigers.clear();
					systemp.push_back(number);
					boundtemp.push_back(1);
					number = 0;
					x++;
				}
			}
			else if (line[i] == '.'){
				fractest++;
			}
			else if (line[i] == '-'){
				negtest++;
			}
			else {
				if(fractest == 0){
					intigers.push_back(line[i]-48);
				}
				else{
					fractions.push_back(line[i]-48);
				}
			}
		}
	}
	
	inputFile.close();

	//for(vector<vector<float> >::iterator it = system.begin(); it != system.end(); ++it)
	//{
		////it is now a pointer to a vector<int>
		//for(vector<float>::iterator jt = it->begin(); jt != it->end(); ++jt)
		//{
			//// jt is now a pointer to an integer.
			//cout << *jt << " ";
		//}
		//cout << endl;
	//}
	//cout << endl;
	//for(vector<vector<int> >::iterator it = boundaryconditions.begin(); it != boundaryconditions.end(); ++it)
	//{
		////it is now a pointer to a vector<int>
		//for(vector<int>::iterator jt = it->begin(); jt != it->end(); ++jt)
		//{
			//// jt is now a pointer to an integer.
			//cout << *jt << " ";
		//}
		//cout << endl;
	//}
	return 0;
}
