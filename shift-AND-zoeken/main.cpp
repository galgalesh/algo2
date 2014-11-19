#include "bitpatroon.h"
#include "shiftand.h"
using std::cout;
using std::endl;


int main(){
	uchar* naald = (unsigned char*)&"nana";
	uint _naaldlengte = 4;
	Shiftand shiftand = Shiftand(naald, _naaldlengte);
	//shiftand.schrijf_letterpatroon();

	std::queue<const uchar*> plaats;
	uchar* hooiberg = (unsigned char*)&"nananana-batman!";
	uint hooiberglengte = 16;

	shiftand.zoek(plaats,hooiberg,hooiberglengte);

	return 0;
}
