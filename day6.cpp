#include <fstream>
#include <string>
#include <print>
#include <algorithm>
#include <vector>
#include <utility>
#include <sstream>


int main() {
	std::ifstream file("d6key.txt");
	std::string row;
   	std::string seg;
	std::vector<std::vector<int>> nums;
	std::vector<int> ops;
	long long total{};

	// Extract number rows and operations row
	while (std::getline(file, row)) {
    	std::vector<int> r;
    	std::stringstream ss(row);
    	// >> grabs segments of the stream, skipping whitespace
    	while (ss>>seg){
    		// If segment is only 1 char (* or +), shift it to 0,1
    		if (seg[0] =='*' || seg[0] =='+') r.push_back(seg[0]-42);
    		else r.push_back(std::stoi(seg));
    	}
    	// If parsing operation line, set vector to ops
    	if (!(row[0] == '*' || row[0] =='+')) nums.push_back(r);
    	// Otherwise push vector to nums
    	else ops =r;
   	}
   	
   	// Calculate expressions
   	for (size_t op = 0; op < ops.size(); ++op){
   		// Convert operation to bool, add being 1 is truthy
   		bool add = ops[op];
   		// Set column total to 1 for multiplication
   		long long coltot = 1;
   		for (size_t n=0; n<nums.size(); ++n){
   			coltot = (add) ? coltot+nums[n][op] : coltot * nums[n][op];
   		}
   		// Subtract initial 1
   		if (add) coltot--;
   		total+= coltot;
   	}

   	std::println("Final total: {}", total);
}
