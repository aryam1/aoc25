#include <fstream>
#include <sstream>
#include <string>
#include <print>
#include <vector>


void xordp(const uint32_t &val, const std::vector<uint32_t> &stps, std::vector<uint32_t> &arr){
	// Guard clause to not go further than 0
	if (val <= 0) return;
	for (auto &s: stps){
		// XOR step to switch all set bits
		uint32_t newval = val ^ s;
		// If better path to a value is found, propogate that down through it's children
		if (arr[newval] > arr[val]+1){
			arr[newval] = arr[val]+1;
			xordp(newval, stps, arr);
		}	
	}
		 
}

int main() {
	// Reads file directly into a string
	std::ifstream file("d10key.txt");
	std::string line;

	// Input has 3 sections
	// First is the desired state
	// Second are the steps the system can take from 0 to get to state
	// Third is joltage

	// Store state as binary number where # is 1 and . is 0
	// A state is also just a binary number where each digit n corresponds to 2^n =1
	// This is done so the steps can be XORed to DP to the state from 0
	std::vector<uint32_t> states;
	std::vector<std::vector<uint32_t>> steps;
	std::vector<std::vector<uint32_t>> jolts;
	
    // Processes the space seperated numbers into segments
    while (std::getline(file, line)) {
		// Convert line to stream
    	std::istringstream ss(line);
        std::string token;
        std::vector<uint32_t> numbers;
		// Jump to each bracketed segment
        while (ss >> token) {
        	if (token.empty()) continue;
            std::istringstream inner(token);
            std::string seg;
            // Track number of bits in current state
            int binary_length;
        	switch (token[0]) {
        		case '[': {
        			uint32_t num = 0;
        			// Parse into number
        			// Skip the brackets
        			binary_length = token.size() - 2;
        			for (size_t i = 1; i < token.size() - 1; ++i) {
        			    num <<= 1;  // shift left by 1
        			    if (token[i] == '#') num |= 1;  // set the last bit to 1
        			    // if '.', we leave the bit as 0
        			}
        			states.push_back(num);
                    break;
                }
                case '(':{
               		uint32_t num = 0;
             	    // remove '(' and ')'
              	    if (token.front() == '(') token = token.substr(1);
               	    if (token.back() == ')') token.pop_back();
               	    std::istringstream inner(token);

               	    // split by comma
               	    while (std::getline(inner, seg, ',')) {
               	    	int idx = std::stoi(seg);
               	    	// convert MSB-based index into proper bit position
               	    	int bitpos = (binary_length - 1) - idx;
               	        num |= (1 << bitpos);  // set the bit at position 'bit'
               	    }
               	    // Add numerical version of step to list
               	    numbers.push_back(num);
               	    break;
                }
                case '{': {
                	// Add previous list of steps to vector
                	steps.push_back(numbers);
                	numbers = {};
                    // Remove '{' and '}'
                    if (token.front() == '{') token = token.substr(1);
                    if (token.back() == '}') token.pop_back();
                    std::istringstream inner(token);
                    
                    // Just add numbers to vector which gets stored in jolts vector
                    while (std::getline(inner, seg, ',')) numbers.push_back(std::stoi(seg));
                    jolts.push_back(numbers);
                    break;
                }
                default:
                    break;
			}
		}	
	}

	// For each index in vector, have to do DP from state to 0
	// XOR state with each step to get new possible state
	int total = 0;
	for (size_t r  = 0; r< states.size();++r){
		const uint32_t &state = states[r];
		// Setup filled vector with greatest power of 2
		// State could be ..#.., but ##### is reachable which is 2^(len state)-1
		// Jolts is same length as state
		uint32_t l = 1 << jolts[r].size();
		std::vector<uint32_t> dp(l,1000000);
		// Set start position
		dp[state] = 0;
		xordp(state, steps[r],dp);
		total += dp[0];
		std::println("Min steps: {}\n",dp[0]);
	}
	std::println("Total steps: {}",total);

	// Part 2 is essentially path finding in an n dimensional space
	// Starting from 0,0,0...n0, what steps can be taken towards joltage point [a,b,c,d,...nx]
	// BFS with heuristic to keep this fast
	// OR, just use Z3 to solve with linear algebra 
	for (auto j:jolts){
		std::println("{}",j.size());
	}
    return 0;
}
