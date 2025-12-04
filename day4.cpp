#include <bits/stdc++.h>

using namespace std;

int main() {
	ifstream file("test.txt");
    string row;
    vector<vector<char>> mat;
    long long total = 0;
    while (std::getline(file, row)) {
    	vector<char> r;
   	    for (char c : row) if (c =='@'||c=='.') r.push_back(c);
   	    mat.push_back(r);
   	}
    int xbound = mat[0].size();
    int ybound = mat.size();
	array<pair<int,int>,8> neighbours = {{
		{0,-1},{0,1},{-1,-1},{-1,0},{-1,1},{1,-1},{1,0},{1,1}
	}};
    for (int y = 0; y < ybound; ++y){
   		for (int x = 0; x < xbound; ++x){
   			int count = 0;
   			if (mat[y][x] == 46) continue;
   			print("\nCurrent cell {} {}\n", y ,x);
   			for (auto [dy, dx] : neighbours) {
   				int ny = y + dy;
   				int nx = x + dx;

   				if (ny < 0 || ny >= ybound) continue;
   				if (nx < 0 || nx >= xbound) continue;
   				if (mat[ny][nx] != 46) count++;
   				print("{} {} {}\n",ny,nx, mat[ny][nx]);
   			}
   			if (count < 4) {
   				total++;
   				mat[y][x] = '!';
   			}
   		}
   	}
    print("Final: {}\n", total);
    for (auto &row : mat) {
	    for (char c : row) {
	        switch (c) {
	            case '.': // black square
	                cout << "\033[40m \033[0m"; // black background
	                break;
	            case '@': // red square
					cout << "\033[41m \033[0m"; // red background
	                break;
	            case '!': // green square
					cout << "\033[42m \033[0m"; // green background
	                break;
	            default: // print any other character normally
	                cout << c;
	        }
	    }
	    cout << "\n"; // single newline between rows
	}
    return 0;
}
