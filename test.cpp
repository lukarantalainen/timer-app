#include <iostream>
#include <regex>
#include <string>

int main() {
  std::string line = "#define BTN_GAMEPAD		0x130";

  std::regex key_rgx(R"(^#define\s+(\w+)\s+(0x[0-9a-fA-F]+|[0-9]+))");
  auto begin = std::sregex_iterator(line.begin(), line.end(), key_rgx);
  auto end = std::sregex_iterator();

  for (std::sregex_iterator i = begin; i != end; ++i) {
    auto match = *i;
    for (auto word{match.begin()+1}; word != match.end(); ++word) {
      std::cout << *word<< "\n";
    }
  }

  return 0;
}
