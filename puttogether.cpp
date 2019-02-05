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
	vector<vector<float> > values;
	vector<vector<int> > boundaryconditions;
	vector<float> systemp;
	vector<int> boundtemp;
	vector<int> intigers;
	vector<int> fractions;
	int i,j,x,y;

	
	inputFile.open(argv[1]);
	
	while(getline(inputFile, line)){
		for (int i = 0; i < line.length(); i++){
			if (line[i] == '_'){
				(systemp).push_back(0);
				(boundtemp).push_back(0);
				x++;
			}
			else if (line[i] == '/'){
				values.push_back(systemp);
				boundaryconditions.push_back(boundtemp);
				systemp.clear();
				boundtemp.clear();
				y++;
				x = 0;
				
			}
			else if (line[i] == '|'){
				values.push_back(systemp);
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

	int imax = values.size(); cout << imax;
	int jmax = values[0].size(); cout << jmax;
	int iterations = 100000;
	for(int k=0; k<iterations; k++) {
	for(i=0; i<imax; i++) {
		for (j=0; j<jmax; j++) {
			if (boundaryconditions[i][j] == 1) {
				continue;
			}
			else if (i == 0 && j == 0) {
				values[i][j] = (values[i+1][j]+values[i][j+1])/2;
			}
			else if (i == imax-1 && j == jmax-1) {
				values[i][j] = (values[i-1][j]+values[i][j-1])/2;
			}
			else if (i == 0 && j == jmax-1) {
				values[i][j] = (values[i+1][j]+values[i][j-1])/2;
			}
			else if (i == imax-1 && j == 0) {
				values[i][j] = (values[i-1][j]+values[i][j+1])/2;
			}
			else if (i == 0) {
				values[i][j] = (values[i+1][j]+values[i][j+1]+values[i][j-1])/3;
			}
			else if (j == 0) {
				values[i][j] = (values[i+1][j]+values[i-1][j]+values[i][j+1])/3;
			}
			else if (i == imax-1) {
				values[i][j] = (values[i-1][j]+values[i][j+1]+values[i][j-1])/3;
			}
			else if (j == jmax-1) {
				values[i][j] = (values[i+1][j]+values[i-1][j]+values[i][j-1])/3;
			}
			else {
				values[i][j] = (values[i+1][j]+values[i-1][j]+values[i][j+1]+values[i][j-1])/4;
			}
		}
	}
}
	for (int i = 0; i < imax; ++i)
	{
		for (int j = 0; j < jmax; ++j)
		{
			cout << values[i][j] << " ";
		}
		cout << endl;
	}

	int MinxLimit = -5 , MaxxLimit = 5 , MinyLimit = -5 , MaxyLimit = 5;

	x = imax; y = jmax;

	ofstream myfile;
	myfile. open ("outputtestheatmap.txt"); //creating a file to output
	
	
	for ( int i = 0 ; i <= x ;  i++ ){
		
		if (i == 0) { //first row
						
			for (int j = 0 ; j <= y ; j++){
				
				if (j == 0) {		//placeholder text, could be anything
				myfile << MinxLimit << " ";
				}
				else if (j == 1) {		//left x limit for the gnuplot
				myfile << MinxLimit << " ";
				}
				else if ( j < y ) { //placeholders, could be anything
				myfile << 0 << " ";
				}
				else { //right limit for the Gnuplot
				myfile << MaxxLimit;
				}
				
			}
		}
		else{
		
			for (int j = 0 ; j <= y ; j++){
				
				if( j == 0 ){
					if ( i == 1){ //top limit for the gnuplot
						myfile << MinyLimit << " ";
					}
				
					else if ( i < x ){ // placeholders
						myfile << 0 << " ";
					}
				
					else{ //bottom limit for the gnuplot
						myfile << MaxyLimit << " ";
					}
				}
				
				else{ //actual data
					myfile << values[i-1][j-1] << " ";
				}
			
			}
	
		}
		myfile << endl; //endline 
	}
	myfile.close(); //closes the file.
	int multip = 2; //(pow(x,0.5)+1); //a simple "pow(x,0.5)" causes the thing to seg fault for some reason, though it still has a good result
	int xo = x/multip , yo = y/multip ;
	
	
	ofstream out;
	out.open ("outputtestvector.txt"); //creating a file to output


	for (int i = 1 ; i < xo ;  i++ ){
				
		for (int j = 1 ; j < yo ; j++){

						
			out << i << "\t";
			out << j << "\t";
			double dx = values[i*multip+1*multip][j*multip]-values[i*multip-1*multip][j*multip];
			double dy = values[i*multip][j*multip+1*multip]-values[i*multip][j*multip-1*multip];
			
			double direction = atan2 (dy,dx);
			double magnitude = pow(pow(dx,2)+pow(dy,2),0.5);
			
			
			out << direction << "\t";
			out << magnitude << endl;
					 
						
		}
						
	}
	
	out.close();
	
	
	// values("./combinedplot.sh");
	
	return 0;
}