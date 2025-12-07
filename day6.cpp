#include <fstream>
#include <string>
#include <print>
#include <algorithm>
#include <vector>
#include <utility>
#include <sstream>
#include <numeric>

int main() {
	std::ifstream file("d6key.txt");
	std::string row;
   	std::string seg;
	std::vector<std::vector<int>> nums;
	std::vector<std::vector<std::string>> nums2;
	std::vector<size_t> widths{};
	std::vector<int> ops;
	long long total{};
	long long total2{};

	// Extract number rows and operations row
	while (std::getline(file, row)) {
    	std::vector<int> r{};
    	std::stringstream ss(row);
    	// >> grabs segments of the stream, skipping whitespace
    	while (ss>>seg){
    		// If segment is only 1 char (* or +), shift it to 0,1
    		if (seg[0] =='*' || seg[0] =='+') r.push_back(seg[0]-42);
    		else {
    			// Track max digits of column
				size_t ind = r.size();
				// If first row, setup width
				if (ind>=widths.size()) widths.push_back(seg.length());
    			else widths[ind] = std::max(seg.length(),widths[ind]);
    			r.push_back(std::stoi(seg));
    		}
    	}
    	// If parsing operation line, set vector to ops
    	if (!(row[0] == '*' || row[0] =='+')) nums.push_back(r);
    	// Otherwise push vector to nums
    	else ops = r;
   	}

	// Reset file stream to beginning
   	file.clear();
   	file.seekg(0);
   	
	// Iterate over rows
	while (std::getline(file, row)) {
		// Break if final row
    	if (row[0] == '*' || row[0] =='+') break;
    	std::vector<std::string> r{};
   	   	// Access the string directly with col widths, for part 2
   	   	size_t ptr{};
   	   	while (ptr < row.length()){
   	   		// Get current columns width
   	   		int w = widths[r.size()];
   	   		// Grab all characters within that columns width
   	   		r.push_back(row.substr(ptr,w));
   	   		// Move pointer to end of column
   	   		ptr += (w+1);
   	   	}
   	   	nums2.push_back(r);
   	}

   	// Calculate expressions
   	for (size_t col = 0; col < ops.size(); ++col){
   		// Convert operation to bool, add being 1 is truthy
   		bool add = ops[col];
   		// Set column total to 1 for multiplication
   		long long coltot = 1;
   		long long coltot2 = 1;

   		int wid = widths[col];
   		std::vector<int> colvals(wid,0);
   		
   		for (size_t row=0; row<nums.size(); ++row){
   			coltot = (add) ? coltot+nums[row][col] : coltot * nums[row][col];

			// Iterate over digits
   			for (int w = 0; w < wid; ++w){
				// Increase current value by factor of 10 and add next value on
   				char val = nums2[row][col][w];
   				if (std::isdigit(val)) colvals[w] = (10*colvals[w]) + val-'0';
   			}
   		}

   		// Calculate column expression for part 2
   		for (auto v:colvals){
   			if (add) coltot2 += v;
   			else coltot2 *= v;
   		}
   		// Subtract initial 1
   		if (add) coltot--, coltot2--;
   		
   		total+= coltot;
   		total2+= coltot2;
   	}
   	std::println("Total 1: {}, Total 2: {}", total,total2);
}
