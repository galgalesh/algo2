#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char ** argv) {
	int k, temp;
	vector<int> a_waarden;
	vector<int> start_waarden;
	vector<int> n_waarden;
	
	ifstream input("input.txt");
	if(input.good()) {
		string line;
		istringstream ss;
		// k inlezen
		getline(input, line);
		ss.clear();
		ss.str(line);
		ss >> k;
		// a-waarden inlezen
		getline(input, line);
		ss.clear();
		ss.str(line);
		while(ss >> temp) {
			a_waarden.push_back(temp);
		}
		// start-waarden inlezen
		getline(input, line);
		ss.clear();
		ss.str(line);
		while(ss >> temp) {
			start_waarden.push_back(temp);
		}
		// n-waarden inlezen
		while(input >> temp) {
			n_waarden.push_back(temp);
		}
	}
	
	cout << "k = " << k << endl;
	cout << "a-waarden:" << endl;
	for(int i=0; i<a_waarden.size(); i++) {
		cout << "\t- " << a_waarden.at(i) << endl;
	}
	cout << "start-waarden:" << endl;
	for(int i=0; i<start_waarden.size(); i++) {
		cout << "\t- " << start_waarden.at(i) << endl;
	}
	cout << "n-waarden:" << endl;
	for(int i=0; i<n_waarden.size(); i++) {
		cout << "\t- " << n_waarden.at(i) << endl;
	}
	
	
	return 0;	
}
