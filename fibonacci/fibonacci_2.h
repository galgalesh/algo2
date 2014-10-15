#include <iostream>
#include <queue>
#include <map>
	using std::map;
using std::ostream;
using std::max;
using std::pair;
using std::deque;
using std::vector;
using std::make_pair;
using std::cout;
using std::endl;

class Fibonacci_2{
	public:
		Fibonacci_2(map<int, int>, vector<map<int,int> >);
		~Fibonacci_2(){};
		int zoek_n_waarde(int gevraagde_n_index);


	private:
		map<int, int> n_waarden_gekend;
		vector<map<int,int> > a_waarden;
};

Fibonacci_2::Fibonacci_2(map<int, int> _n_waarden_gekend, vector<map<int,int> > _a_waarden) {
	this->n_waarden_gekend = _n_waarden_gekend;
	this->a_waarden = _a_waarden;
}

int Fibonacci_2::zoek_n_waarde(int gevraagde_n_index) {
	int gevraagde_n_waarde = 0;
	map<int, int>::iterator it = n_waarden_gekend.find(gevraagde_n_index);
	if(it != n_waarden_gekend.end()) {
		gevraagde_n_waarde = it->second;
	} else {
		for(int i = 0; i < a_waarden.size(); i++){
			gevraagde_n_waarde += zoek_n_waarde(gevraagde_n_waarde-(a_waarden.size()-i));
		}
		n_waarden_gekend[gevraagde_n_index] = gevraagde_n_waarde;
	}
	return gevraagde_n_waarde;
}
