#include <iostream>
#include <string>
using namespace std;

string str("aaaaaaa");

int main(void){
	const string& str1 = str;
	str1.append("aa");
	return 0;
}
