#include <iostream>
	using std::cout;
	using std::endl;
#include <fstream>
	using std::ifstream;
#include <sstream>
	using std::istringstream;
#include <vector>
	using std::vector;
#include <map>
	using std::map;
#include <string>
	using std::string;
#include "fibonacci.h"

int main(int argc, char ** argv) {
	int k;
	vector<pair<int, int> > a_waarden;	// <index, a-waarde>
	deque<int> start_waarden;
	map<int, int> n_waarden;			// <n-waarde, f(n)>
	
	ifstream input("input.txt");
	if(input.good()) {
		string line;
		istringstream ss;
		int temp;
		// k inlezen
		getline(input, line);
		ss.clear();
		ss.str(line);
		ss >> k;
		// a-waarden inlezen
		getline(input, line);
		ss.clear();
		ss.str(line);
		int i = 0;	// index van a
		while(ss >> temp) {
			// enkel nuttige coefficienten opslaan (a != 0)
			if(temp != 0) {
				a_waarden.push_back(make_pair(i, temp));
			}
			i++;
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
			n_waarden.insert(make_pair(temp, 0));
		}
	}
	
	cout << "k = " << k << endl;
	cout << "a-waarden:" << endl;
	for(int i=0; i<a_waarden.size(); i++) {
		cout << "\t[" << a_waarden.at(i).first << "]\t=> " << a_waarden.at(i).second << endl;
	}
	cout << "start-waarden:" << endl;
	for(int i=0; i<start_waarden.size(); i++) {
		cout << "\t- " << start_waarden.at(i) << endl;
	}
	cout << "n-waarden:" << endl;
	map<int, int>::iterator it = n_waarden.begin();
	while(it != n_waarden.end()) {
		cout << "\t- " << it->first << endl;
		it++;
	}
	
	Fibonacci fib(start_waarden, a_waarden);
	
	int temp;
	it = n_waarden.begin();
	while(it != n_waarden.end()) {
		fib.zoek_n_waarde(it->first, temp);
		it->second = temp;
		cout << it->first << "\t=>\t" << it->second << endl;

		it++;		
	}
		
	return 0;	
}
