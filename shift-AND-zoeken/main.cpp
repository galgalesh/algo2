#include "bitpatroon.h"
#include "shiftand.h"
using std::cout;
using std::endl;


int main(){
	uchar* naald = (unsigned char*)&"naald";
	uint _naaldlengte = 5;
	Shiftand shiftand = Shiftand(naald, _naaldlengte);
	shiftand.schrijf_letterpatroon();

	return 0;
}
