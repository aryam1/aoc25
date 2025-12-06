#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <queue>      
#include <utility>
#include <thread>
#include <chrono>
#include <cmath>
#include <print>

struct RGB {int r, g, b;};	
const RGB colours[9] {{24, 15, 61}, {68, 15, 118}, {114, 31, 129}, 
					  {158, 47, 127}, {205, 64, 113}, {241, 96, 93}, 
					  {253, 150, 104}, {254, 202, 141}, {252, 253, 191}
					  };

std::array<std::pair<int,int>,8> neighbours = {{{0,-1},{0,1},{-1,-1},{-1,0},{-1,1},{1,-1},{1,0},{1,1}}};

// Function that grabs the valid 8 directional neighbours of a cell
// if function is called from BFS section, neighbours are decremented in value
// as current cell is being removed, else neighbours are counted
// to give current cell a neighbour count
int scan_neighbours(std::vector<std::vector<int>>& m, 
					std::queue<std::pair<int,int>>& removable,
					int y, int x, bool bfs){
	int xbound = m[0].size();
    int ybound = m.size();

    // If empty cell
    if (m[y][x] < 0) return 0;
    
    for (auto [dy, dx] : neighbours) {
    	int ny = y + dy;
    	int nx = x + dx;
    	
    	// Skip out of bounds cells
    	if (ny < 0 || nx < 0 || ny >= ybound || nx >= xbound) continue;
    	if (bfs){
    		m[ny][nx]-=1;
    		if (m[ny][nx] < 4) removable.push(std::pair{ny,nx});
    	}
    	else if (m[ny][nx] >= 0) m[y][x]+=1;
    }
    // Set current cell to removable, for inital BFS pass
    if (!bfs && m[y][x] < 4) removable.push(std::pair{y,x});
    return 1;
}

int main() {
	std::ifstream file("d4key.txt");
    std::string row;
    std::vector<std::vector<int>> mat;
    int total = 0;
    std::queue<std::pair<int,int>> removable{};
     
	// Construct matrix, converting from & and . to 0 and -1 vectors
    while (std::getline(file, row)) {
    	std::vector<int> r;
   	    for (char c : row) {
   	    	if (c =='@') r.push_back(0);
   	    	if (c =='.') r.push_back(-1);
  	    }
   	    mat.push_back(r);
   	}
   	
    int xbound = mat[0].size();
    int ybound = mat.size();

	// Calculate neighbour counts of cells
    for (int y = 0; y < ybound; ++y){
   		for (int x = 0; x < xbound; ++x){
   			scan_neighbours(mat,removable,y,x,false);
   		}
   	}
   	size_t p1 = removable.size();

  	// Hide cursor
  	std::cout << "\033[?25l";
	// Save cursor position below prompt
	std::cout << "\033[s" << std::flush;
   	
   	// Iterate Multi-Source BFS and draw matrix
   	while (removable.size()>0){
   		// Restore cursor to starting point
   		std::cout << "\033[u";

   		// Calculate fast sigmoid for input scaled frame timing
   		// More operations in a BFS step = faster time
   		double X = p1 - removable.size();
   		double sig = 1/(abs(X/(0.3 + 0.1*(abs(X))))+1);

   		// Visualise matrix in terminal row by row
    	for (auto &row : mat) {
	    	for (int c : row) {
		        // If cell is empty don't print a colour, otherwise val + 1 for index 1 to 9
		        if (c < 0) {
		        	std::cout << "\033[0m  ";
		        	continue;
		        }
		        // Get correct colour and print the RGB values
		        RGB colour = colours[c];
		        std::cout << "\033[48;2;" << colour.r << ";" << colour.g << ";" << colour.b << "m  \033[0m";
	    	}
	    	// Move cursor to next row
	    	std::cout << "\n";
	    }
	    // If it's the first frame, keep frame on screen for a bit longer to show initial state
	    if (removable.size() == p1) std::this_thread::sleep_for(std::chrono::milliseconds(200));
		// Use spin lock to slow down the "animation"
	    int sleep_ms = 1000 * sig;
  		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
	    std::cout << std::flush;

	    // Do the actual BFS step
	    for (int i = removable.size();i--;){
	    	// Pop from queue
	    	auto [y,x] = removable.front();
	    	removable.pop();
	    	// Decrement valid neighbours
	    	int res = scan_neighbours(mat,removable,y,x,true);
	    	
	    	// Skip if BFS source became empty during the iteration
	    	if (res==0) continue;
			// Empty source
	    	mat[y][x] = -1;
	    	total++;
	    }
   	}
   	// Show cursor again
   	std::cout << "\033[?25h" << std::flush;
   	std::println("Initially removable: {}",p1);
   	std::println("Total Removals: {}",total);
    return 0;
}
