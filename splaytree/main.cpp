#include <cstdlib>
#include <ios>

#include "splaytree.h"

using std::cout;
using std::endl;

/*
* maak een foute boom, kijk of hij kleurbaar is
*
*/
int main(int argc, char** argv) {
	Boom<int,int> boom;

	// for(int i = 0; i < 20; i++) {
	// 	boom.voegtoe(i,i);
	// }
	boom.voegtoe(3,3);
	boom.voegtoe(2,2);
	boom.voegtoe(1,1);


	cout << "boom na creatie" << endl;
	cout << boom << endl;
	int zoek = 1;
	int gevonden = 3;
	boom.zoek_top_down(zoek, gevonden);
	cout << "boom na zoeken van 1" << endl;
	cout << boom;

    return 0;
}