#include <fstream>

#include "schijf.h"
#include "btree.h"

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::remove_if;
using std::isalpha;

bool isNotAlpha(char c) {
	return !isalpha(c);
};

int main(int argc, char** argv) {

	// Declare fields
	Schijf< Bknoop<int, int, 5> > schijf;
	Btree<int, int, 5> boom(schijf);

    for(int i = 0; i <= 10; i++) {
    	cout << "Toevoegen: " << i << endl;
    	boom.voeg_toe(i,i);
    }

    /*
    int temp;
    int key = 5;
    cout << "Zoek nr. 5: " << boom.zoek(temp, key) << " data: " << temp << endl;

    key = 500;
    cout << "Zoek nr. 500: " << boom.zoek(temp, key) << " data: " << temp << endl;

    key = 10;
    cout << "Zoek nr. 10: " << boom.zoek(temp, key) << " data: " << temp << endl;
*/
	/*
	// Open file
    ifstream file;
    file.open ("bible.txt");

    // Read file
	string word;
	while (file >> word)
	{
		word.erase(remove_if(word.begin(), word.end(), isNotAlpha), word.end());
	    cout << word << endl;
	}
	*/

    return 0;
}