#include "regexp.h"
#include "symbool.h"
#include "grammatica.h"
using std::cout;
using std::endl;


int main(){
	string s_regex = "(x)((a)|(c))(d)";
	Regexp r_regex(s_regex);
	Grammatica gr(r_regex);
	gr.schrijfHTML();
	return 0;
}
