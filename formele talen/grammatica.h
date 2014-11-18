#include "regexp.h"
#include "symbool.h"
using std::cout;
using std::endl;

class Grammatica {
	public:
		Grammatica(const Regexp& regex);
		Grammatica(const Regexp &regexp, Symbool &symbool);
		string geefS() { return this->s; }
		Symbool geefNiet_terminaal() { return this->niet_terminaal; }
		void schrijf() const;


	private:
		Symbool niet_terminaal;
		string s;
		Grammatica *gr1,*gr2;
		void init(const Regexp &regexp, Symbool &symbool);



};

Grammatica::Grammatica(const Regexp &regexp){
	Symbool s = Symbool();
	init(regexp, s);
}

Grammatica::Grammatica(const Regexp &regexp, Symbool &symbool ){
	init(regexp, symbool);
}

void Grammatica::init(const Regexp &regexp, Symbool &symbool) {
	//this->op = regex.geefOpcode();
	//this->karakter = regex.geefLetter();
	niet_terminaal = symbool;
	++symbool;

	if(regexp.geefOpcode() == Regexp::letter) {
		s = regexp.geefLetter();
		//cout << "LETTER" << endl;
	} else {
		// linkerdeel
		gr1 = new Grammatica(*(regexp.geefEersteOperand()), symbool);
		s += "<" + gr1->geefNiet_terminaal() + ">";

		if(regexp.geefOpcode() == Regexp::ster) {
			s = "a" + s + "|/";
		} else {
			gr2 = new Grammatica(*(regexp.geefTweedeOperand()), symbool);
			if(regexp.geefOpcode() == Regexp::of) {
				s = s + "|" + "<" + gr2->geefNiet_terminaal() + ">";
			} else {
				s = s + "<" + gr2->geefNiet_terminaal() + ">";
			}
		}
	}
	cout << this->niet_terminaal << ": " << this->s << endl;
}

