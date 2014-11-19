#include "bitpatroon.h"
#include <queue>
using std::cout;
using std::endl;

typedef unsigned int uint;
typedef unsigned char uchar;

class Shiftand{
	public:
	//naaldlengte moet kleiner dan patroonlengte zijn,
	//anders wordt een exceptie opgegooid.
	    Shiftand(const uchar* naald,uint _naaldlengte);
	    void zoek(std::queue<const uchar*>& plaats,
	                     const uchar* hooiberg, uint hooiberglengte);
	    void schrijf_letterpatroon();
	private:
	    Bitpatroon letterpatroon[256];  // S
	    uint naaldlengte;				// s.size()
};
    
Shiftand::Shiftand(const uchar* naald, uint _naaldlengte) {
	naaldlengte = _naaldlengte;

	uint temp = 1;
	temp = temp << _naaldlengte-1;


	for (uint i = 0; i < _naaldlengte; i++) {

		uint huidigkarakter_int = naald[i];

		letterpatroon[huidigkarakter_int] = letterpatroon[huidigkarakter_int] | temp;
		temp = temp >> 1;
		//cout << naald[i] << " :" << temp << " " << letterpatroon[huidigkarakter_int] << endl;
	}
}

void Shiftand::schrijf_letterpatroon() {
	for (uint i = 65; i < 122; i++) {
		char huidig = i;
		cout << huidig << " :" << letterpatroon[i] << endl;
	}
}

void Shiftand::zoek(std::queue<const uchar*>& plaats, const uchar* hooiberg, uint hooiberglengte){
	Bitpatroon r = Bitpatroon(256);
	uint  een_op_zijn_plaats = 1;
	een_op_zijn_plaats = een_op_zijn_plaats << naaldlengte-1;

	for(int i = 0; i < hooiberglengte; i++) {
		cout << "R: " << r << "I: " << i << "karakter: " << hooiberg[i] << endl;
		r = r.shiftrechts(1);
		r|=een_op_zijn_plaats;		//OR

		uchar huidigkarakter_char = hooiberg[i];
		uint huidigkarakter_int = huidigkarakter_char;

		r &= letterpatroon[huidigkarakter_int];

		if (r.bits%2 == 1) {
			cout << "gevonden!" << endl;
		}

	}
}