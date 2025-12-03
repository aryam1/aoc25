#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <string_view>
#include <print>


using namespace std;

int main() {
	ifstream file("d2key.txt");
	stringstream buffer;
    buffer << file.rdbuf();
    string contents = buffer.str();
    regex seg(R"((\d+)-(\d+),?)");
    long long total1 = 0;
    long long total2 = 0;
    for (sregex_iterator iter(contents.begin(), contents.end(), seg); iter != sregex_iterator{}; ++iter) {
       	long long start = stoll((*iter)[1].str());
       	long long end = stoll((*iter)[2].str());
       	for (auto i = start; i <= end; ++i){
       		if (i < 10) continue;
       		string val = to_string(i);
       		int l = (1+val.length())/2;
       		
       		// Need to handle the case of substr len 1 seperately and break early 
   			regex pattern("^(" + string(1,val[0]) + ")+$");
   			if (regex_match(val, pattern)) {
   				//print("Matched len 1: {}\n",val);
   				if (l*2 == val.length()) total1 +=i;
   				total2 +=i;
   				continue;
   			}
   			for (int s = l; s > 1; --s){
       			string_view substr(val.data(),s);
       			regex pattern("^(" + string(substr) + ")+$");
   			    if (!(regex_match(val, pattern))) continue;
		    	total2 += i;
		    	if (s==l) total1+=i;
		    	break;
		      	//print("\nFull Value: {}\n", val);     		
   			  	//print("Substring: {}\n",substr);
       		}
       	}
    }
    print("Part 1 Total: {}, Part 2 Total: {}\n",total1, total2);
    return 0;
}
