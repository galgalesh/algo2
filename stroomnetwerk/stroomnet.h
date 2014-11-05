#ifndef __STROOMNET_H
#define __STROOMNET_H
#include "graaf.h"
#include <cassert>
#include <iostream>
using std::vector;
using std::ostream;
using std::cout;
using std::endl;

template< class T >
class Pad:public std::vector< int >{
    public:
        T geefCapaciteit() const{
            return capaciteit;
        }
        void zetCapaciteit(const T& _capaciteit){
            capaciteit=_capaciteit;
        }
    friend ostream& operator<<(ostream& os, const Pad& p){
        os<<"Capaciteit= "<<p.capaciteit<<" :: ";
        if (p.size() > 0){
            os<<p[0];
        }
        for (int i=1; i<p.size(); i++ ){
            os<<"->"<<p[i];
        }
        os<<"\n";
    }
    protected:
        T capaciteit;
};
/**********************************************************************

   Class:Vergrotendpadzoeker
   
   beschrijving: Methodeklasse die, gegeven een stroomnetwerk,
                 en vergrotend pad teruggeeft.
                 van en naar zijn de knoopnummers van bron en doel.

   
***************************************************************************/
template <class T>
class Vergrotendpadzoeker{
    public:
        Vergrotendpadzoeker(const GraafMetTakdata<GERICHT,T>& stroomnetwerk, int van, int naar, Pad<T>& pad);
    protected:
        virtual void foo(int t, int x);
        const GraafMetTakdata<GERICHT,T>& q;
        Pad<T>& p;
        vector<int> l;
        vector<bool> m;
        int v,v2;
};


template <class T>
Vergrotendpadzoeker<T>::Vergrotendpadzoeker(const GraafMetTakdata<GERICHT ,T>& _q,int _v,int _v2,Pad<T>& _p):
                    p(_p), q(_q),v(_v),v2(_v2),
                    l(q.aantalKnopen()), m(q.aantalKnopen(),false){
    p.clear();
    assert(v != v2);
    foo(v,0);
    assert(p.size()!=1);
    if (p.size() > 1){
        T g=*q.geefTakdata(p[0],p[1]);
        for (int i=2; i<p.size(); i++ ){
            T ychg=*q.geefTakdata(p[i-1],p[i]);
            if (ychg<g)
                g=ychg;
        }
        p.zetCapaciteit(g);
    }
}

template <class T>
void Vergrotendpadzoeker<T>::foo(int t,int x){
    m[t]=true;
    const typename GraafMetTakdata<GERICHT,T>::Knoop& a=q[t];
    int ychx=x+1;
    for (typename GraafMetTakdata<GERICHT,T>::Knoop::const_iterator it=a.begin();
                it!=a.end();it++){
        int u=it->first;
        if (*q.geefTakdata(t,u)> 0){
            if (it->first==v2 && ychx+1 > p.size()){
                l[v2]=t;
                p.resize(ychx+1);
                int ychf=v2;
                int i=ychx;
                while (ychf!=v){
                    p[i--]=ychf;
                    ychf=l[ychf];
                }
                p[0]=ychf;
            }
            else if(!m[u]){
                l[u]=t;
                foo(u,ychx);
            }
        }
    }
}

/**********************************************************************

   Class: Stroomnetwerk
   
   beschrijving: Een stroomnetwerk gaat uit van een gewogen gerichte graaf
                 die in de constructor wordt opgegeven
   
***************************************************************************/

template <class T>//T = takdata
class Stroomnetwerk:public GraafMetTakdata<GERICHT, T >{
    public:
        //Copyconstructor. Let op: Graaf<GERICHT>(gr) moet toegevoegd,
        //anders roept de copyconstructor van GraafMetTakdata de defaultconstructor
        //van Graaf op en krijgen we een lege graaf.
        Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& gr):
                            Graaf<GERICHT>(gr),GraafMetTakdata<GERICHT, T>(gr){
            //cout << "CONSTRUCTOR: Stroomnetwerk_copy" << endl;
        };

        Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& gr, int _van, int _naar):
                            Graaf<GERICHT>(gr.aantalKnopen()),van(_van),naar(_naar){
            //cout << "CONSTRUCTOR: Stroomnetwerk_copy + _van _naar" << endl;
            Stroomnetwerk<T> restnetwerk(gr);

            int j = 0;
            //cout << "THIS---------------------------------" << endl;
            //cout << *this << endl;
            //int i = 2/(2-2);
            Pad<T> vergrotendpad;
            Vergrotendpadzoeker<T> vg(restnetwerk, van, naar, vergrotendpad);
            while(vergrotendpad.size() !=0 ){
                cout << "pad vergroten keer: " << j++ << endl;
                restnetwerk-=vergrotendpad;

                *this+=vergrotendpad;
                Vergrotendpadzoeker<T> vg(restnetwerk, van, naar, vergrotendpad);
            }
        }

        Stroomnetwerk<T>& operator-=(const Pad<T>&);
        Stroomnetwerk<T>& operator+=(const Pad<T>&);

        T geefCapaciteit();

    protected:
        int van,naar;
};

template <class T>//T = takdata
T Stroomnetwerk<T>::geefCapaciteit() {
    int capaciteit = 0;
    for (std::map<int, int>::iterator i = this->knopen[van].begin(); i != this->knopen[van].end(); ++i)
    {
        capaciteit += this->takdatavector[i->second];
    }
    return capaciteit;

}

template <class T>
Stroomnetwerk<T>& Stroomnetwerk<T>::operator-=(const Pad<T>& pad) {

    if(pad.size() < 2) {
        std::cout << "ERROR: pad van minder dan 2 knopen" << std::endl;
        return *this;
    }

    for (int i=0; i<(pad.size()-1); i++ ){
        T* takdata_heen = this->geefTakdata(pad[i],pad[i+1]);
        T* takdata_terug = this->geefTakdata(pad[i+1],pad[i]);
        // als de heenverbinding bestaat

        if(takdata_terug == 0) {
            this->voegVerbindingToe(pad[i+1], pad[i], pad.geefCapaciteit());
            takdata_terug = this->geefTakdata(pad[i+1],pad[i]);
        } else {
            *takdata_terug += pad.geefCapaciteit();
        }

        (*takdata_heen) -= pad.geefCapaciteit();

        if(*takdata_heen < 0) {
            cout << "DIT isM"<< endl;
            assert(false);
            // inverteer verbinding
            this->voegVerbindingToe(pad[i+1], pad[i], *takdata_heen * -1);
        }

        if(*takdata_heen <=0) {
            // verwijder oude geinverteerde- of nulverbinding
            this->verwijderVerbinding(pad[i], pad[i+1]);
        }
    }

    return *this;
}

template <class T>
Stroomnetwerk<T>& Stroomnetwerk<T>::operator+=(const Pad<T>& pad) {

    if(pad.size() < 2) {
        std::cout << "ERROR: pad van minder dan 2 knopen" << std::endl;
        return *this;
    }

    for (int i=0; i<(pad.size()-1); i++ ){
        T* takdata_heen = this->geefTakdata(pad[i],pad[i+1]);
        if(takdata_heen == 0) {
            // als heenverbinding niet bestaat
            T* takdata_terug = this->geefTakdata(pad[i+1], pad[i]);
            if(takdata_terug == 0) {
                // als terugverbinding niet bestaat
                this->voegVerbindingToe(pad[i], pad[i+1], pad.geefCapaciteit());
            } else {
                // als terugverbinding wel bestaat
                (*takdata_terug) -= pad.geefCapaciteit();

                if(*takdata_terug < 0) {
                    // inverteer verbinding
                    this->voegVerbindingToe(pad[i], pad[i+1], *takdata_terug * -1);
                }

                if(*takdata_terug <=0) {
                    // verwijder oude geinverteerde- of nulverbinding
                    this->verwijderVerbinding(pad[i+1], pad[i]);
                }
            }
        } else {
            // als de heenverbinding bestaat
            T* takdata_terug = this->geefTakdata(pad[i+1], pad[i]);
            if(takdata_terug != 0) {
                assert(false);
                //als de terugverbinding bestaat
                (*takdata_terug) -= pad.geefCapaciteit();
                if(*takdata_terug < 0) {
                    assert(false);
                }
            } else {
                (*takdata_heen) += pad.geefCapaciteit();
            }


        }
    }

    return *this;
}


#endif
