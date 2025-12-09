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
private:
    Coord* parent;
public:
    int x, y, z;
    int group_size = 1;
    
    Coord(int x, int y, int z) : parent(this),x(x), y(y), z(z) {}

	// Self explanatory squared distance between 2 coords
    long long diff(const Coord& c){
    	long long dx = (long long)x - c.x;
   	    long long dy = (long long)y - c.y;
   	    long long dz = (long long)z - c.z;
   	    return dx*dx + dy*dy + dz*dz;
    }

	// Get root parent 
    Coord* getParent() {
    	// If current coord isn't a root, point it's parent value to
    	// it's parent's parent recursively
    	// Essentially flattening the tree with path compression
    	if (parent != this) parent = parent->getParent();
    	return parent;
    }

	// Join this coord to another coord set by pointing to the other's parent
    void setParent(Coord* other) {
    	// Get the two group roots
        Coord* r1 = getParent();
        Coord* r2 = other->getParent();
        if (r1 == r2) return;

    	// If they're not part of the same group, add this coord to the other's group and update sizes
        r1->parent = r2;
        r2->group_size += r1->group_size;
    }
    
    // Get size of set
    int getSize() {return getParent()->group_size;}
};


std::vector<std::shared_ptr<Coord>> coords;

int main() {
	// Reads file directly into a string
	std::ifstream file("d8key.txt");
	int x, y, z;
	char comma1, comma2;
    // Processes the comma seperated numbers into coords
    while (file >> x >> comma1 >> y >> comma2 >> z) {
    	// Make shared smart pointer so coord.parent doesnt point to the vector which gets fiddled with
    	// Coord object is stored seperately on the heap
    	coords.emplace_back(std::make_shared<Coord>(x, y, z));
    }
    
    // std::println("Coordinates: ");
    // for (const auto& uptr : coords) {
    //     Coord& c = *uptr;
    // 	Coord* p = c.getParent();
    // 	std::println("{} {} {} -> {} {} {}",c.x,c.y,c.z,p->x,p->y,p->z);
    // }

	// Vector of shared pointer pairs of all possible pairs
    std::vector<std::pair<std::shared_ptr<Coord>, std::shared_ptr<Coord>>> pairs;
    for (size_t i = 0; i < coords.size(); ++i){
	   	for (size_t j = i+1; j<coords.size(); ++j){
	   		// Create pairs of coords with the raw pointer gotten from unique pointer
   			pairs.emplace_back(coords[i],coords[j]);
   		}
   	}
   	
	// Sort pairs vector based on the distance between coordinate pairs 
   	sort(pairs.begin(),pairs.end(),
   		 [](const auto& a, const auto& b) {
		 	return a.first->diff(*a.second) < b.first->diff(*b.second);
		 });

	
	// For part 1, set i to 1000 and multiply largest 3 group sizes together
	int i = 0;
	// Pair the closest 1000 pairs for part 1
	for (;i < 1000; i++){
		// go through neighbours, add right to left set
		auto& [c1, c2] = pairs[i];
		c2->getParent()->setParent(c1->getParent());
	}
	// Comparator function for DSU sizes
	auto cmp = [](const std::shared_ptr<Coord>& a, const std::shared_ptr<Coord>& b) { 
		return b->getSize() < a->getSize(); 
	};
	
	// Construct hashset of parents from coordinate vector, sorted by set size descending 
	std::set<std::shared_ptr<Coord>, decltype(cmp)> parentGroups(coords.begin(),coords.end(), cmp);
	
	// Prints groups
	// for (const auto& uptr : parentGroups) {
	//     Coord& c = *uptr;
	//	   std::println("{} {} {} -> {}",c.x,c.y,c.z,c.getSize());
	// }
	
	// Get iterator to first element in the set, move it forward twice
	// to get the product of the 3 largest groups after 1000 joins
	auto it = parentGroups.begin();
	int part1 = 1;
	part1 *= (*it)->getSize();
	++it;
	part1 *= (*it)->getSize();
	++it;
	part1 *= (*it)->getSize(); 

	// Continue adding pairs together for part 2
	// 4950 is the last pair needed to put all coords in 1 group, found through manual binary search
	for (;i < 4950; i++){
		auto& [c1, c2] = pairs[i];
		c2->getParent()->setParent(c1->getParent());
	}
	
	// Get coordinates of final pair to complete the full set, for part 2
	auto& p = pairs[i-1];
	Coord& c1 = *(p.first);
	Coord& c2 = *(p.second);
	
	std::println("Part 1: {}, Part 2: {}",part1,c1.x*c2.x);
    
    return 0;
}
