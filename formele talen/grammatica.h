#include "regexp.h"
#include "symbool.h"
using std::cout;
using std::endl;

class Grammatica {
	public:
		Grammatica(const Regexp& regex);
		void schrijf() const;


	private:
		Symbool sym;
		Regexp
		Grammatica *gr1,*gr2;


};

Grammatica::Grammatica(const Regexp& regex) {
	this->op = regex.geefOpcode();
	this->karakter = regex.geefLetter();

	if(regex.geefOpcode() == ) {

	}
}
