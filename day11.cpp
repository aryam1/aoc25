#include <fstream>
#include <string>
#include <print>
#include <vector>
#include <memory>
#include <unordered_map>
#include <sstream>


// Node class
class Node {
public:
    std::vector<Node*> connections;
	std::string label;
	
    Node(std::string l) : label(l) {}
    // Add graph node edges
    void add(Node* child) {
    	connections.push_back(child);
    }
};

long long dfs(std::unordered_map<uint64_t, long long> &memo, 
			  Node* const node, int tracker, bool part2){
	// Convert node pointer to its bit representation, ~48 bits
	// so it needs to be stored in a 64 bit int
	uint64_t key = reinterpret_cast<uint64_t>(node);
	// Shift node bits up 2 bits to fit in tracker bits
	// only 2 bits needed as there's only 3 possible states (0,1,2)
	key = (key << 2) | tracker;
	// Shift sequence 1 bit and OR in the bool, so the key uniquely represents
	// the dfs state for memoization
	key = (key << 1) | part2;

	// If current state has been computed, return found value
	if (auto it = memo.find(key); it != memo.end()) return it->second;
	
	if (node->label == "out") {
		// If a path reached the end (for part 1) or
		// reached the end and hit the 2 required nodes
		// along the way (part 2)
		long long res = (!part2 || tracker == 3);
		// Assignment in C++ evaluates to the assigned value
		// So this sets the memo table and returns the value at the same time
		return memo[key] = res;
	}
	else if (node->label == "dac") {
		// If dac has been hit before on this path, break as this is a cycle
		// and set the illegal state in memo table
		if ((tracker & 1) == 1) return memo[key] = 0;
		// Set first bit of tracker to signify that dac is in path
		tracker |= 1;
	}
	else if (node->label == "fft"){
		if ((tracker & 2) == 2) return memo[key] = 0;
		// Set second bit of tracker to signify that fft is in path
		tracker |= 2;
	} 
	long long paths = 0;
	// Calculate valid paths from node's children
	for (auto const& c:node-> connections) paths += dfs(memo,c,tracker, part2);
	// Assign and return value
	return memo[key] = paths;
}
 
int main() {
	// Reads file directly into a string
	std::ifstream file("d11key.txt");
	std::string line;
	// Hashmap of node name to pair of node pointer and node connections string
	std::unordered_map<std::string,std::pair<std::shared_ptr<Node>, std::string>> nodes;
    // Processes the gapped numbers into coords
	while (std::getline(file, line)){
		if (line.length() == 0) break;
		// Find the first gap
    	std::string::size_type space = line.find(" ");
		// Seperate label for root node and it's connected nodes
		std::string label = line.substr(0,space-1);
		std::string rest = line.substr(space+1);
		// Make a pair of root node pointer and it's connections
		auto values = std::make_pair(std::make_shared<Node>(label),rest);
		// Construct map pair and insert into map
		auto element = std::make_pair(label,values);
		nodes.insert(element);
	}
	// Add out node
	nodes.insert(std::make_pair("out", std::make_pair(std::make_shared<Node>("out"),"")));

	// For each node, go through it's connections and add pointers 
	for (auto const& [key, value] : nodes){
		std::string seg;
		std::stringstream ss(value.second);

		while (ss >> seg){
			auto target = nodes[seg];
			value.first->add(target.first.get());
		}
	}
	// Memo tables and dfs calls for part 1 and 2
	std::unordered_map<uint64_t, long long> memo1;
	int paths1 = dfs(memo1,nodes["you"].first.get(),0,false);
	std::unordered_map<uint64_t, long long> memo2;
	long long paths2 = dfs(memo2,nodes["svr"].first.get(),0,true);

	std::println("{} Part 1 paths, {} Part 2 paths",paths1, paths2);
    return 0;
}
