#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream file("d1key.txt");
    std::string line;
    int part1 = 0;
    int part2 = 0;
    int ptr = 50;

    while (std::getline(file, line)) {
		int sclr = std::stoi(line.substr(1));
		if (line[0] == 'L') sclr = -sclr;
		int og = ptr;
		ptr += sclr;
		int dv = ptr/100;
		int rem = ptr % 100;
		if (rem < 0) {
			dv = 1-dv;
			rem+=100;
		}
		ptr = rem;
		if (og == 0 && dv > 0) dv--;
		if (ptr == 0) {
			part1++;
			if (dv > 0) dv--;
		}
		part2 += dv;
	}
    std::cout << part1 << ' ' << part2 << std::endl;
    std::cout << part1+part2<<std::endl;
    return 0;
}
