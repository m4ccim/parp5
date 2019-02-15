#include "pch.h"
#include <fstream>
#include <omp.h>
#include <vector>
#include <string>

#include <iostream>
#include <sstream>

using namespace std;

//void read_compare_writeOPENMP(string fileOutputName) {
//
//	ifstream t("D:\\Desktop\\File 1.txt");
//	stringstream buffer;
//	buffer << t.rdbuf();
//	t.close();
//
//	string line;
//	vector<string> strings;
//	getline(buffer, line);
//	int fileOneSize = stoi(line);
//
//	for (int i = 0; i < fileOneSize; i++) {
//		getline(buffer, line);
//		strings.push_back(line);
//	}
//	buffer.clear();
//
//
//	t.open("D:\\Desktop\\File 2.txt");
//	buffer << t.rdbuf();
//	t.close();
//
//	ofstream myfile("D:\\Desktop\\" + fileOutputName);
//
//	while (getline(buffer, line)) {
//		int pos = find(strings.begin(), strings.end(), line) - strings.begin();
//		if (pos < strings.size()) {
//			myfile << line;
//			myfile << " is found at index " << pos << endl;
//		}
//		else {
//			myfile << line;
//			myfile << " is NOT FOUND" << endl;
//		}
//	}
//	myfile.close();
//}

//void read_compare_write(string fileOutputName) {
//	ifstream file("D:\\Desktop\\File 1.txt");
//	string line;
//	vector<string> strings;
//	getline(file, line);
//	int fileOneSize = stoi(line);
//
//	for (int i = 0; i < fileOneSize; i++) {
//		getline(file, line);
//		strings.push_back(line);
//	}
//	file.close();
//
//	file.open("D:\\Desktop\\File 2.txt");
//	ofstream myfile("D:\\Desktop\\"+ fileOutputName);
//
//	while (getline(file, line)) {
//		int pos  = find(strings.begin(), strings.end(), line) - strings.begin();
//		if (pos < strings.size()) {
//			myfile << line;
//			myfile << " is found at index " << pos << endl;
//		}
//		else {
//			myfile << line;
//			myfile << " is NOT FOUND" << endl;
//		}
//	}
//	myfile.close();
//}

//void read_compare_writeOPENMP(string fileOutputName) {
//	string line;
//
//	vector<string> strings1;
//	ifstream file("D:\\Desktop\\File 1.txt");
//	getline(file, line);
//	int fileOneSize = stoi(line);
//#pragma omp parallel for
//	for (int i = 0; i < fileOneSize; i++) {
//		getline(file, line);
//		strings1.push_back(line);
//	}
//	file.close();
//
//	file.open("D:\\Desktop\\File 2.txt");
//	vector<string> strings2;
//	while (getline(file, line)) {
//		strings2.push_back(line);
//	}
//	file.close();
//
//	ofstream myfile("D:\\Desktop\\" + fileOutputName);
//	
//	size_t size = strings2.size();
//#pragma omp parallel for
//	for (int i = 0; i < size; i++) {
//		int pos = find(strings1.begin(), strings1.end(), strings2[i]) - strings1.begin();
//		if (pos < strings1.size()) {
//#pragma omp critical (WriteToFile)
//			{
//				myfile << strings2[i];
//				myfile << " is found at index " << pos << endl;
//			}
//		}
//		else {
//#pragma omp critical (WriteToFile)
//			{
//				myfile << strings2[i];
//				myfile << " is NOT FOUND" << endl;
//			}
//		}
//	}
//	myfile.close();
//}
vector<string> openFile1() {
	string line;
	vector<string> strings;
	ifstream file("D:\\Desktop\\File 1.txt");
	getline(file, line);
	int fileOneSize = stoi(line);
	for (int i = 0; i < fileOneSize; i++) {

		getline(file, line);
		strings.push_back(line);
	}
	file.close();
	return strings;
}

vector<string> openFile2() {
	string line;
	vector<string> strings;
	ifstream file("D:\\Desktop\\File 2.txt");
	while (getline(file, line)) {
		strings.push_back(line);
	}
	file.close();
	return strings;
}

void writeToFile(string filename, vector<string> strings) {
	ofstream myfile("D:\\Desktop\\" + filename);
	size_t newstr_size = strings.size();

	for (int i = 0; i < newstr_size; i++) {
		myfile << strings[i] + "\n";
	}

	myfile.close();
}

void read_compare_write_fix(string fileOutputName) {
	vector<string> strings1 = openFile1();
	vector<string> strings2 = openFile2();

	size_t size = strings2.size();
	vector<string> newstr;

	for (int i = 0; i < size; i++)
	{
		int pos = find(strings1.begin(), strings1.end(), strings2[i]) - strings1.begin();
		if (pos < strings1.size()) {
			//buffer << strings2[i] + " is found at index " + to_string(pos) + "\n";
			newstr.push_back(strings2[i] + " is found at index " + to_string(pos));
		}
		else
		{
			//buffer << strings2[i] + " is NOT FOUND" + "\n";
			newstr.push_back(strings2[i] + " is NOT FOUND");
		}
	}
	writeToFile(fileOutputName, newstr);
}


void read_compare_writeOPENMP_fix(string fileOutputName) {
	vector<string> strings1;
	vector<string> strings2;
#pragma omp parallel sections num_threads (2)
	{
#pragma omp section
		{
		strings1 = openFile1();
		}
#pragma omp section
		{
		strings2 = openFile2();
		}
	}

	size_t size = strings2.size();
	vector<string> newstr = vector<string>(size);
#pragma omp parallel for
	for (int i = 0; i < size; i++) 
	{
		int pos = find(strings1.begin(), strings1.end(), strings2[i]) - strings1.begin();
			if (pos < strings1.size()) {	
				newstr[i] = strings2[i] + " is found at index " + to_string(pos);
			}
			else
			{
				newstr[i] = strings2[i] + " is NOT FOUND";
			}

	}
	writeToFile(fileOutputName, newstr);
}



int main()
{
	cout << omp_get_num_threads() <<endl;
	cout << omp_get_num_procs() << endl;
	cout << omp_get_dynamic() << endl;
	double  dif = 0.0;
	double start = omp_get_wtime();
	double end = omp_get_wtime();
	cout << "coherent execution\n";
	for (int i = 0; i < 5; i++) {
		start = omp_get_wtime(); 

		read_compare_write_fix("l.txt");

		end = omp_get_wtime();
		dif = end - start;
		printf(" Spent time: %lf seconds\n", dif);
	}
	cout << "parallel execution\n";
	for (int i = 0; i < 10; i++) {
		start = omp_get_wtime();

		read_compare_writeOPENMP_fix("2.txt");

		end = omp_get_wtime();
		dif = end - start; 
		printf(" Spent time: %lf seconds\n", dif);
	}


}