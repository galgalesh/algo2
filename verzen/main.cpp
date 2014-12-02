#include "unionfind.h"
#include "vers.h"
#include <iostream>
using std::cout;
using std::endl;



int main () {
	// Unionfind uf = Unionfind(5);
	// cout << "find 1: " << uf.find(1) << endl;
	// uf.uni(4,0);
	// cout << "find na uni 4,0: " << uf.find(4) << " " << uf.find(0) << endl;
	// uf.uni(2,0);
	// cout << "find na uni 2,0: " << uf.find(2) << " " << uf.find(0) << endl;

	Vers v = Vers("AaaaAaaaAaaaA 6 ;: bbbb");
	v.schrijf();

	return 0;
}