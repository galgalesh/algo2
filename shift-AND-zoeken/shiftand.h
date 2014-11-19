#include "bitpatroon.h"
#include <queue>

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
	uint temp = 1;

	for (uint i = 0; i < _naaldlengte; i++) {
		uint huidigkarakter_int = *naald;

		letterpatroon[huidigkarakter_int] | temp;
		temp >> 1;
	}
}

void Shiftand::schrijf_letterpatroon() {
	
}