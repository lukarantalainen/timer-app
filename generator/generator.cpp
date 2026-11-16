#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <set>
#include <string>
#include <vector>

void generateInclude(std::ostream& output) {
  output << "#pragma once\n";
  output << "#include <array>\n";
  output << "#include <linux/input-event-codes.h>\n\n";
}

void generateTypesFromHeader(std::ostream& output) {
  std::string header_path{"/usr/include/linux/input-event-codes.h"};
  std::ifstream input(header_path);

  std::regex key_rgx(R"(^#define\s+(\w+)\s+(0x[0-9a-fA-F]+|[0-9]+))");

  bool start;
  std::string previous;
  int previous_number{};

  std::set<std::string> seen;

  std::string line;
  while (std::getline(input, line)) {
    if (line[0] != '#') continue;
    auto begin = std::sregex_iterator(line.begin(), line.end(), key_rgx);
    auto end = std::sregex_iterator();

    std::smatch match;

    std::regex_search(line, match, key_rgx);

    std::string key_name = "";

    if (match.size() >= 2) {
      key_name = match[1];
    }

    int number{};
    if (match.size() >= 3 && start) {
      number = std::stoul(match[2], 0, 0);
      int diff = number - previous_number;
      for (int i{1}; i < diff; ++i) {
        output << "    \"" << "NONE" << "\", \n";
      }
      previous_number = number;
    }

    std::string current;

    for (int i{}; i < key_name.length(); ++i) {
      if (key_name[i] == '_') {
        current = key_name.substr(0, i);
        break;
      }
    }

    if (current == "SYN" && !seen.count("SYN")) {
      output << "std::array<const char*, SYN_CNT> type_syn {\n";
      seen.insert("SYN");
      start = true;
    } else if (current == "KEY" && !seen.count("KEY")) {
      output << "};\n\nstd::array<const char*, KEY_CNT+10> type_key {\n";
      seen.insert("KEY");
    } else if (current == "REL" && !seen.count("REL")) {
      output << "};\n\nstd::array<const char*, REL_CNT> type_rel {\n";
      seen.insert("REL");
    } else if (current == "ABS" && !seen.count("ABS")) {
      output << "};\n\nstd::array<const char*, ABS_CNT> type_abs {\n";
      seen.insert("ABS");
    } else if (current == "MSC" && !seen.count("MSC")) {
      output << "};\n\nstd::array<const char*, MSC_CNT> type_msc {\n";
      seen.insert("MSC");
    } else if (current == "SW" && !seen.count("SW")) {
      output << "};\n\nstd::array<const char*, SW_CNT> type_sw {\n";
      seen.insert("SW");
    } else if (current == "LED" && !seen.count("LED")) {
      output << "};\n\nstd::array<const char*, LED_CNT> type_led {\n";
      seen.insert("LED");
    } else if (current == "REP" && !seen.count("REP")) {
      output << "};\n\nstd::array<const char*, REP_CNT> type_rep {\n";
      seen.insert("REP");
    } else if (current == "SND" && !seen.count("SND")) {
      output << "};\n\nstd::array<const char*, SND_CNT+5> type_snd {\n";
      seen.insert("SND");
    }
  

    if (start && key_name != "" && !key_name.contains("MAX") &&
        !key_name.contains("CNT")) {
      output << "    \"" << key_name << "\", \n";
    }
    previous = current;
  }
  output << "};\n\n";
}

void generateTypeArray(std::ostream& output) {
  output << "std::array<const char**, EV_MAX> types {\n";
  output << "    type_syn.data(),\n";
  output << "    type_key.data(),\n";
  output << "    type_rel.data(),\n";
  output << "    type_abs.data(),\n";
  output << "    type_msc.data(),\n";
  output << "    type_sw.data(),\n";
  output << "    type_led.data(),\n";
  output << "    type_rep.data(),\n";
  output << "    type_snd.data(),\n";
  output << "};\n";
  output << "\n";
}

void generateMaxSizes(std::ostream& output) {
  output << "std::array<int, 9> max_size {\n";
  output << "    SYN_CNT,\n";
  output << "    KEY_CNT,\n";
  output << "    REL_CNT,\n";
  output << "    ABS_CNT,\n";
  output << "    MSC_CNT,\n";
  output << "    SW_CNT,\n";
  output << "    LED_CNT,\n";
  output << "    REP_CNT,\n";
  output << "    SND_CNT,\n";
  output << "};\n";
  output << "\n";
}

int main(int argc, char* argv[]) {
  if (argc > 1) {
    std::ofstream output(argv[1]);
    generateInclude(output);
    generateTypesFromHeader(output);
    generateTypeArray(output);
    generateMaxSizes(output);
  } else {
    return 1;
  }

  return 0;
}
