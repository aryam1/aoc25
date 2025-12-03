#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <regex>
#include <string_view>


using namespace std;

int main() {
	ifstream file("d2key.txt");
	stringstream buffer;
    buffer << file.rdbuf();
    string contents = buffer.str();
    regex seg(R"((\d+)-(\d+),?)");
    long long total = 0;
    for (sregex_iterator iter(contents.begin(), contents.end(), seg);
         iter != sregex_iterator{}; ++iter)
         {
         	const smatch& m = *iter;
         	long long start = stoll(m[1].str());
         	long long end = stoll(m[2].str());
         	for (auto i = start; i <= end; ++i){
         		string val = to_string(i);
         		if (val.length() % 2 == 1) continue;
         		int half = val.length() / 2;
         		string_view lview(val.data(), half);
         		string_view rview(val.data()+half);
         		if (lview.compare(rview) == 0) total += (long long)stoll(val);
         	}
        }
    cout << total<<endl;
    return 0;
}
