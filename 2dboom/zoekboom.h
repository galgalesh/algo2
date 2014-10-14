#ifndef __ZOEKBOOM_H
#define __ZOEKBOOM_H
#include <cstdlib>
#include <string>
#include "punt2.h"
#include <iostream>
   using std::cout;
   using std::endl;
#include <queue>
    using std::ostream;
#include <utility>
    using std::pair;
    using std::make_pair;
    using std::string;

/**********************************************************************

   Klasse: Binboom met Binknoop: binaire boom
   
   beschrijving: Domme klasse om de gegevens van een boom te bevatten
                 Omdat ze enkel in een wrapper wordt gebruikt, heeft ze
                 alles publiek staan. Ze mag dan ook nooit buiten een
                 wrapper gebruikt worden!
   
***************************************************************************/

class Binknoop;

class Binboom{
    public:
    	Binknoop* k;

        Binboom():k(0){}
        ~Binboom();

        void schrijf(ostream&) const;
        void schrijf(ostream& os,Binknoop* kn) const;
        //pointer naar wortelknoop

        //Binknoop* voorloper(Binknoop*);

    	void voegtoe(const punt2& punt,const string& data);

};

class Binknoop{
    public:
        punt2 sl;
        string data;
        bool sorteer_x;
        Binboom links,rechts;
        Binknoop(const punt2& _sl,const string& _data):sl(_sl),data(_data){}
};

/**********************************************************************

   klasse: Zoekboom
   
   beschrijving: Dit is een wrapper voor een Binboom,
                 die ervoor zorgt dat de sleutels in de Binboom
                 op volgorde staan
   
***************************************************************************/

class Zoekboom{
    public:
        void voegtoe(const punt2&,const string&);
        //geef pointer naar data horend bij een sleutel
        bool zoek(const punt2&);
        friend ostream& operator<<(ostream& os,Zoekboom& b){
            b.deBinboom.schrijf(os);
            return os;
        };

    protected:
        //De binaire boom die de data bevat
        Binboom deBinboom;
};


void Binboom::schrijf(ostream& os) const {
    if (k!=0)
        os<<"("<<k->sl<<","<<k->data<<")";
    else
        os<<"(,)";
}

Binboom::~Binboom(){
            delete k;
}






void Zoekboom::voegtoe(const punt2& punt,const string& data) {
	this->deBinboom.voegtoe(punt, data);
}

void Binboom::voegtoe(const punt2& punt,const string& data) {
	if(this->k == 0) {
		this->k = new Binknoop(punt, data);
	}
}
#endif
