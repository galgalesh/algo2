#include <iostream>
#include <queue>
using std::ostream;
using std::max;
using std::pair;
using std::deque;
using std::vector;
using std::make_pair;
using std::cout;
using std::endl;

class Fibonacci{
	public:
		Fibonacci(deque<int>,vector<pair<int,int> >);
		~Fibonacci(){};

	    friend std::ostream& operator<<(std::ostream& os, const Fibonacci fib){
	        return os<<"k: " << fib.k << endl;
	    }

	    bool zoek_n_waarde(int n_index, int& n_waarde); // zoekt n_waarde ahv index.
	    												// Geeft false als de klasse de n al "voorbij gelopen" is

	private:
		deque<int> n_waarden;					// de k laatste n-waarden
		vector<pair<int,int> > a_waarden; 		// pair<index_van_a,a>
		int k;									// == n_waarden.size()
		int hoogste_n_index;					// de index van de eerste n in n_waarden


};

Fibonacci::Fibonacci(deque<int> _n_waarden, vector<pair<int,int> > _a_waarden) {
	this->n_waarden = _n_waarden;
	this->a_waarden = _a_waarden;
	this->k = _n_waarden.size();
	this->hoogste_n_index = k;
}

bool Fibonacci::zoek_n_waarde(int gevraagde_n_index, int& n_waarde) {

	// kijk of de n_index nog te berekenen is (ofdat hij niet te laag is)
	if(gevraagde_n_index <= (this->hoogste_n_index - this->k)) {
		return false;
	}

	// indien de n_index in de deque zit, geef die
	if(gevraagde_n_index <= hoogste_n_index) {
		deque<int>::iterator it = n_waarden.begin();
		n_waarde = it[k-(hoogste_n_index-gevraagde_n_index)-1];
		return true;
	}

	// n zit verder, dus:
	// schuif de deque op, bereken de volgende n-waarde tot als de waarde gevonden is
	while(hoogste_n_index!=gevraagde_n_index) {
		int getal = 0;
		for(int i = 0; i < a_waarden.size(); i++) {
			getal += a_waarden[i].second * n_waarden[a_waarden[i].first];
		}
		hoogste_n_index++;
		n_waarden.push_back(getal);
		n_waarden.pop_front();
	}
	// geef de n_waarde terug
	n_waarde = n_waarden.back();
	return true;
}
