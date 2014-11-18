#ifndef __STROOMNET_H
#define __STROOMNET_H
#include "graaf.h"
#include <cassert>
#include <iostream>
using std::vector;
using std::ostream;

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


template <class T>
class Vergrotendpadzoeker{
public:
Vergrotendpadzoeker(const GraafMetTakdata<GERICHT,T>& _gr, int _van, int _naar, Pad<T>& _pad):
                    pad(_pad), gr(_gr),van(_van),naar(_naar),
                    voorganger(gr.aantalKnopen()), bezocht(gr.aantalKnopen(),false){
    pad.clear();
    assert(van != naar);
    verwerk(van,0);
    assert(pad.size()!=1);
    if (pad.size() > 1){
        T capaciteit=*gr.geefTakdata(pad[0],pad[1]);
        for (int i=2; i<pad.size(); i++ ){
            T nucapaciteit=*gr.geefTakdata(pad[i-1],pad[i]);
            if (nucapaciteit<capaciteit)
                capaciteit=nucapaciteit;
            assert(capaciteit > 0);
        }
        pad.zetCapaciteit(capaciteit);
    }
}
void verwerk(int knoopnr, int diepte){
//    std::cerr <<" knoopnr "<<knoopnr  <<" <? "<< gr.aantalKnopen()<<"\n";
    assert(knoopnr < gr.aantalKnopen());
    bezocht[knoopnr]=true;
    const typename GraafMetTakdata<GERICHT,T>::Knoop& kn=gr[knoopnr];
    int nudiepte=diepte+1;
    for (typename GraafMetTakdata<GERICHT,T>::Knoop::const_iterator it=kn.begin();
                it!=kn.end();it++){
        int kind=it->first;
        if (*gr.geefTakdata(knoopnr,kind)> 0){
            if (it->first==naar && nudiepte+1 > pad.size()){
                voorganger[naar]=knoopnr;
                pad.resize(nudiepte+1);
                int nunr=naar;
                int i=nudiepte;
                while (nunr!=van){
                    pad[i--]=nunr;
                    nunr=voorganger[nunr];
                }
                assert(i==0);
                assert(nunr==van);
                pad[0]=nunr;
            }
            else if(!bezocht[kind]){
                assert(*gr.geefTakdata(knoopnr,kind)> 0);
                voorganger[kind]=knoopnr;
                verwerk(kind,nudiepte);
            }
        }//if takdata> 0
    }//for
}

const GraafMetTakdata<GERICHT,T>& gr;
Pad<T>& pad;
vector<int> voorganger;
vector<bool> bezocht;
int van,naar;
};

template <class T>//T = takdata
class Stroomnetwerk:public GraafMetTakdata<GERICHT, T >{
public:
//Copyconstructor. Let op: Graaf<GERICHT>(gr) moet toegevoegd,
//anders roept de copyconstructor van GraafMetTakdata de defaultconstructor
//van Graaf op en krijgen we een lege graaf.
Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& gr):
                    Graaf<GERICHT>(gr),GraafMetTakdata<GERICHT, T>(gr){};


Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& gr, int _van, int _naar):
                    Graaf<GERICHT>(gr.aantalKnopen()),van(_van),naar(_naar){
    Stroomnetwerk<T> restnetwerk(gr);
    Pad<T> vergrotendpad;
    Vergrotendpadzoeker<T> vg(restnetwerk, van, naar, vergrotendpad);
    while(vergrotendpad.size() !=0 ){
//        std::cout<<"Restnetwerk\n"<<restnetwerk<<"\n";
//        std::cout<<"Vergrotend pad:\n"<<vergrotendpad<<"\n";
        restnetwerk-=vergrotendpad;
        *this+=vergrotendpad;
        Vergrotendpadzoeker<T> vg(restnetwerk, van, naar, vergrotendpad);
    }
//        std::cout<<"Restnetwerk op einde\n"<<restnetwerk<<"\n";
}
Stroomnetwerk& operator-=(const Pad<T>& pad){
    T padcapaciteit=pad.geefCapaciteit();
    for (int i=1; i<pad.size(); i++ ){
        int start=pad[i-1];//start en eind van de tak
        int eind=pad[i];
        int taknr=this->verbindingsnummer(start,eind);
        assert (taknr >= 0);
        assert(this->takdatavector[taknr]>=padcapaciteit);
        this->takdatavector[taknr]-=padcapaciteit;
//noot: eventueel tak met capaciteit 0 verwijderen.
        vergrootTak(eind,start, padcapaciteit);
    }
}
Stroomnetwerk& operator+=(const Pad<T>& pad){
    T padcapaciteit=pad.geefCapaciteit();
    for (int i=1; i<pad.size(); i++ ){
        T nucapaciteit=padcapaciteit;
        int van=pad[i-1];
        int naar=pad[i];
        int terugtak=this->verbindingsnummer(naar,van);
        if (terugtak != -1){
            if (this->takdatavector[terugtak] <= nucapaciteit){
                nucapaciteit-=this->takdatavector[terugtak];
                this->verwijderVerbinding(naar,van);
                if (nucapaciteit > 0)
                    vergrootTak(van, naar, nucapaciteit);
            }else{
                this->takdatavector[terugtak]-=nucapaciteit;
            }
        }else
            vergrootTak(van, naar, padcapaciteit);
    }
}
void vergrootTak(int start, int eind, T delta){
        int taknr=this->verbindingsnummer(start,eind);
        if (taknr==-1)
            taknr=voegVerbindingToe(start,eind,delta);
        else
            this->takdatavector[taknr]+=delta;
}
T geefCapaciteit(){
    T som=0;
    for (typename GraafMetTakdata<GERICHT,T>::Knoop::const_iterator it=this->knopen[van].begin();
                it!=this->knopen[van].end();it++)
        som+=this->takdatavector[it->second];
    return som;
}
protected:
int van,naar;
};
#endif
