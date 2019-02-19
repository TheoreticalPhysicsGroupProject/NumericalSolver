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

	// NUMERICAL SOLVER

	// Define variables
	int choice, num_it;
	float eps = 0.0001;
	int imax = values.size(), jmax = values[0].size();

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



	double values2[imax][jmax];
	double inrad=6.;
	double outrad=19.;
	double outvol=9.;
	double rad;
	double centrei, centrej;
	centrei= (imax-1) / 2;
	centrej= (jmax-1) / 2;
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
	}

	double analyt2[imax][jmax], difference2[imax][jmax], rad2, dist=jmax-1, voltage= values[0][0], theta, R;
	for(i=0; i<imax; i++) {
		for (j=0; j<jmax; j++) {
			if (boundaryconditions[i][j] == 1) {
				analyt2[i][j]=values[i][j];
				continue;
			}
			else {
				rad2= sqrt(pow(abs(j-centrej), 2)+pow(abs(i-centrei), 2));
				theta = atan(abs(j-centrej)/(abs(i-centrei)));

				analyt2[i][j]= ((2*voltage*pow(R,2))/dist)*(cos(theta)/rad2)+((2*voltage)/dist)*rad2*cos(theta);
			}
		}
	}
	for (int i = 0; i < imax; ++i)
	{
			for (int j = 0; j < jmax; ++j)
			{
				difference2[i][j]= abs(analyt2[i][j] - values[i][j]);
				cout << difference2[i][j] << " ";
			}
			cout << endl;
	}


	// OUTPUT
	int MinxLimit = -5 , MaxxLimit = 5 , MinyLimit = -5 , MaxyLimit = 5; // These are the values of the limits of the labels on the heat-map.

	x = imax; y = jmax; // A conversion of variables

	ofstream myfile; //creating a file to base the gnuplot off of
	myfile. open ("outputtestheatmap.txt"); //creating a file to output
	
	
	for ( int i = 0 ; i <= x ;  i++ ){
		
		if (i == 0) { //first row
						
			for (int j = 0 ; j <= y ; j++){ //this code gives the x limits for the gnuplot
				
				if (j == 0) {
				myfile << MaxxLimit << " ";
				}
				else if (j == 1) {		//left x limit for the gnuplot
				myfile << MaxxLimit << " ";
				}
				else if ( j < y ) { //placeholders, could be anything
				myfile << 0 << " ";
				}
				else { //right limit for the Gnuplot
				myfile << MinxLimit;
				}
				
			}
		}
		else{
		
			for (int j = 0 ; j <= y ; j++){
				
				if( j == 0 ){
					if ( i == 1){ // top y limit for the gnuplot
						myfile << MaxyLimit << " ";
					}
				
					else if ( i < x ){ // Placeholders as no data can exist in y==0, this fills the data
						myfile << 0 << " ";
					}
				
					else{ //bottom limit for the gnuplot
						myfile << MinyLimit << " ";
					}
				}
				
				else{ // data
					
					//if ( values[i-1][j-1] >= 9 ){
					//	values[i-1][j-1] = 100;
					//}
					
					
					myfile << analyt2[i-1][j-1] << " ";
				}
			
			}
	
		}
		myfile << endl; //endline 
	}
	myfile.close(); //closes the Heatmap file.
	
	int multip = 2; // This MUST BE < 1. This exists so that there are less arrows then there are pixels, dividing the total value of data-pixels by this amount. 
	
	int xo = x/multip , yo = y/multip ;
	
	
	ofstream out; //Creting the Vector-map
	out.open ("outputtestvector.txt"); //creating a file to output

	for (int i = 1 ; i < xo ; i++){ // then creating the x values


		for (int j = 1 ; j < yo ;  j++ ){ //creating the y values first

						
			out << -j << "\t"; // if this isn't negative, the values will come out with a pi/2 shift
			out << -i << "\t"; //as above
			
						
			//double dx = 2;
			double dx = values [i*multip + (multip-1)] [j*multip] - values [i*multip - (multip-1)] [j*multip]; // this is, dx = value to the left of system[][] - value to the right of system[][]. the multipliers exist to compensate for the lack of details above
			//double dy = 1;
			double dy = values [i*multip] [j*multip + (multip-1)] - values [i*multip] [j*multip - (multip-1)]; // same as above but with values above and below system[][]. with the multipliers
			
			double direction = atan2 (dx,dy); //this is the atan code for finding the entrie circle instead of simply the first quadrant 
			
			double magnitude = pow(pow(dx,2)+pow(dy,2),0.5); // sqrt(a^2+b^2) = c^2 . pythagorean theorem
			
			
			out << direction << "\t";
			
			/*if(magnitude > ){
				magnitude = 15;
			}*/
			out << magnitude << endl;
			//out << pow(magnitude,0.1) << endl;
					 
						
		}
						
	}
	
	out.close();
	
	return 0;
}