#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char ** argv) {
	ifstream input("input.txt");
	int k;
	input >> k;
	cout << k << endl;
	
	return 0;	
}
