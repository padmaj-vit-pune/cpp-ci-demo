#include <cstdlib>
#include <iostream>

int add(int a, int b) {
    return a + b;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: calc <a> <b>\n";
        return 1;
    }

    int a = std::atoi(argv[1]);
    int b = std::atoi(argv[2]);

    std::cout << add(a, b) << "\n";
    return 0;
}
