#include <cstdlib>
#include <iostream>
#include <queue>
#include <cassert>
using std::ostream;
using std::max;
using std::pair;
using std::make_pair;
using std::min;
using std::max;
using std::cout;
using std::endl;



template <class T,class D>
class Rzwboom;

template <class T,class D>
class Rzwknoop;


template <class T,class D>
class Rzwboom{
friend class Rzwknoop<T,D>;
const static int rood,zwart;
public:
    Rzwboom<T,D>(){
        k=0;
    }
    ~Rzwboom<T,D>(){
        delete k;
    }
//bottom-up toevoegen, geen duplicaten toegestaan.
//geeft false als de sleutel al in de boom zat.
//geldigeKleur=false: knoop wordt toegevoegd zonder op kleur te letten.
    bool voegtoe(const T&,const D&,bool geldigeKleur=true );
//volgende functie geeft false als de sleutel niet kan verwijderd
//worden omdat hij niet in de boom zit.
    bool verwijder(const T&);
    D* zoekdata(const T&);
    int diepte();
// << schrijft uit in level order.
    friend ostream& operator<<(ostream& os,Rzwboom<T,D>& b){
        b.schrijf(os);
        return os;
    }
//-1 is ongeldige boom; zwartediepte kan dus ook gebruikt worden
//om boom te controleren.
    int zwarte_diepte();
    pair<int,int> zwarte_diepte_rec();
//geeft de voorganger als de linkerboom niet leeg is
    Rzwboom<T,D>* ondervoorganger(Rzwknoop<T,D>*);
    bool ouderOK();
    
    bool is_kleurbaar();
    pair<pair<int,int>,int> is_kleurbaar_rec();
private:
    void schrijf(ostream&);
    int geefkleur(){
        if (k!=0)
           return k->rzw;
        else
            return zwart;
    }
//Dit hoort op het eerste gezicht bij de klasse Rzwknoop.
//Maar dit kan niet omdat kn een nulpointer mag zijn
    void schrijf(ostream& os ,Rzwknoop<T,D>* kn);
//alleen roteer oproepen als de operatie kan!
    void roteer(bool naarlinks);
//maakzwart maakt de knoop in plaats->k zwart, en laat plaats wijzen naar
//volgende boom die een probleem kan opleveren bij bottom-up toevoegen.
    void maakzwart(Rzwboom<T,D>*& plaats);
//zoekknoop: postconditie: plaats wijst naar knoop met gezochte sleutel,
//                           of naar lege boom die naar die knoop zou wijzen als hij bestond
//                         ouder heeft waarde die in het ouderveld van de knoop staat/zou moeten staan
    void zoekknoop(const T&,Rzwboom<T,D>*& plaats,Rzwboom<T,D>*& ouder);

//gegevensveld
    Rzwknoop<T,D>* k;
};
template <class T,class D>
const int Rzwboom<T,D>::rood=0;
template <class T,class D>
const int Rzwboom<T,D>::zwart=1;

template <class T,class D>
class Rzwknoop{
    friend class Rzwboom<T,D>;
    public:
        T sl;
        D data;
        int rzw;
        Rzwboom<T,D>* ouder;//wijst naar de boom die naar de ouderknoop wijst
        Rzwboom<T,D> links,rechts;
        Rzwknoop<T,D>(const T& _sl,const D& _data):
                ouder(0),sl(_sl),data(_data),rzw(Rzwboom<T,D>::rood){}
};

template <class T,class D>
void Rzwboom<T,D>::roteer(bool naarlinks){
    Rzwknoop<T,D>* op;
    Rzwknoop<T,D>* neer=k;
    if (naarlinks){
        op=k->rechts.k;
        if (neer->links.k!=0)
            neer->links.k->ouder=&(op->links);
        neer->rechts.k=op->links.k;
        if (op->links.k!=0){
            Rzwknoop<T,D>* kind=op->links.k;
            kind->ouder=&(op->links);
            if (kind->links.k!=0)
                kind->links.k->ouder=&(neer->rechts);
            if (kind->rechts.k!=0)
                kind->rechts.k->ouder=&(neer->rechts);
        }
        if (op->rechts.k!=0)
            op->rechts.k->ouder=this;
        op->links.k=neer;
    }
    else{
        op=k->links.k;
        if (neer->rechts.k!=0)
            neer->rechts.k->ouder=&(op->rechts);
        neer->links.k=op->rechts.k;
        if (op->links.k!=0)
            op->links.k->ouder=this;
        if (op->rechts.k!=0)
            op->rechts.k->ouder=&(op->rechts);
        if (op->rechts.k!=0){
            Rzwknoop<T,D>* kind=op->rechts.k;
            kind->ouder=&(op->rechts);
            if (kind->links.k!=0)
                kind->links.k->ouder=&(neer->links);
            if (kind->rechts.k!=0)
                kind->rechts.k->ouder=&(neer->links);
        }
        op->rechts.k=neer;
    }
    k=op;
    op->ouder=neer->ouder;
    neer->ouder=this;
}



template <class T,class D>
void Rzwboom<T,D>::zoekknoop(const T& sl,Rzwboom<T,D>*& plaats,
                                         Rzwboom<T,D>*& ouder){
    plaats=this;
    ouder=0;
    while (plaats->k!=0 && plaats->k->sl!=sl){
        ouder=plaats;
        if (sl<plaats->k->sl)
            plaats=&(plaats->k->links);
        else
            plaats=&(plaats->k->rechts);
    }
}
        
template <class T,class D>
D* Rzwboom<T,D>::zoekdata(const T& sl){
    Rzwboom<T,D>* plaats;
    Rzwboom<T,D>* ouder;//dummy, eigenlijk overbodig bij zoekdata
    zoekknoop(sl,plaats,ouder);
    if (plaats->k==0)
        return 0;
    else return &(plaats->k->data);
}

template <class T,class D>
void Rzwboom<T,D>::maakzwart(Rzwboom<T,D>*& plaats){
    Rzwboom<T,D>* kind=plaats;
    Rzwboom<T,D>* ouder=kind->k->ouder;
    Rzwboom<T,D>* grootouder=ouder->k->ouder;
    if (grootouder->k->links.geefkleur()==rood && grootouder->k->rechts.geefkleur()==rood){
        grootouder->k->rzw=rood;
        grootouder->k->links.k->rzw=zwart;
        grootouder->k->rechts.k->rzw=zwart;
    }
    else {//geval 1
        bool ouderlinks=(ouder==&(grootouder->k->links));
        bool kindlinks=(kind==&(ouder->k->links));
        if (kindlinks!=ouderlinks){//geval 1b
            if (ouderlinks)
                grootouder->k->links.roteer(true);
            else
                grootouder->k->rechts.roteer(false);
        };
        //geval 1a
        grootouder->k->rzw=rood;
        ouder->k->rzw=zwart;
        grootouder->roteer(!ouderlinks);
    }
    plaats=grootouder;
}

//bottom up
template <class T,class D>
bool Rzwboom<T,D>::voegtoe(const T& sl,const D& data, bool geldigeKleur){
    Rzwboom<T,D>* plaats;
    Rzwboom<T,D>* ouder;
    zoekknoop(sl,plaats,ouder);
    bool nietgevonden=plaats->k==0;
    if (nietgevonden){
        plaats->k=new Rzwknoop<T,D>(sl,data);
        plaats->k->ouder=ouder;
//noot: ouder wijst nooit naar een ledige boom.
        if (geldigeKleur){
            bool dubbelrood=(ouder!=0 && ouder->k->rzw==rood);
            while (dubbelrood){
                if (sl==18){
                    std::cout<<"plaats->k->sl== "<<plaats->k->sl;
                    std::cout<<"ouder->k->sl== "<<ouder->k->sl;
                    std::cout<<'\n';
                }
                maakzwart(plaats);
                assert(plaats->k!=0);
                ouder=plaats->k->ouder;
                dubbelrood=(plaats->k->rzw==rood && ouder !=0
                                              && ouder->k->rzw==rood);
            }
            k->rzw=zwart;//eventueel wortel zwart maken.
        }
    }
    return nietgevonden;
}


template <class T,class D>
void Rzwboom<T,D>::schrijf(ostream& os){
    if (k!=0){
       std::queue<Rzwknoop<T,D>*, std::deque<Rzwknoop<T,D>*> > q;
       q.push(k);
       while(!q.empty()){
           Rzwknoop<T,D>* nu=q.front();
           schrijf(os,nu);
           os<<" links: ";
           schrijf(os,nu->links.k);
           os<<" rechts: ";
           schrijf(os,nu->rechts.k);
           os<<" ouder: ";
           if (nu->ouder==0)
               schrijf(os,0);
           else
               schrijf(os,nu->ouder->k);
           os<<std::endl;
           if (nu->links.k!=0)
               q.push(nu->links.k);
           if (nu->rechts.k!=0)
               q.push(nu->rechts.k);
           q.pop();
       }
    }
    else{
        schrijf(os,k);
    }
}

template <class T,class D>
void Rzwboom<T,D>::schrijf(ostream& os ,Rzwknoop<T,D>* kn){
    if (kn!=0)
        os<<
        (kn->rzw==rood?
            "rood ":"zwart")
        <<"("<<kn->sl<<","<<kn->data<<")";
    else
        os<<"---";
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
        pair<pair<int,int>,int> antwoord_links = this->k->links.is_kleurbaar_rec();
        pair<pair<int,int>,int> antwoord_rechts = this->k->rechts.is_kleurbaar_rec();
        pair<int,int> interval_links = antwoord_links.first;
        pair<int,int> interval_rechts = antwoord_rechts.first;
        int rzw_links = antwoord_links.second;
        int rzw_rechts = antwoord_rechts.second;

        // als kind al heeft aangegeven dat het niet kleurbaar is
        // dan is deze knoop dat ook niet
        if (interval_links.first == -1 || interval_rechts.first == -1 ) {
            return make_pair(make_pair(-1,-1),zwart);
        }

        // intersectie beide intervallen
        pair<int,int> interval_huidig;
        interval_huidig.first = max(interval_links.first, interval_rechts.first);
        interval_huidig.second = min(interval_links.second, interval_rechts.second);

        // debug
        cout << endl << "knoop: " << this->k->sl << endl;
        cout << "antwoord linkertak:  " << interval_links.first << " " << interval_links.second << " knoop rood? " << (rzw_links==rood) << endl;
        cout << "antwoord rechtertak: " << interval_rechts.first << " " << interval_rechts.second << " knoop rood? " << (rzw_rechts==rood) << endl;
        cout << interval_huidig.first << " " << interval_huidig.second << endl;

        // als intersectie geen getallen bevat dan is het niet mogelijk om boom rood-zwart evenwichtig te maken
        // de boom is dus niet kleurbaar
        if (interval_huidig.first > interval_huidig.second) {
            return make_pair(make_pair(-1,-1),zwart); 
        }

        // BOOM IS KLEURBAAR, maak interval klaar om gereturned te worden
        // huidige knoop kan altijd zwart worden, dus max grens ++;
        interval_huidig.second++;

        // huidige knoop kan enkel rood worden als linker en rechterkind beiden zwart zijn
        // kinderen worden pas (mogelijks) rood bij hun minimum-zwarte-diepte
        // kijk of, voor het huidig minimum, de kinderen hun minimum-zwarte-diepte moeten aannemen, en kijk dan of ze voor die lengte rood zijn
        int rzw_huidig;
        if(interval_huidig.first == interval_links.first && rzw_links == rood) {
            // linkerkind moet min-zwarte-diepte aannemen en is rood voor die diepte
            // dus huidige knoop kan enkel zwart zijn
            interval_huidig.first++;
            rzw_huidig = zwart;
        } else if (interval_huidig.first == interval_rechts.first && rzw_rechts == rood){
            // rechterkind moet min-zwarte-diepte aannemen en is rood voor die diepte
            // dus huidige knoop kan enkel zwart zijn
            interval_huidig.first++;
            rzw_huidig = zwart;
        } else {
            // geen van beide kinderen is rood voor de diepte die ze moeten aannemen
            // dus huidige knoop mag rood zijn
            rzw_huidig = rood;
        }

        return make_pair(interval_huidig, rzw_huidig);
    }
}
