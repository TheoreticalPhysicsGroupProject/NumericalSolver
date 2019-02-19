#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

int jacobi(vector<vector<float> >& values, vector<vector<int> >& boundaryconditions, float eps) {
	float error, maxerror, newval;
	int num_it = 0, divisor, imax = values.size(), jmax = values[0].size();
	vector<vector<float> > values_old;
	do {
		maxerror = 0.;
		values_old = values;
		for(int i=0; i<imax; i++) {
			for (int j=0; j<jmax; j++) {
				if (boundaryconditions[i][j] == 0) {
					newval = 0.; divisor = 0;
					if (i != 0) {
						newval += values_old[i-1][j];
						divisor++;
					}
					if (i != (imax-1)) {
						newval += values_old[i+1][j];
						divisor++;
					}
					if (j != 0) {
						newval += values_old[i][j-1];
						divisor++;
					}
					if (j != (jmax-1)) {
						newval += values_old[i][j+1];
						divisor++;
					}
					newval /= (float)divisor;
					error = abs((newval - values_old[i][j]) / newval);
					if (error > maxerror)
						maxerror = error;
					values[i][j] = newval;
				}
			}
		}
		num_it++;
		cout << "Iteration No. " << num_it << endl;
	}
	while (maxerror > eps);
	return num_it;
}

int gauss(vector<vector<float> >& values, vector<vector<int> >& boundaryconditions, float eps) {
	float error, maxerror, newval;
	int num_it = 0, divisor, imax = values.size(), jmax = values[0].size();
	do {
		maxerror = 0.;
		for(int i=0; i<imax; i++) {
			for (int j=0; j<jmax; j++) {
				if (boundaryconditions[i][j] == 0) {
					newval = 0.; divisor = 0;
					if (i != 0) {
						newval += values[i-1][j];
						divisor++;
					}
					if (i != (imax-1)) {
						newval += values[i+1][j];
						divisor++;
					}
					if (j != 0) {
						newval += values[i][j-1];
						divisor++;
					}
					if (j != (jmax-1)) {
						newval += values[i][j+1];
						divisor++;
					}
					newval /= (float)divisor;
					error = abs((newval - values[i][j]) / newval);
					if (error > maxerror)
						maxerror = error;
					values[i][j] = newval;
				}
			}
		}
		num_it++;
	}
	while (maxerror > eps);
	return num_it;
}

int gauss_rb(vector<vector<float> >& values, vector<vector<int> >& boundaryconditions, float eps) {
	float error, maxerror, newval;
	int num_it = 0, divisor, imax = values.size(), jmax = values[0].size();
	do {
		maxerror = 0.;
		for(int k=0; k<2; k++) {
			for(int i=0; i<imax; i++) {
				for (int j=0; j<jmax; j++) {
					if (boundaryconditions[i][j] == 0 && (i + j) % 2 == k) {
						newval = 0.; divisor = 0;
						if (i != 0) {
							newval += values[i-1][j];
							divisor++;
						}
						if (i != (imax-1)) {
							newval += values[i+1][j];
							divisor++;
						}
						if (j != 0) {
							newval += values[i][j-1];
							divisor++;
						}
						if (j != (jmax-1)) {
							newval += values[i][j+1];
							divisor++;
						}
						newval /= (float)divisor;
						error = abs((newval - values[i][j]) / newval);
						if (error > maxerror)
							maxerror = error;
						values[i][j] = newval;
					}
				}
			}
		}
		num_it++;
	}
	while (maxerror > eps);
	return num_it;
}

int gauss_sor(vector<vector<float> >& values, vector<vector<int> >& boundaryconditions, float s, float eps) {
	float error, maxerror, newval;
	int num_it = 0, divisor, imax = values.size(), jmax = values[0].size();
	do {
		maxerror = 0.;
		for(int i=0; i<imax; i++) {
			for (int j=0; j<jmax; j++) {
				if (boundaryconditions[i][j] == 0) {
					newval = 0.;
					divisor = 0;
					if (i != 0) {
						newval += s*values[i-1][j];
						divisor++;
					}
					if (i != (imax-1)) {
						newval += s*values[i+1][j];
						divisor++;
					}
					if (j != 0) {
						newval += s*values[i][j-1];
						divisor++;
					}
					if (j != (jmax-1)) {
						newval += s*values[i][j+1];
						divisor++;
					}
					newval = (1.-s) * values[i][j] + newval / (float)divisor;
					error = abs((newval - values[i][j]) / newval);
					if (error > maxerror)
						maxerror = error;
					values[i][j] = newval;
				}
			}
		}
		num_it++;
	}
	while (maxerror > eps);
	return num_it;
}

int main(int argc, char* argv[]){
	ifstream inputFile;
	string line;
	int negtest = 0;
	int fractest = 0;
	float number = 0.;
	vector<vector<float> > values;
	vector<vector<int> > boundaryconditions;
	vector<float> systemp;
	vector<int> boundtemp;
	vector<int> intigers;
	vector<int> fractions;
	int i = 0, j = 0, x = 0, y = 0;

	
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

	// SOLVER

	int choice;
	float eps = 0.0001;

	// Print results:
	cout << "Input Matrix:" << endl;
	for (int i = 0; i < imax; ++i)
	{
		for (int j = 0; j < jmax; ++j)
		{
			cout << values[i][j] << " ";
		}
		cout << endl;
	}

	cout << "Boundary conditions:" << endl;
	for (int i = 0; i < imax; ++i)
	{
		for (int j = 0; j < jmax; ++j)
		{
			cout << boundaryconditions[i][j] << " ";
		}
		cout << endl;
	}
	cout << "Choose your method:" << endl << 
	"1 - Jacobi" << endl <<
	"2 - Gauß-Seidel" << endl <<
	"3 - Gauß-Seidel with Red-Black Ordering" << endl <<
	"4 - Gauß-Seidel with Successive Over Relaxation" << endl;
	cin >> choice;

	switch (choice) {
		case 1:
		// Jacobi
		num_it = jacobi(values, boundaryconditions, eps);
		break;

		case 2:
		// Gauß-Seidel
		num_it = gauss(values, boundaryconditions, eps);
		break;

		case 3:
		// Gauß-Seidel with Red-Black-ordering
		num_it = gauss_rb(values, boundaryconditions, eps);
		break;

		case 4:
		// Gauß-Seidel with SOR
		num_it = gauss_sor(values, boundaryconditions, 1.2, eps);
		break;

		default:
		cout << "Well, that was not one of the options, was it?" << endl;
	}

	cout << "Resulting matrix:" << endl;

	for (int i = 0; i < imax; ++i)
	{
		for (int j = 0; j < jmax; ++j)
		{
			cout << values[i][j] << " ";
		}
		cout << endl;
	}
	cout << "Number of iterations: " << num_it << endl;

	/*double values2[imax][jmax];
	double inrad=6.;
	double outrad=18.;
	double outvol=9.;
	double rad;
	double centrei, centrej;
	centrei= 38;
	centrej= 18;
	//cout << centrei << endl;
	//cout << centrej << endl;
	for(i=0; i<imax; i++) {
		for (j=0; j<jmax; j++) {
			if (boundaryconditions[i][j] == 1) {
				values2[i][j]=values[i][j];
				continue;
			}
			else {
				rad= sqrt(pow(abs(j-centrej), 2)+pow(abs(i-centrei), 2));
				//cout << rad << endl;
				values2[i][j]= (outvol*log(inrad/rad))/(log(inrad/outrad));
				//cout << values[i][j] << endl;
			}

		}
	}
	cout << "Second" << endl;
	double difference[imax][jmax];
	for (int i = 0; i < imax; ++i)
	{
			for (int j = 0; j < jmax; ++j)
			{
				difference[i][j]= values2[i][j] - values[i][j];
				cout << difference[i][j] << " ";
			}
			cout << endl;
	}*/

	// OUTPUT
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
			double dx = values[i*multip+(multip-1)][j*multip]-values[i*multip-(multip-1)][j*multip];
			double dy = values[i*multip][j*multip+(multip-1)]-values[i*multip][j*multip-(multip-1)];
			
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