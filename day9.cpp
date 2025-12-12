#include <fstream>
#include <string>
#include <print>
#include <vector>
#include <set>
#include <map>
#include <array>
#include <deque>

constexpr int W = 250;

int main() {
	// Sorted sets of unique coords for coordinate compression
	std::set<long long> x_coords, y_coords;
	std::vector<std::array<long long,2>> coords;
	// Reads file directly into a string
	std::ifstream file("d9key.txt");
	long long x, y;
	char comma1;
	
	// Processes the comma seperated numbers into coords
    while (file >> y >> comma1 >> x) {
    	// Emplace values to avoid copy constructors
    	x_coords.emplace(x);
    	y_coords.emplace(y);
    	coords.emplace_back(std::array{ x, y }); 
    }
    
	// Setup inverted maps of cordinates to indicies for constant look ups
	std::map<long long, int> inverted_x, inverted_y;
	
	x = 1;
	y = 1;
	// Populate arrays and inverted maps, with buffer region of 1 element 
	for (auto it = x_coords.begin(); it != x_coords.end(); ++it){
		inverted_x[*it] = x;
		++x;
	}	
	for (auto it = y_coords.begin(); it != y_coords.end(); ++it){
		inverted_y[*it] = y;
		++y;
	}
   	// There's under 250 unique values for x and y, so a 250x250 grid can be used
   	std::array<std::array<int,W>,W> compressed = {0};

   	// Setup compressed grid with 1s for original coordinates
   	for (auto& [x_val, y_val]:coords){	
   		int mapped_x = inverted_x[x_val];
   		int mapped_y = inverted_y[y_val];
		compressed[mapped_x][mapped_y] = 1;
   	}
   	
	// Connect all horizontal runs of coordinates
   	for (int row = 0; row <W; ++row){
   		// Boolean to track scanning state (valid is between coordinates) 
   		bool valid = false;
   		for (int ele = 0; ele < W; ++ele){
   			// Every time a coordinate is found, flip state
   			// I.e from the initial invalid state, the first coordinate indicates the start of a run
   			// When another coordinate is encountered, the run is over and what follows is an invalid region
   			if (compressed[row][ele]==1) {
   				valid = !valid;
   				continue;
   			}
   			// If in valid region, set cell to valid
   			if (valid) compressed[row][ele]=2;
   		} 
   	}
   	// Connect all vertical runs of coordinates
 	for (int row = 0; row <W; ++row){
 		bool valid = false;
 	   	for (int ele = 0; ele < W; ++ele){
 	   		if (compressed[ele][row]==1) {
 	   			valid = !valid;
 	   			continue;
 	   		}
 	   		if (valid) compressed[ele][row]=2;
 	   	} 
	}

	std::deque<std::pair<int,int>> dq;

	// Edge safe bfs queue lambda
	auto try_push = [&](int r, int c) {
		// Return if invalid coord or a wall
	    if (r < 0 || r >= W || c < 0 || c >= W) return;
	    if (compressed[r][c] != 0) return;
	    compressed[r][c] = -1;   // mark as outside
	    dq.emplace_back(r,c);
	};
	// Push invalid origin point
	try_push(0,0);

	// BFS flood fill invalid region
	int dr[4] = {1,-1,0,0};
	int dc[4] = {0,0,1,-1};

	// While there's invalid cells, check the neighbours
	while (!dq.empty()) {
	    auto [r,c] = dq.front(); dq.pop_front();
	    for (int k = 0; k < 4; ++k) {
	        try_push(r + dr[k], c + dc[k]);
	    }
	}

	// All 0s are valid interior regions to be filled
	for (int r = 0; r < W; ++r)
	    for (int c = 0; c < W; ++c)
	        if (compressed[r][c] == 0) compressed[r][c] = 3;
	
	// Revert invalid regions to 0
	for (int r = 0; r < W; ++r)
	    for (int c = 0; c < W; ++c)
	        if (compressed[r][c] == -1) compressed[r][c] = 0;

	// Could be smart to find only valid squares to track max valid square, but simpler to scan all coordinates, check if 4 edges are valid and track accordingly

	long long area1 = 0;
	long long area2 = 0;
	for (size_t i = 0; i < coords.size(); ++i){
	   	for (size_t j = 0; j<coords.size(); ++j){
	   		auto [x1,y1] =coords[i];
	   		auto [x2,y2] =coords[j];
	   		
	   		long long a = (1+std::abs(x1 - x2)) * (1+std::abs(y1 - y2));
	   		area1 = std::max(area1, a);

	   		// Get compressed coordinates
	   		int inverted_x1 = inverted_x[x1]; 
	   		int inverted_x2 = inverted_x[x2]; 
	   		int inverted_y1 = inverted_y[y1]; 
	   		int inverted_y2 = inverted_y[y2];

	   		// Swap coordinates so that scan is always done from top left to bottom right
			if (inverted_x1 > inverted_x2) std::swap(inverted_x1, inverted_x2);
			if (inverted_y1 > inverted_y2) std::swap(inverted_y1, inverted_y2);
			
	   		// Check all 4 edges are valid
	   		bool valid = true;
	   		for (int i = inverted_x1; i <= inverted_x2; ++i) {
	   			if (compressed[i][inverted_y1] == 0 || compressed[i][inverted_y2] == 0) valid = false;
	   		}
	   		for (int j = inverted_y1; j <= inverted_y2; ++j) {
	   			if (compressed[inverted_x1][j] == 0 || compressed[inverted_x2][j] == 0) valid = false;
	   		}
	   		if (valid && a > area2) area2 = a;
   		}
   	}
   	std::println("Part 1: {}, Part 2: {}",area1,area2);
   	return 0;
}





