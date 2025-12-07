#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <print>
#include <locale>

// Global memo table
std::vector<std::vector<long long>> memo{};

long long dfs(auto const &mat, size_t row, int col){
	// Scan downwards for each node
	while (row < mat.size()) {
		// Call DFS on splitting of beam
		if (mat[row][col] == '^') {
			// Check if splitter has already been scanned and stored
			if (memo[row][col]) return memo[row][col];
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
    int splits{};

    // Iterate over all cells
    for (int row = 1; row < rows; ++row){
    	for (int col = 0; col < cols; ++col){
    		// If above cell is a beam
    		if (matrix[row-1][col] == 'S') {
    			// If current cell is a splitter
    			if (matrix[row][col] == '^') {
    				// Add beams to the left and right
    				matrix[row][col-1] = 'S';
    				matrix[row][col+1] = 'S';
    				splits++;
    			}
    			// Continue beam downwards
    			else matrix[row][col] = 'S';
    		}	
    	}
    }
    // Find column of start beam
    int start_ind = std::find(matrix[0].begin(), matrix[0].end(),'S') - matrix[0].begin();

	// Initialise memo table to 0s
	memo.resize(matrix.size(), std::vector<long long>(matrix[0].size(), 0));

	// DFS for path enumeration
    long long timelines = dfs(matrix, 0,start_ind);
    
    std::println("Splits: {}",splits);
    std::println("Timelines: {}",timelines);
    return 0;
}
