#include <cstdlib>
#include <ios>

#include "zoekboom.h"
#include "punt2.h"
#include <string>

using namespace std;
using std::pair;
using std::cout;
using std::endl;

/*
 * 
 */
int main(int argc, char** argv) {
	Zoekboom boom;
	punt2 punt(3,2);
	punt2 punt1(1,2);
	punt2 punt5(4,2);
	punt2 punt3(2,3);
	punt2 punt4(1,4);

	string s = "";

	boom.voegtoe(punt, s);
	boom.voegtoe(punt1, s);
	boom.voegtoe(punt5, s);
	boom.voegtoe(punt3, s);
	boom.voegtoe(punt4, s);

    cout << boom << endl;

    return 0;
}