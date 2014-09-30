/* 
 * File:   main.cpp
 * Author: Lowie
 *
 * Created on 24 september 2014, 12:06
 */

#include <cstdlib>
#include <ios>

#include "rzwboom.h"

using namespace std;
using std::pair;
using std::cout;
using std::endl;

/*
 * 
 */
int main(int argc, char** argv) {
    
    Rzwboom<int,int> boom;
    for (int i = 0; i < 100; i++) {
        boom.voegtoe(i,i,false);
    }

    //oom.voegtoe(1,1,true);

    
    cout << boom.diepte() << endl;
    cout << boom.zwarte_diepte() << endl;
    cout << "is kleurbaar?: " << boom.is_kleurbaar() << endl;
    return 0;
}

template <class T,class D>
int Rzwboom<T,D>::diepte() {
    
    if (this->k == 0) {
        return 1;
    } else {
        int diepte_links = this->k->links.diepte()+1;
        int diepte_rechts = this->k->rechts.diepte()+1;
        return (diepte_links > diepte_rechts)? diepte_links : diepte_rechts;
    }
}

template <class T,class D>
int Rzwboom<T,D>::zwarte_diepte() {
    return this->zwarte_diepte_rec().first;
}

// geeft pair terug met <zwartediepte, is_zwart>
template <class T,class D>
pair<int,int> Rzwboom<T,D>::zwarte_diepte_rec() {
    if (this->k == 0) {
        return make_pair(1,1);
    } else {
        // get depth and color of left and right subtree
        pair<int,int> antwoord_links = this->k->links.zwarte_diepte_rec();
        pair<int,int> antwoord_rechts = this->k->rechts.zwarte_diepte_rec();
        int diepte_links = antwoord_links.first;
        int diepte_rechts = antwoord_rechts.first;
        int rzw_links = antwoord_links.second;
        int rzw_rechts = antwoord_rechts.second;

        // did lower tree say it is "bad"?
        if(diepte_links == -1 || diepte_rechts == -1) {
            return make_pair(-1,this->k->rzw);
        }
        // do depths match?
        if(diepte_links != diepte_rechts) {
            return make_pair(-1,this->k->rzw);
        }
        //TODO: check if color is correct
        if (this->k->rzw + rzw_links < 1 || this->k->rzw + rzw_rechts < 1) {
            return make_pair(-1,this->k->rzw);
        }


        int diepte = diepte_links + this->k->rzw;
        return make_pair(diepte,this->k->rzw);
    }
}

template <class T,class D>
bool Rzwboom<T,D>::is_kleurbaar() {

    return this->is_kleurbaar_rec().first.first != -1;
}

// geeft dubbele pair terug <<min_zwarte_lengte,max_zwarte_lengte>, rzw bij min_zwarte_lengte>
template <class T,class D>
pair<pair<int,int>,int> Rzwboom<T,D>::is_kleurbaar_rec() {
    if (this->k == 0) {
        //Knoop is null, dus boom is een blad. Blad is sowieso zwart dus min en max zijn 1.
        return make_pair(make_pair(1,1),zwart);
    } else {
        // get lengt of child trees
        pair<pair<int,int>,int> antwoord_links, antwoord_rechts;
        antwoord_links = this->k->links.is_kleurbaar_rec();
        antwoord_rechts = this->k->rechts.is_kleurbaar_rec();
        int min_lengte_links = antwoord_links.first.first;
        int max_lengte_links = antwoord_links.first.second;
        int min_lengte_rechts = antwoord_rechts.first.first;
        int max_lengte_rechts = antwoord_rechts.first.second;
        int rzw_links = antwoord_links.second;
        int rzw_rechts = antwoord_rechts.second;

     //   cout << "links: " << min_lengte_links << " " << max_lengte_links << "rechts: "<< min_lengte_rechts << " " << max_lengte_rechts;
//cout << endl << "ok1 " << endl;
        // check if child-trees are impossible to make black-red
        if (min_lengte_links == -1 || min_lengte_rechts == -1 ) {
            return make_pair(make_pair(-1,-1),zwart);
        }
//cout << endl << "ok2 " << endl;


        cout << endl << min_lengte_links << " " << max_lengte_links;
        cout << endl << min_lengte_rechts << " " << max_lengte_rechts << endl;


        // intersectie beide intervallen
        int huidig_grens_min = max(min_lengte_rechts, min_lengte_rechts);
        int huidig_grens_max = min(max_lengte_links, max_lengte_rechts);
        // kijk als intersectie getallen bevat
        if (huidig_grens_min > huidig_grens_max) {
            return make_pair(make_pair(-1,-1),zwart); 
        }
        cout << endl << huidig_grens_min << " " << huidig_grens_max << endl;

     //   cout << endl << "ok " << endl;

        int min_lengte_rzw;
        huidig_grens_max++;
        if (huidig_grens_min == min_lengte_links) {
            if (rzw_links == rood) {
                return make_pair(make_pair(huidig_grens_min++, huidig_grens_max),zwart);

            } else {
                return make_pair(make_pair(huidig_grens_min, huidig_grens_max),rood);
            }
        } else {
            if (rzw_rechts == rood) {
                return make_pair(make_pair(huidig_grens_min++, huidig_grens_max),zwart);

            } else {
                return make_pair(make_pair(huidig_grens_min, huidig_grens_max),rood);
            }
        }
    }
}