#include <cstdlib>
#include <ios>

#include "rzwboom.h"

using namespace std;
using std::pair;
using std::cout;
using std::endl;

/*
* maak een foute boom, kijk of hij kleurbaar is
*
*/
int main(int argc, char** argv) {

    Rzwboom<int,int> boom;
    for (int i = 0; i < 3; i++) {
        boom.voegtoe(i,i,false);
    }
    
    cout << boom.diepte() << endl;
    cout << boom.zwarte_diepte() << endl;
    cout << "is kleurbaar?: " << boom.is_kleurbaar() << endl;
    
    return 0;
}