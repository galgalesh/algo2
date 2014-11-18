#ifndef __BTREE
#define __BTREE
#include "schijf.h"
#include <iostream>
using std::endl;
using std::cerr;
using std::ostream;

using std::cout;
using std::endl;

//Dit bestand bevat code en hoofdingen van
template<class T, class D, unsigned int m> class Btree;
template<class T, class D, unsigned int m> class Bknoop;
// T: key
// D: value
// m: maximum grootte knoop

template<class T,class D,unsigned int m> class Bknoop {
    friend class Btree<T,D,m>;
    typedef Bknoop<T,D,m> Knoop;

    private:
        static int crashtest;

    public:
        T sleutel[m];
        D data[m];
        blokindex index[m+1];   // verwijzingen naar kind knopen
        unsigned int k;         // aantal sleutels in knoop
        bool isblad;            // knoop = blad ?

        // indien meer dan 3 knopen in het geheugen > crash! (vereiste opgave)
        Bknoop<T,D,m>(){
            //int i = 4/(-4+crashtest++);
            //cout << "Crashtest: " << crashtest << endl;
        };

        // bij destructie crashtest verlagen
        ~Bknoop<T,D,m>(){
            //crashtest--;
            //cout << "~Crashtest: " << crashtest << endl;
        }

        // const ?
        Knoop& operator=(const Knoop& b){
            k=b.k;
            isblad=b.isblad;

            for (unsigned int i=0;i<k;i++){
                sleutel[i]=b.sleutel[i];
                data[i]=b.data[i];
            }

            if (!isblad){
                for (unsigned int i=0;i<=k;i++){
                    index[i]=b.index[i];
                }
            }

            return *this;
        }

        D* k_zoek(T& key);
        void k_voeg_toe(int plaats, T& key, D& value);

};

template<class T, class D,unsigned int m> class Btree {
    typedef Bknoop<T,D,m> Knoop;

    private:
       Schijf<Knoop>& schijf;
       Knoop* ouder;
       blokindex wortelindex;

    public:
        Btree(Schijf<Knoop>& s):schijf(s){
            // ouder == wortel
            ouder = new Knoop();
            ouder->k=0;
            ouder->isblad=true;
            wortelindex=schijf.schrijf(*ouder);
            delete ouder;
            ouder = 0;
        }

        bool zoek(D& ret, T& key);
        bool zoek_rec(D& ret, T& key, blokindex b);
        void voeg_toe(T& key, D& value);
        void voeg_toe_rec(T& key, D& value, blokindex b);

};


// Knoop functies
template<class T, class D,unsigned int m>
int Bknoop<T,D,m>::crashtest = 0;

template<class T, class D,unsigned int m>
D* Bknoop<T,D,m>::k_zoek(T& key) {
    return NULL;
};

template<class T, class D,unsigned int m>
void Bknoop<T,D,m>::k_voeg_toe(int plaats, T& key, D& value) {

    if (k >= m) {
        cout << "ERROR: Knoop zit vol, kan niet toevoegen." << endl;
        return;
    }

    if (!isblad) {
        cout << "ERROR: Knoop is geen blad, kan niet toevoegen." << endl;
        return;
    }

    for (int j = k; j > plaats; --j)
    {
        sleutel[j] = sleutel[j-1];
        data[j] = data[j-1];
    }

    sleutel[plaats] = key;
    data[plaats] = value;
    k++;
};


// Boom functies
template<class T, class D,unsigned int m>
bool Btree<T,D,m>::zoek(D& ret, T& key) {
    return zoek_rec(ret, key, wortelindex);
}

template<class T, class D,unsigned int m>
bool Btree<T,D,m>::zoek_rec(D& ret, T& key, blokindex b) {
    Knoop* knoop = new Knoop();
    schijf.lees(*knoop, b);

    // TODO: ergens oneindige recursie/lus/...
    // Bij uitvoeren krijg je segmentation fault
    
    int i = 0;
    while(i < knoop->k-1 && key > knoop->sleutel[i]) {
        //cout << "I " << key << " < " << knoop->sleutel[i] << endl;
        ++i;
    }

    if (key == knoop->sleutel[i]) {
        ret = knoop->data[i];
        delete knoop;
        return true;
    }
    else if (!knoop->isblad) {
        blokindex kind = knoop->index[i];
        delete knoop;
        return zoek_rec(ret, key, kind);
    }
    else {
        delete knoop;
        return false;
    }
};

template<class T, class D,unsigned int m>
void Btree<T,D,m>::voeg_toe(T& key, D& value) {
    voeg_toe_rec(key, value, wortelindex);
}

template<class T, class D,unsigned int m>
void Btree<T,D,m>::voeg_toe_rec(T& key, D& value, blokindex b) {
    Knoop knoop;
    schijf.lees(knoop, b);

    int i = 0;
    while(i < knoop.k-1 && key > knoop.sleutel[i]) {
        ++i;
    }

    // sleutel zit er al in
    if (key == knoop.sleutel[i]) {
        return;
    }

    // blad dat niet vol is
    if (knoop.isblad && knoop.k < m) {
        knoop.k_voeg_toe(i, key, value);
        schijf.herschrijf(knoop, b);
    }

    // blad dat vol is > splitsen
    else if (knoop.isblad) {
        // TODO: 1 knoop te veel in het geheugen
        cout << "Splitsen" << endl;
        int midden = m/2;

        // Linkse knoop creëren
        Knoop* left = new Knoop();
        left->isblad = true;
        for (int i = 0; i < midden; ++i)
            left->k_voeg_toe(i, knoop.sleutel[i], knoop.data[i]);
        
        blokindex left_index = schijf.schrijf(*left);
        delete left;

        // Rechtse knoop creëren
        Knoop* right = new Knoop();
        right->isblad = true;
        for (int i = midden+1; i < m; ++i)
            right->k_voeg_toe(i-(midden+1), knoop.sleutel[i], knoop.data[i]);
        
        blokindex right_index = schijf.schrijf(*right);
        delete right;

        // blad is wortel
        if (ouder == 0) {
            ouder = new Knoop();
            ouder->isblad = false;
        }

        // TODO: niet toevoegen op 0 maar correcte plaats
        ouder->sleutel[0] = knoop.sleutel[midden];
        ouder->data[0] = knoop.data[midden];
        ouder->index[0] = left_index;
        ouder->index[1] = right_index;

        schijf.herschrijf(*ouder, b);
        delete ouder;
    }

    // TODO: delete knoop;
};

#endif


