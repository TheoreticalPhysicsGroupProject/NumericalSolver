#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <time.h>
#include <chrono>
#include <time.h>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/times.h"
#include <sys/resource.h>
#include <sys/times.h>
#include <unistd.h>
#include <mach/mach.h>

using namespace std;
using namespace std::chrono;

void error_calc(float &oldval, float &newval, float &eps, int &contcond){
	float error;
	if (newval < eps)
		error = abs(newval - oldval);
	else
		error = abs((newval - oldval) / newval);
	if (error > eps) {
		contcond = 1;
	}
}


double cpu_time() {
	struct rusage usage;
	getrusage(RUSAGE_SELF, &usage);
	double c= usage.ru_utime.tv_usec + usage.ru_stime.tv_usec;
	return c;
}

size_t currentRSS()
{
    struct mach_task_basic_info info;
    mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &count) == KERN_SUCCESS)
        return (size_t)info.resident_size;
    return (size_t)0; /* query failed */
}


int jacobi(vector<vector<float> >& values, vector<vector<int> >& boundaryconditions, float eps) {
	float error, newval;
	int num_it = 0, divisor, contcond = 0;
	float change = 0.;
	vector<vector<float> > values_old;
	do {
		contcond = 0;
		values_old = values;
		for(int i=0; i<values.size(); i++) {
			for (int j=0; j<values[0].size(); j++) {
				if (boundaryconditions[i][j] == 0) {
					newval = 0.; divisor = 0;
					if (i != 0) {
						newval += values_old[i-1][j];
						divisor++;
					}
					if (i != (values.size()-1)) {
						newval += values_old[i+1][j];
						divisor++;
					}
					if (j != 0) {
						newval += values_old[i][j-1];
						divisor++;
					}
					if (j != (values[0].size()-1)) {
						newval += values_old[i][j+1];
						divisor++;
					}
					newval /= (float)divisor;
					error_calc(values_old[i][j], newval, eps, contcond);
					values[i][j] = newval;
				}
			}
		}
		num_it++;
	}
	while (contcond == 1);
	cout << "Iteration No. " << num_it << endl;
	return num_it;
}

int gauss(vector<vector<float> >& values, vector<vector<int> >& boundaryconditions, float eps) {
	float error, newval;
	int num_it = 0, divisor, contcond = 0;
	do {
		contcond = 0;
		for(int i=0; i<values.size(); i++) {
			for (int j=0; j<values[0].size(); j++) {
				if (boundaryconditions[i][j] == 0) {
					newval = 0.; divisor = 0;
					if (i != 0) {
						newval += values[i-1][j];
						divisor++;
					}
					if (i != (values.size()-1)) {
						newval += values[i+1][j];
						divisor++;
					}
					if (j != 0) {
						newval += values[i][j-1];
						divisor++;
					}
					if (j != (values[0].size()-1)) {
						newval += values[i][j+1];
						divisor++;
					}
					newval /= (float)divisor;
					error_calc(values[i][j], newval, eps, contcond);
					values[i][j] = newval;
				}
			}
		}
		num_it++;
	}
	while (contcond == 1);
	cout << "Iteration No. " << num_it << endl;
	return num_it;
}


int gauss_rb(vector<vector<float> >& values, vector<vector<int> >& boundaryconditions, float eps) {
	float error, newval;
	int num_it = 0, divisor, contcond = 0;
	do {
		contcond = 0;
		for(int k=0; k<2; k++) {
			for(int i=0; i<values.size(); i++) {
				for (int j=0; j<values[0].size(); j++) {
					if (boundaryconditions[i][j] == 0 && (i + j) % 2 == k) {
						newval = 0.; divisor = 0;
						if (i != 0) {
							newval += values[i-1][j];
							divisor++;
						}
						if (i != (values.size()-1)) {
							newval += values[i+1][j];
							divisor++;
						}
						if (j != 0) {
							newval += values[i][j-1];
							divisor++;
						}
						if (j != (values[0].size()-1)) {
							newval += values[i][j+1];
							divisor++;
						}
						newval /= (float)divisor;
						error_calc(values[i][j], newval, eps, contcond);
						values[i][j] = newval;
					}
				}
			}
		}
		num_it++;
	}
	while (contcond == 1);
	cout << "Iteration No. " << num_it << endl;
	return num_it;
}

int gauss_sor(vector<vector<float> >& values, vector<vector<int> >& boundaryconditions, float s, float eps, int max_it) {
	float error, newval;
	int num_it = 0, divisor, contcond = 0;
	do {
		contcond = 0;
		for(int i=0; i<values.size(); i++) {
			for (int j=0; j<values[0].size(); j++) {
				if (boundaryconditions[i][j] == 0) {
					newval = 0.;
					divisor = 0;
					if (i != 0) {
						newval += s*values[i-1][j];
						divisor++;
					}
					if (i != (values.size()-1)) {
						newval += s*values[i+1][j];
						divisor++;
					}
					if (j != 0) {
						newval += s*values[i][j-1];
						divisor++;
					}
					if (j != (values[0].size()-1)) {
						newval += s*values[i][j+1];
						divisor++;
					}
					newval = (1.-s) * values[i][j] + (newval / (float)divisor);
					error_calc(values[i][j], newval, eps, contcond);
					values[i][j] = newval;
				}
			}
		}
		num_it++;
		if (num_it > max_it && max_it != 0) {
			num_it = -1;
			break;
		}
		cout << "Iteration No. " << num_it << endl;
	}
	while (contcond == 1);
	return num_it;
}


int main(int argc, char* argv[]){
	high_resolution_clock::time_point start, end;
	start= high_resolution_clock::now();
	ifstream inputFile;
	string line;
	int negtest = 0, fractest = 0, i = 0, j = 0, x = 0, y = 0;
	float number = 0.;
	int multip = 1;
	vector<vector<float> > values;
	vector<vector<int> > boundaryconditions;
	vector<float> systemp;
	vector<int> boundtemp, intigers, fractions;

	
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

	// Parameters
	float eps = 0.00001;
	int kmax = 100;

	// Internal variables
	int choice, num_it, imax = values.size(), jmax = values[0].size();
	high_resolution_clock::time_point t1, t2;
	double func_cputime, beforecputime, aftercputime;
	float duration;
	float s, s_ideal; int num_it_ideal = 0; vector<vector<float> > values_sor, values_orig;// In case SOR is chosen
	//double numdiff[imax][jmax];
	//vector<vector<float> > valuesdos; 
	//valuesdos = values;

	// Print input
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

	/*for ( int d=0; d<2; ++d)
	{
		if(d == 0)
		{*/

			// Choice of method
			cout << "Choose your method:" << endl << 
			"1 - Jacobi" << endl <<
			"2 - Gauß-Seidel" << endl <<
			"3 - Gauß-Seidel with Red-Black Ordering" << endl <<
			"4 - Gauß-Seidel with Successive Over Relaxation" << endl;
			cin >> choice;


			switch (choice) {
				case 1:
				// Jacobi
				beforecputime=cpu_time();
				t1 = high_resolution_clock::now();
				num_it = jacobi(values, boundaryconditions, eps);
				t2 = high_resolution_clock::now();
		    	duration = duration_cast<microseconds>( t2 - t1 ).count();
		    	aftercputime=cpu_time();
		    	func_cputime= aftercputime - beforecputime;
				break;

				case 2:
				// Gauß-Seidel
				beforecputime=cpu_time();
				t1 = high_resolution_clock::now();
				num_it = gauss(values, boundaryconditions, eps);
				t2 = high_resolution_clock::now();
		    	duration = duration_cast<microseconds>( t2 - t1 ).count();
		    	aftercputime=cpu_time();
		    	func_cputime= aftercputime - beforecputime;
				break;

				case 3:
				// Gauß-Seidel with Red-Black-ordering
				beforecputime=cpu_time();
				t1 = high_resolution_clock::now();
				num_it = gauss_rb(values, boundaryconditions, eps);
				t2 = high_resolution_clock::now();
		    	duration = duration_cast<microseconds>( t2 - t1 ).count();
		    	aftercputime=cpu_time();
		    	func_cputime= aftercputime - beforecputime;
				break;

				case 4:
				// Gauß-Seidel with SOR
				beforecputime=cpu_time();
				t1 = high_resolution_clock::now();
				values_orig = values;

				for (int k = 0; k<kmax; k++) {
					values_sor = values_orig;
					s = 1+(1/kmax)*k;
					cout << "s = " << s << endl;
					num_it = gauss_sor(values_sor, boundaryconditions, s, eps, num_it_ideal);
					if (num_it == -1)
						continue;
					else if (k == 0 or num_it < num_it_ideal) {
						num_it_ideal = num_it;
						s_ideal = s;
						values = values_sor;
					}
				}
				t2 = high_resolution_clock::now();
		    	duration = duration_cast<microseconds>( t2 - t1 ).count();
		    	aftercputime=cpu_time(); 
		    	func_cputime= aftercputime - beforecputime;
				break;

				default:
				cout << "Well, that was not one of the options, was it?" << endl;
			}
		/*}
		else if(d == 1)
		{
			// Choice of method
			cout << "Choose your method:" << endl << 
			"1 - Jacobi" << endl <<
			"2 - Gauß-Seidel" << endl <<
			"3 - Gauß-Seidel with Red-Black Ordering" << endl <<
			"4 - Gauß-Seidel with Successive Over Relaxation" << endl;
			cin >> choice;


			switch (choice) {
				case 1:
				// Jacobi
				beforecputime=cpu_time();
				t1 = high_resolution_clock::now();
				num_it = jacobi(valuesdos, boundaryconditions, eps);
				t2 = high_resolution_clock::now();
		    	duration = duration_cast<microseconds>( t2 - t1 ).count();
		    	aftercputime=cpu_time();
		    	func_cputime= aftercputime - beforecputime;
				break;

				case 2:
				// Gauß-Seidel
				beforecputime=cpu_time();
				t1 = high_resolution_clock::now();
				num_it = gauss(valuesdos, boundaryconditions, eps);
				t2 = high_resolution_clock::now();
		    	duration = duration_cast<microseconds>( t2 - t1 ).count();
		    	aftercputime=cpu_time();
		    	func_cputime= aftercputime - beforecputime;
				break;

				case 3:
				// Gauß-Seidel with Red-Black-ordering
				beforecputime=cpu_time();
				t1 = high_resolution_clock::now();
				num_it = gauss_rb(valuesdos, boundaryconditions, eps);
				t2 = high_resolution_clock::now();
		    	duration = duration_cast<microseconds>( t2 - t1 ).count();
		    	aftercputime=cpu_time();
		    	func_cputime= aftercputime - beforecputime;
				break;

				case 4:
				// Gauß-Seidel with SOR
				beforecputime=cpu_time();
				t1 = high_resolution_clock::now();
				values_orig = valuesdos;

				for (int k = 0; k<20; k++) {
					values_sor = values_orig;
					s = 1+0.05*k;
					cout << "s = " << s << endl;
					num_it = gauss_sor(values_sor, boundaryconditions, s, eps, num_it_ideal);
					if (num_it == -1){
						continue;
					}
					else if (k == 0 or num_it < num_it_ideal) {
						num_it_ideal = num_it;
						s_ideal = s;
						valuesdos = values_sor;
					}
					
				}
				t2 = high_resolution_clock::now();
		    	duration = duration_cast<microseconds>( t2 - t1 ).count();
		    	aftercputime=cpu_time(); 
		    	func_cputime= aftercputime - beforecputime;
				break;

				default:
				cout << "Well, that was not one of the options, was it?" << endl;
			}
		}
	}*/

	// Print out numerical solution
	cout << "Resulting matrix:" << endl;
	for (int i = 0; i < imax; ++i)
	{
		for (int j = 0; j < jmax; ++j)
		{
			cout << values[i][j] << " ";
		}
		cout << endl;
	}
	
	if (choice == 4) {
		cout << "Ideal s: " << s_ideal << " at " << num_it_ideal << " iterations." << endl;
		cout << "Function duration: " << duration << "micro seconds" << endl;
	}
	else{
		cout << "Number of iterations: " << num_it << endl; // Print number of iterations
		cout << "Function duration: " << duration << " micro seconds" << endl;
	}


	// ANALYTICAL SOLVER

	// Problem 1

	// Parameters
	double inrad=6., outrad=19., outvol=9.;

	// Internal variables
	double analyt1[imax][jmax], difference[imax][jmax], rad, centrei, centrej;
	centrei= (imax-1) / 2;
	centrej= (jmax-1) / 2;

	for(i=0; i<imax; i++) {
		for (j=0; j<jmax; j++) {
			if (boundaryconditions[i][j] == 1) {
				analyt1[i][j]=values[i][j];
				continue;
			}
			else {
				rad= sqrt(pow(abs(j-centrej), 2)+pow(abs(i-centrei), 2));
				//cout << rad << endl;
				analyt1[i][j]= (outvol*log(inrad/rad))/(log(inrad/outrad));
				//cout << values[i][j] << endl;
			}

		}
	}
	for (int i = 0; i < imax; ++i)
	{
			for (int j = 0; j < jmax; ++j)
			{
				difference[i][j]= abs(analyt1[i][j] - values[i][j]);
				//cout << difference[i][j] << " ";
			}
			//cout << endl;
	}

	double analyt2[imax][jmax], difference2[imax][jmax], rad2, dist= jmax-2, voltage= abs(values[0][0]), theta, R=6;
	for(i=0; i<imax; i++) {
		for (j=0; j<jmax; j++) {
			if (boundaryconditions[i][j] == 1) {
				analyt2[i][j]=values[i][j];
				continue;
			}
			else {
				rad2= sqrt(pow(abs(j-centrej), 2)+pow(abs(i-centrei), 2));
				theta = atan(abs(j-centrej)/(abs(i-centrei)));
				if (j < centrej) {

					analyt2[i][j]= ((2*voltage*pow(R,2))/dist)*(sin(theta)/rad2)+((2*voltage)/dist)*rad2*sin(theta);
				}
				else{
					analyt2[i][j]= -(((2*voltage*pow(R,2))/dist)*(sin(theta)/rad2)+((2*voltage)/dist)*rad2*sin(theta));
				}
			}
		}
	}
	for (int i = 0; i < imax; ++i)
	{
			for (int j = 0; j < jmax; ++j)
			{
				difference2[i][j]= abs(analyt2[i][j] - values[i][j]);
				//cout << difference2[i][j] << " ";
			}
			//cout << endl;
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
					
					
					myfile << values[i-1][j-1] << " ";
				}
			
			}
	
		}
		myfile << endl; //endline 
	}
	myfile.close(); //closes the Heatmap file.
	
	 // This MUST BE < 1. This exists so that there are less arrows then there are pixels, dividing the total value of data-pixels by this amount. 
	
	int xo = x/multip , yo = y/multip ;
	
	
	ofstream out; //Creting the Vector-map
	out.open ("outputtestvector.txt"); //creating a file to output

	for (int i = 1 ; i < xo ; i++){ // then creating the x values


		for (int j = 1 ; j < yo ;  j++ ){ //creating the y values first

						
			out << -j << "\t"; // if this isn't negative, the values will come out with a pi/2 shift
			out << -i << "\t"; //as above
			
						
			//double dx = 2;
			double dx = analyt2 [i*multip + (multip-1)] [j*multip] - analyt2 [i*multip - (multip)] [j*multip]; // this is, dx = value to the left of system[][] - value to the right of system[][]. the multipliers exist to compensate for the lack of details above
			//double dy = 1;
			double dy = analyt2 [i*multip] [j*multip + (multip-1)] - analyt2 [i*multip] [j*multip - (multip)]; // same as above but with values above and below system[][]. with the multipliers
			
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

	if(choice != 4){
		double cpu_usage, totcputime;
		totcputime= cpu_time();
		cpu_usage= (func_cputime/totcputime)*100;
		cout << "CPU time until now: " << totcputime << " " << endl;
		cout << "CPU time inside Function " << func_cputime << " " << endl;
		cout << "Numerical solver assigned CPU time: " << cpu_usage << " %" << endl;
		end= high_resolution_clock::now();
		double realtime= duration_cast<microseconds>( end - start ).count();
		double cpu_u= (totcputime/realtime)*100;
		cout << "CPU usage of process: " << cpu_u << " %" << endl;
	}
	else{
		double cpu_usage, totcputime;
		totcputime= cpu_time();
		cpu_usage= (func_cputime/totcputime)*100;
		cout << "CPU time until now: " << totcputime << " " << endl;
		cout << "CPU time inside Function " << func_cputime << " " << endl;
		cout << "Numerical solver assigned CPU time: " << cpu_usage << " %" << endl;
		end= high_resolution_clock::now();
		double realtime= duration_cast<microseconds>( end - start ).count();
		double cpu_u= (totcputime/realtime)*100;
		cout << "CPU usage of process: " << cpu_u << " %" << endl;
	}
	double rss= currentRSS();
	cout << "RAM: " << rss << endl;
	return 0;
}
