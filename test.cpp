#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>


int main() {

    std::ifstream file("./output/input", std::ios_base::binary);

    std::vector<char*> buffer(24);
    file.read(reinterpret_cast<char*>(buffer.data()), 24);

    for (auto c : buffer) {
        printf("%x",c);
    }

    return 0;
}
