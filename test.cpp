#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <algorithm>

struct RGB {
    int r, g, b;
};

int main() {
    const int rows = 10;
    const int cols = 10;
    const int values = 10; // 0 = coolest, 9 = hottest

    // Fire colors in RGB
    const RGB fireColors[values] = {
        {13, 8, 135},   // #0d0887
        {70, 3, 159},   // #46039f
        {114, 1, 168},  // #7201a8
        {156, 23, 158}, // #9c179e
        {189, 55, 134}, // #bd3786
        {216, 87, 107}, // #d8576b
        {237, 121, 83}, // #ed7953
        {251, 159, 58}, // #fb9f3a
        {253, 202, 38}, // #fdca26
        {240, 249, 33}  // #f0f921
    };

    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 0));
    std::srand(std::time(nullptr));

    // Add newlines to start below prompt
    std::cout << std::string(rows, '\n');

    for (int step = 0; step < 100; ++step) {
        // update matrix
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j) {
                int change = (std::rand() % 3) - 1; // -1,0,+1
                matrix[i][j] = std::clamp(matrix[i][j] + change, 0, values - 1);
            }

        // move cursor up to overwrite previous frame
        std::cout << "\033[" << rows << "A";

        // print matrix with true color
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                RGB c = fireColors[matrix[i][j]];
                std::cout << "\033[48;2;" << c.r << ";" << c.g << ";" << c.b << "m  \033[0m";
            }
            std::cout << "\n";
        }

        std::cout << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
