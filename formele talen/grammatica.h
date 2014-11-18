#include "regexp.h"
#include "symbool.h"
using std::cout;
using std::endl;

class Grammatica {
	public:
		Grammatica(const Regexp& regex);
		Grammatica(const Regexp &regexp, Symbool &symbool);
		string geefLetter() { return this->letter; }
		Symbool geefNiet_terminaal() { return this->niet_terminaal; }
		Regexp::opcode geefOpcode() { return this->code; }
		void schrijf() const;
		void schrijfHTML() const;

	private:
		Symbool niet_terminaal;
		string letter;
		Grammatica *gr1,*gr2;
		void init(const Regexp &regexp, Symbool &symbool);
		Regexp::opcode code;
		void schrijfHTML_rec() const;
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
		letter = regexp.geefLetter();
	} else {
		gr1 = new Grammatica(*(regexp.geefEersteOperand()), symbool);

		if(!regexp.geefOpcode() == Regexp::ster) {
			gr2 = new Grammatica(*(regexp.geefTweedeOperand()), symbool);
		}
	}
}

void Grammatica::schrijfHTML() const {
	cout << "<!DOCTYPE html>" << endl;
	cout << "<html><head></head><body>" << endl;
	cout << "<table><tbody>" << endl;
	this->schrijfHTML_rec();
	cout << "</tbody></table>" << endl;
	cout << "</body></html>" << endl;
}

void Grammatica::schrijfHTML_rec() const {
	cout << "	<tr>" << endl;
	cout << "		<td>&lt;<strong>" + this->niet_terminaal + "</strong>&gt;</td>" << endl;
	cout << "		<td>&nbsp;::=&nbsp;</td>" << endl;
	if(this->code == Regexp::letter) {
		cout << "		<td>" + letter;
	} else {
		cout << "		<td>&lt;<strong>" + gr1->niet_terminaal +"</strong>&gt;";

		if(!this->code == Regexp::ster) {
			cout <<  "&lt;<strong>Y</strong>&gt;";
		}

	}
	cout << "</td>" << endl;
	cout << "	</tr>" << endl;
	if(this->code != Regexp::letter) {
		this->gr1->schrijfHTML_rec();

		if(!this->code == Regexp::ster) {
			this->gr2->schrijfHTML_rec();
		}
	}

}

