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
	punt2 punt(1,2);
	string s = "lol";

	boom.voegtoe(punt, s);

    cout << boom << endl;

    return 0;
}