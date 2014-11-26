#include "thompsonna.h"
#include "na.h"
using std::cout;
using std::endl;


int main(){
	Regexp regexp = Regexp("a|b");
	ThompsonNA thompson = ThompsonNA(regexp);
	thompson.schrijf(cout);
	Na na(thompson);
	na.verwerkKarakter('a');
	//cout << "verbinding 3: " << thompson[3] << endl;
	cout << na;
	return 0;
}

