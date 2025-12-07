#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <print>
#include <thread>
#include <chrono>
#include <cmath>

// Global memo table
std::vector<std::vector<long long>> memo{};

// Highest path count is around 10^14
int m_base = 15;

struct RGB {int r, g, b;};
const RGB colours[15] {
 {75, 41, 145}, {101, 42, 152}, {126, 44, 160}, {151, 47, 161},
 {176, 51, 158},{198, 58, 154}, {216, 68, 145}, {234, 79, 136}, 
 {241, 97, 128},{248, 114, 121},{249, 134, 119},{247, 155, 121},
 {245, 176, 128},{241, 196, 145},{237, 217, 163}
 };

int splits{};

long long dfs(auto const &mat, size_t row, int col){
	// Scan downwards for each node
	while (row < mat.size()) {
		// Call DFS on splitting of beam
		if (mat[row][col] == '^') {
			// Check if splitter has already been scanned and stored
			if (memo[row][col]) return memo[row][col];
			// Increment split count
			splits++;
			long long l = dfs(mat,row,col-1);
			long long r = dfs(mat,row,col+1);
			// Memoise children paths of splitter
			memo[row][col] = l+r;
			return l+r;
		}
		row++;
	}
	// If not split, there's only 1 path
	return 1;
} 

int main() {
	std::ifstream file("d7key.txt");
	std::vector<std::vector<char>> matrix;
	std::string line;

	// Load rows
    while (std::getline(file, line)) {
    	if (line.length() == 0) break;
    	// Push string to matrix as chars
    	matrix.push_back(std::vector<char>{line.begin(),line.end()});
    }
    int rows = matrix.size();
    int cols = matrix[0].size();

    // Find column of start beam
    int start_ind = std::find(matrix[0].begin(), matrix[0].end(),'S') - matrix[0].begin();

	// Initialise memo table to 0s
	memo.resize(matrix.size(), std::vector<long long>(matrix[0].size(), 0));
	
	// DFS for path enumeration and to count splits
    long long timelines = dfs(matrix, 0,start_ind);

    std::println("Splits: {}",splits);
    std::println("Timelines: {}",timelines);

    // Hide cursor
   	std::cout << "\033[?25l";
  	// Save cursor position below prompt
  	std::cout << "\033[s" << std::flush;
  	// Restore cursor to starting point
    std::cout << "\033[u";
  	// Visualise matrix in terminal row by row
    for (auto &row : memo) {
  	    for (long long c : row) {
  	    	// If cell is empty don't print a colour, otherwise val + 1 for index 1 to 9
  		   	if (c == 0) {
  		   		//std::cout << "\033[0m  ";
  		   		continue;
  		   	}
  		   	int lg = std::log10(c);
  		   	
  		    // Get correct colour and print the RGB values
  		    RGB colour = colours[lg];
  		    //std::cout << "\033[48;2;" << colour.r << ";" << colour.g << ";" << colour.b << "m  \033[0m";
  	    	// Move cursor to next row
  	    	//std::cout << "\n";
	  	    int sleep_ms = 10;
	    	//std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
	  		//std::cout << std::flush;
  	    }
     	// Show cursor again
     	std::cout << "\033[?25h" << std::flush;
	}
	return 0;
}
