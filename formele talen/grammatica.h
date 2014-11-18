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
		Regexp::opcode geefOpcode() { return this->code; }
		void schrijf() const;

	private:
		Symbool niet_terminaal;
		string s;
		Grammatica *gr1,*gr2;
		void init(const Regexp &regexp, Symbool &symbool);
		Regexp::opcode code;
};

Grammatica::Grammatica(const Regexp &regexp){
	Symbool s = Symbool();
	init(regexp, s);
}

Grammatica::Grammatica(const Regexp &regexp, Symbool &symbool ){
	init(regexp, symbool);
}

void Grammatica::init(const Regexp &regexp, Symbool &symbool) {
	niet_terminaal = symbool;
	code = regexp.geefOpcode();
	++symbool;

	if(regexp.geefOpcode() == Regexp::letter) {
		s = regexp.geefLetter();
	} else {
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
}

void Grammatica::schrijf() const {
	cout << this->niet_terminaal << ": " << this->s << endl;

	if(this->code != Regexp::letter) {
		gr1->schrijf();

		if(!this->code == Regexp::ster) {
			gr2->schrijf();
		}
	}
}

