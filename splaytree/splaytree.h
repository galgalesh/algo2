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
class Boom;

template <class T,class D>
class Knoop;


template <class T,class D>
class Boom{
    friend class Knoop<T,D>;
    public:
        Boom<T,D>(){
            k=0;
        }
        ~Boom<T,D>(){
            delete k;
        }

        void voegtoe(T, D);
        bool zoek_top_down(T& zoeksleutel, D& gezochtedata);
        bool is_correct();
        bool is_correct_rec(T&, bool linkerkind);

        friend ostream& operator<<(ostream& os,Boom<T,D>& b){
            b.schrijf(os);
            return os;
        }
    private:
        void schrijf(ostream&);
        //Dit hoort op het eerste gezicht bij de klasse Knoop.
        //Maar dit kan niet omdat kn een nulpointer mag zijn
        void schrijf(ostream& os ,Knoop<T,D>* kn);

        void voegtoe_kleinste(Knoop<T,D>*&);
        void voegtoe_grootste(Knoop<T,D>*&);

        //gegevensveld
        Knoop<T,D>* k;
};

template <class T,class D>
class Knoop{
    friend class Boom<T,D>;
    public:
        T sl;
        D data;
        Boom<T,D> links,rechts;
        Knoop<T,D>(const T& _sl,const D& _data):
                sl(_sl),data(_data){}
        void voegtoe(T, D);
};


template<class T, class D>
void Boom<T,D>::voegtoe(T sl, D data) {
    if(this->k == 0) {
        k = new Knoop<T,D>(sl, data);
    } else {
        this->k->voegtoe(sl, data);
    }
}

template<class T, class D>
void Knoop<T,D>::voegtoe(T _sl, D _data) {
    if(sl > _sl) {
        this->links.voegtoe(_sl, _data);
    } else {
        this->rechts.voegtoe(_sl, _data);
    }
}

template<class T, class D>
void Boom<T,D>::voegtoe_kleinste(Knoop<T,D>*& _k){
    Knoop<T,D>** k_loper = &this->k;
    while (*k_loper != 0) {
        k_loper = &((*k_loper)->links.k);
    }
    *k_loper = _k;
}

template<class T, class D>
void Boom<T,D>::voegtoe_grootste(Knoop<T,D>*& _k){
    Knoop<T,D>** k_loper = &this->k;
    while (*k_loper != 0) {
        k_loper = &((*k_loper)->rechts.k);
    }
    *k_loper = _k;
}

template<class T, class D>
bool Boom<T,D>::zoek_top_down(T& zoeksleutel, D& gezochtedata) {
    Boom<T,D> L;
    Boom<T,D> R;
    Knoop<T,D> *temp;

    // kunnen we nog verder?
    if(this->k == 0) {
        return false;
    }

    while (true) {
        // hebben we de sleutel gevonden?
        if(this->k->sl == zoeksleutel) {
            gezochtedata = this->k->data;
            // linkerkant en rechterkant terug toevoegen
            this->voegtoe_kleinste(L.k);
            this->voegtoe_grootste(R.k);
            // pointers op 0 zetten, anders worden objecten ook verwijderd...
            L.k = 0;
            R.k = 0;

            return true;
        }

        // welke kant moeten we afdalen?
        if(this->k->sl > zoeksleutel) {
            // links afdalen
            if(this->k->links.k == 0) { return false; }
            if ( this->k->links.k->sl == zoeksleutel
                || (this->k->links.k->sl > zoeksleutel && this->k->links.k->links.k == 0)
                || (this->k->links.k->sl < zoeksleutel && this->k->links.k->rechts.k == 0) ) {
                // linkerkind is eindknoop
                temp = this->k->links.k;
                this->k->links.k = 0;
                R.voegtoe_kleinste(this->k);
                this->k = temp;
            } else if(this->k->links.k->sl > zoeksleutel) {
                // haal linkervleugel er af
                temp = this->k->links.k->links.k;
                this->k->links.k->links.k = 0;
                // zet volledige boom behalve linkervleugel op R
                R.voegtoe_kleinste(this->k);
                // zet linkervleugel als root
                this->k = temp;
            } else if(this->k->links.k->sl < zoeksleutel) {
                // haal vleugel die we willen hebben er af
                temp = this->k->links.k->rechts.k;
                this->k->links.k->rechts.k = 0;
                // zet linkervleugel op L
                L.voegtoe_grootste(this->k->links.k);
                this->k->links.k = 0;
                // zet huidige boom op R 
                R.voegtoe_kleinste(this->k);
                // zet vleugel die we willen hebben als root
                this->k = temp;
            }
        } else {
            if(this->k->rechts.k == 0) { return false; }
            if ( this->k->rechts.k->sl == zoeksleutel
                || (this->k->rechts.k->sl > zoeksleutel && this->k->rechts.k->links.k == 0)
                || (this->k->rechts.k->sl < zoeksleutel && this->k->rechts.k->rechts.k == 0) ) {
                // rechterkind is eindknoop
                temp = this->k->rechts.k;
                this->k->rechts.k = 0;
                L.voegtoe_grootste(this->k);
                this->k = temp;
            } else if(this->k->rechts.k->sl < zoeksleutel) {
                // haal linkerondervleugel er af
                temp = this->k->rechts.k->rechts.k;
                this->k->rechts.k->rechts.k = 0;
                // zet volledige boom behalve linkerondervleugel op R
                L.voegtoe_grootste(this->k);
                // zet rechtervleugel als root
                this->k = temp;
            } else if(this->k->rechts.k->sl > zoeksleutel) {
                // haal vleugel die we willen hebben er af
                temp = this->k->rechts.k->links.k;
                this->k->rechts.k->links.k = 0;
                // zet rechtervleugel op R
                R.voegtoe_kleinste(this->k->rechts.k);
                this->k->rechts.k = 0;
                // zet huidige boom op L 
                L.voegtoe_grootste(this->k);
                // zet vleugel die we willen hebben als root
                this->k = temp;
            }
        }
    }

    return false;
}

template <class T,class D>
bool Boom<T,D>::is_correct(){
    if(this->k == 0) {
        return true;
    } else {
        return (this->k->links.is_correct_rec(this->k->sl, true) && this->k->rechts.is_correct_rec(this->k->sl, false));
    }
}

template <class T,class D>
bool Boom<T,D>::is_correct_rec(T& oudersleutel, bool linkerkind){
    if(this->k == 0) {
        return true;
    } else {
         if(linkerkind && oudersleutel < this->k->sl || !linkerkind && oudersleutel > this->k->sl ) {
            return false;
         }
        return (this->k->links.is_correct_rec(this->k->sl, true) && this->k->rechts.is_correct_rec(this->k->sl, false));
    }
}

















template <class T,class D>
void Boom<T,D>::schrijf(ostream& os){
    if (k!=0){
       std::queue<Knoop<T,D>*, std::deque<Knoop<T,D>*> > q;
       q.push(k);
       while(!q.empty()){
           Knoop<T,D>* nu=q.front();
           schrijf(os,nu);
           os<<" links: ";
           schrijf(os,nu->links.k);
           os<<" rechts: ";
           schrijf(os,nu->rechts.k);
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
void Boom<T,D>::schrijf(ostream& os ,Knoop<T,D>* kn){
    if (kn!=0)
        os <<"("<<kn->sl<<","<<kn->data<<")";
    else
        os<<"---";
}


