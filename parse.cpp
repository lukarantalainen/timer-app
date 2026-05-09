#include <iostream>
#include <regex>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <iterator>

#include <linux/input-event-codes.h>

#define STR(a) #a
#define TEST 0

std::string starts_with(std::string s) {
    for (int i{}; i<s.length(); ++i) {
        if (s[i] == '_') {
            return std::string(s.begin(), s.begin()+i);
        }
    }
    return "?";
}

int main() {
    
    std::string test_path{"/home/lukar/Projects/timer-app/text.txt"};
    std::string header_path{"/usr/include/linux/input-event-codes.h"};
    std::ifstream input(header_path);

    std::string line;

    std::ofstream out("parse.txt");
    
    std::regex rgx("(?:#define )([^\\s]+)");

    std::vector<std::string> syn{};
    std::array<const char*, KEY_CNT> key{};
    std::array<const char*, REL_CNT> rel{};
    std::array<const char*, ABS_CNT> abs{};
    std::array<const char*, MSC_CNT> msc{};
    std::array<const char*, SW_CNT> sw{};
    std::array<const char*, LED_CNT> led{};
    std::array<const char*, SND_CNT> snd{};
    std::array<const char*, REP_CNT> rep{};
    std::array<const char*, 0> ff{};
    std::array<const char*, 0> pwr{};
    std::array<const char*, 0> rel{};



    while (std::getline(input, line)) {
        auto words_begin = 
        std::sregex_iterator(line.begin(), line.end(), rgx);
        auto words_end = std::sregex_iterator();


        for (std::sregex_iterator i = words_begin; i != words_end; ++i)
        {
            std::smatch match = *i;
            std::string match_str = match[1];
            //std::cout << match_str << "\n";
            out << match_str << "\n";
            
            if (std::string(match_str.begin(), match_str.begin()+3) == "SYN") {
                SYN.push_back(match_str);
            }
        }
    }

    for (auto s : SYN) {
        std::cout << s << "\n";
    }
    


    return 0;
}
