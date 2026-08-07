#include <linux/input-event-codes.h>

#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <set>
#include <string>
#include <vector>

#define STR(a) #a
#define TEST 0

std::string starts_with(std::string s) {
  for (int i{}; i < s.length(); ++i) {
    if (s[i] == '_') {
      return std::string(s.begin(), s.begin() + i);
    }
  }

  return "?";
}

void generateInclude(std::ostream& output) {
  output << "#pragma once\n";
  output << "#include <array>\n";
  output << "#include <linux/input-event-codes.h>\n\n";
}

void generateTypesFromHeader(std::ostream& output) {
  std::string header_path{"/usr/include/linux/input-event-codes.h"};
  std::ifstream input(header_path);

  std::string line;

  std::regex rgx("(?:#define )([^\\s]+)");

  bool start;
  std::string previous;

  std::set<std::string> seen;

  while (std::getline(input, line)) {
    auto words_begin = std::sregex_iterator(line.begin(), line.end(), rgx);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
      std::smatch match = *i;
      std::string match_str = match[1];
      // std::cout << match_str << "\n";

      std::string current;

      for (int i{}; i < match_str.length(); ++i) {
        if (match_str[i] == '_') {
          current = match_str.substr(0, i);
          break;
        }
      }

      if (current != previous) {
        if (current == "SYN") {
          output << "std::array<const char*, SYN_CNT> type_syn{\n";
          start = true;
        } else if (current == "KEY" && !seen.count("KEY")) {
          output << "};\n\nstd::array<const char*, KEY_CNT> type_key{\n";
          seen.insert("KEY");
        } else if (current == "REL" && !seen.count("REL")) {
          output << "};\n\nstd::array<const char*, REL_CNT> type_rel{\n";
          seen.insert("REL");
        } else if (current == "ABS" && !seen.count("ABS")) {
          output << "};\n\nstd::array<const char*, ABS_CNT> type_abs{\n";
          seen.insert("ABS");
        } else if (current == "MSC" && !seen.count("MSC")) {
          output << "};\n\nstd::array<const char*, MSC_CNT> type_msc{\n";
          seen.insert("MSC");
        } else if (current == "SW" && !seen.count("SW")) {
          output << "};\n\nstd::array<const char*, SW_CNT+3> type_sw{\n";
          seen.insert("SW");
        } else if (current == "LED" && !seen.count("LED")) {
          output << "};\n\nstd::array<const char*, LED_CNT> type_led{\n";
          seen.insert("LED");
        } else if (current == "REP" && !seen.count("REP")) {
          output << "};\n\nstd::array<const char*, REP_CNT+2> type_rep{\n";
          seen.insert("REP");
        } else if (current == "SND" && !seen.count("SND")) {
          output << "};\n\nstd::array<const char*, SND_CNT> type_snd{\n";
          seen.insert("SND");
        } 
      }

      if (start) output << "    \"" << match_str << "\", \n";
      previous = current;
    }
  }
}

void generateTypeArray(std::ostream& output) {
  output << "};\n";

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
}

void generateMaxSizes(std::ostream& output) {
  output << "std::array<int, 9> max_size {\n";
  output <<  "SYN_CNT,\n";
  output <<  "KEY_CNT,\n";
  output <<  "REL_CNT,\n";
  output <<  "ABS_CNT,\n";
  output <<  "MSC_CNT,\n";
  output <<  "SW_CNT,\n";
  output <<  "LED_CNT,\n";
  output <<  "REP_CNT,\n";
  output <<  "SND_CNT,\n";
  output << "};\n";
}

int main(int argc, char* argv[]) {

  if (argc>1) {
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
