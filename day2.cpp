#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <string_view>
#include <print>
#include <algorithm>

// All factors of vector index, smaller than index
// Stored descending so longest substrings are checked first
std::vector<std::vector<int>> factors = {
	{1}, //0
	{1},
	{1},
	{1},
	{2,1},
	{1}, //5
	{3,2,1},
	{1},
	{4,2,1},
	{3,1},
	{5,2,1}, //10
	{1},
	{6,4,3,2,1},
	{1},
	{7,2,1},
	{5,3,1},  //15
	{8,4,2,1},
	{1},
	{9,6,3,2,1},
	{1},
	{10,5,4,2,1} //20	
};

int main() {
	// Reads file directly into a string
	std::ifstream file("d2key.txt");
	std::vector<std::pair<long long, long long>> ranges;
	std::string range;
    long long total1 = 0;
    long long total2 = 0;
    // Processes the comma seperated ranges into pairs
    while (std::getline(file, range,',')) {
    	if (range.length() == 0) break;
    	std::string::size_type dash = range.find("-");
    	long long start = stoll(range.substr(0,dash));
    	long long end = stoll(range.substr(dash+1));
    	ranges.push_back(std::pair{start,end});
    }
    for (auto [start, end]:ranges){
    	// Iterate through all values of pair range
       	for (long long i = std::max(static_cast<long long>(10),start); i <= end; ++i){

			// Convert number to string
       		std::string val = std::to_string(i);
       		// Get length factors and half length
       		int l = (1+val.length())/2;
       		std::vector facts = factors[val.length()];
       		
      		for (int f : facts){
       			// Assert each char == char-f
       			bool valid = [&]() {
       			    for (auto it = val.begin() + f; it != val.end(); ++it) {
       			    	if (*it != *(it - f)) return false;
       			    }
       			    return true;
       			}();
       			
       			// If valid, dont check other factors
       			if (!valid) continue;
       			total2 += i;
		    	if (f==l) total1+=i;
		    	break;
       		}
       	}
    }
    std::println("Part 1 Total: {}, Part 2 Total: {}",total1, total2);
    return 0;
}
