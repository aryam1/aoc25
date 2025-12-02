#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream file("d1key.txt");
    std::string line;
    int pos[100]{};
    int ptr = 50;

    while (std::getline(file, line)) {
	int sclr = std::stoi(line.substr(1));
	if (line[0] == 'L') sclr = -sclr;
	ptr = (ptr + sclr % 99 + 100) % 100;
	pos[ptr] += 1;
    }
    std::cout << pos[0] << std::endl;
    return 0;
}
