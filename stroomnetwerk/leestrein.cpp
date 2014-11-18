#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <cstdlib>
using std::cout;
using std::endl;
using std::string;
#include "tinyxml.h"
#include "tinystr.cpp"
#include "tinyxml.cpp"
#include "tinyxmlparser.cpp"
#include "tinyxmlerror.cpp"

#include "stroomnet.h"

template<class T>
T getAttribuut(TiXmlElement *el, string name)
{
    std::stringstream ss;
    T ret;

    TiXmlAttribute *attr = el->FirstAttribute();
    while (attr != 0)
    {
        if (attr->Name() == name) {
            ss << attr->Value();
            ss >> ret;
            return ret;
        }
        attr = attr->Next();
    }
}

int main()
{
    TiXmlDocument doc( "treingraaf.xml" );
    bool loadOkay = doc.LoadFile();
    assert(loadOkay);

    string inf = "hoofdredacteur-directeur";
    string prot = "kleinschaligheidstoeslag";

    GraafMetKnoopEnTakdata <GERICHT, string, double> gr;

    // Alle steden toevoegen
    std::map<string, int> map; // stad -> knoopnr
    TiXmlNode *netwerk;
    netwerk = doc.RootElement();
    assert( netwerk );
    for (TiXmlNode *knooppunt = netwerk->FirstChild(); knooppunt != 0; knooppunt = knooppunt->NextSibling())
    {
        string stad = getAttribuut<string>(dynamic_cast<TiXmlElement *>(knooppunt), "nummer");

        map[stad] = gr.voegKnoopToe(stad);

        // Knoop ontdubbelen indien != vertrek/einde
        if (stad != inf && stad != prot) {
            map[stad + "_UIT"] = gr.voegKnoopToe(stad + "_UIT");
            gr.voegVerbindingToe(map[stad], map[stad + "_UIT"], 1.0);
        }
    }

    // Alle verbindingen toevoegen
    for (TiXmlNode *knooppunt = netwerk->FirstChild(); knooppunt != 0; knooppunt = knooppunt->NextSibling())
    {
        string van = getAttribuut<string>(dynamic_cast<TiXmlElement *>(knooppunt), "nummer");

        for (TiXmlNode *route = knooppunt->FirstChild(); route != 0; route = route->NextSibling())
        {
            string doel = getAttribuut<string>(dynamic_cast<TiXmlElement *>(route), "doel");
            // enkel aantal bepalen dus gewicht  niet nodig
            //double afstand = getAttribuut<double>(dynamic_cast<TiXmlElement *>(route), "afstand");
            
            if (van == inf || van == prot) {
                gr.voegVerbindingToe(map[van], map[doel], 1.0);
            } else {
                gr.voegVerbindingToe(map[van + "_UIT"], map[doel], 1.0);
            }
        }
    }

    Stroomnetwerk<double> sn(gr, map[inf], map[prot]);
    cout<<sn<<endl;
    cout << sn.geefCapaciteit() << endl;

    return 0;
}

