#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <print>
#include <array>

int main() {
	std::ifstream file("d12key.txt");
	std::string line;
	// Directly store the shaded areas of each piece
	// rather than parse them in programatically
	std::array<int,6> pieces{6,7,5,7,7,7};
	int valid{0};
	int invalid{0};
	int total{0};

    // Start parsing at the systems line, ignoring the input pieces
    while (std::getline(file, line)) {
    	if (line.length() == 0) continue;
    	if (line[2] != 'x') continue;
		// Get the problem's dimensions
    	int x = std::stoi(line.substr(0, 2));
    	int y = std::stoi(line.substr(3, 2));
    	total++;
    	// Parse in the counts of the required pieces (0-5)
    	// for each piece, subtract from overall area the case
    	// where all pieces are perfectly packed (so the piece area itself)
    	// and the case where the area can be sparsely packed
    	// (so the 3x3 footprint area that all pieces have)
    	// Sparse can onlu be packed for dimensions that are divisible by 3
    	// So the area for it has to be calculated from the lowest
    	// multiple of 3 for x and y
    	int sparseArea{(x-(x%3)) * (y-(y%3))};
    	int packedArea{x*y};

		// If there is free space after sparseArea is minimised
		// then the system can be packed in any which way
		// If there is negative space after packedArea is minimised
		// then the system can never be packed
		for (int i{0}; i < 6; ++i){
    		int count = std::stoi(line.substr((i*3)+7,(i*3)+9));
    		sparseArea-=(9*count);
    		packedArea-=(pieces[i]*count);
    	}
    	if (sparseArea >= 0) valid++;
    	else if (packedArea < 0) invalid++;
    }
    std::println("{} Total, {} Valid, {} Invalid",total,valid,invalid);
    return 0;
}
