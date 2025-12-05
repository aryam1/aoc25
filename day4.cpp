#include <bits/stdc++.h>
#include <chrono>
#include <thread>

using namespace std;

struct RGB {int r, g, b;};

const RGB colours[10] = {
	{13, 8, 135},{70, 3, 159},
    {114, 1, 168},{156, 23, 158},
    {189, 55, 134},{216, 87, 107},
    {237, 121, 83},{251, 159, 58},
    {253, 202, 38},{240, 249, 33}
};	

array<pair<int,int>,8> neighbours = {{{0,-1},{0,1},{-1,-1},{-1,0},{-1,1},{1,-1},{1,0},{1,1}}};


int scan_neighbours(vector<vector<int>>& m, 
						 queue<pair<int,int>>& removable,
						 int y, int x, bool bfs){
	int xbound = m[0].size();
    int ybound = m.size();
    
    if (m[y][x] < 0) return 0;
    
    for (auto [dy, dx] : neighbours) {
    	int ny = y + dy;
    	int nx = x + dx;
    	if (ny < 0 || nx < 0 || ny >= ybound || nx >= xbound) continue;
    	if (bfs){
    		m[ny][nx]-=1;
    		if (m[ny][nx] < 4) removable.push(pair(ny,nx));
    	}
    	else if (m[ny][nx] >= 0) m[y][x]+=1;
    }
    if (!bfs && m[y][x] < 4) removable.push(pair(y,x));
    return 1;
}

int main() {
	ifstream file("d4key.txt");
    string row;
    vector<vector<int>> mat;
    int total = 0;
    queue<pair<int,int>> removable{};
     
	// Construct Matrix
    while (std::getline(file, row)) {
    	vector<int> r;
   	    for (char c : row) {
   	    	if (c =='@') r.push_back(0);
   	    	if (c =='.') r.push_back(-1);
  	    }
   	    mat.push_back(r);
   	}
   	
    int xbound = mat[0].size();
    int ybound = mat.size();

	// Calculate Neighbours
    for (int y = 0; y < ybound; ++y){
   		for (int x = 0; x < xbound; ++x){
   			scan_neighbours(mat,removable,y,x,false);
   		}
   	}
   	int p1 = removable.size();

  	// hide cursor
  	cout << "\033[?25l";
	// save cursor position below prompt
	cout << "\033[s" << flush;
   	
   	// Iterate Multi-Source BFS

   	const double base_sleep_us = 500000;   // 500 ms
   	const double min_sleep_us = 50000;     // 50 ms
   	
   	while (removable.size()>0){
   		// restore cursor to starting point
   		cout << "\033[u";
    	for (auto &row : mat) {
	    	for (int c : row) {
		        // use 0th color if value is negative, otherwise val + 1 for index 1 to 9
		        int idx = (c < 0) ? 0 : c+1;
		        RGB colour = colours[idx];
		        cout << "\033[48;2;" << colour.r << ";" << colour.g << ";" << colour.b << "m  \033[0m";
	    	}
	    	cout << "\n";
	    }
	    double factor = static_cast<double>(removable.size()) / (xbound * ybound);
	    int sleep_us = max(min_sleep_us, base_sleep_us * factor);
  		this_thread::sleep_for(chrono::microseconds(sleep_us));
	    cout << flush;
	    for (int i = removable.size();i--;){
	    	auto [y,x] = removable.front();
	    	removable.pop();
	    	int res = scan_neighbours(mat,removable,y,x,true);
	    	if (res==0) continue;
	    	mat[y][x] = -1;
	    	total++;
	    }
   	}
   	// show cursor again
   	cout << "\033[?25h" << flush;
   	print("Initially removable: {}\n",p1);
   	print("\nTotal Removals: {}\n",total);
    return 0;
}
