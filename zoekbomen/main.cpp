#include <cstdlib>
#include <ios>

#include "zoekboom.h"

using namespace std;
using std::pair;
using std::cout;
using std::endl;

/*
 * 
 */
int main(int argc, char** argv) {
	Zoekboom<int,int> boom;

	//boom.voegtoe(1,2);
	//boom.voegtoe(2,3);

    for(int i = 0; i < 100; i++) {
    	boom.voegtoe(i,i);
    }

    cout << boom << endl;
    cout << boom.is_tree_correct() << endl;

    return 0;
}