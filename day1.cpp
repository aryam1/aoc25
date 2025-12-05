#include <iostream>
#include <fstream>
#include <string>
#include <print>

int main() {
	// Bring in file as a stream
    std::ifstream file("d1key.txt");
    std::string line;
    int pt1 = 0;
    int pt2 = 0;
    int ptr = 50;
    // Loop for all inputs
    while (std::getline(file, line)) {
    	// Convert operation to number
		int sclr = std::stoi(line.substr(1));
		// Iterate through turns
		for (int i =0; i <sclr; i++){
			// Adjust negative spins and modulo to 0-99
			if (line[0] == 'L') ptr = (ptr-1+100)%100;
			else ptr=(ptr+1)%100;
			// If a turn crosses 0, track for pt2
			if (ptr == 0) pt2++;
		}
		// If an operation finished on 0, track for pt1
		if (ptr == 0) pt1++;
	}
	std::println("Part 1 {}, Part 2 {}", pt1, pt2);
    return 0;
}
