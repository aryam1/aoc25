#include <fstream>
#include <string>
#include <print>
#include <vector>
#include <array>
#include <algorithm>
#include <cctype>


int main() {
	// Load file as stream
	std::ifstream file("d3key.txt");
    std::string bank;
    int total1 = 0;
    long long total2 = 0;
    
    // For each row of numbers
    while (std::getline(file, bank)) {
    	std::vector<int> v;
    	// Push number string into digit vector, using char arithmetic
   	    for (char c : bank) {
   	        if (isdigit((unsigned char)c)) v.push_back(c - '0');
   	    }
   	    // 2 pointer sliding window approach
   	    std::vector<int>::iterator prev, curr;
   	    const int batts = 12;
   	    // Buffer is the available digits - desired output length
   	    // Each digit has this initial buffer as its maximum available domain 
   	    int buffer = v.size() - (batts-1);
   	    prev = v.begin();
   	    std::array<int,batts> val{};
   	    
   	    for (int i = 0; i < batts; ++i){
   	    	// Maximise the current digit from its selection of digits within the buffer
   	    	curr = std::max_element(prev,prev+buffer);
   	    	val[i] = *curr;
   	    	// Shrink buffer by however many digits were wasted, for the remaining digits
   	    	// If max was always first digit, i.e 9999..., buffer would never shrink
   	    	buffer -= (curr-prev);
   	    	// Bring in left pane of sliding window, so more like jumping window
   	    	prev = curr+1;
   	    }
   	    auto tens = std::max_element(val.begin(), val.end()-1);
   	    total1 += (*tens)*10 + *(std::max_element(tens+1, val.end()));
   	    
   	    // Construct long long integer
   	    std::string valstr;
   	    for (int x : val) valstr += '0' + x;
   	    total2 += stoll(valstr);
    }
    std::println("Joltage Total:\nPart 1: {}\nPart 2: {}", total1,total2);
    return 0;
}
