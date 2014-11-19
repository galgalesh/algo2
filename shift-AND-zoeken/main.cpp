#include "bitpatroon.h"
#include "shiftand.h"
using std::cout;
using std::endl;


int main(){
	uchar* naald = (unsigned char*)&"naald";
	uint _naaldlengte = 5;
	Shiftand shiftand = Shiftand(naald, _naaldlengte);
	//shiftand.schrijf_letterpatroon();

	std::queue<const uchar*> plaats;
	uchar* hooiberg = (unsigned char*)&"naaldddd";
	uint hooiberglengte = 8;

	shiftand.zoek(plaats,hooiberg,hooiberglengte);

	return 0;
}
