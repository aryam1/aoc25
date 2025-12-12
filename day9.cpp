#include <fstream>
#include <string>
#include <print>
#include <algorithm>
#include <vector>
#include <utility>
#include <array>
#include <memory>
#include <set>

// Coordinate class
class Coord {
public:
    long long x, y;    
    Coord* prev = nullptr;
    Coord* next = nullptr;
    std::vector<std::shared_ptr<Coord>> neighbours;

    Coord(int x, int y) : x(x), y(y) {}

    // distance between 2 coords on same axis
    int diff(Coord* c) { return std::abs(x - c->x) + std::abs(y - c->y);}
    
};

int main() {
	std::vector<std::shared_ptr<Coord>> coords;
	// Reads file directly into a string
	std::ifstream file("d9key.txt");
	// 50187 48595 y values of big cutout
	long long x, y;
	char comma1;
    // Processes the comma seperated numbers into coords
    while (file >> y >> comma1 >> x) {
    	// Make shared smart pointer so coord pointers dont point to the vector which gets fiddled with
  	    // Coord object is stored seperately on the heap
    	coords.emplace_back(std::make_shared<Coord>(x, y));
    }
    
	long long area = 0;
	for (size_t i = 0; i < coords.size(); ++i){
	   	for (size_t j = 0; j<coords.size(); ++j){
	   		long long a = (1+std::abs(coords[i]->x-coords[j]->x)) * (1+std::abs(coords[i]->y-coords[j]->y));
	   		area = std::max(area, a);
   		}
   	}
   	std::println("Part 1: {}",area);
   	
    // Sort by x then y
    auto xcmp = [](const auto& a, const auto& b) {return (a->x == b->x) ? a->y < b->y : a->x < b->x;};
	// Sort by y then x
	auto ycmp = [](const auto& a, const auto& b) {return (a->y == b->y) ? a->x < b->x : a->y < b->y;};

	// Sort by x value then add reference of closest neighbour to each node
    sort(coords.begin(),coords.end(),xcmp);
    for (size_t i = 0; i < coords.size()-1; ++i ){
    	// If on same axis, add each coord to the other's list
    	if (coords[i]->x == coords[i+1]->x) {
    		coords[i]->neighbours.push_back(coords[i+1]);
    		coords[i+1]->neighbours.push_back(coords[i]);
    	}
    }
	// Sort by y value then add reference of closest neighbour to each node
    sort(coords.begin(),coords.end(),ycmp);
    for (size_t i = 0; i < coords.size()-1; ++i ){
    	// If on same axis, add each coord to the other's list
       	if (coords[i]->y == coords[i+1]->y) {
       		coords[i]->neighbours.push_back(coords[i+1]);
       		coords[i+1]->neighbours.push_back(coords[i]);
    	}
    }

	// lets first connect all the coordinates to their neighbours
	// after load, choose the start coordinate scan for it's nearest neighbours in all 4 directions
	// closest one is it's next
	// set next and set target's prev
	// do while current.prev is void
	// simplest way to scan is to have 2 sorted arrays, sorted by x and y
	// then bs to find the x and y ind, look left and right for the corresponding neighbour coords
	// acc i can make it linear by adding a vector to each coord to track its axis aligned neighbours
	// the min is set

	Coord* curr = coords[0].get();
	while (curr->next == nullptr){
		int dist = 100000;
		Coord* closest = nullptr;
		// Set closest neighbour as next, as long as it's not prev
		for (auto& n: curr->neighbours){
			Coord* nptr = n.get();
			if (nptr == curr->prev) continue;
			int d = curr->diff(nptr);
			if (d<dist){
				dist = d;
				closest = nptr;
			}
		}
		curr->next = closest;
		closest->prev = curr;
		curr = closest;
	}
   	
    return 0;
}
