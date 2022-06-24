#include <fstream>
#include <iostream>

int main() {
    std::ofstream ram_file;
    ram_file.open("RAM.txt");

    for (int i = 0; i < 2048; ++i) {
        for (int j = 0; j < 3; ++j) {
            ram_file << i << " ";
            ++i;
        }
        ram_file << i << std::endl;
    }
    ram_file.close();

    return 1;
}