#ifndef GRAAF_H_INCLUDED
#define GRAAF_H_INCLUDED

#include <cassert>
//Deze header bevat de code voor de klassen:
//
//
//    Graaf<RichtType>: basisklasse met grafen zonder tak- of knoopdata
//    GraafMetTakdata<RichtType,TakDatatype>
//    GraafMetKnoopdata<RichtType,KnoopDatatype>
//    GraafMetKnoopEnTakdata<RichtType,KnoopDatatype,Takdatatype>
//
//
//Evenals de bijbehorende klasse van GraafExcepties.
/**********************************************************************

   Class: Graaf

   beschrijving: Graaf is de basisklasse voor een hiërarchie van ijlegraafklassen.
                  Deze hiërarchie zal klassen bevatten voor grafen met data behorend bij
                  knopen en/of takken. Deze data worden opgeslagen in vectoren,
                  zodat kan worden opgezocht op index.
        Ad knopen:      Deze zijn opeenvolgend genummerd. Knopen kunnen niet individueel verwijderd worden.
                        Wel kunnen alle knopen samen verwijderd worden (door wis()), en kunnen knopen
                        één voor één toegevoegd worden.
        Ad verbindingen: Deze kunnen naar hartenlust toegevoegd en verwijderd worden. Als een
                        verbinding wordt verwijderd, wordt het nummer op de stack van
                        vrijgekomenVerbindingsnummers gezet. Worden verbindingen toegevoegd,
                        dan worden eerst deze vrijgekomen nummers opnieuw gebruikt, alvorens nieuwe nummers
                        worden toegevoegd. hoogsteVerbindingsnummer kan nooit verkleinen, en de
                        vector met takdata zal altijd hoogsteVerbindingsnummer elementen bevatten.


***************************************************************************/
/**********************************************************************

   Class: GraafMetTakdata<RichtType, Takdata>

   beschrijving: Deelklasse van vorigen, met data bij elke tak.

***************************************************************************/

#ifndef __GRAAF_H
#define __GRAAF_H


#include <stdexcept>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <stack>
#include <exception>
#include <queue>
#include <stack>

using std::cout;
using std::endl;
using std::pair;
using std::vector;
using std::iterator;
using std::map;
using std::queue;
using std::stack;

enum RichtType { GERICHT, ONGERICHT };
enum Kleur { WIT, GRIJS, ZWART };

class GraafExceptie : public std::logic_error {
    public:
        GraafExceptie(const std::string &boodschap_) : std::logic_error(boodschap_) {}
};

std::ostream &operator<<(std::ostream &os, const GraafExceptie& exc){
    return os << exc.what();
}


template<RichtType RT>
class Graaf{
    public:
        typedef std::map<int, int>  Knoop;      // beeldt knoopnummer (van buur) af op verbindingsnummer

        // Construeert een graaf met gegeven RichtType en aantal knopen (default 0), zonder verbindingen.
        Graaf(int n=0);
        // Geeft true indien het richttype GERICHT is, false indien het ONGERICHT is.
        bool isGericht() const;

        // Voegt een nieuwe 'lege' knoop toe, d.w.z. zonder verbindingen.
        // Geeft knoopnummer van toegevoegde knoop terug (begint bij 0).
        virtual int voegKnoopToe();

        // Voegt verbinding toe tussen knoopnummers 'van' en 'naar'.
        // Gooit GraafExceptie indien verbinding al bestaat of knoopnummers ongeldig zijn.
        // Geeft verbindingsnummer van toegevoegde verbinding terug (begint bij 0).
        // Bij ongerichte graaf wordt terugverbinding ook toegevoegd (met zelfde verbindingnummer!)
        virtual int voegVerbindingToe(int van, int naar);

        // Verwijdert de verbinding tussen knoopnummers 'van' en 'naar', incl. de terugverbinding indien ongericht.
        // Gooit GraafExceptie indien knoopnummers ongeldig zijn.
        // Geen exceptie indien de verbinding niet bestond.
        // Andere verbindingen worden niet 'hernummerd'. Er komen dus mogelijks "gaten" in de verbindingsnummers.
        virtual void verwijderVerbinding(int van, int naar);

        // Geeft het aantal knopen van de graaf.
        int aantalKnopen() const;

        // Geeft het aantal verbindingen van de graaf.
        // Bij ongerichte graaf worden verbindingen NIET dubbel geteld!
        int aantalVerbindingen() const;

        // Geeft het verbindingsnummer van de verbinding tussen 'van' en 'naar'.
        // Geeft -1 indien die verbinding niet bestaat.
        // Gooit een GraafExceptie indien knoopnummers ongeldig zijn.
        // Opgelet: performantie is O(log(v)) waarbij v aantal verbindingen vanuit 'van' is.
        int verbindingsnummer(int van, int naar) const;

        // Verwijdert alle knopen en verbindingen en herstart de verbindingsnummer
        virtual void wis();

        // Toegang tot de knopen:
        const Knoop &operator[](int i) const { return knopen[i]; }
              Knoop &operator[](int i)       { return knopen[i]; }  // deze kan als lvalue gebruikt worden


        // Schrijft de gegevens van de volledige graaf naar outputstream os.
        virtual void schrijf(std::ostream &os) const;

        // Schrijft de gegevens van de knoop met knoopnummer k naar outputstream os.
        virtual void schrijfKnoop(std::ostream &os, int k) const;

        // Schrijft de gegevens van de verbinding met verbindingsnummer v naar outputstream os.
        virtual void schrijfVerbinding(std::ostream &os, int v) const;


        // Stap 1: omgekeerde graaf opstellen
        // richting van elke verbinding omkeren
        Graaf<RT> keerOm();

        // Stap 2: Diepte-eerst zoeken
        // map: knoopnummer -> postordernummer
        map<int, int> diepte_eerst_zoeken();

        // Stap 3:
        //
        void componenten_maken(map<int,int> knooppostordermap);

        // Componentgraaf maken van graaf
        void wordt_componentengraaf_van(const Graaf&);

        // Kijkt of het mogelijk is om van a naar b te gaan en omgekeerd.
        // Maw; kijkt of knoop in zelfde sterk samenhangende component zit
        bool is_bereikbaar_kring(int a, int b);

        bool isBereikbaar(int, int);

        // zoekt een weg van a naar b;
        // geeft vector terug met de gevolgde weg van de kring
        // Werkt enkel op knopen die in zelfde component zitten
        // gaat er van uit dat de weg mogelijk is
        vector<int> zoek_pad(int a, int b);

        // zoekt een weg van a naar b en terug;
        // (werkt dus enkel op knopen die in zelfde component zitten)
        // geeft vector terug met de gevolgde weg van de kring
        // gaat er van uit dat de weg mogelijk is
        vector<int> zoek_kring(int a, int b);

    protected:
        // hulpfuncties
        void controleerKnoopnummer(int k) const;   // throw indien k ongeldig
        void voegVerbindingToeInDatastructuur(int van, int naar,int verbindingsnummer);
        void verwijderVerbindingUitDatastructuur(int van, int naar);
    protected:
        //datavelden
        std::vector<Knoop>  knopen;
        int                 hoogsteVerbindingsnummer;
        RichtType           richttype;
        std::stack<int>     vrijgekomenVerbindingsnummers;
    public:    std::vector<int>    componenten;
};

template<RichtType RT>
std::ostream &operator<<(std::ostream& os, const Graaf<RT>& g);


// --- implementatie ---

template<RichtType RT>
bool Graaf<RT>::isBereikbaar(int knoop_a, int knoop_b)
{
    bool isBereikbaar = false;

    Knoop a = this->knopen[knoop_a];
    map<int,int>::iterator it = a.begin();
    while ( it != a.end() )    // buren overlopen
    {
        if ( it->first == knoop_b )
            isBereikbaar = true;
        it++;
    }

    return isBereikbaar;
}

template<RichtType RT>
void Graaf<RT>::wordt_componentengraaf_van(const Graaf& a)
{
    int hoogste_componentnr = 0;
    // aantal componenten
    for(int i=0;i<a.componenten.size(); i++)
    {
        if ( a.componenten[i] > hoogste_componentnr )
            hoogste_componentnr = a.componenten[i];
    }

    this->knopen.resize(hoogste_componentnr+1);
    this->hoogsteVerbindingsnummer = a.hoogsteVerbindingsnummer;

    for(int i=0;i<a.aantalKnopen();i++)
    {
        Knoop k = a.knopen[i];
        map<int,int>::iterator it = k.begin();
        while(it != k.end())
        {
            int eigencomponentnr = a.componenten[i];
            int buurcomponentnr = a.componenten[it->first];
            int verbindingsnr = it->second;

            if(  eigencomponentnr != buurcomponentnr )
            {
                this->knopen[eigencomponentnr][buurcomponentnr] = verbindingsnr; // stl map insert pair
            }
            it++;
        }
    }
}

template<RichtType RT>
void Graaf<RT>::componenten_maken(map<int,int> knoop_postordermap)
{
    this->componenten.resize(this->aantalKnopen());

    vector<Kleur> kleuren(this->aantalKnopen(),WIT);

    int componentTeller = 0;

    stack<int> s;

    while ( knoop_postordermap.size() > 0 )
    {
        // hoogste postorder bepalen
        // eerste keer is dit = aantal knopen
        int max_knoop = 0;
        int max_postorder = 0;
        map<int,int>::iterator it = knoop_postordermap.begin();
        while ( it != knoop_postordermap.end() )
        {
            if ( it->second > max_postorder )
            {
                max_knoop = it->first;
                max_postorder = it->second;
            }
            it++;
        }

        int start = max_knoop;
        //cout << "start met: " << start << " " << knoop_postordermap.size()<< endl;
        if( kleuren[start] == WIT )
        {
            s.push(start);

            while(s.size() > 0)
            {
                int huidig = s.top();

                if(kleuren[huidig] == WIT)
                {
                    kleuren[huidig] = GRIJS;
                    map<int,int>::iterator it = this->knopen[huidig].begin();
                    while(it != this->knopen[huidig].end())
                    {
                        if(kleuren[it->first] == WIT)
                        {
                            s.push(it->first);
                        }
                        it++;
                    }
                }
                else if( kleuren[huidig] == GRIJS )
                {
                    kleuren[huidig] = ZWART;
                    componenten[huidig] = componentTeller;
                    //cout << "verwijder " << huidig << endl;
                    knoop_postordermap.erase(huidig);
                    s.pop();
                }
                else {
                    cout << "MAYDAY" << endl;
                }
            }
        }
        else if (kleuren[start] == ZWART)
        {
            cout << "MAYDAY" << endl;
        }

        componentTeller++;
        //cout << "SIZE/ " << knoop_postordermap.size() << endl;
    }
}

template<RichtType RT>
map<int, int> Graaf<RT>::diepte_eerst_zoeken()
{
    map<int, int> postordernummers;
    int postordernr = 0;

    vector<Kleur> kleuren(this->aantalKnopen(),WIT);

    stack<int> s;

    for(int start = 0; start < this->aantalKnopen(); start++)
    {
        //cout << "start" << start << endl;
        if( kleuren[start] == WIT )
        {
            s.push(start);
            while(s.size() > 0)
            {
                int huidig = s.top();

                if(kleuren[huidig] == WIT)
                {
                    kleuren[huidig] = GRIJS;
                    map<int,int>::iterator it = this->knopen[huidig].begin();
                    while(it != this->knopen[huidig].end())
                    {
                        if(kleuren[it->first] == WIT)
                        {
                            s.push(it->first);
                        }
                        it++;
                    }
                }
                else if( kleuren[huidig] == GRIJS )
                {
                    kleuren[huidig] = ZWART;
                    //cout << "Knoop: " << huidig << "krijgt postordernr " << postordernr << endl;
                    postordernummers[huidig] = postordernr;
                    postordernr++;
                    s.pop();
                }
                else {
                    cout << "MAYDAY" << endl;
                }
            }
        }
    }

    return postordernummers;
}

template<RichtType RT>
Graaf<RT> Graaf<RT>::keerOm(){
    Graaf<RT> omgekeerde;
    omgekeerde.knopen.resize(this->aantalKnopen());
    omgekeerde.hoogsteVerbindingsnummer = this->hoogsteVerbindingsnummer;

    for(int i = 0; i<this->aantalKnopen(); i++)
    {
        Knoop van = this->knopen[i];
        map<int,int>::iterator it = van.begin();
        while(it != van.end())
        {
            omgekeerde.knopen[it->first][i]= it->second; // stl map insert pair
            it++;
        }
    }
    return omgekeerde;
}

template<RichtType RT>
bool Graaf<RT>::is_bereikbaar_kring(int a, int b)
{
    return componenten[a] = componenten[b];
}

template<RichtType RT>
vector<int> Graaf<RT>::zoek_pad(int a, int b)
{
    vector<Kleur> knoop_kleur(knopen.size(), WIT);
    vector<int> knoop_ouderknoop(knopen.size());
    vector<int> knoop_ouder_verbinding(knopen.size());
    queue<int> q;
    bool gevonden = false;
    int aantal_verbindingen = 0;

    q.push(a);
    knoop_kleur[a] = GRIJS;

    // loop alle niveaus af, tot hij gevonden is
    while(!gevonden && q.size() > 0)
    {
        int niveau_size = q.size();

        // loop het huidig niveau van de queue af
        // kijk of een van de buren b is, zoniet, plaats ze op de queue
        while(!gevonden && niveau_size > 0)
        {
            map<int,int>::iterator it = this->knopen[q.front()].begin();
            while(!gevonden && it != this->knopen[q.front()].end())
            {
                // kijk of verbinding in dezelfde component is en of knoop nog wit is
                if(componenten[it->first] == componenten[a] && knoop_kleur[it->first] == WIT)
                {
                    knoop_kleur[it->first] = GRIJS;
                    knoop_ouderknoop[it->first] = q.front();
                    knoop_ouder_verbinding[it->first] = it->second;
                    if(it->first == b) {
                        gevonden = true;
                    }
                }
                it++;
            }

            niveau_size--;
            q.pop();
        }
        aantal_verbindingen++;
    }

    // knoop is gevonden, maak nu de weg terug
    int terugweg_knoop = b;
    vector<int> weg(aantal_verbindingen);
    for(int i = weg.size()-1; i >=0; i--)
    {
        weg[i] = knoop_ouder_verbinding[terugweg_knoop];
        terugweg_knoop = knoop_ouderknoop[terugweg_knoop];
    }

    return weg;
}

template<RichtType RT>
vector<int> Graaf<RT>::zoek_kring(int a, int b) {
    vector<int> heen = this->zoek_pad(a, b);
    vector<int> terug = this->zoek_pad(b,a);
    heen.insert(heen.end(), terug.begin(), terug.end());

    return heen;
}

template<RichtType RT>
void Graaf<RT>::controleerKnoopnummer(int k) const{
    if (k<0 || (size_t)k>=knopen.size()){
        std::ostringstream out;
        out << "Ongeldig knoopnummer "<<k
            <<". Moet >= 0 en < "<<knopen.size()<<" zijn.";
        throw GraafExceptie(out.str());
    }
}

template<RichtType RT>
Graaf<RT>::Graaf(int n) : knopen(n), hoogsteVerbindingsnummer(0){};


template<RichtType RT>
bool Graaf<RT>::isGericht() const { return true; }//voor gerichte graaf

template<>
bool Graaf<ONGERICHT>::isGericht() const { return false; }//voor ongerichte graaf


template<RichtType RT>
int Graaf<RT>::voegKnoopToe(){
    int n = knopen.size();
    knopen.resize(n+1); // default constructor voor nieuwe knoop wordt opgeroepen (hier lege map)
    return n;
}


template<RichtType RT>
int Graaf<RT>::voegVerbindingToe(int van, int naar){
    controleerKnoopnummer(van);
    controleerKnoopnummer(naar);

    if (knopen[van].count(naar) > 0)
    {
        std::ostringstream out;
        out << "verbinding " << van << "-" << naar << " bestaat al";
        throw GraafExceptie(out.str());
    }
    else {
        int verbindingsnummer;
        if (!vrijgekomenVerbindingsnummers.empty()){
            verbindingsnummer=vrijgekomenVerbindingsnummers.top();
            vrijgekomenVerbindingsnummers.pop();
        }else
            verbindingsnummer=hoogsteVerbindingsnummer++;
        voegVerbindingToeInDatastructuur(van,naar,verbindingsnummer);
        return verbindingsnummer;
    }
}

template<RichtType RT>//voor gerichte graaf
void Graaf<RT>::voegVerbindingToeInDatastructuur(int van, int naar, int verbindingsnummer){
    knopen[van][naar] = verbindingsnummer;
}

template<>
void Graaf<ONGERICHT>::voegVerbindingToeInDatastructuur(int van, int naar, int verbindingsnummer){
    knopen[van][naar] = verbindingsnummer;
    knopen[naar][van] = verbindingsnummer;
}

template<RichtType RT>
void Graaf<RT>::verwijderVerbinding(int van, int naar){
    controleerKnoopnummer(van);
    controleerKnoopnummer(naar);
    if (knopen[van].find(naar)!=knopen[van].end()){//verbinding bestaat
        vrijgekomenVerbindingsnummers.push(knopen[van][naar]);
        verwijderVerbindingUitDatastructuur(van,naar);
    }
    // (geen exception indien verbinding niet bestaat)
}

template<RichtType RT>
void Graaf<RT>::verwijderVerbindingUitDatastructuur(int van, int naar){
    knopen[van].erase(naar);
}

template<>
void Graaf<ONGERICHT>::verwijderVerbindingUitDatastructuur(int van, int naar){
    knopen[van].erase(naar);
    knopen[naar].erase(van);
}

template<RichtType RT>
int Graaf<RT>::aantalKnopen() const { return knopen.size(); }

template<RichtType RT>
int Graaf<RT>::aantalVerbindingen() const {
    return hoogsteVerbindingsnummer-vrijgekomenVerbindingsnummers.size();
}

template<RichtType RT>
int Graaf<RT>::verbindingsnummer(int van, int naar) const{
    controleerKnoopnummer(van);
    controleerKnoopnummer(naar);
    Knoop::const_iterator verbindingit = knopen[van].find(naar);

    if (verbindingit == knopen[van].end())
        return -1;
    else
        return (*verbindingit).second;
}

template<RichtType RT>
void Graaf<RT>::wis(){
    knopen.clear();
    hoogsteVerbindingsnummer = 0;
    while (!vrijgekomenVerbindingsnummers.empty())
        vrijgekomenVerbindingsnummers.pop();
}

template<RichtType RT>
void Graaf<RT>::schrijf(std::ostream &os) const{
    os << "Graaf: " << aantalKnopen() << " knopen en "
        << aantalVerbindingen() << " verbindingen:" << std::endl;
    for (int k=0; k<aantalKnopen(); k++)
        schrijfKnoop(os, k);
}

template<RichtType RT>
void Graaf<RT>::schrijfKnoop(std::ostream &os, int k) const{
    os<<"Knoop "<<k<<"::\n";
    for (Knoop::const_iterator it=knopen[k].begin(); it!=knopen[k].end(); ++it)
    {
        os << "  ->" << it->first;
        schrijfVerbinding(os, it->second);
    }
}

template<RichtType RT>
void Graaf<RT>::schrijfVerbinding(std::ostream &os, int v) const{
    os << " via verbinding nr. " << v << std::endl;
}

template<RichtType RT>
std::ostream &operator<<(std::ostream &os, const Graaf<RT> &g){
    g.schrijf(os);
    return os;
}

template<RichtType RT,class Takdata>
class GraafMetTakdata: public virtual Graaf<RT>{
public:
    GraafMetTakdata(int n=0):Graaf<RT>(n){};
    //Noot: toevoegfunctie zonder takdata op te geven kan alleen gebruikt als de klasse
    //      Takdata een defaultconstructor heeft.
    virtual int voegVerbindingToe(int van, int naar);
    virtual int voegVerbindingToe(int van, int naar, const Takdata&);
    //Noot: verwijderVerbinding wordt ongewijzigd overgenomen van Graaf!

    //TakData vrijgeven (eventueel voor wijziging). Geeft nullpointer als tak niet bestaat
    //Noot: pointers teruggegeven door geefTakdata worden ongeldig
    //door toevoegen van een tak.
    const Takdata* geefTakdata(int van,int naar) const;
          Takdata* geefTakdata(int van,int naar)      ;

    virtual void wis();


    // Schrijft de gegevens van de verbinding met verbindingsnummer v naar outputstream os.
    virtual void schrijfVerbinding(std::ostream &os, int v) const;

protected:
    std::vector<Takdata> takdatavector;
};

template<RichtType RT,class Takdata>
int GraafMetTakdata<RT,Takdata>::voegVerbindingToe(int van, int naar){
    return this->voegVerbindingToe(van,naar,Takdata());
}


template<RichtType RT,class Takdata>
int GraafMetTakdata<RT,Takdata>::voegVerbindingToe(int van, int naar, const Takdata& td){
    bool isnieuwtaknummer=this->vrijgekomenVerbindingsnummers.empty();
    int taknummer=Graaf<RT>::voegVerbindingToe(van,naar);
    if (isnieuwtaknummer)
        takdatavector.push_back(td);
    else
        takdatavector[taknummer]=td;
    return taknummer;
}



template<RichtType RT,class Takdata>
const Takdata* GraafMetTakdata<RT,Takdata>::geefTakdata(int van,int naar) const{
    int taknummer=this->verbindingsnummer(van,naar);
    if (taknummer!=-1)
        return &takdatavector[taknummer];
    else
        return 0;
}

template<RichtType RT,class Takdata>
Takdata* GraafMetTakdata<RT,Takdata>::geefTakdata(int van,int naar){
    int taknummer=this->verbindingsnummer(van,naar);
    if (taknummer!=-1)
        return &takdatavector[taknummer];
    else
        return 0;
}


template<RichtType RT,class Takdata>
void GraafMetTakdata<RT,Takdata>::wis(){
    Graaf<RT>::wis();
    takdatavector.clear();
}

template<RichtType RT,class Takdata>
void  GraafMetTakdata<RT,Takdata>::schrijfVerbinding(std::ostream &os, int v) const{
    os << " via verbinding nr. " << v <<" (Data: "<<takdatavector[v]<<")"<< std::endl;
}

template<RichtType RT, class Knoopdata>
class GraafMetKnoopdata:public virtual Graaf<RT>{
public:
    // Construeert een graaf met gegeven RichtType, en lijst van Knoopdata;
    template<class InputIterator>
    GraafMetKnoopdata(InputIterator start,InputIterator eind);
    GraafMetKnoopdata():Graaf<RT>(){};

    virtual int voegKnoopToe();
    virtual int voegKnoopToe(const Knoopdata&);

    const Knoopdata* geefKnoopdata(int knoopnr) const;
          Knoopdata* geefKnoopdata(int knoopnr)      ;
    virtual void wis();
    virtual void schrijfKnoop(std::ostream &os, int k) const;
protected:
    //datavelden
    std::vector<Knoopdata>  knoopdatavector;
};
template<RichtType RT, class Knoopdata>
template<class InputIterator>
GraafMetKnoopdata<RT,Knoopdata>::GraafMetKnoopdata(InputIterator start,InputIterator eind)
    :Graaf<RT>(0){
        for(;start!=eind;start++)
            voegKnoopToe(*start);
}

template<RichtType RT, class Knoopdata>
int GraafMetKnoopdata<RT,Knoopdata>::voegKnoopToe(){
    return this->voegKnoopToe(Knoopdata());
}

template<RichtType RT, class Knoopdata>
int GraafMetKnoopdata<RT,Knoopdata>::voegKnoopToe(const Knoopdata& kd){
    int ret=Graaf<RT>::voegKnoopToe();
    knoopdatavector.push_back(kd);
    return ret;
}

template<RichtType RT,class Knoopdata>
const Knoopdata* GraafMetKnoopdata<RT,Knoopdata>::geefKnoopdata(int knoopnummer) const{
    this->controleerKnoopnummer(knoopnummer);
    return &knoopdatavector[knoopnummer];
}

template<RichtType RT,class Knoopdata>
Knoopdata* GraafMetKnoopdata<RT,Knoopdata>::geefKnoopdata(int knoopnummer){
    this->controleerKnoopnummer(knoopnummer);
    return &knoopdatavector[knoopnummer];
}


template<RichtType RT,class Knoopdata>
void GraafMetKnoopdata<RT,Knoopdata>::wis(){
    Graaf<RT>::wis();
    knoopdatavector.clear();
}


template<RichtType RT, class Knoopdata>
void GraafMetKnoopdata<RT,Knoopdata>::schrijfKnoop(std::ostream &os, int k) const{
    os << "knoop " << k <<"(Data: "<<knoopdatavector[k]<< "):" << std::endl;
    for (std::map<int, int>::const_iterator it=this->knopen[k].begin(); it!=this->knopen[k].end(); ++it){
        os << "  ->" << it->first;
        this->schrijfVerbinding(os, it->second);
    }
}

template<RichtType RT, class Knoopdata, class Takdata>
class GraafMetKnoopEnTakdata:public GraafMetKnoopdata<RT,Knoopdata>,
                             public GraafMetTakdata<RT, Takdata>{
public:
    template<class InputIterator>
    GraafMetKnoopEnTakdata(InputIterator start,InputIterator eind):
        GraafMetKnoopdata<RT,Knoopdata>(start,eind){};
    GraafMetKnoopEnTakdata():
        GraafMetKnoopdata<RT,Knoopdata>(){};
    virtual void wis(){
        GraafMetKnoopdata<RT,Knoopdata>::wis();
        this->takdatavector.clear();
    }
};

#endif // __GRAAF_H

#endif // GRAAF_H_INCLUDED
