#include <fstream>
#include <sstream>
#include <string>
#include <print>
#include <vector>


int main() {
	// Reads file directly into a string
	std::ifstream file("test.txt");
	std::string line;

	// Input has 3 sections
	// First is the desired state
	// Second are the steps the system can take from 0 to get to state
	// Third is joltage

	// Store state as binary number where # is 1 and . is 0
	// A state is also just a binary number where each digit n corresponds to 2^n =1
	// This is done so the steps can be XORed to DP to the state from 0
	std::vector<int> states;
	std::vector<std::vector<int>> steps;
	std::vector<std::vector<int>> jolts;
	
    // Processes the space seperated numbers into segments
    while (std::getline(file, line)) {
		// Convert line to stream
    	std::istringstream ss(line);
        std::string token;
        std::vector<int> numbers;
		// Jump to each bracketed segment
        while (ss >> token) {
        	if (token.empty()) continue;
            std::istringstream inner(token);
            std::string seg;
            
        	switch (token[0]) {
        		case '[': {
        			int num = 0;
        			// Parse into number
        			// Skip the brackets
        			for (size_t i = 1; i < token.size() - 1; ++i) {
        			    num <<= 1;  // shift left by 1
        			    if (token[i] == '#') num |= 1;  // set the last bit to 1
        			    // if '.', we leave the bit as 0
        			}
        			states.push_back(num);
                    break;
                }
                case '(':{
               		int num = 0;
             	    // remove '(' and ')'
              	    if (token.front() == '(') token = token.substr(1);
               	    if (token.back() == ')') token.pop_back();
               	    std::istringstream inner(token);

               	    // split by comma
               	    while (std::getline(inner, seg, ',')) {
               	        num |= (1 << std::stoi(seg));  // set the bit at position 'bit'
               	    }
               	    // Add numerical version of step to list
               	    numbers.push_back(num);
               	    std::println("{} = {}",token,num);
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
		std::println("");
	}
	
	std::println("{}",states);
	std::println("{}",steps);
	std::println("{}",jolts);
	    
    return 0;
}
