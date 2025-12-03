#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream file("d1key.txt");
    std::string line;
    int ans = 0;
    int ptr = 50;

    while (std::getline(file, line)) {
		int sclr = std::stoi(line.substr(1));
		for (int i =0; i <sclr; i++){
			if (line[0] == 'L') ptr = (ptr-1+100)%100;
			else ptr=(ptr+1)%100;
			if (ptr == 0) ans++;
		}
	}
	std::cout << ans;
    return 0;
}
