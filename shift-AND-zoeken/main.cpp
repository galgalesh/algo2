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

	cout << naald << " is " << plaats.size() << " keer gevonden in " << hooiberg << ":" << endl;
	while (!plaats.empty())
    {
    	cout << " - ";
    	for(int i = 0; i < _naaldlengte; i++) {
    		cout << plaats.front()[i];
    	}
    	plaats.pop();
    	cout << endl;
    }

	return 0;
}
