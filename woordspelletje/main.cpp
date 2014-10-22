#include <iostream>
#include "graaf.h"

using namespace std;

int main()
{
    Graaf<GERICHT> g;

    g.voegKnoopToe();
    g.voegKnoopToe();
    g.voegKnoopToe();
    g.voegKnoopToe();

    g.voegVerbindingToe(0,2);
    g.voegVerbindingToe(2,1);
    g.voegVerbindingToe(1,2);

    // stap 1
    Graaf<GERICHT> omg = g.keerOm();

    cout << g << endl;
    //cout << omg << endl;

    // stap 2
    map<int, int> post = omg.diepte_eerst_zoeken();

    // // beginnen bij hoogste postorder nummer
    // map<int,int>::iterator it = post.begin();
    // while(it!=post.end())
    // {
    //     cout << it->first << it->second << endl;
    //     it++;
    // }

    // stap 3
    g.componenten_maken(post);

    for(int i = 0; i<g.aantalKnopen(); i++)
    {
        cout << i << " " << g.componenten[i] << endl;
    }

    // componentgraaf maken
    // Graaf<GERICHT> b;
    // b.wordt_componentengraaf_van(g);
    // cout << b << endl;

    cout << "kring: ";
    vector<int> kring = g.zoek_kring(1,2);
    for(int i = 0; i < kring.size(); i++) {
        cout << kring[i] << " ";
    }
    cout << endl;

    return 0;
}
