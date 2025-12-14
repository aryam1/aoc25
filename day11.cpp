#include <fstream>
#include <string>
#include <print>
#include <vector>
#include <memory>
#include <unordered_map>
#include <sstream>
#include <queue>


// Node class
class Node {
public:
    std::vector<Node*> connections;
	std::string label;
	std::unordered_map<std::string,int> visited;
	
    Node(std::string l) : label(l), visited{{l,1}} {}
    void add(Node* child) {
    	connections.push_back(child);
    }
};

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
		//std::println("{}: {} Nodes added",key,value.first->edges);
	}

	std::queue<Node*> bfs{};
	Node* start = nodes["you"].first.get();
	bfs.push(start);
	int paths{}, paths2{};
	
	while (!bfs.empty()){
    	// Pop from queue
    	Node* node = bfs.front();
    	bfs.pop();
    	
    	if (node->label == "out"){
    		paths++;
			continue;
    	}
		// Add current node to 
    	for (auto const& c:node->connections) bfs.push(c);
	}
	std::println("{} Part 1 paths, {} Part 2 paths",paths, paths2);

    return 0;
}
