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

	for(int i = 0; i < 20; i++) {
	 	boom.voegtoe(i,i);
	}

	bool fout = false;
	for(int i = 0; i < 20; i++) {
		int antwoord = -1;
	 	boom.zoek_top_down(i, antwoord);
	 	if(antwoord != i || !boom.is_correct()) {
	 		fout = true;
	 		cout << "fout bij" << i << endl;
	 	}
	}
	cout << boom << endl;

	if(!fout) {
		cout << "algoritme werkt!" << endl;
	}


    return 0;
}