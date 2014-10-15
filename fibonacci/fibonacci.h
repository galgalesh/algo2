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

	private:
		deque<int> n_waarden;					// de k laatste n-waarden
		vector<pair<int,int> > a_waarden; 	// pair<index_van_a,a>
		int k;								// == n_waarden.size()


};

Fibonacci::Fibonacci(deque<int> _n_waarden, vector<pair<int,int> > _a_waarden) {
	this->n_waarden = _n_waarden;
	this->a_waarden = _a_waarden;
}