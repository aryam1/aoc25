#include <fstream>
#include <string>
#include <print>
#include <algorithm>
#include <vector>
#include <utility>
#include <array>


int main() {
	std::vector<std::array<long long,2>> coords;
	// Reads file directly into a string
	std::ifstream file("d9key.txt");
	long long x, y;
	char comma1;
    // Processes the comma seperated numbers into coords
    while (file >> y >> comma1 >> x) {
    	coords.push_back({x, y});
    }
    auto cmp = [](const std::array<long long,2>& a, const std::array<long long,2>& b) {return a[1]<b[1];};
    sort(coords.begin(),coords.end(),cmp);
    std::println("{}",coords);
    
	long long area = 0;
	for (size_t i = 0; i < coords.size(); ++i){
	   	for (size_t j = 0; j<coords.size(); ++j){
	   		long long a = (1+std::abs(coords[i][0]-coords[j][0])) * (1+std::abs(coords[i][1]-coords[j][1]));
	   		//std::println("({},{}) * ({},{}) = {} ", coords[i][0],coords[i][1],coords[j][0],coords[j][1],a);
	   		area = std::max(area, a);
   		}
   	}
   	
   	
   	std::println("Part 1: {}",area);    
    return 0;
}
