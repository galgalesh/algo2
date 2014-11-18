#include "regexp.h"
#include "symbool.h"
#include "grammatica.h"
using std::cout;
using std::endl;


int main(){
	string s_regex = "(x)((a)|(c))(d)";
	Regexp r_regex(s_regex);
	cout << "regex bij start: "<< endl;
	cout << r_regex << endl <<endl;

	// cout << r_regex.geefOpcode() << endl;

	Grammatica gr(r_regex);
	gr.schrijf();
	// while() {
	// 	// steek de mogelijke kinderen in s
	// 	// 
	// }





	return 0;
}
